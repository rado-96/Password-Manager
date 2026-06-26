#include "CryptoManager.h"

#include <cryptlib.h>
#include <aes.h>
#include <modes.h>
#include <filters.h>
#include <hex.h>
#include <osrng.h>

using namespace CryptoPP;

CryptoManager::CryptoManager(const std::vector<unsigned char>& key)
    : key(key) {
}

// Generates a random 16-byte IV
std::string CryptoManager::generateIV() const {
    AutoSeededRandomPool prng;
    byte iv[AES::BLOCKSIZE];
    prng.GenerateBlock(iv, sizeof(iv));

    return std::string(reinterpret_cast<char*>(iv), AES::BLOCKSIZE);
}

std::string CryptoManager::encrypt(const std::string& plaintext) const {
    std::string iv = generateIV();
    std::string cipher;
    std::string encoded;

    CBC_Mode<AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(
        key.data(), key.size(),
        reinterpret_cast<const byte*>(iv.data())
    );

    StringSource ss(
        plaintext,
        true,
        new StreamTransformationFilter(
            encryptor,
            new StringSink(cipher)
        )
    );

    // Store IV + ciphertext together (IV is required for decryption)
    std::string combined = iv + cipher;

    StringSource(
        combined,
        true,
        new HexEncoder(new StringSink(encoded))
    );

    return encoded;
}

std::string CryptoManager::decrypt(const std::string& ciphertext) const {
    std::string decoded;
    std::string recovered;

    StringSource(
        ciphertext,
        true,
        new HexDecoder(new StringSink(decoded))
    );

    std::string iv = decoded.substr(0, AES::BLOCKSIZE);
    std::string cipher = decoded.substr(AES::BLOCKSIZE);

    CBC_Mode<AES>::Decryption decryptor;
    decryptor.SetKeyWithIV(
        key.data(), key.size(),
        reinterpret_cast<const byte*>(iv.data())
    );

    StringSource(
        cipher,
        true,
        new StreamTransformationFilter(
            decryptor,
            new StringSink(recovered)
        )
    );

    return recovered;
}
