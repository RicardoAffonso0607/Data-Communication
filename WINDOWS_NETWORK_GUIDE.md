# Guia — Data-Communication em Redes Diferentes (Windows)

Este documento explica como fazer o Sender (em uma rede/máquina) se comunicar com o Receiver (em outra rede/máquina) usando a aplicação Data-Communication em Windows.

Cenários cobertos:
1. **Mesma LAN local** — duas máquinas na mesma rede Wi-Fi ou cabeada (simplest)
2. **Redes diferentes via Internet pública** — máquinas em redes diferentes (requer port forwarding ou VPN)
3. **Teste com WSL em Windows** — sender em WSL, receiver em nativa Windows (ou vice-versa)

---

## Cenário 1: Mesma LAN Local (Recomendado para teste)

Neste cenário, as duas máquinas estão na mesma rede local (mesma Wi-Fi ou switch).

### Pré-requisitos
- Ambas máquinas com Windows e o projeto compilado (use WSL ou MSYS2 — ver guia anterior).
- Conectadas à mesma rede (Wi-Fi ou ethernet).
- Firewall do Windows com porta 4444 aberta (veremos como abrir).

### Passos

**Passo 1: Descobrir o IP local da máquina Receiver**

Na máquina que vai rodar o Receiver, abra Command Prompt (cmd) ou PowerShell:
```powershell
ipconfig
```
Procure por "IPv4 Address" sob a seção de rede ativa. Exemplo de saída:
```
Ethernet adapter Ethernet:
   IPv4 Address. . . . . . . . . : 192.168.1.100
   Subnet Mask . . . . . . . . . : 255.255.255.0
```
Anote o IP (ex.: `192.168.1.100`).

**Passo 2: Abrir porta 4444 no Firewall do Windows (Receiver)**

A máquina Receiver precisa permitir conexões entrantes na porta 4444. Abra PowerShell como Administrador e execute:
```powershell
New-NetFirewallRule -DisplayName "Data-Communication" -Direction Inbound -LocalPort 4444 -Protocol TCP -Action Allow
```
Para verificar:
```powershell
Get-NetFirewallRule -DisplayName "Data-Communication"
```

Se depois quiser remover a regra:
```powershell
Remove-NetFirewallRule -DisplayName "Data-Communication"
```

**Passo 3: Editar `config.txt` na máquina Sender**

Na máquina Sender, no diretório do projeto, abra `config.txt` e altere:
```
RECEIVER_IP=192.168.1.100
PORT=4444
```
(Substitua `192.168.1.100` pelo IP real obtido no Passo 1.)

**Passo 4: Iniciar o Receiver**

Na máquina Receiver, abra um PowerShell (ou WSL), vá até o diretório do projeto e rode:
```bash
./build/meu_programa
```
Você verá a aplicação abrir com a interface de Receiver. Ela vai exibir "⏳ Waiting for message..." — isso é normal.

**Passo 5: Iniciar o Sender**

Na máquina Sender (depois que Receiver estiver rodando):
```bash
./build/meu_programa
```
Você verá a aplicação abrir. Na interface Sender:
- Digite uma mensagem no campo "Original Message".
- Observe que Encrypted, Binary, NRZ e RZ atualizam em tempo real.
- Clique "Send".

Se a conexão foi bem-sucedida, você verá "✓ Message Received!" no Receiver.

**Passo 6: Descriptografar no Receiver**

No Receiver, após receber a mensagem, clique o botão "Decrypt" para ver a mensagem original descriptografada.

### Troubleshooting — Mesma LAN

| Problema | Causa Provável | Solução |
|----------|---|---|
| "Failed to connect with Receiver" | IP incorreto ou porta bloqueada | Verifique IP com `ipconfig`, abra firewall (Passo 2) |
| Receiver não recebe mensagem | Firewall está bloqueando | Confirme regra de firewall com `Get-NetFirewallRule` |
| Conexão falha após alguns segundos | Receiver não iniciado | Inicie Receiver *antes* de clicar Send no Sender |
| IP muda ao reiniciar | Roteador atribuindo IP dinâmico | Use reserva DHCP no roteador ou IP estático no Windows |

---

## Cenário 2: Redes Diferentes via Internet Pública

Neste cenário, Sender está em uma rede (ex.: Casa) e Receiver está em outra (ex.: Escritório). Ambas acessam a Internet via roteadores com IPs públicos diferentes.

