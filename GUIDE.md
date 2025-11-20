# 🔐 Data-Communication - Guia Rápido

## 📋 Status do Projeto

✅ **CONCLUÍDO** - Todos os problemas corrigidos e funcionando!

## 🚀 Quick Start

### Compilar:
```bash
cd /home/ricardo/Desktop/university/com_dados/Data-Communication
make clean && make all
```

### Executar (Opção 1 - Dois Terminais):

**Terminal 1 - Receiver:**
```bash
make receiver
```

**Terminal 2 - Sender:**
```bash
make sender
```

### Executar (Opção 2 - Script Automático):
```bash
./test_communication.sh
```

### Executar (Opção 3 - Em Background):
```bash
make run-both
```

---

## 🎮 Como Usar

### **No Sender:**
1. Escreva uma mensagem no campo "Mensagem Original"
2. A mensagem será criptografada automaticamente
3. Você verá o padrão binário e o gráfico da codificação
4. Clique em "Enviar" para transmitir

### **No Receiver:**
1. Aguarde a mensagem (verá um aviso quando chegar)
2. Você verá os dados binários no campo "Mensagem Recebida"
3. O gráfico de decodificação será gerado automaticamente
4. Clique em "Descriptografar" para visualizar a mensagem original

---

## 🧪 Teste Rápido

Se quiser testar sem interface gráfica (em SSH, por exemplo):

```bash
# Terminal 1:
./build/meu_programa "receiver" &
sleep 2

# Terminal 2:
./send_test_message.sh "Olá, Mundo!"
```

---

## 📝 Arquivos Importantes

- `makefile` - Configuração de build
- `src/main.cpp` - Ponto de entrada
- `src/window_manager/WindowManager.cpp` - Lógica da UI
- `src/socket/Socket.cpp` - Comunicação de rede
- `src/cripto/Cripto.cpp` - Criptografia

---

## ✨ Recursos Implementados

✅ Comunicação Sender/Receiver via TCP sockets
✅ Criptografia de mensagens
✅ Conversão para binário
✅ Visualização em gráficos (waveform)
✅ Interface gráfica com ImGui
✅ Notificações visuais
✅ Sincronização entre threads
✅ Tratamento de erros

---

## 🛠️ Troubleshooting

### Porta 4444 já em uso:
```bash
pkill -f "meu_programa"
lsof -i :4444  # Ver quem está usando a porta
```

### Erro ao compilar:
```bash
make clean
make all
```

### Receiver não conecta:
- Certifique-se de que o Receiver está rodando
- Verifique se a porta 4444 está livre
- Tente `make killall-app` para matar processos anteriores

---

## 📚 Documentação Adicional

Veja os arquivos:
- `RESUMO_CORRECOES.md` - Detalhes técnicos
- `RECEIVER_UPDATES.md` - Atualizações do receiver

---

**Desenvolvido com ❤️**
**Data: Novembro 2025**
