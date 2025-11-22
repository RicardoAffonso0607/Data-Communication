# 📊 Algoritmos de Codificação - NRZ e RZ

## Visão Geral

Agora o sistema possui dois **novos gráficos interativos** que mostram a codificação de linha binária usando dois algoritmos diferentes:

- **NRZ** (Non-Return-to-Zero)
- **RZ** (Return-to-Zero)

Você pode **alternar entre eles com um checkbox** para comparar visualmente as diferenças!

---

## O que é Codificação de Linha?

Codificação de linha é a forma como representamos bits (0s e 1s) como sinais elétricos em uma transmissão de dados.

### 📌 NRZ (Non-Return-to-Zero)

```
Bit 0: -1.0 (nível baixo, mantém durante todo o bit)
Bit 1: +1.0 (nível alto, mantém durante todo o bit)

Exemplo: sequência "101"
┌────────────────┐
│ +1.0           ├─────────────┐
│     ┌──────────┘             ├────────────┐
├──────────────────────────────┤            │
│                              │            │
│                      -1.0    │            │
└──────────────────────────────┴────────────┘
  1        0        1
```

**Características:**
- ✅ Simples e eficiente
- ✅ Usa pouca energia
- ❌ Linha de base varia (pode ter problema de DC)
- ❌ Difícil sincronizar se muitos 0s ou 1s seguidos

---

### 📌 RZ (Return-to-Zero)

```
Bit 1: +1.0 (primeira metade), volta para 0 (segunda metade)
Bit 0: -1.0 (primeira metade), volta para 0 (segunda metade)

Exemplo: sequência "101"
┌────────┐
│ +1.0   │        ┌────────┐
│        └────┐   │        └────┐
├─────────────┴───────────────────┤
│                │                │
│           -1.0 │                │
└────────────────┴─────────────────┘
  1        0        1
```

**Características:**
- ✅ Sempre retorna a zero (melhor sincronização)
- ✅ Mais fácil detectar limites de bits
- ❌ Usa mais banda (o dobro de mudanças)
- ❌ Consome mais energia

---

## Como Usar na GUI

### 🎮 Sender (Lado do Remetente)

```
┌────────────────────────────────────┐
│ Codificação de Linha              │
│ Algoritmo: NRZ (Non-Return-to-Zero)
│                                    │
│ ☑ Usar NRZ#sender (desative para RZ)
│                                    │
│ [GRÁFICO 150px de altura]         │
│ ▁▂▃▄▅▆▇█▇▆▅▄▃▂▁▔▕                 │
│                                    │
└────────────────────────────────────┘
```

**Para alternar:**
1. Escreva uma mensagem no campo "Mensagem Original"
2. Clique fora do campo (atualiza os gráficos)
3. **Marque/desmarque o checkbox** "Usar NRZ#sender"
4. Veja o gráfico mudar em tempo real!

### 🎮 Receiver (Lado do Receptor)

```
┌────────────────────────────────────┐
│ Codificação de Linha (Recebida)   │
│ Algoritmo: NRZ (Non-Return-to-Zero)
│                                    │
│ ☑ Usar NRZ#receiver (desative para RZ)
│                                    │
│ [GRÁFICO 150px de altura]         │
│ ▁▂▃▄▅▆▇█▇▆▅▄▃▂▁▔▕                 │
│                                    │
└────────────────────────────────────┘
```

Aparece quando uma mensagem é **recebida** do sender.

---

## Comparação Visual: NRZ vs RZ

### Exemplo: Binary "0101" (2 bits cada)

**NRZ:**
```
     ┌─────┐       ┌─────┐
  +1 │     │       │     │
  0  │─────┴───────┴─────┴───
 -1  │         ┌─────┐
     │         │     │
     └─────────┘     │
     0       1    0   1
```

