#!/bin/bash

# Script para testar a criptografia/descriptografia

echo "╔════════════════════════════════════════════════════╗"
echo "║     Teste de Criptografia/Descriptografia          ║"
echo "╚════════════════════════════════════════════════════╝"
echo ""

cd "$(dirname "$0")" || exit

# Compilar
echo "🔨 Compilando..."
make clean > /dev/null 2>&1
make all > /dev/null 2>&1

if [ ! -f "./build/meu_programa" ]; then
    echo "❌ Erro ao compilar"
    exit 1
fi

echo "✅ Compilação OK"
echo ""

# Matar processos anteriores
pkill -f "meu_programa" || true
sleep 1

# Iniciar receiver
echo "🟦 Iniciando Receiver..."
./build/meu_programa "receiver" > /tmp/receiver_test.log 2>&1 &
RECEIVER_PID=$!
sleep 3

# Verificar se está rodando
if ! ps -p $RECEIVER_PID > /dev/null; then
    echo "❌ Erro ao iniciar Receiver"
    cat /tmp/receiver_test.log
    exit 1
fi

echo "✅ Receiver iniciado (PID: $RECEIVER_PID)"
echo ""

# Criar programa de teste para enviar mensagem
cat > /tmp/crypto_test.c << 'TESTEOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Simular criptografia simples (como o programa faz)
    const char* key = "mykey123";
    size_t keySize = strlen(key);
    
    // Mensagem de teste
    const char* message = "ABC";
    size_t msgSize = strlen(message);
    
    // 1. Encrypt (XOR)
    char* encrypted = malloc(msgSize + 1);
    for (size_t i = 0; i < msgSize; i++) {
        encrypted[i] = message[i] ^ key[i % keySize];
    }
    encrypted[msgSize] = '\0';
    
    // 2. To Binary
    char* binary = malloc(msgSize * 8 + 1);
    for (size_t i = 0; i < msgSize; i++) {
        for (int j = 7; j >= 0; j--) {
            binary[i * 8 + (7 - j)] = ((encrypted[i] >> j) & 1) ? '1' : '0';
        }
    }
    binary[msgSize * 8] = '\0';
    
    printf("Original: %s\n", message);
    printf("Binary: %s\n", binary);
    printf("Binary size: %lu\n", strlen(binary));
    
    // Enviar via socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4444);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        send(sock, binary, strlen(binary), 0);
        printf("✓ Mensagem enviada\n");
        close(sock);
    } else {
        printf("✗ Erro ao conectar\n");
    }
    
    free(encrypted);
    free(binary);
    return 0;
}
TESTEOF

gcc /tmp/crypto_test.c -o /tmp/crypto_test

echo "📤 Enviando mensagem de teste: 'ABC'"
/tmp/crypto_test

sleep 2

echo ""
echo "📋 Resultado no Receiver:"
echo "════════════════════════════"
cat /tmp/receiver_test.log

echo ""
echo "════════════════════════════"
echo ""

# Cleanup
kill $RECEIVER_PID 2>/dev/null || true

echo "✅ Teste concluído"
echo ""
echo "💡 Se você viu 'ABC' descriptografado, a criptografia está OK!"
