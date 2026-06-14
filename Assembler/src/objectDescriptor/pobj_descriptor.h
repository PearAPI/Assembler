#pragma once

#include <cstdint>
#include <vector>
#include <cstring>
#include <string>

struct ObjectHeader {
    char magic[4] = {'P', 'E', 'A', 'R'};
    uint16_t version = 1;
    uint16_t codeSize = 0;       
    uint16_t symbolCount = 0;    
    uint16_t relocCount = 0;     
};

struct SymbolEntry {
    char name[32];
    uint16_t offset;

    SymbolEntry(const std::string& strName, uint16_t addr) : offset(addr) {
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, strName.c_str(), sizeof(name) - 1);
    }
};

struct RelocationEntry {
    uint16_t patchOffset;
    char targetName[32];

    RelocationEntry(uint16_t offset, const std::string& target) : patchOffset(offset) {
        std::memset(targetName, 0, sizeof(targetName));
        std::strncpy(targetName, target.c_str(), sizeof(targetName) - 1);
    }
};

struct PearObjectFile {
    ObjectHeader header;
    std::vector<uint16_t> machineCode;
    std::vector<SymbolEntry> symbols;
    std::vector<RelocationEntry> relocs;
};