#pragma once

#include "Lexer/Token.h"

class Lexer {
private:
    std::string source;
    uint64_t pos = 0;
    uint64_t line = 1;

    std::vector<Token> tokens;

private:
    bool isAtEnd() const;
    char peek() const;
    char advance();
    
    void skipWhitespace();
    void skipWhitespaceAndComments();
    std::string readNumber();
    std::string readIdentifierOrInstruction();

public:
    Lexer(const std::string& source);
    ~Lexer() = default;

    std::vector<Token> tokenize();
};