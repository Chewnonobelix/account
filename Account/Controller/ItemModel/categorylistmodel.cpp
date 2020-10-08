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
    removeRows(0, rowCount());
}

void CategoryListModel::init()
{
    insertRows(0, m_db->selectCategory()[currentType()].size());

    for (auto i = 0; i < rowCount(); i++) {
        setData(index(i), QVariant::fromValue(m_db->selectCategory()[currentType()].values()[i]));
    }
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

    if (row < 0 || row > rowCount())
        return QVariant();

    qDebug() << "Debug" << enumrole << row;

    switch (enumrole) {
    case CategoryRole::DisplayRole:
        if (row == rowCount())
            return QVariant(QVariant::String);
        else
            return QVariant::fromValue(m_db->selectCategory()[currentType()].values()[row].name());
    case CategoryRole::TypeRole:
        return QVariant::fromValue(currentType());
    case CategoryRole::IndexRole:
        return QVariant::fromValue(row);
    }

    return QVariant();
}

QHash<int, QByteArray> CategoryListModel::roleNames() const
{
    static auto ret = QHash<int, QByteArray>{{int(CategoryRole::DisplayRole), "display"},
                                             {int(CategoryRole::IndexRole), "index"},
                                             {int(CategoryRole::TypeRole), "type"}};
    return ret;
}
