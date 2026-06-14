#include "Parser/Parser.h"

#include "AST/AST.h"

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();

    while (!isAtEnd()) {
        if (peek().type == TokenType::LabelDef) {
            Token labelToken = advance();
            program->addStatement(std::make_unique<LabelDefNode>(labelToken.value, labelToken.line));
        } else if (peek().type == TokenType::Instruction) {
            program->addStatement(parseInstruction());
        } else {
            std::cerr << "Unrecognized Token (" << peek().value << " | type " << TokenTypeToString(peek().type) << ") on line " << peek().line << "\n";
            advance();
        }
    }

    return program;
}

std::unique_ptr<InstructionNode> Parser::parseInstruction() {
    Token instrToken = advance();

    Mnemonics mnemonic = stringToMnemonic(instrToken.value);

    if (mnemonic == Mnemonics::UNKNOWN)
        throw std::runtime_error("Invalid Instruction on line " + std::to_string(instrToken.line));

    auto instruction = std::make_unique<InstructionNode>(mnemonic, instrToken.line, instrToken.value);

    // Wir lesen Operanden, solange welche da sind
    while (!isAtEnd() && peek().type != TokenType::Instruction && peek().type != TokenType::LabelDef) {
        instruction->addOperand(parseOperand());

        if (peek().type == TokenType::Comma) {
            advance();
        } else {
            break;
        }
    }

    return instruction;
}

std::unique_ptr<ASTNode> Parser::parseOperand() {
    Token token = advance();

    if (token.type == TokenType::Register) {
        return std::make_unique<RegisterNode>(token.value, token.line);
    } else if (token.type == TokenType::Immediate) {
        uint16_t val = (uint16_t)std::stoi(token.value);
        return std::make_unique<ImmediateNode>(val, token.line);
    } else if (token.type == TokenType::Memory) {
        return std::make_unique<MemoryNode>(token.value, token.line);
    } else if (token.type == TokenType::Identifier) {
        return std::make_unique<IdentifierNode>(token.value, token.line);
    }

    throw std::runtime_error("Ungültiger Operand in Zeile " + std::to_string(token.line));
}