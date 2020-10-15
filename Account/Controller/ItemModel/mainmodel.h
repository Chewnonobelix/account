#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "../core/abstractcontroller.h"
#include "Model/entry.h"
#include "Model/total.h"
#include <QAbstractItemModel>
#include <QList>

class MainModel : public QAbstractTableModel
{
 Q_OBJECT

 Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
 Q_PROPERTY(int sort READ sort WRITE setSort NOTIFY sortChanged)
 Q_PROPERTY(int columnSort READ columnSort WRITE setColumnSort NOTIFY columnSortChanged)

 struct Model
 {
  Entry e;
  Total t;
 };

 private:
	 enum class MainRole {
	  TypeRole = Qt::UserRole + 1,
	  TitleRole,
	  DateRole,
	  TotalRole,
	  EstimatedRole,
	  ValueRole
	 };

	 int m_currentIndex = -1;
	 QList<Model> m_model;
	 int m_columnSort = 0;
	 Qt::SortOrder m_sort = Qt::AscendingOrder;

 public:
	 MainModel();
	 MainModel(const MainModel &);
	 ~MainModel() = default;
	 int currentIndex() const;
	 void setCurrentIndex(int currentIndex);
	 int columnSort() const;
	 void setColumnSort(int columnSort);
	 Qt::SortOrder sort() const;
	 void setSort(int sort);
	 void insertData(Entry);
	 void clear();
	 Q_INVOKABLE Model at(int) const;
 signals:
	 void currentIndexChanged();
	 void sortChanged();
	 void columnSortChanged();
 public slots:
	 void onUpdateEntry(QUuid = QUuid());

 public: //Model methods
	 Qt::ItemFlags flags(QModelIndex const & = QModelIndex()) const;
	 QVariant data(QModelIndex const &, int) const;
	 QVariant headerData(int, Qt::Orientation = Qt::Orientation(), int = 0) const;
	 int rowCount(const QModelIndex &parent = QModelIndex()) const;
	 int columnCount(const QModelIndex &parent = QModelIndex()) const;
	 void sort(int column, Qt::SortOrder = Qt::AscendingOrder);
	 QHash<int, QByteArray> roleNames() const;
};

Q_DECLARE_METATYPE(MainModel)

#endif // MAINMODEL_H
