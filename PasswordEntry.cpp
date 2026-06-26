#include "PasswordEntry.h"

PasswordEntry::PasswordEntry(
	const std::string& site,
	const std::string& username,
	const std::string& encryptedPassword,
	const std::string& createdAt
)

	: site(site),
	  username(username),
	  encryptedPassword(encryptedPassword),
	  createdAt(createdAt) {}

std::string PasswordEntry::getSite() const {
	return site;
}

std::string PasswordEntry::getUsername() const {
	return username;
}

std::string PasswordEntry::getEncryptedPassword() const {
	return encryptedPassword;
}

std::string PasswordEntry::getCreatedAt() const {
	return createdAt;
}
