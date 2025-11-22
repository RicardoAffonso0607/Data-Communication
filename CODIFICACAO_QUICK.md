# ⚡ NRZ/RZ - Quick Reference

## TL;DR

| Aspecto | NRZ | RZ |
|---------|-----|-----|
| **Bit 0** | -1.0 (constante) | -1.0 → 0.0 |
| **Bit 1** | +1.0 (constante) | +1.0 → 0.0 |
| **Eficiência** | ✅ Melhor | ❌ Usa mais banda |
| **Sincronização** | ❌ Difícil | ✅ Melhor |
| **Visualização** | Degraus | Pulsos |

## 🎮 Como Usar

1. **Escreva** uma mensagem em "Mensagem Original"
2. **Clique fora** do campo (gera binário e codificações)
3. **Veja** o checkbox "Usar NRZ" na seção "Codificação de Linha"
4. **Marque/desmarque** para alternar entre NRZ e RZ
5. **Observe** o gráfico mudar em tempo real

## 📊 Visual

```
NRZ (degraus):              RZ (pulsos):
┌─────┐     ┌──────         ┌──┐      ┌──┐
│     │     │               │  │      │  │
│     └─────┘               └──┴──────┘  └──┘
│                           Volta a zero!
└──── Mantém nível ────────
```

## 🔑 Conceitos

**NRZ:** "Não Volta a Zero"
- Sinal permanece em +1 ou -1
- Simples, eficiente
- Problema: baseline variance

**RZ:** "Volta a Zero"  
- Sinal volta a 0 após cada bit
- Melhor para sincronização
- Usa mais banda

## 📍 Localização na GUI

### Sender
```
↓ Mensagem em Binário
↓
📊 Forma de Onda (Codificação) [gráfico original]
↓ [NOVO]
📊 Codificação de Linha
   Algoritmo: NRZ (Non-Return-to-Zero)
   ☑ Usar NRZ#sender (desative para RZ)
   [GRÁFICO 150px]
↓
Enviar
```

### Receiver
```
↓ Mensagem Recebida (Binária)
↓
📊 Forma de Onda (Decodificação)
↓ [NOVO]
📊 Codificação de Linha (Recebida)
   Algoritmo: NRZ (Non-Return-to-Zero)
   ☑ Usar NRZ#receiver (desative para RZ)
   [GRÁFICO 150px]
```

## 🛠️ Arquivos Criados/Modificados

**Novos:**
- `include/codificacao/Codificacao.h`
- `src/codificacao/Codificacao.cpp`

**Atualizados:**
- `include/window_manager/WindowManager.h` (+include, +campos)
- `src/window_manager/WindowManager.cpp` (+gráficos, +checkboxes)
- `makefile` (+Codificacao.cpp)

## 🎯 Casos de Uso

| Caso | O Que Fazer |
|------|-------------|
| Aprender | Gere binário e veja NRZ vs RZ |
| Debug | Receba mensagem, analise codificação |
| Teste | Edite binário, veja impacto na onda |

## ✅ Status

- ✅ Compilação bem-sucedida
- ✅ Gráficos funcionando
- ✅ Checkboxes interativos
- ✅ Atualiza em tempo real

---

**Resumo:** Dois algoritmos de codificação, um checkbox, visual instantâneo! 🚀📊
