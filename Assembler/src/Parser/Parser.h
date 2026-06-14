#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

#include "Lexer/Token.h"
#include "AST/AST.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos = 0;

    // --- Die Navigations-Helfer ---

    bool isAtEnd() const {
        return peek().type == TokenType::EndOfFile;
    }

    const Token& peek() const {
        return tokens[pos];
    }

    const Token& previous() const {
        return tokens[pos - 1];
    }

    const Token& advance() {
        if (!isAtEnd()) pos++;
        return previous();
    }

    // Der wichtigste Helfer für saubere Syntax!
    // Fragt: "Ist das nächste Token von diesem Typ? Wenn ja, friss es. Wenn nein, wirf einen Fehler."
    Token consume(TokenType type, const std::string& message) {
        if (peek().type == type) return advance();
        
        // Hier könntest du später echtes Error-Handling einbauen
        std::cerr << "Syntax Fehler Zeile " << peek().line << ": " << message << "\n";
        throw std::runtime_error(message); 
    }


    std::unique_ptr<InstructionNode> parseInstruction();
    std::unique_ptr<ASTNode> parseOperand();

public:
    Parser(const std::vector<Token>& t) : tokens(t) {}

    std::unique_ptr<ProgramNode> parse();
};