#!/bin/bash

# Script para testar comunicação entre Sender e Receiver

cd "$(dirname "$0")" || exit

echo "================================"
echo "Teste de Comunicação - Sender/Receiver"
echo "================================"
echo ""

# Mata qualquer instância anterior
pkill -f "meu_programa" || true
sleep 1

# Compila se necessário
echo "🔨 Compilando projeto..."
make clean > /dev/null 2>&1
make all > /dev/null 2>&1

if [ ! -f "./build/meu_programa" ]; then
    echo "❌ Erro ao compilar. Abortando."
    exit 1
fi

echo "✅ Compilação concluída"
echo ""

# Inicia o receiver em background
echo "🟦 Iniciando Receiver em background..."
./build/meu_programa "receiver" &
RECEIVER_PID=$!
echo "   PID do Receiver: $RECEIVER_PID"
echo ""

# Aguarda um pouco para o receiver inicializar
sleep 2

# Inicia o sender
echo "🟩 Iniciando Sender..."
echo ""
echo "💡 Instruções:"
echo "   1. Digite uma mensagem no campo 'Mensagem Original'"
echo "   2. Clique em 'Enviar'"
echo "   3. No Receiver, você verá a notificação e o gráfico"
echo "   4. Clique em 'Descriptografar' para visualizar a mensagem"
echo ""

./build/meu_programa "sender"

# Aguarda um pouco e mata o receiver
sleep 1
echo ""
echo "================================"
echo "Encerrando..."
kill $RECEIVER_PID 2>/dev/null || true
echo "✅ Teste finalizado"
