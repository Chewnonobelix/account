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
}

Qt::SortOrder MainModel::sort() const
{
    return m_sort;
}

void MainModel::setSort(int sort)
{
    m_sort = Qt::SortOrder(sort);
    emit sortChanged();
}

void MainModel::insertData(Entry e)
{
    QMultiMap<QDate, Model> model;
    for (auto it : m_model)
        model.insert(it.e.date(), it);

    model.insert(e.date(), Model{e, Total()});

    Total t;
    for (auto it : model) {
        t = t + it.e;
        it.t = t;
    }

    m_model = model.values();

    beginInsertRows(QModelIndex(), rowCount() - 1, rowCount());
    insertRow(rowCount() - 1);
    endInsertRows();

    qDebug() << "Insert" << rowCount();
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
 auto it = m_model[row];

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
 }
}

QVariant MainModel::headerData(int section, Qt::Orientation, int) const
{
    auto ret = QList<QString>{tr("type"), tr("date"), tr("title"), tr("value"), tr("total")};
    return ret[section];
}

int MainModel::rowCount(const QModelIndex &) const
{
    return m_model.size();
}

int MainModel::columnCount(const QModelIndex &) const
{
    return 5;
}

void MainModel::sort(int column, Qt::SortOrder order)
{
    setColumnSort(column);
    setSort(order);
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

void MainModel::onUpdateEntry(QUuid)
{
    clear();
    setCurrentIndex(-1);
    auto entries = AbstractController::db()->selectEntry();
    for (auto it : entries)
        insertData(it);
}

void MainModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_model.size());
    removeRows(0, rowCount());
    endRemoveRows();
    m_model.clear();
}
