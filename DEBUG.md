# 🐛 Debug & Troubleshooting

## 📊 Verificar Estado do Sistema

### Ver processos rodando:
```bash
ps aux | grep meu_programa
```

### Ver portas em uso:
```bash
lsof -i :4444  # Verificar porta 4444
netstat -ln | grep 4444
```

### Matar todos os processos:
```bash
make killall-app
# ou
pkill -f "meu_programa"
```

---

## 🔍 Verificar Compilação

### Verificar se tem erros:
```bash
make clean
make all 2>&1 | grep error
```

### Ver avisos (warnings):
```bash
make all 2>&1 | grep warning
```

### Recompilar tudo do zero:
```bash
make clean
rm -rf ./build
make all
```

---

## 🧪 Testes de Comunicação

### Teste 1: Verificar se receiver está ouvindo
```bash
./build/meu_programa "receiver" &
sleep 2
echo "Teste" | nc localhost 4444
```

### Teste 2: Múltiplas mensagens (Novo - Conexão Contínua)
```bash
# Terminal 1: Inicia receiver
./build/meu_programa "receiver"

# Terminal 2: Inicia sender
./build/meu_programa "sender"

# Agora o sender pode enviar múltiplas mensagens sem desconectar!
# Cada mensagem atualiza automaticamente a tela do receiver
```

### Teste 3: Usar script de teste
```bash
./build/meu_programa "receiver" > /tmp/receiver.log 2>&1 &
sleep 2
./send_test_message.sh "Mensagem de teste"
cat /tmp/receiver.log
```

### Teste 4: Teste manual completo
```bash
# Terminal 1
timeout 30 ./build/meu_programa "receiver"

# Terminal 2 (após receiver começar)
timeout 30 ./build/meu_programa "sender"
```

---

## 📝 Logs e Mensagens

### Ver logs do receiver:
```bash
./build/meu_programa "receiver" 2>&1 | tee /tmp/receiver.log
```

### Ver logs do sender:
```bash
./build/meu_programa "sender" 2>&1 | tee /tmp/sender.log
```

### Analisar logs:
```bash
grep "Mensagem" /tmp/receiver.log
grep "Enviada\|Erro" /tmp/sender.log
```

---

## 🔧 Problemas Comuns

### Problema: "Connection refused"
```
Solução:
1. Verifique se receiver está rodando
2. Verifique se porta 4444 está livre: lsof -i :4444
3. Tente: make killall-app && sleep 2 && make receiver
```

### Problema: "Address already in use"
```
Solução:
1. Matar processos: pkill -f "meu_programa"
2. Aguardar 30 segundos (TIME_WAIT)
3. Tentar novamente
```

### Problema: "Segmentation fault"
```
Solução:
1. Recompilar: make clean && make all
2. Usar debugger: gdb ./build/meu_programa
```

### Problema: Mensagem recebida mas não aparece na UI
```
Solução:
1. Verificar callbacks: grep -n "setMessageCallback" src/window_manager/WindowManager.cpp
2. Verificar sincronização: grep -n "message_received" src/window_manager/WindowManager.cpp
3. Recompilar com debug: make clean && make all
```

---

## 🎯 Verificar Funcionalidades

### Checklist de Teste:

- [ ] Compilação sem erros: `make all` retorna OK
- [ ] Receiver inicia: `./build/meu_programa "receiver"`
- [ ] Sender conecta: `./build/meu_programa "sender"`
- [ ] Mensagem é enviada e recebida
- [ ] Notificação aparece no receiver
- [ ] Gráfico é gerado automaticamente
- [ ] Botão "Descriptografar" fica ativo
- [ ] Mensagem descriptografada aparece
- [ ] Sem crashes ou segmentation faults

---

## 🔬 Debug com Debugger

### Compilar com símbolos de debug:
```bash
# O makefile já tem -g por padrão
make clean && make all
```

### Executar com gdb:
```bash
gdb ./build/meu_programa
(gdb) run "receiver"
```

### Breakpoints úteis:
```gdb
break WindowManager::createReceiverWindow
break Socket::run_receiver_server
break Socket::setMessageCallback
run "receiver"
```

---

## 📊 Verificar Estrutura

### Arquivos importantes:
```bash
ls -la src/
ls -la include/
ls -la src/socket/
ls -la src/window_manager/
ls -la src/cripto/
```

### Incluir headers:
```bash
grep -n "#include" src/window_manager/WindowManager.cpp
```

### Verificar símbolos compilados:
```bash
nm ./build/./src/socket/Socket.o | grep setMessageCallback
```

---

## 🚀 Performance

### Ver tempo de compilação:
```bash
time make clean && make all
```

### Ver tamanho do binário:
```bash
ls -lh ./build/meu_programa
```

### Ver uso de memória:
```bash
./build/meu_programa "receiver" &
sleep 2
ps aux | grep meu_programa
```

---

## 📚 Recursos

### Man pages úteis:
```bash
man socket
man connect
man bind
man listen
man accept
man send
man recv
```

### Ver configuração:
```bash
pkg-config --cflags --libs glfw3
pkg-config --cflags --libs glu
```

---

**Dica**: Se tudo mais falhar, tente:
```bash
make clean
rm -rf build/
make all
pkill -f meu_programa
./test_communication.sh
```

---

*Última atualização: 20 de Novembro de 2025*