**RZ:**
```
     ┌──┐   ┌──┐   ┌──┐   ┌──┐
  +1 │  │   │  │   │  │   │  │
  0  │──┴───┴──┴───┴──┴───┴──┴──
 -1 │     ┌──┐   ┌──┐   ┌──┐
     │     │  │   │  │   │  │
     └─────┘  └───┘  └───┘  └──
     0       1    0   1
```

---

## Casos de Uso

### 📚 Educativo
```
Querendo entender como bits se transformam em sinais?
→ Escreva uma mensagem
→ Veja o binário gerado
→ Compare NRZ e RZ em tempo real
→ Aprenda a diferença!
```

### 🔬 Debug
```
Recebendo mensagem e quer verificar qualidade?
→ Veja como foi codificada (NRZ ou RZ)
→ Identifique padrões
→ Verifique se há sincronização correta
```

### 🧪 Simulação
```
Quer simular interferência de sinal?
→ Edite o binário manualmente
→ Veja como muda a forma de onda
→ Compare qual algoritmo é mais robusto
```

---

## Detalhes Técnicos

### Classe Codificacao

```cpp
class Codificacao {
public:
    enum Tipo { NRZ, RZ };
    
    static std::vector<float> gerarNRZ(const std::string& binaryData);
    static std::vector<float> gerarRZ(const std::string& binaryData);
    static std::vector<float> gerar(const std::string& binaryData, Tipo tipo);
    static const char* getNomeTipo(Tipo tipo);
};
```

### Como Funciona

1. **Entrada:** String binária (ex: "01010101")
2. **Processamento:**
   - NRZ: Cada bit = 10 amostras do mesmo nível (-1.0 ou +1.0)
   - RZ: Cada bit = 10 amostras (5 com valor, 5 retornando a 0)
3. **Saída:** `vector<float>` com 80 amostras (10 por bit)
4. **Rendering:** ImGui::PlotLines() mostra o gráfico

### Amostras por Bit

```
Cada bit é representado por 10 amostras:
┌─────────────┐
│ Amostra 1-5 │ (conteúdo do bit: -1, 0, ou +1)
│ Amostra 6-10│ (RZ retorna a 0, NRZ mantém)
└─────────────┘

Resultado: Gráfico suave e legível
```

---

## Arquivos Modificados

### Novos Arquivos
- `include/codificacao/Codificacao.h` - Cabeçalho da classe
- `src/codificacao/Codificacao.cpp` - Implementação

### Modificados
- `include/window_manager/WindowManager.h`
  - Adicionado include da classe Codificacao
  - Adicionados campos para armazenar waveforms (NRZ/RZ)
  - Adicionados campos para tipo de codificação ativo

- `src/window_manager/WindowManager.cpp`
  - Callback atualiza gráficos de codificação
  - Gráficos atualizam quando mensagens mudam
  - Checkboxes para alternar NRZ/RZ

- `makefile`
  - Adicionada compilação de `src/codificacao/Codificacao.cpp`

---

## Fluxo de Dados

```
Mensagem Original
    ↓
    Criptografa
    ↓
Mensagem Binária
    ├→ [Gera NRZ] → senderNRZ_Waveform
    └→ [Gera RZ]  → senderRZ_Waveform
    ↓
Usuario clica checkbox
    ↓
ImGui mostra a waveform correspondente
    ↓
[GRÁFICO INTERATIVO]
```

---

## Próximas Melhorias (Opcional)

- [ ] Adicionar AMI (Alternate Mark Inversion)
- [ ] Adicionar Manchester Encoding
- [ ] Adicionar 4B5B
- [ ] Mostrar espectro de frequência
- [ ] Simular ruído/interferência
- [ ] Exportar gráficos como imagem

---

## Status

✅ Implementado e compilado com sucesso
✅ Funcionando em sender e receiver
✅ Checkboxes interativos
✅ Gráficos atualizando em tempo real

**Pronto para testar!** 🚀

---

**Resumo:**
Agora você pode ver **visualmente** como seus bits são codificados em sinais! 
NRZ e RZ lado a lado, com apenas um clique para alternar. 📊✨
