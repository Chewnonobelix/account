#include "categorylistmodel.h"

CategoryListModel::CategoryListModel() : QAbstractListModel(), m_db(AbstractController::db())
{
 connect(m_db, &InterfaceDataSave::s_updateCategory, this, &CategoryListModel::onUpdateCategory);
 onUpdateCategory();
}

Account::TypeEnum CategoryListModel::currentType() const
{
 return m_currentType;
}

void CategoryListModel::setCurrentType(Account::TypeEnum type)
{
 m_currentType = type;

 reset();
 init();
 emit currentTypeChanged();
}

void CategoryListModel::onUpdateCategory()
{
 reset();
 init();
}

void CategoryListModel::reset()
{
 beginRemoveRows(QModelIndex(), 0, rowCount());
 removeRows(0, rowCount());
 endRemoveRows();
}

void CategoryListModel::init()
{
 if (!m_db)
  return;

 auto list = m_db->selectCategory()[currentType()].values();

 beginInsertRows(QModelIndex(), 0, list.size());
 insertRows(0, rowCount());
 endInsertRows();
}

int CategoryListModel::rowCount(const QModelIndex &) const
{
 return m_db->selectCategory()[currentType()].size();
}

Qt::ItemFlags CategoryListModel::flags(const QModelIndex &) const
{
 return Qt::ItemIsSelectable;
}

QVariant CategoryListModel::data(const QModelIndex &index, int role) const
{
 auto enumrole = CategoryRole(role);
 auto row = index.row();

 if (row < 0 || row >= rowCount())
  return QVariant();

 QVariant ret;
 switch (enumrole) {
 case CategoryRole::DisplayRole:
  if (row == rowCount())
   ret = QVariant(QVariant::String);
  else
   ret = QVariant::fromValue(m_db->selectCategory()[currentType()].values()[row].name());
  break;
 case CategoryRole::TypeRole:
  ret = QVariant::fromValue(currentType());
  break;
 }

 qDebug() << "Debug" << enumrole << row << ret;

 return ret;
}

QHash<int, QByteArray> CategoryListModel::roleNames() const
{
 static auto ret = QHash<int, QByteArray>{{int(CategoryRole::DisplayRole), "display"},
										  {int(CategoryRole::TypeRole), "type"}};
 return ret;
}
