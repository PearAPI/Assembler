#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <stdint.h>

struct SymbolTable {
    std::unordered_map<std::string, uint16_t> labels; // Name -> Adresse
    
    bool addLabel(const std::string& name, uint16_t address) {
        if (labels.count(name) > 0) {
            return false; // Fehler: Label ist schon vergeben!
        }
        labels[name] = address;
        return true;
    }
    
    uint16_t getAddress(const std::string& name) {
        if (labels.find(name) == labels.end()) {
            throw std::runtime_error("Undefined label: " + name);
        }
        return labels[name];
    }
};