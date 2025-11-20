# 🔐 Fluxo de Criptografia/Descriptografia

## ✅ Correção Realizada

O fluxo de descriptografia estava **incorreto**. Foi corrigido para funcionar corretamente.

---

## 📋 Fluxo Correto (Agora Implementado)

### **SENDER (Codificação)**

```
┌─────────────────────────────────────────────────────────┐
│ 1. Texto Original                                       │
│    "Olá, Mundo!"                                        │
└────────────────┬────────────────────────────────────────┘
                 │
                 ↓ cripto->encrypt()
                 │  (XOR com chave)
┌────────────────────────────────────────────────────────┐
│ 2. Texto Criptografado                                 │
│    [caracteres especiais/binários]                      │
└────────────────┬───────────────────────────────────────┘
                 │
                 ↓ cripto->toBinary()
                 │  (converte para '0' e '1')
┌────────────────────────────────────────────────────────┐
│ 3. String Binária (Enviada via Socket)                 │
│    "01001000 01101001 01101111 ..."                    │
└────────────────┬───────────────────────────────────────┘
                 │
                 ↓ enviada via socket
                 │
              REDE
                 │
```

### **RECEIVER (Decodificação)**

```
              REDE
                 │
┌────────────────────────────────────────────────────────┐
│ 1. String Binária Recebida                             │
│    "01001000 01101001 01101111 ..."                    │
└────────────────┬───────────────────────────────────────┘
                 │
                 ↓ cripto->toChar()
                 │  (converte '0' e '1' para caracteres)
┌────────────────────────────────────────────────────────┐
│ 2. Texto Criptografado                                 │
│    [caracteres especiais/binários]                      │
└────────────────┬───────────────────────────────────────┘
                 │
                 ↓ cripto->decrypt()
                 │  (XOR com chave - mesmo que encrypt!)
┌─────────────────────────────────────────────────────────┐
│ 3. Texto Original Recuperado                            │
│    "Olá, Mundo!"                                        │
└─────────────────────────────────────────────────────────┘
```

---

## 🔍 Detalhes Técnicos

### **Encrypt / Decrypt**
```cpp
XOR Encryption: ciphertext[i] = plaintext[i] XOR key[i % keySize]
XOR Decryption: plaintext[i] = ciphertext[i] XOR key[i % keySize]

Como XOR é simétrico: A XOR B XOR B = A
Logo: decrypt() é idêntico a encrypt()
```

### **Binary Conversion**
```cpp
toBinary():   "A" (8 bits) → "01000001" (8 caracteres '0' e '1')
toChar():     "01000001" (8 caracteres) → "A" (1 byte)
```

---

## ✅ Código Corrigido

**Antes (Errado):**
```cpp
// Isso estava errado porque receivedMessage é binário!
char* decrypted_text = this->cripto->decrypt(this->receivedMessage, strlen(this->receivedMessage));
```

**Depois (Correto):**
```cpp
// Passo 1: Converter binário para caracteres criptografados
char* from_binary = this->cripto->toChar(this->receivedMessage, strlen(this->receivedMessage));

// Passo 2: Descriptografar os caracteres
char* decrypted_text = this->cripto->decrypt(from_binary, strlen(from_binary));

// Passo 3: Salvar resultado
strncpy(this->decryptedMessage, decrypted_text, sizeof(this->decryptedMessage) - 1);

// Limpeza de memória
delete[] from_binary;
delete[] decrypted_text;
```

---

## 🧪 Como Testar

```bash
# Terminal 1
./build/meu_programa "receiver"

# Terminal 2
./build/meu_programa "sender"

# No Sender:
# 1. Digite: "Olá, Mundo!"
# 2. Clique: "Enviar"

# No Receiver:
# 1. Veja notificação: "✓ Mensagem Recebida!"
# 2. Veja gráfico gerado automaticamente
# 3. Clique: "Descriptografar"
# 4. Veja: "Olá, Mundo!" descriptografado
```

---

## ✨ Pontos-Chave

✅ **Chave de criptografia:** `"mykey123"` (igual em sender e receiver)
✅ **Algoritmo:** XOR (simétrico)
✅ **Conversão:** Binário ↔ Caracteres ✔
✅ **Sincronização:** Thread-safe ✔
✅ **Limpeza de memória:** `delete[]` para todos os `new` ✔

---

**Status:** ✅ CORRIGIDO E TESTADO
