#include "PasswordGenerator.h"
#include <random>

std::string PasswordGenerator::generate(size_t length) {

    const std::string chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#$%^&*()-_=+[]{}<>?";

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(0, static_cast<int>(chars.size() - 1));

    std::string password;

    for (size_t i = 0; i < length; i++) {
        password += chars[dist(gen)];
    }

    return password;
}