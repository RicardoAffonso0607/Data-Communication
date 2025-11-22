#pragma once

#include <vector>
#include <string>

class Codificacao {
public:
    enum Tipo {
        NRZ,  // Non-Return-to-Zero
        RZ    // Return-to-Zero
    };

    /**
     * Generate waveform based on binary data
     * @param binaryData: string with 0s and 1s
     * @param tipo: NRZ or RZ
     * @return vector of floats representing the waveform
     */
    static std::vector<float> generateNRZ(const std::string& binaryData);
    static std::vector<float> generateRZ(const std::string& binaryData);
    
    /**
     * Generate waveform based on selected type
     */
    static std::vector<float> gerar(const std::string& binaryData, Tipo tipo);
    
    /**
     * Return type name as string
     */
    static const char* getTypeName(Tipo tipo);
};