**Aviso importante:** Expor aplicações na Internet pública requer cuidado com segurança. Este guia assume uso educacional/teste controlado. Para produção, considere VPN ou firewall configurável.

### Opção A: Port Forwarding (mais direto, mas requer acesso ao roteador)

**Pré-requisitos:**
- Acesso ao painel do roteador da máquina Receiver.
- Saber o IP público do roteador Receiver.
- Rede relativamente estável (IPs não mudam frequentemente).

**Passos:**

1. **Descobrir o IP local e IP público do Receiver**

   Na máquina Receiver, obtenha o IP local:
   ```powershell
   ipconfig
   ```
   Exemplo: `192.168.1.100`

   Para obter o IP público, acesse um site como https://www.whatismyipaddress.com ou rode:
   ```powershell
   # via PowerShell
   (Invoke-WebRequest -Uri "https://api.ipify.org?format=json").Content | ConvertFrom-Json | Select-Object ip
   ```
   Exemplo: `203.0.113.45` (fictício)

2. **Configurar Port Forwarding no roteador Receiver**

   - Acesse o painel do roteador (geralmente `http://192.168.1.1` ou `http://192.168.0.1`).
   - Procure por "Port Forwarding" ou "Encaminhamento de Porta".
   - Configure:
     - **Porta externa**: 4444 (ou outra porta pública; se seu ISP bloqueia 4444, tente 8888, 9999, etc.)
     - **IP local**: 192.168.1.100
     - **Porta interna**: 4444
     - **Protocolo**: TCP
   - Salve e aplique.

3. **Aguarde estabilização**

   Port forwarding pode levar 1-5 minutos para entrar em vigor.

4. **Na máquina Sender, editar `config.txt`:**
   ```
   RECEIVER_IP=203.0.113.45
   PORT=4444
   ```
   (Substitua pelo IP público real e porta configurada.)

5. **Iniciar Receiver (mesmo que antes)**
   ```bash
   ./build/meu_programa
   ```

6. **Iniciar Sender e enviar mensagem**
   ```bash
   ./build/meu_programa
   ```
   Digite mensagem e clique "Send".

**Vantagens:** Simples, sem software adicional.
**Desvantagens:** Requer acesso ao roteador; segurança depende do firewall; IP público pode mudar.

### Opção B: Usar VPN (mais seguro, mais complexo)

VPN cria uma rede virtual privada entre as duas máquinas, mesmo em redes públicas diferentes.

**Ferramentas recomendadas:**
- **Hamachi (LogMeIn)** — fácil, interface gráfica, freemium.
- **OpenVPN** — código aberto, mais controle, steeper learning curve.
- **WireGuard** — moderno, rápido, mas requer configuração manual.
- **Ngrok** — tunneling de porta, simples para prototipagem (não é VPN, mas alternativa).

**Exemplo com Hamachi (simplest):**

1. Baixe Hamachi em https://www.vpn.net/ e instale em ambas máquinas.
2. Crie uma conta (gratuito).
3. Em cada máquina, abra Hamachi, faça login e crie ou junte-se a uma rede (ex.: "DataComm").
4. Hamachi atribuirá IPs virtuais à cada máquina (ex.: 25.x.x.x).
5. Na máquina Sender, edite `config.txt`:
   ```
   RECEIVER_IP=25.x.x.x
   PORT=4444
   ```
   (Substitua pelo IP Hamachi do Receiver.)
6. Inicie Receiver e Sender normalmente.

**Vantagens:** Seguro, sem expor IP público, não precisa mexer em roteador, IP virtual permanece fixo.
**Desvantagens:** Requer instalação e conta adicional.

### Opção C: Ngrok (Tunneling, mais para teste rápido)

Ngrok cria um túnel seguro de um servidor público para seu app local. Útil para demo rápida.

1. Baixe Ngrok em https://ngrok.com/ (freemium).
2. Na máquina Receiver, execute:
   ```bash
   ngrok tcp 4444
   ```
   Você verá output como:
   ```
   Forwarding tcp://0.tcp.ngrok.io:xxxxx -> localhost:4444
   ```
   Anote o endereço (ex.: `0.tcp.ngrok.io:xxxxx`).

3. Na máquina Sender, edite `config.txt`:
   ```
   RECEIVER_IP=0.tcp.ngrok.io
   PORT=xxxxx
   ```

4. Inicie Receiver e Sender.

