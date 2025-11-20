#!/bin/bash

# Verification script to test the new multi-message functionality

echo "================================================"
echo "    Teste de Múltiplas Mensagens"
echo "================================================"
echo ""

# Check if executable exists
if [ ! -f "./build/meu_programa" ]; then
    echo "❌ Executável não encontrado!"
    echo "Por favor, execute: make clean && make all"
    exit 1
fi

echo "✅ Executável encontrado"
echo ""

# Kill any existing processes
echo "🔄 Limpando processos anteriores..."
pkill -f "meu_programa" 2>/dev/null
sleep 1

echo ""
echo "================================================"
echo "    INSTRUÇÕES DE TESTE"
echo "================================================"
echo ""
echo "Abra 2 terminais:"
echo ""
echo "Terminal 1 (Receiver):"
echo "  cd /home/ricardo/Desktop/university/com_dados/Data-Communication"
echo "  ./build/meu_programa receiver"
echo ""
echo "Terminal 2 (Sender):"
echo "  cd /home/ricardo/Desktop/university/com_dados/Data-Communication"
echo "  ./build/meu_programa sender"
echo ""
echo "================================================"
echo "    COMO TESTAR"
echo "================================================"
echo ""
echo "1️⃣  No Sender:"
echo "   - Digite uma mensagem em 'Mensagem Original'"
echo "   - Clique em 'Enviar'"
echo ""
echo "2️⃣  No Receiver:"
echo "   - Verá notificação verde: '✓ Mensagem Recebida!'"
echo "   - Campo 'Mensagem Recebida' mostrará dados binários"
echo "   - Campo 'Mensagem Criptografada' mostrará o texto"
echo "   - Gráfico será gerado automaticamente"
echo ""
echo "3️⃣  Repita:"
echo "   - Digite nova mensagem no Sender"
echo "   - Clique 'Enviar' novamente"
echo "   - Receiver receberá AUTOMATICAMENTE"
echo "   - ⭐ Sem necessidade de reconectar!"
echo ""
echo "4️⃣  Para descriptografar:"
echo "   - No Receiver, clique em 'Descriptografar'"
echo "   - Mensagem original aparecerá"
echo ""
echo "================================================"
echo ""
echo "🔍 Tecnicamente:"
echo "   • Conexão permanece ABERTA entre Sender e Receiver"
echo "   • Múltiplas mensagens na mesma conexão"
echo "   • Callback atualiza tela automaticamente"
echo "   • Sem lags ou delays notáveis"
echo ""
echo "✅ Sistema pronto para teste!"
