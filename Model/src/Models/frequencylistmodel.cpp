#include "Model/Models/frequencylistmodel.h"

#include <QString>

namespace {

bool uuidFromVariant(const QVariant &value, QUuid *out) {
  if (!out || !value.isValid())
    return false;

  if (value.canConvert<QUuid>()) {
    *out = value.toUuid();
    return true;
  }

  const QString text = value.toString().trimmed();
  if (text.isEmpty()) {
    *out = QUuid();
    return true;
  }

  const QUuid uuid = QUuid::fromString(text);
  if (uuid.isNull())
    return false;

  *out = uuid;
  return true;
}

template <typename Enum> bool enumFromVariant(const QVariant &value, Enum *out) {
  if (!out)
    return false;

  if (value.canConvert<Enum>()) {
    *out = value.value<Enum>();
    return true;
  }

  bool ok = false;
  const int rawValue = value.toInt(&ok);
  if (!ok)
    return false;

  *out = static_cast<Enum>(rawValue);
  return true;
}

} // namespace

FrequencyListModel::FrequencyListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int FrequencyListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_frequencies.size();
}

QVariant FrequencyListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_frequencies.size())
    return {};

  return roleData(m_frequencies.at(index.row()), role);
}

bool FrequencyListModel::setData(const QModelIndex &index, const QVariant &value,
                                 int role) {
  const FrequencyPtr frequency = frequencyAt(index.row());
  if (!frequency)
    return false;

  switch (role) {
  case IdRole: {
    QUuid id;
    if (!uuidFromVariant(value, &id))
      return false;
    frequency->setId(id);
    return true;
  }
  case IntervalRole: {
    OpenAccountEnums::Frequency interval;
    if (!enumFromVariant(value, &interval))
      return false;
    frequency->setFrequency(interval);
    return true;
  }
  case DateFormatRole:
    frequency->setDateFormat(value.toString());
    return true;
  case CustomIntervalDaysRole: {
    bool ok = false;
    const int days = value.toInt(&ok);
    if (!ok)
      return false;
    frequency->setCustomIntervalDays(days);
    return true;
  }
  default:
    break;
  }

  return false;
}

Qt::ItemFlags FrequencyListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> FrequencyListModel::roleNames() const {
  return {{FrequencyRole, "frequencyObject"},
          {IdRole, "id"},
          {IntervalRole, "frequency"},
          {DateFormatRole, "dateFormat"},
          {CustomIntervalDaysRole, "customIntervalDays"},
          {PrototypeRole, "prototype"}};
}

FrequencyPtr FrequencyListModel::frequencyAt(int row) const {
  if (row < 0 || row >= m_frequencies.size())
    return {};

  return m_frequencies.at(row);
}

bool FrequencyListModel::contains(const QUuid &frequencyId) const {
  return indexOf(frequencyId) >= 0;
}

void FrequencyListModel::setFrequencies(const QList<FrequencyPtr> &frequencies) {
  QList<FrequencyPtr> nextFrequencies;
  nextFrequencies.reserve(frequencies.size());

  auto alreadyAdded = [&nextFrequencies](const FrequencyPtr &candidate) {
    for (const FrequencyPtr &existing : nextFrequencies) {
      if (existing == candidate)
        return true;
      if (existing && candidate && !existing->id().isNull() &&
          existing->id() == candidate->id()) {
        return true;
      }
    }
    return false;
  };

  for (const FrequencyPtr &frequency : frequencies) {
    if (!frequency || alreadyAdded(frequency))
      continue;

    nextFrequencies.append(frequency);
  }

  const int oldCount = m_frequencies.size();

  for (const FrequencyPtr &frequency : m_frequencies)
    disconnectFrequency(frequency.data());

  beginResetModel();
  m_frequencies = nextFrequencies;
  endResetModel();

  for (const FrequencyPtr &frequency : m_frequencies)
    connectFrequency(frequency);

  if (oldCount != m_frequencies.size())
    emit countChanged();
}

bool FrequencyListModel::addFrequency(const FrequencyPtr &frequency) {
  if (!frequency || indexOf(frequency.data()) >= 0 ||
      (!frequency->id().isNull() && contains(frequency->id()))) {
    return false;
  }

  const int row = m_frequencies.size();
  beginInsertRows(QModelIndex(), row, row);
  m_frequencies.append(frequency);
  connectFrequency(frequency);
  endInsertRows();

  emit countChanged();
  return true;
}

