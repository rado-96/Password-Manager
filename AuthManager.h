#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <string>
#include <vector>

class AuthManager {
public:
	bool authenticate();
	const std::vector<unsigned char>& getKey() const;

private:
	std::vector<unsigned char> derivedKey;

	std::string promptPassword() const;
	std::vector<unsigned char> loadOrCreateSalt() const;

	std::string hashPassword(const std::string& password) const;

	bool masterPasswordExists() const;
	bool createMasterPassword();
	bool verifyMasterPassword(const std::string& password);
};

#endif
