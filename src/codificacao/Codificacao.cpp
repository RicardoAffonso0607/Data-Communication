#include "../../include/codificacao/Codificacao.h"

std::vector<float> Codificacao::generateNRZ(const std::string& binaryData) {
    /**
     * NRZ (Non-Return-to-Zero):
     * - Bit '0' = -1.0 (low level)
     * - Bit '1' = +1.0 (high level)
     * - Maintains the level during the entire bit period
     */
    std::vector<float> waveform;
    waveform.reserve(binaryData.size() * 10); // 10 samples per bit
    
    for (char bit : binaryData) {
        float level = (bit == '1') ? 1.0f : -1.0f;
        // 10 samples maintaining the same level
        for (int i = 0; i < 10; ++i) {
            waveform.push_back(level);
        }
    }
    
    return waveform;
}

std::vector<float> Codificacao::generateRZ(const std::string& binaryData) {
    /**
     * RZ (Return-to-Zero):
     * - Bit '0' = returns to 0 in the middle of the period
     * - Bit '1' = maintains +1.0 in first half, returns to 0 in second half
     * - Always returns to zero at the end of each period
     */
    std::vector<float> waveform;
    waveform.reserve(binaryData.size() * 10); // 10 samples per bit
    
    for (char bit : binaryData) {
        if (bit == '1') {
            // Bit '1': rises to 1.0 in first half, returns to 0 in second half
            for (int i = 0; i < 5; ++i) {
                waveform.push_back(1.0f);
            }
            for (int i = 0; i < 5; ++i) {
                waveform.push_back(0.0f);
            }
        } else {
            // Bit '0': maintains -1.0 in first half, returns to 0 in second half
            for (int i = 0; i < 5; ++i) {
                waveform.push_back(-1.0f);
            }
            for (int i = 0; i < 5; ++i) {
                waveform.push_back(0.0f);
            }
        }
    }
    
    return waveform;
}

std::vector<float> Codificacao::gerar(const std::string& binaryData, Tipo tipo) {
    if (tipo == NRZ) {
        return generateNRZ(binaryData);
    } else {
        return generateRZ(binaryData);
    }
}

const char* Codificacao::getTypeName(Tipo tipo) {
    return (tipo == NRZ) ? "NRZ (Non-Return-to-Zero)" : "RZ (Return-to-Zero)";
}
