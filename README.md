# 🔐 Data-Communication - Projeto Completo

## 📋 Status: ✅ PRONTO PARA USO

Todos os problemas foram identificados e corrigidos.

---

## 🚀 Quick Start

### Compilar e Executar:
```bash
cd /home/ricardo/Desktop/university/com_dados/Data-Communication

# Compilar
make clean && make all

# Executar (2 terminais)
# Terminal 1:
./build/meu_programa "receiver"

# Terminal 2:
./build/meu_programa "sender"
```

---

## 🎯 Como Usar

### **Sender (Remetente):**
1. Digite uma mensagem no campo "Mensagem Original"
2. Observar a criptografia e conversão para binário automáticas
3. Ver o gráfico da forma de onda
4. Clique em "Enviar"

### **Receiver (Receptor):**
1. Aguarde a notificação "✓ Mensagem Recebida!"
2. Observe o gráfico de decodificação gerado automaticamente
3. Veja os dados binários no campo "Mensagem Recebida"
4. Clique em "Descriptografar" para ver a mensagem original

---

## 🔧 Comandos Make

```bash
make all              # Compilar tudo
make clean            # Limpar arquivos compilados
make sender           # Executar sender
make receiver         # Executar receiver
make run-both         # Executar ambos (receiver em background)
make killall-app      # Matar todos os processos
```

---

## 📚 Documentação

| Arquivo | Descrição |
|---------|-----------|
| `GUIDE.md` | Guia rápido de uso |
| `CRYPTO_FLOW.md` | Fluxo de criptografia/descriptografia |
| `CORRECAO_DESCRIPTOGRAFIA.md` | Detalhes da última correção |
| `DEBUG.md` | Dicas de troubleshooting |
| `FINAL_SUMMARY.txt` | Sumário visual |

---

## 🧪 Scripts de Teste

```bash
# Teste automático completo
./test_communication.sh

# Teste de criptografia
./test_crypto.sh

# Enviar mensagem de teste
./send_test_message.sh "sua mensagem aqui"
```

---

## ✨ Funcionalidades Implementadas

✅ Comunicação TCP bidirecional (Socket)
✅ Criptografia XOR com chave
✅ Conversão texto ↔ binário
✅ Visualização em gráficos (waveform)
✅ Interface gráfica com ImGui
✅ Notificações visuais com cores
✅ Sincronização thread-safe entre threads
✅ Descrição controlada por botão
✅ Limpeza automática de memória

---

## 🔒 Segurança

- **Chave de criptografia:** `"mykey123"` (igual em sender e receiver)
- **Algoritmo:** XOR (simétrico - encrypt == decrypt)
- **Sincronização:** Callbacks thread-safe
- **Memória:** Liberada com `delete[]`

---

## 🐛 Correção Recente

**Problema:** Descriptografia não funcionava
**Causa:** Tentava descriptografar string binária diretamente
**Solução:** Converter binário → caracteres → descriptografar

Veja `CORRECAO_DESCRIPTOGRAFIA.md` para detalhes.

---

## ✅ Checklist Final

- [x] Compilação sem erros
- [x] Sender envia mensagens
- [x] Receiver recebe mensagens
- [x] Notificações funcionam
- [x] Gráficos são gerados
- [x] Descriptografia corrigida
- [x] Sincronização thread-safe
- [x] Documentação completa

---

**🎉 Projeto Completo e Funcional!**

*Última atualização: 20 de Novembro de 2025*