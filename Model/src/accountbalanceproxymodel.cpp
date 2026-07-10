#include "Model/accountbalanceproxymodel.h"

#include <algorithm>

#include <QAbstractItemModel>

#include "Model/total.h"
#include "Model/transaction.h"

AccountBalanceProxyModel::AccountBalanceProxyModel(QObject *parent)
    : QAbstractProxyModel(parent) {}

void AccountBalanceProxyModel::setSourceModel(
    QAbstractItemModel *sourceModel) {
  if (sourceModel == this->sourceModel())
    return;

  if (QAbstractItemModel *previous = this->sourceModel())
    previous->disconnect(this);

  QAbstractProxyModel::setSourceModel(sourceModel);
  connectSourceModel(sourceModel);
  rebuild();
}

void AccountBalanceProxyModel::connectSourceModel(
    QAbstractItemModel *sourceModel) {
  if (!sourceModel)
    return;

  // Any structural or content change to the source invalidates the running
  // balance, so recompute the whole series.
  connect(sourceModel, &QAbstractItemModel::rowsInserted, this,
          &AccountBalanceProxyModel::rebuild);
  connect(sourceModel, &QAbstractItemModel::rowsRemoved, this,
          &AccountBalanceProxyModel::rebuild);
  connect(sourceModel, &QAbstractItemModel::rowsMoved, this,
          &AccountBalanceProxyModel::rebuild);
  connect(sourceModel, &QAbstractItemModel::dataChanged, this,
          &AccountBalanceProxyModel::rebuild);
  connect(sourceModel, &QAbstractItemModel::layoutChanged, this,
          &AccountBalanceProxyModel::rebuild);
  connect(sourceModel, &QAbstractItemModel::modelReset, this,
          &AccountBalanceProxyModel::rebuild);
}

int AccountBalanceProxyModel::roleFor(const QByteArray &name) const {
  const QAbstractItemModel *source = sourceModel();
  if (!source)
    return -1;

  const QHash<int, QByteArray> names = source->roleNames();
  for (auto it = names.cbegin(); it != names.cend(); ++it) {
    if (it.value() == name)
      return it.key();
  }
  return -1;
}

QDate AccountBalanceProxyModel::anchorDate() const {
  return m_referenceDate.isValid() ? m_referenceDate : QDate::currentDate();
}

QDate AccountBalanceProxyModel::windowStart(const QDate &windowEnd) const {
  switch (m_window) {
  case Month:
    return windowEnd.addMonths(-1);
  case Year:
    return windowEnd.addYears(-1);
  case Decade:
    return windowEnd.addYears(-10);
  case All:
    break;
  }
  return {}; // invalid => no lower bound
}

QDate AccountBalanceProxyModel::bucketKey(const QDate &date) const {
  switch (m_pointFrequency) {
  case Monthly:
    return {date.year(), date.month(), 1};
  case Yearly:
    return {date.year(), 1, 1};
  case Daily:
    break;
  }
  return date;
}

void AccountBalanceProxyModel::rebuild() {
  beginResetModel();

  m_rows.clear();
  m_sourceToProxy.clear();

  QAbstractItemModel *source = sourceModel();
  if (source) {
    const int dateRole = roleFor("date");
    const int valueRole = roleFor("value");
    const int movementRole = roleFor("movement");

    struct Entry {
      int sourceRow;
      QDate date;
      TransactionPtr transaction;
    };

    QList<Entry> entries;
    const int rows = source->rowCount();
    entries.reserve(rows);

    for (int row = 0; row < rows; ++row) {
      const QModelIndex sourceIndex = source->index(row, 0);

      const QDate date = source->data(sourceIndex, dateRole).toDate();
      const double value = source->data(sourceIndex, valueRole).toDouble();
      const auto movement =
          source->data(sourceIndex, movementRole)
              .value<OpenAccountEnums::Movement>();

      // Rebuild a lightweight Transaction so the accumulation can be delegated
      // to Total (which owns the Credit/Debit/Both sign rule).
      auto transaction = QSharedPointer<Transaction>::create();
      transaction->setDate(date);
      transaction->setValue(value);
      transaction->setMovement(movement);
      entries.append({row, date, transaction});
    }

    // Sort chronologically; keep source order stable for same-day entries so
    // the running balance is deterministic.
    std::stable_sort(entries.begin(), entries.end(),
                     [](const Entry &a, const Entry &b) {
                       return a.date < b.date;
                     });

    // Restrict to the requested window (anchored on referenceDate, or today).
    const QDate lowerBound = windowStart(anchorDate());

    // Accumulate into a Total: the balance at a point is the initial balance
    // plus Total::evaluate() over every transaction up to and including it.
    // Earlier-than-window transactions are still added to the Total so the
    // first visible point already carries the full history.
    //
    // Entries are date-sorted, so a bucket is a contiguous run of equal keys;
    // each bucket yields one point holding the end-of-period balance and the
    // date of its last transaction.
    Total total;
    m_rows.reserve(entries.size());
    QDate currentKey;
    for (const Entry &entry : entries) {
      total.addTransaction(entry.transaction);

      if (lowerBound.isValid() && entry.date < lowerBound)
        continue;

      const double balance = m_initialBalance + total.evaluate();

      const QDate key = bucketKey(entry.date);
      if (m_rows.isEmpty() || key != currentKey) {
        currentKey = key;
        m_rows.append({entry.sourceRow, entry.date, balance});
      } else {
        Row &last = m_rows.last();
        last.sourceRow = entry.sourceRow;
        last.date = entry.date;
        last.balance = balance;
      }
      m_sourceToProxy.insert(entry.sourceRow, m_rows.size() - 1);
    }
  }

  // Refresh cached ranges used by the QML axes.
  if (m_rows.isEmpty()) {
    m_minBalance = m_initialBalance;
    m_maxBalance = m_initialBalance;
  } else {
    m_minBalance = m_maxBalance = m_rows.first().balance;
    for (const Row &row : m_rows) {
      m_minBalance = std::min(m_minBalance, row.balance);
      m_maxBalance = std::max(m_maxBalance, row.balance);
    }
  }

  endResetModel();

  emit countChanged();
  emit rangeChanged();
}

