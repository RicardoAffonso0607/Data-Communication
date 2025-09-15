#pragma once

#include <cstring> 
#include <bitset>

class Cripto {
    public:
        // The constructor now accepts a key of type char* and its size.
        Cripto(const char* encryptionKey, size_t keySize);
        ~Cripto();

        char* encrypt(const char* plainText, size_t keySize);
        char* decrypt(const char* encryptedText, size_t keySize);

        char* toBinary(const char* input, size_t keySize);
        char* toChar(const char* binaryInput, size_t binarySize); 

    private:
        char* key;
        size_t keySize;
};
