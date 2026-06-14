#pragma once

#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <string>

enum class Mnemonics {
    UNKNOWN,

    NOP,
    HLT,
    PUSH,
    POP,
    CFLAGS,
    PUSHF,
    POPF,
    MOV,

    INC,
    DEC,
    ADD,
    SUB,
    SHL,
    SHR,
    AND,
    OR,
    XOR,
    NOT,

    CALL,
    RET,
    JMP,
    CMP,

    JE,
    JNE,
    JG,
    JL,
    JGE,
    JLE,
};

enum class Register_Width {
    bits_8 = 0,
    bits_16,
};

const std::unordered_map<std::string, Mnemonics> instructionMap = {
    {"nop", Mnemonics::NOP},
    {"hlt", Mnemonics::HLT},
    {"push", Mnemonics::PUSH},
    {"pop", Mnemonics::POP},
    {"cflags", Mnemonics::CFLAGS},
    {"pushf", Mnemonics::PUSHF},
    {"popf", Mnemonics::POPF},
    {"mov", Mnemonics::MOV},

    {"inc", Mnemonics::INC},
    {"dec", Mnemonics::DEC},
    {"add", Mnemonics::ADD},
    {"sub", Mnemonics::SUB},
    {"shl", Mnemonics::SHL},
    {"shr", Mnemonics::SHR},
    {"and", Mnemonics::AND},
    {"or", Mnemonics::OR},
    {"xor", Mnemonics::XOR},
    {"not", Mnemonics::NOT},

    {"call", Mnemonics::CALL},
    {"ret", Mnemonics::RET},
    {"jmp", Mnemonics::JMP},
    {"cmp", Mnemonics::CMP},

    {"je", Mnemonics::JE},
    {"jne", Mnemonics::JNE},
    {"jg", Mnemonics::JG},
    {"jl", Mnemonics::JL},
    {"jge", Mnemonics::JGE},
    {"jle", Mnemonics::JLE},    
};

const std::unordered_set<std::string> registerNames = {
    "ax", "ah", "al",
    "bx", "bh", "bl",
    "cx", "ch", "cl",
    "dx", "dh", "dl",
    "si", "sil",
    "di", "dil",
    "sp", 
    "bp", 
};

const std::unordered_map<std::string, Register_Width> registerRegistry = {
    {"ax", Register_Width::bits_16}, {"al", Register_Width::bits_8}, {"ah", Register_Width::bits_8},
    {"bx", Register_Width::bits_16}, {"bl", Register_Width::bits_8}, {"bh", Register_Width::bits_8},
    {"cx", Register_Width::bits_16}, {"cl", Register_Width::bits_8}, {"ch", Register_Width::bits_8},
    {"dx", Register_Width::bits_16}, {"dl", Register_Width::bits_8}, {"dh", Register_Width::bits_8},
    {"si", Register_Width::bits_16}, {"sil", Register_Width::bits_8},
    {"di", Register_Width::bits_16}, {"dil", Register_Width::bits_8},
    {"sp", Register_Width::bits_16},
    {"bp", Register_Width::bits_16},
};

inline Mnemonics stringToMnemonic(const std::string& str) {
    auto it = instructionMap.find(str);
    if (it != instructionMap.end()) {
        return it->second;
    }

    return Mnemonics::UNKNOWN;
};

static inline Register_Width getWidth(const std::string& regName) {
    auto it = registerRegistry.find(regName);
    if (it != registerRegistry.end()) {
        return it->second;
    }
    
    throw std::runtime_error("Unknown register: " + regName);
}

static inline uint16_t getRegisterCode(const std::string& regName) {
    if (regName == "ax") return 0x00;
    if (regName == "bx") return 0x01;
    if (regName == "cx") return 0x02;
    if (regName == "dx") return 0x03;
    
    if (regName == "al") return 0x04;
    if (regName == "bl") return 0x05;
    if (regName == "cl") return 0x06;
    if (regName == "dl") return 0x07;

    if (regName == "ah") return 0x08;
    if (regName == "bh") return 0x09;
    if (regName == "ch") return 0x0A;
    if (regName == "dh") return 0x0B;

    if (regName == "sp") return 0x0C;
    if (regName == "bp") return 0x0D;
    if (regName == "si") return 0x0E;
    if (regName == "di") return 0x0F;

    return 0x00;
}