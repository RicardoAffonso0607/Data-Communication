#include <iostream>
#include <cstring>
#include "include/cripto/Cripto.h"

int main() {
    const char* key = "mykey123";
    size_t keyLength = strlen(key);
    Cripto cripto(key, keyLength);

    // Test message
    const char* original = "teste com mais de uma frase aqui";
    size_t orig_len = strlen(original);
    
    std::cout << "Original: '" << original << "'" << std::endl;
    std::cout << "Original length: " << orig_len << std::endl;
    
    // Step 1: Encrypt
    char* encrypted = cripto.encrypt(original, orig_len);
    // ⚠️ IMPORTANT: Don't use strlen() on encrypted data!
    // It may contain null bytes. Use the known size instead.
    std::cout << "Encrypted length (actual): " << orig_len << std::endl;
    std::cout << "Encrypted length (strlen WRONG): " << strlen(encrypted) << std::endl;
    
    // Step 2: To binary
    char* binary = cripto.toBinary(encrypted, orig_len);  // ← Pass correct size!
    std::cout << "Binary length: " << strlen(binary) << std::endl;
    std::cout << "Expected binary length: " << (orig_len * 8) << std::endl;
    std::cout << "Binary (first 100 chars): " << std::string(binary, 100) << std::endl;
    
    // Step 3: To char
    char* back_to_char = cripto.toChar(binary, strlen(binary));  // Use strlen since binary is text
    std::cout << "Back to char length: " << strlen(back_to_char) << std::endl;
    
    // Step 4: Decrypt
    char* decrypted = cripto.decrypt(back_to_char, orig_len);  // ← Pass correct size!
    std::cout << "Decrypted: '" << decrypted << "'" << std::endl;
    std::cout << "Decrypted length: " << strlen(decrypted) << std::endl;
    
    // Cleanup
    delete[] encrypted;
    delete[] binary;
    delete[] back_to_char;
    delete[] decrypted;
    
    return 0;
}
