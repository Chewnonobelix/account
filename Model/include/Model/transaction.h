#pragma once

#include <QObject>
#include <QString>
#include <QDate>
#include <QUuid>
#include <QJsonObject>
#include <QEnableSharedFromThis>
#include "metadata.h"
#include "enums.h"

class Total; // forward declaration

// Transaction model for monetary operations.
// Base classes: QObject + MetaData + QEnableSharedFromThis<Transaction>.
class Transaction : public QObject, public MetaData, public QEnableSharedFromThis<Transaction>
{
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(OpenAccountEnums::Support support READ support WRITE setSupport NOTIFY supportChanged)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(OpenAccountEnums::Movement movement READ movement WRITE setMovement NOTIFY movementChanged)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setIsVisible NOTIFY isVisibleChanged)
    Q_PROPERTY(QUuid accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)

public:
    explicit Transaction(QObject* parent = nullptr);
    explicit Transaction(const QJsonObject& json, QObject* parent = nullptr);

    // Deleted copy operations (QObject-derived)
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    // Accessors
    QUuid id() const { return metaData<QUuid>(Key::Id); }
    double value() const { return metaData<double>(Key::Value); }
    QString description() const { return metaData<QString>(Key::Description); }
    OpenAccountEnums::Support support() const { return metaData<OpenAccountEnums::Support>(Key::Support); }
    QDate date() const { return metaData<QDate>(Key::Date); }
    OpenAccountEnums::Movement movement() const { return metaData<OpenAccountEnums::Movement>(Key::Movement); }
    bool isVisible() const { return metaData<bool>(Key::IsVisible); }
    QUuid accountId() const { return metaData<QUuid>(Key::AccountId); }
    QString category() const { return metaData<QString>(Key::Category); }

public slots:
    // Setters (by copy)
    void setId(QUuid v);
    void setValue(double v);
    void setDescription(QString v);
    void setSupport(OpenAccountEnums::Support v);
    void setDate(QDate v);
    void setMovement(OpenAccountEnums::Movement v);
    void setIsVisible(bool v);
    void setAccountId(QUuid v);
    void setCategory(QString v);

signals:
    void idChanged();
    void valueChanged();
    void descriptionChanged();
    void supportChanged();
    void dateChanged();
    void movementChanged();
    void isVisibleChanged();
    void accountIdChanged();
    void categoryChanged();

public:
    QJsonObject toJson() const;

    // Operators building/merging Totals
    friend Total operator+(const Transaction& a, const Transaction& b);
    friend Total operator+(const Transaction& a, const Total& b);

private:
    struct Key {
        static constexpr auto Id          = "id";
        static constexpr auto Value       = "value";
        static constexpr auto Description = "description";
        static constexpr auto Support     = "support";
        static constexpr auto Date        = "date";
        static constexpr auto Movement    = "movement";
        static constexpr auto IsVisible   = "isVisible";
        static constexpr auto AccountId   = "accountId";
        static constexpr auto Category    = "category";
    };

    // Helper to emit changed signals when metadata actually changes
    template<typename T, typename Signal>
    void assignIfChanged(const char* key, T v, Signal signal) {
        if (metaData<T>(key) == v)
            return;
        setMetadata(key, v);
        emit (this->*signal)();
    }
};
