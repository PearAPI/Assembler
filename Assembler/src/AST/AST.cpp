// AST.cpp
#include "AST/AST.h"
#include "AST/Visitor.h"

void ProgramNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void InstructionNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void RegisterNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void ImmediateNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void MemoryNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void LabelDefNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void IdentifierNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}