#include "mainmodel.h"

MainModel::MainModel()
{
  connect(AbstractController::db(),
		  &InterfaceDataSave::s_updateEntry,
		  this,
		  &MainModel::onUpdateEntry);

  onUpdateEntry();
}

MainModel::MainModel(const MainModel &model)
	: QAbstractTableModel(), m_currentIndex(model.currentIndex()), m_model(model.m_model),
	  m_columnSort(model.columnSort()), m_sort(model.sort())
{}

int MainModel::currentIndex() const
{
  return m_currentIndex;
}

void MainModel::setCurrentIndex(int currentIndex)
{
  m_currentIndex = currentIndex;
  emit currentIndexChanged();
}

int MainModel::columnSort() const
{
  return m_columnSort;
}

void MainModel::setColumnSort(int columnSort)
{
  m_columnSort = columnSort;
  emit columnSortChanged();
  sort(this->columnSort(), this->sort());
}

Qt::SortOrder MainModel::sort() const
{
  return m_sort;
}

void MainModel::setSort(int sort)
{
  m_sort = Qt::SortOrder(sort);
  emit sortChanged();
  this->sort(this->columnSort(), this->sort());
}

void MainModel::insertData(Entry e)
{
  m_model.insert(e.date(), Model{e, Total()});

  Total t;
  for (auto &it : m_model) {
	t = t + it.e;
	it.t = t;
  }

  m_displayModel = m_model.values();

  setRow();
}

void MainModel::setRow()
{
  clear();
  beginInsertRows(QModelIndex(), rowCount() - 1, rowCount());
  insertRow(rowCount() - 1);
  endInsertRows();
}

Qt::ItemFlags MainModel::flags(QModelIndex const &) const
{
  return Qt::ItemFlags();
}

QVariant MainModel::at(int row, int column) const
{
  return data(index(row, column), columnsOrder[column]);
}

QVariant MainModel::at(int row) const
{
  return data(index(row, 0), -1);
}

QVariant MainModel::data(QModelIndex const &index, int role) const
{
  if (!index.isValid())
	return QVariant();

  auto row = index.row();
  auto it = m_displayModel[row];

  if (role == -1)
	return QVariant::fromValue(it.e);

  auto en = MainRole(role);

  //    qDebug() << "Data" <<en<< role << row << (QJsonObject) it.e /*<< it.t*/;
  QString ret;
  switch (en) {
	case MainRole::DateRole:
	  return it.e.date().toString("dd-MM-yyyy");
	  break;
	case MainRole::EstimatedRole:
	  return it.e.estimated();
	  break;
	case MainRole::TitleRole:
	  return it.e.title();
	  break;
	case MainRole::TotalRole:
	  return it.t.value();
	  break;
	case MainRole::TypeRole:
	  return it.e.type();
	  break;
	case MainRole::TypeDisplayRole:
	  ret = it.e.type() == Account::TypeEnum::Income ? "+" : "-";
	  if (it.e.estimated())
		ret = "*" + ret;
	  return ret;
	  break;
	case MainRole::ValueRole:
	  return it.e.value();
	  break;
	case MainRole::IdRole:
	  return it.e.id();
	  break;
  }

  return QVariant();
}

QVariant MainModel::headerData(int section, Qt::Orientation, int) const
{
  auto ret = QList<QString>{tr("type"), tr("date"), tr("title"), tr("value"), tr("total")};
  return ret[section];
}

int MainModel::rowCount(const QModelIndex &) const
{
  return m_displayModel.size();
}

int MainModel::columnCount(const QModelIndex &) const
{
  return 5;
}

void MainModel::sort(int column, Qt::SortOrder order)
{
  auto columnName = roleNames()[columnsOrder[column]];

  std::sort(m_displayModel.begin(), m_displayModel.end(), [=](const Model& m1, const Model& m2) {
	bool ret = false;
	if(MainRole(columnsOrder[column]) == MainRole::TotalRole)
	  ret = m1.t.value() < m2.t.value();
	else
	  ret = m1.e.metaData<QString>(columnName) < m2.e.metaData<QString>(columnName);

	return order == Qt::AscendingOrder ? ret : !ret;
  });

  clear();
  setRow();
}

QHash<int, QByteArray> MainModel::roleNames() const
{
  static auto ret = QHash<int, QByteArray>{{int(MainRole::TypeRole), "type"},
										   {int(MainRole::TitleRole), "title"},
										   {int(MainRole::TotalRole), "total"},
										   {int(MainRole::DateRole), "date"},
										   {int(MainRole::EstimatedRole), "estimated"},
										   {int(MainRole::ValueRole), "value"},
										   {int(MainRole::TypeDisplayRole), "typeDisplay"},
										   {int(MainRole::IdRole), "id"}};
  return ret;
}

void MainModel::onUpdateEntry(QUuid id)
{
  clear();
  m_model.clear();
  m_displayModel.clear();
  auto entries = AbstractController::db()->selectEntry();
  for (auto it : entries){
	insertData(it);
  }
  sort(columnSort(), sort());

  for(auto it = 0; it < m_displayModel.size(); it++)
	if(m_displayModel[it].e.id() == id) {
	  setCurrentIndex(it);
	}

}

void MainModel::clear()
{
  beginRemoveRows(QModelIndex(), 0, m_model.size());
  removeRows(0, rowCount());
  endRemoveRows();
}

QVariant MainModel::dateList() const
{
  return m_dateList;
}

void MainModel::setDateList(QVariant list)
{
  m_dateList = list;

  m_displayModel.clear();

  if(m_dateList.toList().isEmpty()) {
	m_displayModel = m_model.values();
  }
  else {
	auto dl = m_dateList.toList();
	for(auto it: dl) {
	  auto values = m_model.values(QDate::fromString(it.toString(), "dd-MM-yyyy"));
	  m_displayModel << values;
	}
  }

  setRow();
}
