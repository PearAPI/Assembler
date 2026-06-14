#pragma once

#include <iostream>

#include "formatErrorMessage.h"
#include "AST/ASTNode.h"
#include "AST/Visitor.h"
#include "AST/visitors/symbolTable.h"

class SymbolTableVisitor : public ASTVisitor {
private:
    uint16_t currentAddress = 0;

    uint16_t estimateInstructionSize(const InstructionNode& node);

    std::vector<std::string> errors;

    void addError(const std::string& msg, size_t line) {
        // Nutze hier deine formatErrorMessage Funktion von vorhin!
        errors.push_back(formatErrorMessage(msg, line, "Symbol-Fehler"));
    }
public:
    SymbolTable table;

    bool hasErrors() const { return !errors.empty(); }
    
    void printErrors() const {
        for (const auto& err : errors) {
            std::cerr << err << "\n";
        }
    }

    void visit(ProgramNode& node) override {
        // Wir gehen alle Zeilen (Statements) von oben nach unten durch
        for (const auto& stmt : node.statements) {
            stmt->accept(*this);
        }
    }

    void visit(LabelDefNode& node) override {
        std::cout << "visiting label: " << node.name << " at address << " << currentAddress << "\n";
        table.addLabel(node.name, currentAddress);
    }

    void visit(InstructionNode& node) override {
        uint16_t size = estimateInstructionSize(node);
        std::cout << "visiting current instruction: " << node.mnemonicName << " with size " << size << " at address " << currentAddress << "\n";

        currentAddress += size;
    }

    // --- FEHLENDES TEIL 2: C++ glücklich machen ---
    void visit(RegisterNode& node) override {}
    void visit(ImmediateNode& node) override {}
    void visit(MemoryNode& node) override {}
    void visit(IdentifierNode& node) override {}
};
