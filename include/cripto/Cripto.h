#pragma once

#include <cstring> 
#include <bitset>
#include <vector>

class Cripto {
    public:
        Cripto(const char* encryptionKey, size_t keySize);
        ~Cripto();

        char* encrypt(const char* plainText, size_t keySize);
        char* decrypt(const char* encryptedText, size_t keySize);

        char* toBinary(const char* input, size_t keySize);
        char* toChar(const char* binaryInput, size_t binarySize); 
        std::vector<float> generateWaveform(const char* binaryInput, size_t binarySize, float highValue = 1.0f, float lowValue = 0.0f, size_t samplesPerBit = 10);

    private:
        char* key;
        size_t keySize;
};
