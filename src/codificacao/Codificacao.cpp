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

std::string Codificacao::generateMessageNRZ(const std::string& binaryData) {
    /**
     * NRZ Message Format:
     * - Bit '0' = represented as "0"
     * - Bit '1' = represented as "+"
     */
    std::string message;
    message.reserve(binaryData.size());
    
    for (char bit : binaryData) {
        message += (bit == '1') ? '+' : '0';
    }
    
    return message;
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

std::string Codificacao::generateMessageRZ(const std::string& binaryData) {
    /**
     * RZ Message Format:
     * - Bit '0' = represented as "-0" (negative level, returns to zero)
     * - Bit '1' = represented as "+0" (positive level, returns to zero)
     */
    std::string message;
    message.reserve(binaryData.size() * 2);
    
    for (char bit : binaryData) {
        if (bit == '1') {
            message += "+0";
        } else {
            message += "-0";
        }
    }
    
    return message;
}

std::vector<float> Codificacao::gerar(const std::string& binaryData, Tipo tipo) {
    if (tipo == NRZ) {
        return generateNRZ(binaryData);
    } else {
        return generateRZ(binaryData);
    }
}

std::string Codificacao::generateMessage(const std::string& binaryData, Tipo tipo) {
    if (tipo == NRZ) {
        return generateMessageNRZ(binaryData);
    } else {
        return generateMessageRZ(binaryData);
    }
}

const char* Codificacao::getTypeName(Tipo tipo) {
    return (tipo == NRZ) ? "NRZ (Non-Return-to-Zero)" : "RZ (Return-to-Zero)";
}
