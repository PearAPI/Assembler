#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Lexer/Mnemonics.h"
#include "AST/ASTNode.h"


class ProgramNode : public ASTNode {
public:
    // Eine Liste von einzigartigen Pointern auf die Kinder-Knoten
    std::vector<std::unique_ptr<ASTNode>> statements;

    ProgramNode() : ASTNode(0) {}

    bool isMemory() const override { return false; }

    void addStatement(std::unique_ptr<ASTNode> stmt) {
        statements.push_back(std::move(stmt));
    }

    void accept(ASTVisitor& visitor) override;

    NodeType getType() const override { return NodeType::Program; }
};

class InstructionNode : public ASTNode {
public:
    Mnemonics mnemonic; // Aus deinem Enum (z.B. Mnemonic::MOV)
    std::string mnemonicName; // Aus deinem String (z.B. "mov")
    
    // Die Operanden (z.B. RegisterNode oder ImmediateNode)
    std::vector<std::unique_ptr<ASTNode>> operands;

    bool isMemory() const override { return false; }

    InstructionNode(Mnemonics m, size_t l, std::string name) : ASTNode(l), mnemonic(m), mnemonicName(name) {}

    void addOperand(std::unique_ptr<ASTNode> op) {
        operands.push_back(std::move(op));
    }

    void accept(ASTVisitor& visitor) override;

    NodeType getType() const override { return NodeType::Instruction; }
};

class RegisterNode : public ASTNode {
public:
    std::string name; // z.B. "ax", "bx"

    RegisterNode(const std::string& n, size_t l) : ASTNode(l), name(n) {}

    bool isMemory() const override { return false; }

    void accept(ASTVisitor& visitor) override;

    std::string getName() const override { return this->name; }

    NodeType getType() const override { return NodeType::Register; }
};

class ImmediateNode : public ASTNode {
public:
    uint16_t value; // Die echte, berechnete Zahl!

    ImmediateNode(uint16_t v, size_t l) : ASTNode(l), value(v) {}

    bool isMemory() const override { return false; }

    void accept(ASTVisitor& visitor) override;

    NodeType getType() const override { return NodeType::Immediate; }
};

class MemoryNode : public ASTNode {
public:
    std::string content; // z.B. "100h" oder "bx" oder "bx+8"

    MemoryNode(const std::string& c, size_t l) : ASTNode(l), content(c) {}
    
    bool isMemory() const override { return true; }
    bool isLabelReference() const { return false; }
    bool isAbsoluteAddress() const { return false; }
    
    bool usesRegister() const {
        // Prüfe hier, ob der Inhalt der Klammer ein Register ist.
        // Wenn du z.B. einen String hast: return (name == "bx" || name == "ax");
        // Wenn du eine innere Node hast: return innerNode->isRegister();
        return false; // Passe das an deine Logik an!
    }

    void accept(ASTVisitor& visitor) override;

    NodeType getType() const override { return NodeType::Memory; }
};

class LabelDefNode : public ASTNode {
public:
    std::string name; // z.B. "main" (ohne den Doppelpunkt!)

    LabelDefNode(const std::string& n, size_t l) : ASTNode(l), name(n) {}

    bool isMemory() const override { return false; }

    void accept(ASTVisitor& visitor) override;

    NodeType getType() const override { return NodeType::LabelDef; }
};

class IdentifierNode : public ASTNode {
public:
    std::string name; // Der Name des Sprungziels (z.B. "main" oder "loop_start")

    IdentifierNode(const std::string& n, size_t l) : ASTNode(l), name(n) {}

    bool isMemory() const override { return false; }

    void accept(ASTVisitor& visitor) override;

    std::string getName() const override { return this->name; }

    NodeType getType() const override { return NodeType::Identifier; }
};