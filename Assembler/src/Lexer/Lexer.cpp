#include "Lexer/Lexer.h"

#include <algorithm>
#include <iostream>
#include <cctype>

#include "Lexer/Mnemonics.h"

bool Lexer::isAtEnd() const {
    return pos >= source.length();
}

char Lexer::peek() const {
    if (isAtEnd()) {
        return '\0';
    }
    return source[pos];
}

char Lexer::advance() {
    return source[pos++];
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && isspace(peek())) {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }
}

void Lexer::skipWhitespaceAndComments() {
    skipWhitespace();

    if (!isAtEnd() && peek() == ';') {
        while (!isAtEnd() && peek() != '\n') {
            advance();
        }
        skipWhitespace();
    }
}

std::string Lexer::readIdentifierOrInstruction() {
    size_t startPos = pos;

    // 1. Das Wort einlesen: Solange es Buchstaben, Zahlen oder Unterstriche sind
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }

    // Den fertigen String aus dem Quelltext ausschneiden
    std::string text = source.substr(startPos, pos - startPos);

    std::string nomalizedText = text;
    std::transform(nomalizedText.begin(), nomalizedText.end(), nomalizedText.begin(), [](unsigned char c) { return std::tolower(c); });

    return nomalizedText;
}

std::string Lexer::readNumber() {
    size_t startPos = pos;

    // Lese Zeichen, die in Assembler-Zahlen vorkommen dürfen:
    // Ziffern (0-9), Hex-Buchstaben (a-f, A-F) und die Marker 'x' und 'h'
    while (!isAtEnd() && (std::isxdigit(peek()) ||
                          peek() == 'x' || peek() == 'X' ||
                          peek() == 'h' || peek() == 'H')) {
        advance();
    }

    return source.substr(startPos, pos - startPos);
}

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        skipWhitespaceAndComments();  // Erstmal Müll (Leerzeichen/Kommentare) weg

        if (isAtEnd()) break;

        char c = peek();

        // 1. Weiche für Identifier, Instructions (und Labels)
        if (std::isalpha(c) || c == '_') {
            std::string text = readIdentifierOrInstruction();
            if (!isAtEnd() && peek() == ':') {
                advance();
                tokens.push_back({TokenType::LabelDef, text, line});
            } 
            else if (stringToMnemonic(text) != Mnemonics::UNKNOWN) { 
                tokens.push_back({TokenType::Instruction, text, line});
            } else {
                tokens.push_back({TokenType::Identifier, text, line});
            }
        } else if (std::isdigit(c)) {
            tokens.push_back({TokenType::Immediate, readNumber(), line});
        } else if (c == ',') {
            tokens.push_back({TokenType::Comma, ",", line});
            advance();
        } else if (c == '[') {
            advance();
            skipWhitespace();

            std::string content;

            char next = peek();
            if (std::isdigit(next)) {
                content = readNumber();
            } else {
                content = readIdentifierOrInstruction();
            }

            tokens.push_back({TokenType::Memory, content, line});
            skipWhitespace();

            if (peek() == ']') {
                advance();
            } else {
                std::cerr << "Syntax Fehler: Fehlendes ']' in Zeile " << line << "\n";
            }

        } else {
            std::cerr << "Unbekanntes Zeichen: " << c << " in Zeile " << line << "\n";
            advance();
        }
    }

    tokens.push_back({TokenType::EndOfFile, "", line});

    int index = 0;
    for (const auto& token : tokens) {
        if(token.type == TokenType::Identifier) {
            if (registerNames.find(token.value) != registerNames.end()) {
                tokens[index].type = TokenType::Register;
            }
        }
        index++;
    }


    return tokens;
}
