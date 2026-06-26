#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>
#include "AuthManager.h"
#include "CryptoManager.h"
#include "StorageManager.h"
#include "Vault.h"
#include "PasswordEntry.h"
#include "PasswordGenerator.h"

// Header
void printHeader() {
    std::cout << "============================================\n";
    std::cout << "               PASSWORD MANAGER             \n";
    std::cout << "============================================\n";
    std::cout << "         Author: Radoslav Doychinov         \n";
    std::cout << "============================================\n\n";
}

// Password Masking
std::string maskPassword(const std::string& password) {
    return std::string(password.length(), '*');
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void secureClear(std::string& data) {
    std::fill(data.begin(), data.end(), 0);
    data.clear();
}

// Automatic Date Generation
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%d-%m-%Y");

    return oss.str();
}

int main() {
    AuthManager auth;

    printHeader();

    if (!auth.authenticate()) {
        return 1;
    }

    CryptoManager crypto(auth.getKey());
    StorageManager storage(crypto);
    Vault vault;

    if (storage.loadVault(vault)) {
        std::cout << "Vault loaded. Entries: "
            << vault.getEntries().size()
            << "\n";
    }
    else {
        std::cout << "Starting with an empty vault.\n";
    }

    int choice = 0;

    do {

        std::cout << "\n===== PASSWORD MANAGER =====\n\n";
        std::cout << "1. Add Password\n";
        std::cout << "2. View Entries\n";
        std::cout << "3. Reveal Password\n";
        std::cout << "4. Delete Entries\n";
        std::cout << "5. Exit\n";
        std::cout << "==============================\n";
        std::cout << "Choice: ";

        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {

        // Adding entries to the vault.
        case 1:
        {

            std::string site;
            std::string username;
            std::string password;
            std::string createdAt;

            std::cout << "Site: ";
            std::getline(std::cin, site);
            if (site.empty()) {
                std::cout << "Error: Field cannot be left empty!\n";
                break;
            }

            std::cout << "Username: ";
            std::getline(std::cin, username);

            int passwordChoice;
            std::cout << "\n1. Enter my own password\n";
            std::cout << "2. Generate a secure password\n";
            std::cout << "Choice: ";

            std::cin >> passwordChoice;
            clearInputBuffer();

            if (passwordChoice == 1) {
                std::cout << "Please enter your password: ";
                std::getline(std::cin, password);
            }
            else if (passwordChoice == 2) {
                int length;

                std::cout << "Password length: ";
                std::cin >> length;
                clearInputBuffer();

                password = PasswordGenerator::generate(length);

                std::cout << "\n--- Generated Password: ---\n";
                std::cout << password << "\n";
            }
            else {
                std::cout << "Invalid choice.\n";
                break;
            }

            createdAt = getCurrentDate();

            std::string encrypted =
                crypto.encrypt(password);

            secureClear(password);

            vault.addEntry(
                PasswordEntry(
                    site,
                    username,
                    encrypted,
                    createdAt
                )
            );

            storage.saveVault(vault);

            std::cout << "Password saved.\n";

            break;
        }

        // Retrieving entries.
        case 2:
        {

            const auto& entries = vault.getEntries();

            if (entries.empty()) {
                std::cout << "No stored passwords.\n";
                break;
            }

            std::cout << "\n--- Stored Entries ---\n\n";

            for (size_t i = 0; i < entries.size(); ++i) {
                std::string decrypted = crypto.decrypt(
                    entries[i].getEncryptedPassword()
                );

                std::cout << i + 1 << ". " << entries[i].getSite() << "\n";

                std::cout << "  Username: " << entries[i].getUsername() << "\n";

                std::cout << "  Password: " << maskPassword(decrypted) << "\n";

                std::cout << "  Created: " << entries[i].getCreatedAt() << "\n\n";

                secureClear(decrypted);
            }

            break;
        }

        // Reviewing passwords.
        case 3:
        {

            const auto& entries = vault.getEntries();

            if (entries.empty()) {
                std::cout << "No stored passwords.\n";
                break;
            }

            // Show list of entries
            std::cout << "\n--- Select entry to reveal ---\n\n";

            for (size_t i = 0; i < entries.size(); ++i) {
                std::cout << i + 1 << ". "
                    << entries[i].getSite()
                    << " (" << entries[i].getUsername() << ")\n";
            }

            int index;
            std::cout << "\nEnter entry number: ";
            std::cin >> index;
            clearInputBuffer();

            if (index < 1 || index > entries.size()) {
                std::cout << "Invalid option selected.\n";
                break;
            }

            const auto& entry = entries[index - 1];

            std::string decrypted = crypto.decrypt(entry.getEncryptedPassword());
            std::cout << "\n--- REVEALED PASSWORD ---\n";
            std::cout << "\nPassword: " << decrypted << "\n";

            secureClear(decrypted);

            break;
        }

        // Delete entries.
        case 4:
        {

            const auto& entries = vault.getEntries();

            if (entries.empty()) {
                std::cout << "No entries to delete.\n";
                break;
            }

            std::cout << "\n--- Delete entry ---\n\n";

            for (size_t i = 0; i < entries.size(); ++i) {
                std::cout << i + 1 << ". "
                    << entries[i].getSite()
                    << " (" << entries[i].getUsername() << ")\n";
            }

            int index;
            std::cout << "\nEnter entry number you with to delete: ";
            std::cin >> index;
            clearInputBuffer();

            if (index < 1 || index > entries.size()) {
                std::cout << "Invalid option selected.\n";
                break;
            }

            if (vault.removeEntryByIndex(index - 1)) {
                storage.saveVault(vault);
                std::cout << "Entry deleted successfully!\n";
            }

            break;
        }

        case 5:
            storage.saveVault(vault);
            std::cout << "Goodbye.\n";
            break;

        default:
            std::cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    return 0;
}