#pragma once

#include <string>
#include <vector>

#include "AST/ASTNode.h"
#include "AST/Visitor.h"
#include "AST/visitors/symbolTable.h"

class ObjectCodeGenerator : public ASTVisitor {
   private:
    struct InstructionEncoding {
        std::vector<uint16_t> code;
        bool isGroup;
    };

    // Hilfsfunktionen (die eigentliche Bit-Schieberei)
    uint16_t generateInstructionWord(const InstructionNode& node, bool requiresModRM, const InstructionEncoding& encoding);
    uint16_t generateModRMWord(const InstructionNode& node, uint16_t additionalArgs, const InstructionEncoding& encoding);

    InstructionEncoding getOpcode(const InstructionNode& node);

   public:
    std::vector<uint16_t> machineCode;  // Das fertige Endprodukt!
    SymbolTable& symbolTable;           // Referenz auf die Tabelle aus Pass 1

    ObjectCodeGenerator(SymbolTable& table) : symbolTable(table) {}

    uint16_t getOperandTypeCode(const ASTNode* operand) {
        if (operand->isRegister()) return 0x00;
        if (operand->isImmediate() || operand->isIdentifier()) return 0x01;
        if (operand->isMemory()) {
            auto* memNode = static_cast<const MemoryNode*>(operand);
            if (memNode->usesRegister()) {
                return 0x03;  // [bx]
            } else {
                return 0x02;  // [0x100] oder [var]
            }
        }
        return 0x00;
    }

    void visit(ProgramNode& node) override {
        for (const auto& stmt : node.statements) {
            stmt->accept(*this);
        }
    }

    void visit(LabelDefNode& node) override {
    }

    void visit(InstructionNode& node) override {
        InstructionEncoding encoding = getOpcode(node);

        // 2. ModR/M erzwingen, wenn es ein Gruppen-Befehl ist!
        bool requiresModRM = encoding.isGroup;
        uint16_t additionalArgs = 0;

        // Prüfen, ob die Operanden zusätzlich ein ModR/M-Wort fordern
        for (const auto& operand : node.operands) {
            if (operand->isImmediate() || operand->isMemory() || operand->isIdentifier()) {
                requiresModRM = true;
                additionalArgs++;
            }
        }

        // 3. Erstes Wort schreiben (Main-Opcode)
        machineCode.push_back(generateInstructionWord(node, requiresModRM, encoding));

        // 4. Zweites Wort schreiben (ModR/M + Subcode)
        if (requiresModRM) {
            machineCode.push_back(generateModRMWord(node, additionalArgs, encoding));
        }

        // 5. Daten/Adressen anhängen
        for (const auto& operand : node.operands) {
            if (operand->isImmediate()) {
                machineCode.push_back(operand->getValue());
            } else if (operand->isIdentifier()) {
                uint16_t address = symbolTable.getAddress(operand->getName());
                machineCode.push_back(address);
            }
        }
    }

    // Leere Overrides für die restlichen Nodes, um C++ glücklich zu machen
    void visit(RegisterNode& node) override {}
    void visit(ImmediateNode& node) override {}
    void visit(MemoryNode& node) override {}
    void visit(IdentifierNode& node) override {}
};