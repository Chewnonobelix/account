#pragma once

#include <QDate>
#include <QList>
#include <QPointer>
#include <QQmlEngine>
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QUuid>
#include <QVariantMap>

#include "account.h"
#include "enums.h"
#include "metatypes.h"
#include "model_global.h"

class MODEL_EXPORT AccountTransactionFilterProxyModel
    : public QSortFilterProxyModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(Account *account READ account WRITE setAccount NOTIFY accountChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)
  Q_PROPERTY(QList<QDate> dateFilter READ dateFilter WRITE setDateFilter NOTIFY dateFilterChanged)
  // Range filter: unlike dateFilter (an exact-date whitelist, e.g. from an
  // OBCalendar multi-selection), dateFrom/dateTo match a contiguous span
  // (e.g. a Budget period) in O(1) per row instead of needing every date in
  // the range enumerated into a list. Either bound may be left invalid to
  // leave that side open-ended; both are AND-ed with every other filter.
  Q_PROPERTY(QDate dateFrom READ dateFrom WRITE setDateFrom NOTIFY dateFromChanged)
  Q_PROPERTY(QDate dateTo READ dateTo WRITE setDateTo NOTIFY dateToChanged)
  Q_PROPERTY(QList<OpenAccountEnums::Support> supportFilter READ supportFilter WRITE setSupportFilter NOTIFY supportFilterChanged)
  Q_PROPERTY(QList<QUuid> categoryFilter READ categoryFilter WRITE setCategoryFilter NOTIFY categoryFilterChanged)
  Q_PROPERTY(QString descriptionFilter READ descriptionFilter WRITE setDescriptionFilter NOTIFY descriptionFilterChanged)

public:
  explicit AccountTransactionFilterProxyModel(QObject *parent = nullptr);

  Account *account() const { return m_account; }
  int count() const { return rowCount(); }
  QList<QDate> dateFilter() const { return m_dateFilter; }
  QDate dateFrom() const { return m_dateFrom; }
  QDate dateTo() const { return m_dateTo; }
  QList<OpenAccountEnums::Support> supportFilter() const { return m_supportFilter; }
  QList<QUuid> categoryFilter() const { return m_categoryFilter; }
  QString descriptionFilter() const { return m_descriptionFilterText; }

  void setAccount(Account *account);
  void setDateFilter(const QList<QDate> &dateFilter);
  void setDateFrom(QDate dateFrom);
  void setDateTo(QDate dateTo);
  void setSupportFilter(const QList<OpenAccountEnums::Support> &supportFilter);
  void setCategoryFilter(const QList<QUuid> &categoryFilter);
  void setDescriptionFilter(const QString &descriptionFilter);

  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;

signals:
  void accountChanged();
  void countChanged();

  void dateFilterChanged();
  void dateFromChanged();
  void dateToChanged();
  void supportFilterChanged();
  void categoryFilterChanged();
  void descriptionFilterChanged();

protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

private:
  void invalidateAccountFilter();
  void invalidateFilter();

private:
  QPointer<Account> m_account;
  QList<QDate> m_dateFilter;
  QDate m_dateFrom;
  QDate m_dateTo;
  QList<OpenAccountEnums::Support> m_supportFilter;
  QList<QUuid> m_categoryFilter;
  QString m_descriptionFilterText;
  QRegularExpression m_descriptionFilter;
};
