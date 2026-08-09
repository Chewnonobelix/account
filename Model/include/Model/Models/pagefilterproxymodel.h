#pragma once

#include <QQmlEngine>
#include <QSortFilterProxyModel>

#include "model_global.h"

// Windows a source model down to at most pageSize contiguous rows, picked by
// currentPage (1-based). Unlike the other proxy models in this module,
// filterAcceptsRow here depends on each row's *position* rather than its
// data, so a structural change anywhere in the source (an insert/remove that
// may not even touch the current page) can flip which rows belong in the
// window. QSortFilterProxyModel only re-runs filterAcceptsRow for the rows
// actually touched by a given signal, so every source structural signal
// forces a full re-filter via updatePageCount(), queued so it runs after the
// base class has finished reacting to that same signal.
class MODEL_EXPORT PageFilterProxyModel : public QSortFilterProxyModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int pageSize READ pageSize WRITE setPageSize NOTIFY pageSizeChanged)
  Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
  Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  explicit PageFilterProxyModel(QObject *parent = nullptr);

  int pageSize() const { return m_pageSize; }
  int currentPage() const { return m_currentPage; }
  int pageCount() const { return m_pageCount; }
  int count() const { return rowCount(); }

  void setPageSize(int pageSize);
  void setCurrentPage(int currentPage);

  void setSourceModel(QAbstractItemModel *sourceModel) override;

signals:
  void pageSizeChanged();
  void currentPageChanged();
  void pageCountChanged();
  void countChanged();

protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

private slots:
  void updatePageCount();

private:
  void reapplyFilter();

private:
  int m_pageSize = 100;
  int m_currentPage = 1;
  int m_pageCount = 1;
};
