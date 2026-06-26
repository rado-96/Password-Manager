#include "Vault.h"

void Vault::addEntry(const PasswordEntry& entry) {
	entries.push_back(entry);
}

bool Vault::removeEntry(const std::string& site) {
	for (auto it = entries.begin(); it != entries.end(); ++it) {
		if (it->getSite() == site) {
			entries.erase(it);
			return true;
		}
	}
	return false;
}

bool Vault::removeEntryByIndex(size_t index) {
	if (index >= entries.size()) {
		return false;
	}

	entries.erase(entries.begin() + index);
	return true;
}

const std::vector<PasswordEntry>& Vault::getEntries() const {
	return entries;
}