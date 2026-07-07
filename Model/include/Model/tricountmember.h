#pragma once

#include "metadata.h"
#include "model_global.h"
#include <QJsonObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QUuid>

/**
 * @brief A participant of a Tricount.
 *
 * A member is either linked to an OpenBudget Profile (profileId set) or a
 * plain guest (profileId null). When linked, an accountId may be provided so
 * that the transactions generated from the Tricount are attached to the
 * member's real account.
 */
class MODEL_EXPORT TricountMember : public QObject, public MetaData {
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QUuid profileId READ profileId WRITE setProfileId NOTIFY profileIdChanged)
    Q_PROPERTY(QUuid accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)
    Q_PROPERTY(bool guest READ isGuest NOTIFY profileIdChanged)

public:
    explicit TricountMember(QObject* parent = nullptr);
    explicit TricountMember(const QJsonObject& json, QObject* parent = nullptr);
    ~TricountMember() override = default;

    TricountMember(const TricountMember&) = delete;
    TricountMember& operator=(const TricountMember&) = delete;
    TricountMember(TricountMember&&) = delete;
    TricountMember& operator=(TricountMember&&) = delete;

    // Accessors
    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] QString name() const { return metaData<QString>(Key::name); }
    [[nodiscard]] QUuid profileId() const { return metaData<QUuid>(Key::profileId); }
    [[nodiscard]] QUuid accountId() const { return metaData<QUuid>(Key::accountId); }

    /** True when the member is not linked to an OpenBudget Profile. */
    [[nodiscard]] bool isGuest() const { return profileId().isNull(); }
    /** True when id is non-null and a name is set. */
    [[nodiscard]] bool isValid() const;

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

public slots:
    void setId(QUuid v);
    void setName(QString v);
    void setProfileId(QUuid v);
    void setAccountId(QUuid v);

signals:
    void idChanged();
    void nameChanged();
    void profileIdChanged();
    void accountIdChanged();
    void changed();

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto name = "name";
        static constexpr auto profileId = "profileId";
        static constexpr auto accountId = "accountId";
    };
};

using TricountMemberPtr = QSharedPointer<TricountMember>;
Q_DECLARE_METATYPE(TricountMemberPtr)
