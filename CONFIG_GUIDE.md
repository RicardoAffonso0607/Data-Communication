# 📋 Configuração - Guia de Uso

## Visão Geral

O sistema agora usa um arquivo **`config.txt`** para configurar IP e porta, eliminando a necessidade de recompilar para mudar essas configurações.

## Como Usar

### 1. Arquivo `config.txt`

Localizado na raiz do projeto:

```
# Cripto-Com Configuration File
RECEIVER_IP=127.0.0.1
PORT=4444
```

### 2. Modificar para Máquinas Diferentes

#### **Cenário: Mesma rede local**

Se receiver está em `192.168.1.100`:

```
RECEIVER_IP=192.168.1.100
PORT=4444
```

#### **Cenário: Internet (com port forwarding)**

Se receiver tem IP público `200.150.100.50`:

```
RECEIVER_IP=200.150.100.50
PORT=4444
```

#### **Cenário: Máquina local (desenvolvimento)**

```
RECEIVER_IP=127.0.0.1
PORT=4444
```

### 3. Executar (sem recompilar!)

```bash
# Edite config.txt com o IP desejado
nano config.txt

# Apenas execute normalmente
./build/meu_programa receiver
./build/meu_programa sender
```

## Como Descobrir o IP do Receiver

**Linux/macOS:**
```bash
# Ver todos os IPs locais
ip addr show
# ou
ifconfig

# IP mais simples
hostname -I
```

**Windows:**
```bash
ipconfig
```

## Formatos Suportados

```
# Comentários começam com #
# Linhas em branco são ignoradas

# Espaços são automaticamente removidos
RECEIVER_IP = 192.168.1.100
PORT = 4444

# Também funciona sem espaços
RECEIVER_IP=192.168.1.100
PORT=4444
```

## Comportamento Padrão

Se `config.txt` não existir ou estiver vazio:

- `RECEIVER_IP` = `127.0.0.1`
- `PORT` = `4444`

## Exemplo de Fluxo Completo

**Máquina A (Receiver) - IP: 192.168.1.100**

```
# config.txt
RECEIVER_IP=127.0.0.1
PORT=4444

$ ./build/meu_programa receiver
✅ Config loaded from 'config.txt'
✅ Receiver server running on port 4444...
```

**Máquina B (Sender) - IP: 192.168.1.200**

```
# config.txt
RECEIVER_IP=192.168.1.100
PORT=4444

$ ./build/meu_programa sender
✅ Config loaded from 'config.txt'
✅ Conectado ao Receiver em 192.168.1.100:4444
```

## Troubleshooting

| Problema | Solução |
|---|---|
| "⚠️ Config file 'config.txt' not found" | Crie `config.txt` na raiz do projeto |
| "Connection refused" | Verifique se RECEIVER_IP está correto |
| "Network unreachable" | Teste com `ping RECEIVER_IP` |
| "Permission denied" | Libere a porta no firewall |

## Arquivos Relacionados

- `config.txt` - Arquivo de configuração
- `include/config/Config.h` - Classe que lê a configuração
- `src/window_manager/WindowManager.cpp` - Usa a configuração
- `include/socket/Socket.h` - Socket aceita IP e porta como parâmetros

---

**Resumo:** Agora é simples testar em máquinas diferentes! Basta editar `config.txt` e executar. ✨
