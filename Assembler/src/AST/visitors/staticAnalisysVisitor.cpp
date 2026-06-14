#include "AST/visitors/staticAnalisysVisitor.h"

void StaticAnalysisVisitor::visit(InstructionNode& node) {
    // 1. Zuerst gehen wir tief in die Kinder, falls dort Fehler versteckt sind
    for (auto& operand : node.operands) {
        operand->accept(*this);
    }

    // 2. Jetzt prüfen wir die Logik des Befehls selbst
    switch (node.mnemonic) {
        case Mnemonics::NOP:
        case Mnemonics::HLT:
        case Mnemonics::CFLAGS:
        case Mnemonics::PUSHF:
        case Mnemonics::POPF:
        case Mnemonics::RET:
            if (node.operands.size() > 0)
                addError(node.mnemonicName + " has no operands", node.line);
            break;

        case Mnemonics::PUSH:
        case Mnemonics::POP:
        case Mnemonics::NOT:
            if (node.operands.size() != 1)
                addError(node.mnemonicName + " has only one operands", node.line);

            else if ((node.mnemonic == Mnemonics::POP || node.mnemonic == Mnemonics::NOT) && node.operands[0]->isImmediate()) {
                addError("Destination for " + node.mnemonicName + " cannot be an immediate value", node.line);
            }

            break;
        case Mnemonics::MOV:
        case Mnemonics::SHL:
        case Mnemonics::SHR:
        case Mnemonics::ADD:
        case Mnemonics::SUB:
        case Mnemonics::AND:
        case Mnemonics::OR:
        case Mnemonics::XOR:
        case Mnemonics::CMP:
            if (node.operands.size() != 2) {
                addError(node.mnemonicName + " expects exactly 2 operands", node.line);
            } else {
                if (node.operands[0]->isImmediate()) {
                    addError("Destination for " + node.mnemonicName + " cannot be an immediate value", node.line);
                }
                if (node.operands[0]->isMemory() && node.operands[1]->isMemory()) {
                    addError("Memory-to-Memory operations are not allowed for " + node.mnemonicName, node.line);
                }

                if (node.mnemonic == Mnemonics::MOV && node.operands[0]->isRegister() && node.operands[1]->isRegister()) {
                    RegisterNode& dest = dynamic_cast<RegisterNode&>(*node.operands[0]);
                    RegisterNode& src = dynamic_cast<RegisterNode&>(*node.operands[1]);

                    if (getWidth(src.name) > getWidth(dest.name)) {
                        addError("Register width mismatch: " + src.name + " (" +
                                     std::to_string((((int)getWidth(src.name)) + 1) * 8) + " bits) is too large for " +
                                     dest.name + " (" + std::to_string((((int)getWidth(dest.name)) + 1) * 8) + " bits)",
                                     node.line);
                    }
                }
            }
            break;
        case Mnemonics::CALL:
        case Mnemonics::JMP:
        case Mnemonics::JE:
        case Mnemonics::JNE:
        case Mnemonics::JG:
        case Mnemonics::JL:
        case Mnemonics::JGE:
        case Mnemonics::JLE:
            if (node.operands.size() != 1) {
                addError(node.mnemonicName + " expects exactly 1 jump target", node.line);
            } else if (node.operands[0]->isImmediate()) {
                addError("Direct immediate jumps are discouraged, use a label for " + node.mnemonicName, node.line);
            }
            break;
        default:
            break;
    }
}

void StaticAnalysisVisitor::visit(LabelDefNode& node) {
    if(node.name.length() > 32)
        addError("lable cant be longer than 32 characters")
}