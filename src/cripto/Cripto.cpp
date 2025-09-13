#include "../../include/cripto/Cripto.h"

// Construtor
Cripto::Cripto(const std::string& chaveCriptografia) {
    this->chave = chaveCriptografia;
}

// Função para criptografar
std::string Cripto::criptografar(const std::string& textoPuro) {
    if (this->chave.empty()) {
        return textoPuro;
    }

    // Declaração da string de resultado.
    std::string textoCriptografado = ""; 
    size_t chaveLen = this->chave.length();

    // Laço para realizar a criptografia.
    for (size_t i = 0; i < textoPuro.length(); ++i) {
        // Agora você pode usar o += para adicionar cada caractere um por um.
        textoCriptografado += textoPuro[i] ^ this->chave[i % chaveLen];
    }

    return textoCriptografado;
}

// Função para descriptografar (a lógica é a mesma!)
std::string Cripto::descriptografar(const std::string& textoCriptografado) {
    // Para a criptografia XOR, o processo de descriptografia é o mesmo que o de criptografia
    return criptografar(textoCriptografado);
}