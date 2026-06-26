#include "StorageManager.h"
#include <fstream>
#include "json.hpp"
#include <iostream>
#include <filesystem>

using json = nlohmann::json;

StorageManager::StorageManager(const CryptoManager& crypto)
	: crypto(crypto) {}

bool StorageManager::saveVault(const Vault& vault) {
    std::filesystem::create_directories("data");

    json j;

    for (const auto& entry : vault.getEntries()) {
        j["entries"].push_back({
            {"site", entry.getSite()},
            {"username", entry.getUsername()},
            {"password", entry.getEncryptedPassword()},
            {"created", entry.getCreatedAt()}
            });
    }

    std::string plaintext = j.dump(4);
    std::string encrypted = crypto.encrypt(plaintext);

    std::ofstream out(vaultPath, std::ios::binary);
    if (!out) {
        std::cout << "FAILED TO WRITE VAULT FILE\n";
        return false;
    }

    out << encrypted;
    return true;
}

bool StorageManager::loadVault(Vault& vault) {
    vault = Vault(); // Prevents duplication of the entries.

    std::ifstream in(vaultPath, std::ios::binary);
    if (!in) {
        std::cout << "No vault file found.\n";
        return false;
    }

    std::string encrypted(
        (std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>()
    );

    std::string plaintext = crypto.decrypt(encrypted);

    if (plaintext.empty()) {
        std::cout << "Decryption failed or vault is empty.\n";
        return false;
    }

    json j;

    try {
        j = json::parse(plaintext);
    }
    catch (const std::exception& e) {
        std::cout << "JSON parse error: " << e.what() << std::endl;
        return false;
    }

    if (!j.contains("entries")) {
        std::cout << "No 'entries' key found in vault.\n";
        return false;
    }

    for (const auto& item : j["entries"]) {
        vault.addEntry(PasswordEntry(
            item["site"],
            item["username"],
            item["password"],
            item["created"]
        ));
    }

    return true;
}