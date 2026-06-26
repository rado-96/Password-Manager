#ifndef CRYPTO_MANAGER_H 
#define CRYPTO_MANAGER_H

#include <string>
#include <vector>

class CryptoManager {
public:
	CryptoManager(const std::vector<unsigned char>& key);

	std::string encrypt(const std::string& plaintext) const;
	std::string decrypt(const std::string& ciphertext) const;

private:
	std::vector<unsigned char> key;

	std::string generateIV() const;
};

#endif 

