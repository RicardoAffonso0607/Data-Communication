#pragma once

#include <string>
#include <vector>

class Cripto {
private:
    std::string chave;

public:
    // Construtor que recebe a chave como string
    Cripto(const std::string& chaveCriptografia);

    // Função para criptografar uma string
    std::string criptografar(const std::string& textoPuro);

    // Função para descriptografar uma string
    std::string descriptografar(const std::string& textoCriptografado);
};
