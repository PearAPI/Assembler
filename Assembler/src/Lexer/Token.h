#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class TokenType {
    Instruction,    // example: mov, add
    Register,       // example: ax, bx
    Immediate,      // 5, 100h
    Memory,         // [100h]
    Comma,          // ,
    LabelDef,       // example: main:
    Identifier,     // jmp main_loop    ; "main_loop" ist das Identifier-Token
    EndOfFile
};

typedef struct {
    TokenType type;
    std::string value;
    uint64_t line;
} Token;

using TokenStream = std::vector<Token>;

static std::string TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Instruction: return "Instruction";
        case TokenType::Register: return "Register";
        case TokenType::Immediate: return "Immediate";
        case TokenType::Memory: return "Memory";
        case TokenType::Comma: return "Comma";
        case TokenType::LabelDef: return "LabelDef";
        case TokenType::Identifier: return "Identifier";
        case TokenType::EndOfFile: return "EndOfFile";
        default: return "Unknown";
    }
}
