#pragma once

#include <QAbstractProxyModel>
#include <QDate>
#include <QDateTime>
#include <QHash>
#include <QList>

#include "enums.h"
#include "model_global.h"

/**
 * @brief Proxy model turning a flat transaction model into a cumulative
 *        balance time-series.
 *
 * It wraps a source model exposing transactions through the "date", "value"
 * and "movement" roles (typically a TransactionListModel or an
 * AccountTransactionFilterProxyModel), reorders its rows by date and computes,
 * for each row, the running balance obtained by accumulating signed amounts
 * (Credit => +value, Debit => -value, Both => +value) on top of an initial
 * balance.
 *
 * Two granularity knobs shape the resulting series:
 *  - @c window restricts the displayed time span (last month, year, decade or
 *    everything). The balance still accumulates every earlier transaction, so
 *    the first visible point already reflects the account's full history.
 *  - @c pointFrequency buckets transactions into one point per day, month or
 *    year, keeping the end-of-period balance.
 *
 * Two columns are exposed so a QML ChartView can bind directly through a
 * VXYModelMapper:
 *
 * @code
 * ChartView {
 *     DateTimeAxis { id: axisX; format: "dd/MM/yy" }
 *     ValueAxis    { id: axisY }
 *     LineSeries   { id: series; axisX: axisX; axisY: axisY }
 *     VXYModelMapper {
 *         series: series
 *         model: balanceProxy
 *         xColumn: AccountBalanceProxyModel.TimestampColumn // 0
 *         yColumn: AccountBalanceProxyModel.BalanceColumn   // 1
 *     }
 * }
 * @endcode
 *
 * The custom roles (timestamp/date/balance) also make the series usable from a
 * plain Repeater or delegate.
 */
class MODEL_EXPORT AccountBalanceProxyModel : public QAbstractProxyModel {
  Q_OBJECT

  Q_PROPERTY(double initialBalance READ initialBalance WRITE setInitialBalance
                 NOTIFY initialBalanceChanged)
  Q_PROPERTY(Window window READ window WRITE setWindow NOTIFY windowChanged)
  Q_PROPERTY(PointFrequency pointFrequency READ pointFrequency WRITE
                 setPointFrequency NOTIFY pointFrequencyChanged)
  Q_PROPERTY(QDate referenceDate READ referenceDate WRITE setReferenceDate
                 NOTIFY referenceDateChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)
  Q_PROPERTY(double minBalance READ minBalance NOTIFY rangeChanged)
  Q_PROPERTY(double maxBalance READ maxBalance NOTIFY rangeChanged)
  Q_PROPERTY(QDateTime minDate READ minDate NOTIFY rangeChanged)
  Q_PROPERTY(QDateTime maxDate READ maxDate NOTIFY rangeChanged)

public:
  enum Column { TimestampColumn = 0, BalanceColumn = 1, ColumnCount = 2 };
  Q_ENUM(Column)

  enum Role { TimestampRole = Qt::UserRole + 1, DateRole, BalanceRole };
  Q_ENUM(Role)

  // Span of history that is displayed, counted back from the most recent
  // transaction.
  enum Window { Month, Year, Decade, All };
  Q_ENUM(Window)

  // How many points are emitted: one per day, month or year.
  enum PointFrequency { Daily, Monthly, Yearly };
  Q_ENUM(PointFrequency)

  explicit AccountBalanceProxyModel(QObject *parent = nullptr);

  // QAbstractProxyModel interface
  void setSourceModel(QAbstractItemModel *sourceModel) override;
  QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  double initialBalance() const { return m_initialBalance; }
  void setInitialBalance(double initialBalance);

  Window window() const { return m_window; }
  void setWindow(Window window);

  PointFrequency pointFrequency() const { return m_pointFrequency; }
  void setPointFrequency(PointFrequency pointFrequency);

  // Date the window is anchored on. Defaults to today (QDate::currentDate())
  // when left invalid; set it explicitly to freeze the anchor.
  QDate referenceDate() const { return m_referenceDate; }
  void setReferenceDate(const QDate &referenceDate);

  int count() const { return m_rows.size(); }
  double minBalance() const { return m_minBalance; }
  double maxBalance() const { return m_maxBalance; }
  QDateTime minDate() const;
  QDateTime maxDate() const;

  Q_INVOKABLE QVariantMap get(int row) const;

signals:
  void initialBalanceChanged();
  void windowChanged();
  void pointFrequencyChanged();
  void referenceDateChanged();
  void countChanged();
  void rangeChanged();

private slots:
  void rebuild();

private:
  struct Row {
    int sourceRow = -1;
    QDate date;
    double balance = 0.0;
  };

  void connectSourceModel(QAbstractItemModel *sourceModel);
  int roleFor(const QByteArray &name) const;
  QDate anchorDate() const;
  QDate windowStart(const QDate &windowEnd) const;
  QDate bucketKey(const QDate &date) const;

private:
  QList<Row> m_rows;
  QHash<int, int> m_sourceToProxy;
  double m_initialBalance = 0.0;
  double m_minBalance = 0.0;
  double m_maxBalance = 0.0;
  Window m_window = All;
  PointFrequency m_pointFrequency = Daily;
  QDate m_referenceDate; // invalid => QDate::currentDate()
};
