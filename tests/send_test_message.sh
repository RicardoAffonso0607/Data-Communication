#!/bin/bash

# Script para testar envio de mensagem

if [ $# -eq 0 ]; then
    MESSAGE="Olá, isto é uma mensagem de teste!"
else
    MESSAGE="$*"
fi

# Cria um programa C simples para enviar a mensagem
cat > /tmp/test_sender.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4444);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Error connecting to server");
        close(sock);
        return 1;
    }

    const char* message = argv[1];
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Error sending data");
        close(sock);
        return 1;
    }

    printf("Message sent: %s\n", message);
    close(sock);
    return 0;
}
EOF

gcc /tmp/test_sender.c -o /tmp/test_sender
/tmp/test_sender "$MESSAGE"