QModelIndex
AccountBalanceProxyModel::mapToSource(const QModelIndex &proxyIndex) const {
  if (!proxyIndex.isValid() || !sourceModel())
    return {};

  const int row = proxyIndex.row();
  if (row < 0 || row >= m_rows.size())
    return {};

  return sourceModel()->index(m_rows.at(row).sourceRow, 0);
}

QModelIndex
AccountBalanceProxyModel::mapFromSource(const QModelIndex &sourceIndex) const {
  if (!sourceIndex.isValid())
    return {};

  const int proxyRow = m_sourceToProxy.value(sourceIndex.row(), -1);
  if (proxyRow < 0)
    return {};

  return index(proxyRow, 0);
}

QModelIndex AccountBalanceProxyModel::index(int row, int column,
                                            const QModelIndex &parent) const {
  if (parent.isValid() || row < 0 || row >= m_rows.size() || column < 0 ||
      column >= ColumnCount)
    return {};

  return createIndex(row, column);
}

QModelIndex AccountBalanceProxyModel::parent(const QModelIndex &) const {
  return {};
}

int AccountBalanceProxyModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_rows.size();
}

int AccountBalanceProxyModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return ColumnCount;
}

QVariant AccountBalanceProxyModel::data(const QModelIndex &index,
                                        int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_rows.size())
    return {};

  const Row &row = m_rows.at(index.row());
  const double timestamp =
      static_cast<double>(QDateTime(row.date.startOfDay()).toMSecsSinceEpoch());

  switch (role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    // Column-based access used by VXYModelMapper.
    return index.column() == BalanceColumn ? QVariant(row.balance)
                                           : QVariant(timestamp);
  case TimestampRole:
    return timestamp;
  case DateRole:
    return row.date;
  case BalanceRole:
    return row.balance;
  default:
    break;
  }

  return {};
}

QVariant AccountBalanceProxyModel::headerData(int section,
                                              Qt::Orientation orientation,
                                              int role) const {
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return {};

  switch (section) {
  case TimestampColumn:
    return QStringLiteral("timestamp");
  case BalanceColumn:
    return QStringLiteral("balance");
  default:
    return {};
  }
}

QHash<int, QByteArray> AccountBalanceProxyModel::roleNames() const {
  return {{TimestampRole, "timestamp"},
          {DateRole, "date"},
          {BalanceRole, "balance"}};
}

void AccountBalanceProxyModel::setInitialBalance(double initialBalance) {
  if (qFuzzyCompare(m_initialBalance, initialBalance))
    return;

  m_initialBalance = initialBalance;
  emit initialBalanceChanged();
  rebuild();
}

void AccountBalanceProxyModel::setWindow(Window window) {
  if (m_window == window)
    return;

  m_window = window;
  emit windowChanged();
  rebuild();
}

void AccountBalanceProxyModel::setPointFrequency(PointFrequency pointFrequency) {
  if (m_pointFrequency == pointFrequency)
    return;

  m_pointFrequency = pointFrequency;
  emit pointFrequencyChanged();
  rebuild();
}

void AccountBalanceProxyModel::setReferenceDate(const QDate &referenceDate) {
  if (m_referenceDate == referenceDate)
    return;

  m_referenceDate = referenceDate;
  emit referenceDateChanged();
  rebuild();
}

QDateTime AccountBalanceProxyModel::minDate() const {
  if (m_rows.isEmpty())
    return {};

  return QDateTime(m_rows.first().date.startOfDay());
}

QDateTime AccountBalanceProxyModel::maxDate() const {
  if (m_rows.isEmpty())
    return {};

  return QDateTime(m_rows.last().date.startOfDay());
}

QVariantMap AccountBalanceProxyModel::get(int row) const {
  if (row < 0 || row >= m_rows.size())
    return {};

  const Row &entry = m_rows.at(row);
  QVariantMap result;
  result.insert(QStringLiteral("timestamp"),
                static_cast<double>(
                    QDateTime(entry.date.startOfDay()).toMSecsSinceEpoch()));
  result.insert(QStringLiteral("date"), entry.date);
  result.insert(QStringLiteral("balance"), entry.balance);
  return result;
}
