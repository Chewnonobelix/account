#pragma once

#include <QObject>
#include <QPointer>

#include "Model/Business/account.h"
#include "Model/Business/profile.h"
#include "controller_global.h"

namespace Controller {

// Base class for every domain controller (Account, Transaction, Category, ...).
//
// Holds the profile and account currently selected in the application, shared
// by every concrete controller so they all read/write against the same
// selection instead of each one tracking it independently.
class CONTROLLER_EXPORT AbstractController : public QObject {
	Q_OBJECT

	Q_PROPERTY(Profile *currentProfile READ currentProfile WRITE setCurrentProfile NOTIFY currentProfileChanged)
	Q_PROPERTY(Account *currentAccount READ currentAccount WRITE setCurrentAccount NOTIFY currentAccountChanged)

public:
	explicit AbstractController(QObject *parent = nullptr);
	~AbstractController() override = default;

	[[nodiscard]] Profile *currentProfile() const;
	[[nodiscard]] Account *currentAccount() const;

public slots:
	void setCurrentProfile(Profile *profile);
	void setCurrentAccount(Account *account);

signals:
	void currentProfileChanged();
	void currentAccountChanged();

private:
	QPointer<Profile> m_currentProfile;
	QPointer<Account> m_currentAccount;
};

} // namespace Controller
