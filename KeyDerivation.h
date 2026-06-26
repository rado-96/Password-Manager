#ifndef KEY_DERIVATION_H
#define KEY_DERIVATION_H

#include <string>
#include <vector>

std::vector <unsigned char> deriveKey(
	const std::string& masterPassword,
	const std::vector <unsigned char>& salt
);

std::vector <unsigned char> generateSalt();
std::vector <unsigned char> loadOrCreateSalt();

#endif

