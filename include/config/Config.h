#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

class Config {
private:
    std::map<std::string, std::string> settings;
    
    // Trim whitespace from string
    static std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }

public:
    Config() {
        loadFromFile("config.txt");
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "⚠️  Config file '" << filename << "' not found. Using defaults." << std::endl;
            setDefaults();
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip empty lines and comments
            if (line.empty() || line[0] == '#') continue;

            // Parse key=value
            size_t pos = line.find('=');
            if (pos == std::string::npos) continue;

            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));

            if (!key.empty() && !value.empty()) {
                settings[key] = value;
            }
        }
        file.close();
        std::cout << "✅ Config loaded from '" << filename << "'" << std::endl;
    }

    void setDefaults() {
        settings["RECEIVER_IP"] = "127.0.0.1";
        settings["PORT"] = "4444";
    }

    std::string get(const std::string& key) const {
        auto it = settings.find(key);
        if (it != settings.end()) {
            return it->second;
        }
        // Return empty string if not found
        return "";
    }

    std::string get(const std::string& key, const std::string& defaultValue) const {
        auto it = settings.find(key);
        if (it != settings.end()) {
            return it->second;
        }
        return defaultValue;
    }

    int getInt(const std::string& key, int defaultValue = 0) const {
        auto it = settings.find(key);
        if (it != settings.end()) {
            try {
                return std::stoi(it->second);
            } catch (...) {
                return defaultValue;
            }
        }
        return defaultValue;
    }

    void print() const {
        std::cout << "\n📋 Current Configuration:" << std::endl;
        for (const auto& pair : settings) {
            std::cout << "  " << pair.first << " = " << pair.second << std::endl;
        }
        std::cout << std::endl;
    }
};
