#ifndef VAULT_H
#define VAULT_H

#include <vector>
#include "PasswordEntry.h"

class Vault {
private:
	std::vector <PasswordEntry> entries;

public:
	void addEntry(const PasswordEntry& entry);
	bool removeEntry(const std::string& site);
	bool removeEntryByIndex(size_t index);
	const std::vector<PasswordEntry>& getEntries() const;
};

#endif
