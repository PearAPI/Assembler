#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "AST/Visitor.h"
#include "formatErrorMessage.h"

class StaticAnalysisVisitor : public ASTVisitor {
private:
    std::vector<std::string> errors;

    // Ein kleines Hilfswerkzeug, um Fehler hübsch zu formatieren
    void addError(const std::string& msg) {
        errors.push_back(msg);
    }

    void addError(const std::string& msg, size_t line) {
        // Hier wird dein String in das schöne Template gegossen
        errors.push_back(formatErrorMessage(msg, line, "Logic-Error"));
    }

public:
    bool hasErrors() const { return !errors.empty(); }
    
    void printErrors() const {
        for (const auto& err : errors) {
            std::cerr << "Syntax-Error: " << err << "\n";
        }
    }

    void visit(ProgramNode& node) override {
        for (auto& stmt : node.statements) {
            stmt->accept(*this);
        }
    }

    void visit(InstructionNode& node) override;
    void visit(LabelDefNode& node) override;
    
    void visit(RegisterNode& node) override {}
    void visit(ImmediateNode& node) override {}
    void visit(MemoryNode& node) override {}
    void visit(IdentifierNode& node) override {}
};