**Nota:** Ngrok freemium tem limite de connexões e tempo. Para uso prolongado, considere VPN ou port forwarding.

---

## Cenário 3: Teste WSL ↔ Windows Nativa

Se está testando com uma máquina rodando WSL e outra rodando a aplicação nativamente (ou vice-versa), ajuste o IP:

1. Na máquina WSL, para obter o IP:
   ```bash
   hostname -I
   ```
   Você receberá um IP local (ex.: `172.20.0.123`).

2. Na máquina Sender (nativa Windows), edite `config.txt`:
   ```
   RECEIVER_IP=172.20.0.123
   PORT=4444
   ```

3. Abra firewall do Windows em ambas (se necessário).

4. Siga os mesmos passos de inicialização.

---

## Dicas Gerais para Redes Remotas

### 1. Testar conectividade antes de rodar app

Use `ping` e `telnet` para confirmar que a porta está acessível:

```powershell
# Test ping (ICMP)
ping 192.168.1.100

# Test TCP port (requer telnet instalado; se não tiver, instale ou use PowerShell)
Test-NetConnection -ComputerName 192.168.1.100 -Port 4444
```

Se a porta estiver aberta, você verá:
```
TcpTestSucceeded : True
```

### 2. Verificar logs e erros

Rode ambas instâncias com logs:
```bash
./build/meu_programa 2>&1 | tee run.log
```
Envie `run.log` para debug se houver problemas.

### 3. Firewall — Regras para Sender (opcional)

Se o Sender também precisa aceitar conexões, abra a porta de saída (menos comum):
```powershell
New-NetFirewallRule -DisplayName "Data-Communication Out" -Direction Outbound -LocalPort 4444 -Protocol TCP -Action Allow
```

### 4. Monitorar tráfego de rede

Use ferramentas como **Wireshark** (gratuito) para capturar e analisar pacotes enviados:
```powershell
# Em admin PowerShell
# Instale ou baixe Wireshark em https://www.wireshark.org/
```
Filtre por `tcp.port == 4444` para ver apenas tráfego relevante.

### 5. Latência e timeouts

Se houver latência alta (redes remotas lentas), o aplicativo pode timeout. Você pode aumentar timeout no código (Socket.cpp):
```cpp
// exemplo: aumentar timeout de conexão
setsockopt(this->fd, SOL_SOCKET, SO_RCVTIMEO, ...);
```
Mas para testes iniciais, não é necessário.

---

## Checklist Rápido

Antes de rodar em rede remota:

- [ ] **Receiver compilado e pronto** (./build/meu_programa existe)
- [ ] **Sender compilado e pronto**
- [ ] **IP do Receiver descoberto** (ipconfig)
- [ ] **Firewall aberto** (regra de firewall criada no Receiver)
- [ ] **config.txt atualizado no Sender** (IP e porta corretos)
- [ ] **Conectividade testada** (ping ou Test-NetConnection bem-sucedido)
- [ ] **Receiver iniciado ANTES do Sender** (importante!)
- [ ] **Logs monitorados** (run.log se houver erro)

---

## Troubleshooting Avançado

| Sintoma | Diagnóstico | Solução |
|---------|-----------|--------|
| "Connection refused" | Porta não aberta ou Receiver não rodando | Confirme Receiver está rodando, firewall aberto |
| "Connection timeout" | Receiver desligado ou IP errado | Tente ping; valide IP em config.txt |
| Mensagem chega truncada | Problema no protocolo de rede | Verifique se ambas versões do código são iguais (git pull); revise Socket.cpp |
| Firewall bloqueando silenciosamente | Windows Defender ativado | Execute regra de firewall; verifique em Settings → Privacy & Security → Firewall |
| Múltiplas conexões simultâneas falham | Socket server não aceita múltiplas conexões | Código atual suporta 1 Receiver; para multiplos, refactor Socket.cpp |

---

## Próximas Melhorias (Opcional)

Para robuustez em produção:
1. **Adicionar autenticação** — token/password ao conectar.
2. **Criptografia TLS/SSL** — encryt tráfego de rede (além da XOR do app).
3. **Persistência** — guardar mensagens em arquivo/banco dados.
4. **Interface web** — usar websockets e HTML5 para acesso via navegador (sem compilação nativa).

---

**Perguntas? Dúvidas?** Revise os logs (`run.log`) e use `Test-NetConnection` para debug de conectividade.
