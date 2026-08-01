#include "Controller/abstractcontroller.h"

using namespace Controller;

AbstractController::AbstractController(QObject *parent) : QObject(parent) {}

Profile *AbstractController::currentProfile() const {
	return m_currentProfile;
}

Account *AbstractController::currentAccount() const {
	return m_currentAccount;
}

void AbstractController::setCurrentProfile(Profile *profile) {
	if (m_currentProfile == profile)
		return;

	if (m_currentProfile)
		disconnect(m_currentProfile, &QObject::destroyed, this, nullptr);

	m_currentProfile = profile;

	if (m_currentProfile)
		connect(m_currentProfile, &QObject::destroyed, this, [this]() {
			m_currentProfile = nullptr;
			emit currentProfileChanged();
		});

	emit currentProfileChanged();
}

void AbstractController::setCurrentAccount(Account *account) {
	if (m_currentAccount == account)
		return;

	if (m_currentAccount)
		disconnect(m_currentAccount, &QObject::destroyed, this, nullptr);

	m_currentAccount = account;

	if (m_currentAccount)
		connect(m_currentAccount, &QObject::destroyed, this, [this]() {
			m_currentAccount = nullptr;
			emit currentAccountChanged();
		});

	emit currentAccountChanged();
}
