#pragma once

#include <QDate>
#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include <QVariantMap>

#include "model_global.h"

// Filters a TransactionListModel (or another compatible source) down to
// transactions still awaiting confirmation: estimated == true and dated
// strictly before referenceDate. Built for OBValidateTransactionsPopup,
// which surfaces exactly this set for the user to confirm or postpone.
class MODEL_EXPORT EstimatedTransactionFilterProxyModel
    : public QSortFilterProxyModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int count READ count NOTIFY countChanged)
  // Filter cutoff: only estimated transactions dated strictly before this
  // pass. Defaults to QDate::currentDate() at construction, so the common
  // case ("what's still pending as of today") needs no setup; settable for
  // tests, or to ask what was/will be pending as of some other date.
  Q_PROPERTY(QDate referenceDate READ referenceDate WRITE setReferenceDate NOTIFY referenceDateChanged)

public:
  explicit EstimatedTransactionFilterProxyModel(QObject *parent = nullptr);

  int count() const { return rowCount(); }
  QDate referenceDate() const { return m_referenceDate; }
  void setReferenceDate(QDate date);

  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;

signals:
  void countChanged();
  void referenceDateChanged();

protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

private:
  void invalidateFilter();

private:
  QDate m_referenceDate;
};
