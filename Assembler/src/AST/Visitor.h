#pragma once

#include "AST/AST.h"

// Das Interface
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    // Jede Node-Art bekommt ihre eigene, maßgeschneiderte Funktion
    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(InstructionNode& node) = 0;
    virtual void visit(RegisterNode& node) = 0;
    virtual void visit(ImmediateNode& node) = 0;
    virtual void visit(MemoryNode& node) = 0;
    virtual void visit(LabelDefNode& node) = 0;
    virtual void visit(IdentifierNode& node) = 0;
};