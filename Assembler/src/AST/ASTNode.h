#pragma once

#include <string>
#include <cstdint>

class ASTVisitor; // Forward Declaration

enum class NodeType {
    Program, 
    Instruction, 
    Register, 
    Immediate, 
    Memory, 
    LabelDef,
    Identifier // Für Sprungziele wie bei "jmp main"
};

class ASTNode {
public:
    size_t line; // Lebensrettend für Fehlermeldungen!

    
    ASTNode(size_t l) : line(l) {}
    virtual ~ASTNode() = default;
    
    virtual void accept(ASTVisitor& visitor) = 0;
    virtual NodeType getType() const = 0; 
    
    virtual std::string getName() const { return ""; }
    virtual uint16_t getValue() const { return 0; }

    virtual bool isMemory() const = 0;
    
    bool isRegister() const  { return getType() == NodeType::Register; }
    bool isImmediate() const { return getType() == NodeType::Immediate; }
    bool isMemory() const    { return getType() == NodeType::Memory; }
    bool isIdentifier() const{ return getType() == NodeType::Identifier; }
};