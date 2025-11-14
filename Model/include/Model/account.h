#pragma once

#include <QObject>
#include <QString>
#include <QDate>
#include <QUuid>
#include <QJsonObject>
#include <QJsonValue>
#include "model_global.h"
#include "metadata.h"

class MODEL_EXPORT Account : public QObject, public MetaData {
    Q_OBJECT

    Q_PROPERTY(QString number READ number WRITE setNumber NOTIFY numberChanged)
    Q_PROPERTY(QString bank READ bank WRITE setBank NOTIFY bankChanged)
    Q_PROPERTY(double interest READ interest WRITE setInterest NOTIFY interestChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDate opening READ opening WRITE setOpening NOTIFY openingChanged)

public:
    explicit Account(QObject *parent = nullptr);
    Account(const QJsonObject &json, QObject *parent = nullptr);
    Account(const Account &other);
    Account &operator=(const Account &other);
    Account(Account &&other) noexcept;
    Account &operator=(Account &&other) noexcept;
    ~Account() override = default;

    QString number() const { return metaData<QString>(Key::number); }
    void setNumber(QString v) { if (metaData<QString>(Key::number) == v) return; setMetadata(Key::number, v); emit numberChanged(); emit changed(); }

    QString bank() const { return metaData<QString>(Key::bank); }
    void setBank(QString v) { if (metaData<QString>(Key::bank) == v) return; setMetadata(Key::bank, v); emit bankChanged(); emit changed(); }

    double interest() const { return metaData<double>(Key::interest); }
    void setInterest(double v) { if (qFuzzyCompare(metaData<double>(Key::interest), v)) return; setMetadata(Key::interest, v); emit interestChanged(); emit changed(); }

    QString description() const { return metaData<QString>(Key::description); }
    void setDescription(QString v) { if (metaData<QString>(Key::description) == v) return; setMetadata(Key::description, v); emit descriptionChanged(); emit changed(); }

    QDate opening() const { return metaData<QDate>(Key::opening); }
    void setOpening(QDate v) { if (metaData<QDate>(Key::opening) == v) return; setMetadata(Key::opening, v); emit openingChanged(); emit changed(); }

    QUuid id() const { return metaData<QUuid>(Key::id); }
    void setId(QUuid v) { if (metaData<QUuid>(Key::id) == v) return; setMetadata(Key::id, v); emit changed(); }

    QJsonObject toJson() const;

signals:
    void changed();
    void numberChanged();
    void bankChanged();
    void interestChanged();
    void descriptionChanged();
    void openingChanged();

private:
    struct Key {
        inline static constexpr const char *number = "number";
        inline static constexpr const char *bank = "bank";
        inline static constexpr const char *interest = "interest";
        inline static constexpr const char *description = "description";
        inline static constexpr const char *opening = "opening";
        inline static constexpr const char *id = "id";
    };
};
