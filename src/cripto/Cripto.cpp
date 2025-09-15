#include "../../include/cripto/Cripto.h"

// Constructor
Cripto::Cripto(const char* encryptionKey, size_t keySize) {
    // Allocate memory for the key and copy it.
    this->key = new char[keySize + 1];
    strcpy(this->key, encryptionKey);
    this->keySize = keySize;
}

Cripto::~Cripto() {
    delete[] this->key;
}

char* Cripto::encrypt(const char* plainText, size_t messageSize) {
    char* encryptedText = new char[messageSize + 1]; // +1 for null terminator
    if (!this->key) {
        // If the key doesn't exist, copy the plain text without encrypting.
        strncpy(encryptedText, plainText, messageSize);
        encryptedText[messageSize] = '\0';
        return encryptedText;
    }

    for (size_t i = 0; i < messageSize; ++i) {
        // Perform the XOR operation and store it in the destination buffer.
        encryptedText[i] = plainText[i] ^ this->key[i % this->keySize];
    }
    
    encryptedText[messageSize] = '\0';
    return encryptedText;
}

char* Cripto::decrypt(const char* encryptedText, size_t messageSize) {
    // For XOR encryption, the decryption process is the same as encryption.
    // Just call the same function.
    return encrypt(encryptedText, messageSize);
}

char* Cripto::toBinary(const char* input, size_t inputSize) {
    // Each character will be represented by 8 bits.
    size_t binarySize = inputSize * 8;
    char* binaryRepresentation = new char[binarySize + 1]; // +1 for null terminator
    binaryRepresentation[binarySize] = '\0'; // Null-terminate the string

    for (size_t i = 0; i < inputSize; ++i) {
        std::bitset<8> bits(input[i]);
        std::string bitString = bits.to_string(); // Convert to string of '0's and '1's
        // Copy the bit string into the binary representation
        for (size_t j = 0; j < 8; ++j) {
            binaryRepresentation[i * 8 + j] = bitString[j];
        }
    }

    return binaryRepresentation;
}

char* Cripto::toChar(const char* binaryInput, size_t binarySize) {
    // Each 8 bits represent one character.
    size_t charSize = binarySize / 8;
    char* charRepresentation = new char[charSize + 1]; // +1 for null terminator
    charRepresentation[charSize] = '\0'; // Null-terminate the string

    for (size_t i = 0; i < charSize; ++i) {
        std::bitset<8> bits(std::string(binaryInput + i * 8, 8));
        charRepresentation[i] = static_cast<char>(bits.to_ulong());
    }

    return charRepresentation;
}
