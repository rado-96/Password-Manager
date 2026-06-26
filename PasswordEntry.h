#ifndef PASSWORD_ENTRY_H
#define PASSWORD_ENTRY_H

#include <string>

class PasswordEntry {
private:
	std::string site;
	std::string username;
	std::string encryptedPassword;
	std::string createdAt;

public:
	PasswordEntry(
		const std::string& site,
		const std::string& username,
		const std::string& encryptedPassword,
		const std::string& createdAt
	);

	std::string getSite() const;
	std::string getUsername() const;
	std::string getEncryptedPassword() const;
	std::string getCreatedAt() const;
};

#endif

