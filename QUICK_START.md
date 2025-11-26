# Quick Start — Data-Communication Project

Esta é a forma mais rápida de começar a usar o projeto. Escolha o script correspondente ao seu sistema operacional.

## 🐧 Linux (Ubuntu, Fedora, Arch)

### Método 1: Script Automático (Recomendado)

```bash
# 1. Certifique-se que você está no diretório do projeto
cd Data-Communication

# 2. Dê permissão de execução ao script
chmod +x install-linux.sh

# 3. Execute o script
./install-linux.sh
```

O script irá:
- ✅ Detectar sua distribuição Linux
- ✅ Instalar todas as dependências (gcc, GLFW, OpenGL, etc.)
- ✅ Compilar o projeto
- ✅ Criar `config.txt` (se não existir)
- ✅ Perguntar se deseja rodar a aplicação

**Suporta:**
- Debian / Ubuntu (apt)
- Fedora / RHEL (dnf)
- Arch / Manjaro (pacman)

### Método 2: Manual

Se preferir instalar manualmente:
- Veja `INSTALL_LINUX.md` para instruções passo-a-passo
- Veja `WINDOWS_NETWORK_GUIDE.md` para configurar redes

---

## 🪟 Windows (10/11)

### Método 1: PowerShell (Recomendado)

```powershell
# 1. Abra PowerShell como Administrador
# (Clique com botão direito em PowerShell → "Run as Administrator")

# 2. Navegue até o diretório do projeto
cd C:\path\to\Data-Communication

# 3. Execute o script
.\install-windows.ps1

# Se tiver erro de "execution policy", execute primeiro:
# Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

O script irá:
- ✅ Detectar Windows version (10/11)
- ✅ Verificar se WSL2 ou MSYS2 estão instalados
- ✅ Instalar WSL2 (recomendado para Windows 11) ou MSYS2 (nativo Windows)
- ✅ Instalar dependências
- ✅ Compilar o projeto
- ✅ Opcionalmente rodar a aplicação

### Método 2: Batch Script

```cmd
# 1. Abra Command Prompt como Administrador
# (Clique com botão direito em cmd.exe → "Run as Administrator")

# 2. Navegue até o diretório do projeto
cd C:\path\to\Data-Communication

# 3. Execute o script
install-windows.bat
```

Oferece as mesmas opções que o PowerShell, com interface mais simples.

### Método 3: Manual

Se preferir instalar manualmente:
- Veja `INSTALL_WINDOWS.md` para instruções passo-a-passo
- Veja `WINDOWS_NETWORK_GUIDE.md` para configurar redes

---

## 🚀 Executar a Aplicação

Após a instalação, você terá o executável compilado. Para rodar:

### Linux
```bash
./build/meu_programa
```

### Windows
```cmd
.\build\meu_programa.exe
```
Ou clique duas vezes no arquivo `meu_programa.exe` no explorador.

---

## 🔧 Configuração (config.txt)

Antes de usar o programa entre máquinas diferentes, edite `config.txt`:

```
RECEIVER_IP=127.0.0.1
PORT=4444
```

- **Localhost testing** (mesma máquina): deixe `127.0.0.1`
- **Mesma rede local**: use IP local descoberto com `ipconfig` (Windows) ou `ifconfig` (Linux)
- **Redes diferentes**: veja `WINDOWS_NETWORK_GUIDE.md` para port forwarding ou VPN

---

## 📝 Exemplos de Uso

### Teste Local (mesma máquina)

```bash
# Terminal 1 — Receiver
./build/meu_programa

# Terminal 2 — Sender (ou segunda instância)
./build/meu_programa
```

Digite uma mensagem no Sender, clique "Send". Receiver receberá e você poderá "Decrypt".

### Teste em Rede Local

1. Edite `config.txt` no Sender com IP do Receiver (ex.: `192.168.1.100`)
2. Abra firewall no Receiver (Windows) ou permita porta 4444 no Linux
3. Inicie Receiver em uma máquina
4. Inicie Sender em outra máquina
5. Envie mensagens

Para mais detalhes, veja `WINDOWS_NETWORK_GUIDE.md`.

---

## ❓ Troubleshooting

### "Script cannot be executed" (PowerShell)
Rode primeiro:
```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### "Permission denied" (Linux)
```bash
chmod +x install-linux.sh
./install-linux.sh
```

### Build falha por dependências
- **Linux**: O script tenta instalar automaticamente. Se falhar, verifique sua distribuição em `INSTALL_LINUX.md`.
- **Windows**: Certifique-se de ter WSL2 (Windows 11) ou MSYS2 instalados. Veja `INSTALL_WINDOWS.md`.

### Conexão falha entre máquinas
Veja `WINDOWS_NETWORK_GUIDE.md` para:
- Descobrir IP local
- Abrir firewall
- Port forwarding
- VPN setup

---

## 📖 Documentação Completa

- **Linux**: `INSTALL_LINUX.md`
- **Windows**: `INSTALL_WINDOWS.md`
- **Redes**: `WINDOWS_NETWORK_GUIDE.md`
- **Configuração**: `CONFIG_GUIDE.md`

---

## ✅ Checklist Rápido

- [ ] Clonou o repositório
- [ ] Executou o script de instalação (`install-linux.sh` ou `install-windows.ps1`)
- [ ] Script compilou sem erros
- [ ] `config.txt` foi criado
- [ ] Executable existe em `./build/meu_programa`
- [ ] Consegue abrir a aplicação com duplo-clique (Windows) ou `./build/meu_programa` (Linux)

---

## 🎯 Próximos Passos

1. **Teste local** — envie mensagens na mesma máquina para familiarizar
2. **Teste LAN** — configure IP local em `config.txt` e teste entre máquinas
3. **Teste remoto** (opcional) — veja `WINDOWS_NETWORK_GUIDE.md` para port forwarding ou VPN

---

**Pronto para começar? Execute o script apropriado e aproveite! 🚀**
