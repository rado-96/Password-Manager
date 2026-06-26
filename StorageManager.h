#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>
#include "Vault.h"
#include "CryptoManager.h"

class StorageManager {
public:
	StorageManager(const CryptoManager& crypto);

	bool loadVault(Vault& vault);
	bool saveVault(const Vault& vault);

private:
	const CryptoManager& crypto;
	const std::string vaultPath = "data/vault.enc";
};

#endif
