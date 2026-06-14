#include "AST/visitors/symbolTableVisitor.h"

uint16_t SymbolTableVisitor::estimateInstructionSize(const InstructionNode& node) {
    uint16_t size = 2; // Basis: Das 16-Bit Op-Code Wort
    
    bool requiresModRM = false;
    uint16_t additionalArgumentCount = 0; // Das wird später dein Bit 6-7 im ModR/M!

    for (const auto& operand : node.operands) {
        if (operand->isRegister()) {
            continue; 
        } 
        else if (operand->isImmediate()) {
            requiresModRM = true;
            additionalArgumentCount++;
        }
        else if (operand->isMemory()) {
            requiresModRM = true;
            additionalArgumentCount++; 
        }
        else if(operand->isIdentifier()) {
            requiresModRM = true;
            additionalArgumentCount++; 
        }
    }

    // Wenn der Befehl ein ModR/M Wort verlangt
    if (requiresModRM) {
        size += 2; 
    }

    // Jedes "Additional Argument" frisst exakt ein 16-Bit Wort (2 Bytes)
    size += (additionalArgumentCount * 2);

    return size;
}