bool FrequencyListModel::addFrequency(QObject *frequency) {
  Frequency *rawFrequency = qobject_cast<Frequency *>(frequency);
  if (!rawFrequency)
    return false;

  return addFrequency(FrequencyPtr(rawFrequency, [](Frequency *) {}));
}

bool FrequencyListModel::removeFrequency(const QUuid &frequencyId) {
  return removeFrequencyAt(indexOf(frequencyId));
}

bool FrequencyListModel::removeFrequencyAt(int row) {
  if (row < 0 || row >= m_frequencies.size())
    return false;

  disconnectFrequency(m_frequencies.at(row).data());

  beginRemoveRows(QModelIndex(), row, row);
  m_frequencies.removeAt(row);
  endRemoveRows();

  emit countChanged();
  return true;
}

void FrequencyListModel::clear() {
  if (m_frequencies.isEmpty())
    return;

  for (const FrequencyPtr &frequency : m_frequencies)
    disconnectFrequency(frequency.data());

  beginResetModel();
  m_frequencies.clear();
  endResetModel();

  emit countChanged();
}

QObject *FrequencyListModel::at(int row) const {
  const FrequencyPtr frequency = frequencyAt(row);
  return frequency ? frequency.data() : nullptr;
}

QVariantMap FrequencyListModel::get(int row) const {
  const FrequencyPtr frequency = frequencyAt(row);
  if (!frequency)
    return {};

  const QHash<int, QByteArray> names = roleNames();
  QVariantMap result;
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), roleData(frequency, it.key()));

  return result;
}

int FrequencyListModel::indexOf(const QUuid &frequencyId) const {
  if (frequencyId.isNull())
    return -1;

  for (int row = 0; row < m_frequencies.size(); ++row) {
    const FrequencyPtr &frequency = m_frequencies.at(row);
    if (frequency && frequency->id() == frequencyId)
      return row;
  }

  return -1;
}

int FrequencyListModel::indexOf(const Frequency *frequency) const {
  if (!frequency)
    return -1;

  for (int row = 0; row < m_frequencies.size(); ++row) {
    if (m_frequencies.at(row).data() == frequency)
      return row;
  }

  return -1;
}

void FrequencyListModel::emitRolesChanged(Frequency *frequency,
                                          const QList<int> &roles) {
  const int row = indexOf(frequency);
  if (row < 0)
    return;

  const QModelIndex modelIndex = index(row, 0);
  emit dataChanged(modelIndex, modelIndex, roles);
}

void FrequencyListModel::connectFrequency(const FrequencyPtr &frequency) {
  if (!frequency)
    return;

  Frequency *rawFrequency = frequency.data();

  connect(rawFrequency, &Frequency::idChanged, this, [this, rawFrequency]() {
    emitRolesChanged(rawFrequency, {IdRole});
  });
  connect(rawFrequency, &Frequency::frequencyChanged, this, [this, rawFrequency]() {
    emitRolesChanged(rawFrequency, {IntervalRole});
  });
  connect(rawFrequency, &Frequency::dateFormatChanged, this, [this, rawFrequency]() {
    emitRolesChanged(rawFrequency, {DateFormatRole});
  });
  connect(rawFrequency, &Frequency::customIntervalDaysChanged, this, [this, rawFrequency]() {
    emitRolesChanged(rawFrequency, {CustomIntervalDaysRole});
  });
  connect(rawFrequency, &Frequency::prototypeChanged, this, [this, rawFrequency]() {
    emitRolesChanged(rawFrequency, {PrototypeRole});
  });
}

void FrequencyListModel::disconnectFrequency(Frequency *frequency) {
  if (!frequency)
    return;

  disconnect(frequency, nullptr, this, nullptr);
}

QVariant FrequencyListModel::roleData(const FrequencyPtr &frequency, int role) const {
  if (!frequency)
    return {};

  switch (role) {
  case FrequencyRole:
    return QVariant::fromValue(static_cast<QObject *>(frequency.data()));
  case IdRole:
    return frequency->id();
  case IntervalRole:
    return QVariant::fromValue(frequency->frequency());
  case DateFormatRole:
    return frequency->dateFormat();
  case CustomIntervalDaysRole:
    return frequency->customIntervalDays();
  case PrototypeRole:
    return QVariant::fromValue(frequency->prototype());
  default:
    break;
  }

  return {};
}
