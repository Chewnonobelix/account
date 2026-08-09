#include "Model/Models/pagefilterproxymodel.h"

PageFilterProxyModel::PageFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
  connect(this, &QAbstractItemModel::rowsInserted, this,
          &PageFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::rowsRemoved, this,
          &PageFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::modelReset, this,
          &PageFilterProxyModel::countChanged);
}

void PageFilterProxyModel::setPageSize(int pageSize) {
  const int clamped = qMax(1, pageSize);
  if (m_pageSize == clamped)
    return;

  m_pageSize = clamped;
  emit pageSizeChanged();
  updatePageCount();
}

void PageFilterProxyModel::setCurrentPage(int currentPage) {
  const int clamped = qBound(1, currentPage, m_pageCount);
  if (m_currentPage == clamped)
    return;

  m_currentPage = clamped;
  emit currentPageChanged();
  reapplyFilter();
}

void PageFilterProxyModel::setSourceModel(QAbstractItemModel *sourceModel) {
  if (this->sourceModel())
    disconnect(this->sourceModel(), nullptr, this, nullptr);

  QSortFilterProxyModel::setSourceModel(sourceModel);

  if (sourceModel) {
    connect(sourceModel, &QAbstractItemModel::rowsInserted, this,
            &PageFilterProxyModel::updatePageCount, Qt::QueuedConnection);
    connect(sourceModel, &QAbstractItemModel::rowsRemoved, this,
            &PageFilterProxyModel::updatePageCount, Qt::QueuedConnection);
    connect(sourceModel, &QAbstractItemModel::modelReset, this,
            &PageFilterProxyModel::updatePageCount, Qt::QueuedConnection);
  }

  updatePageCount();
}

bool PageFilterProxyModel::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  if (!sourceModel() || sourceParent.isValid())
    return false;

  const int start = (m_currentPage - 1) * m_pageSize;
  const int end = start + m_pageSize;
  return sourceRow >= start && sourceRow < end;
}

void PageFilterProxyModel::updatePageCount() {
  const int rows = sourceModel() ? sourceModel()->rowCount() : 0;
  const int nextPageCount = qMax(1, (rows + m_pageSize - 1) / m_pageSize);

  if (nextPageCount != m_pageCount) {
    m_pageCount = nextPageCount;
    emit pageCountChanged();
  }

  const int clampedPage = qBound(1, m_currentPage, m_pageCount);
  if (clampedPage != m_currentPage) {
    m_currentPage = clampedPage;
    emit currentPageChanged();
  }

  reapplyFilter();
}

void PageFilterProxyModel::reapplyFilter() {
  beginFilterChange();
  endFilterChange(QSortFilterProxyModel::Direction::Rows);
}
