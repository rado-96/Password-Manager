#include "AuthManager.h"
#include <iostream>
#include <fstream>
#include <pwdbased.h>
#include <sha.h>
#include <osrng.h>
#include <hex.h>
#include <filters.h>
#include <cryptlib.h>
#include <algorithm>

using namespace CryptoPP;

bool AuthManager::authenticate() {

    if (!masterPasswordExists()) {
        std::cout << "\nNo master password found.\n";

        if (!createMasterPassword()) {
            return false;
        }
        std::cout << "Master password created.\n";
    }

    std::string password = promptPassword();
    if (!verifyMasterPassword(password)) {
        std::cout << "Authentication failed.\n";

        return false;
    }

    std::cout << "Access granted!\n";

    std::vector<unsigned char> salt = loadOrCreateSalt();
    derivedKey.resize(32);
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf;

    pbkdf.DeriveKey(
        derivedKey.data(),
        derivedKey.size(),
        0,
        reinterpret_cast<const byte*>(password.data()),
        password.size(),
        salt.data(),
        salt.size(),
        100000
    );

    std::fill(password.begin(), password.end(), 0);
    password.clear();

    return true;
}

const std::vector<unsigned char>& AuthManager::getKey() const {
    return derivedKey;
}

std::string AuthManager::hashPassword(
    const std::string& password) const {

    SHA256 hash;
    std::string digest;

    StringSource(
        password,
        true,
        new HashFilter(
            hash,
            new HexEncoder(
                new StringSink(digest)
            )
        )
    );
    
    return digest;
}

bool AuthManager::masterPasswordExists() const {
    std::ifstream file("master.hash");
    return file.good();
}

bool AuthManager::createMasterPassword() {
    std::string password;
    std::string confirm;

    std::cout << "Create a master password: ";
    std::getline(std::cin, password);

    std::cout << "Confirm master password: ";
    std::getline(std::cin, confirm);

    if (password != confirm) {
        std::cout << "Passwords do not match.\n";
        return false;
    }

    std::ofstream out("master.hash");

    if (!out)
        return false;

    out << hashPassword(password);

    std::fill(password.begin(), password.end(), 0);
    password.clear();

    std::fill(confirm.begin(), confirm.end(), 0);
    confirm.clear();

    return true;
}

bool AuthManager::verifyMasterPassword(const std::string& password) {
    std::ifstream in("master.hash");

    if (!in)
        return false;

    std::string storedHash;
    std::getline(in, storedHash);

    return storedHash == hashPassword(password);
}

std::string AuthManager::promptPassword() const {
    std::string password;
    std::cout << "Enter master password: ";
    std::getline(std::cin >> std::ws, password);
    return password;
}

std::vector<unsigned char> AuthManager::loadOrCreateSalt() const {
    std::vector<unsigned char> salt(16);

    std::ifstream in("salt.bin", std::ios::binary);
    if (in.good()) {
        in.read(reinterpret_cast<char*>(salt.data()), salt.size());
    }
    else {
        AutoSeededRandomPool rng;
        rng.GenerateBlock(salt.data(), salt.size());

        std::ofstream out("salt.bin", std::ios::binary);
        out.write(reinterpret_cast<const char*>(salt.data()), salt.size());
    }

    return salt;
}
