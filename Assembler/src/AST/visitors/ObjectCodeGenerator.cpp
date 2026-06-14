#include "AST/visitors/ObjectCodeGenerator.h"

#include "Lexer/Mnemonics.h"

uint16_t ObjectCodeGenerator::generateInstructionWord(const InstructionNode& node, bool requiresModRM, const InstructionEncoding& encoding) {
    uint16_t word = 0x0000;

    if (requiresModRM) {
        word |= 0x0001;
    }

    uint16_t mainCode = (!encoding.code.empty()) ? encoding.code[0] : 0x00;
    word |= (mainCode & 0x1F) << 1;

    uint16_t srcRegCode = 0;
    uint16_t destRegCode = 0;

    if (node.operands.size() > 0 && node.operands[0]->isRegister()) {
        destRegCode = getRegisterCode(node.operands[0]->getName());
    }

    if (node.operands.size() > 1 && node.operands[1]->isRegister()) {
        srcRegCode = getRegisterCode(node.operands[1]->getName());
    }

    word |= (srcRegCode & 0x07) << 6;
    word |= (destRegCode & 0x07) << 9;

    return word;
}

uint16_t ObjectCodeGenerator::generateModRMWord(const InstructionNode& node, uint16_t additionalArgs, const InstructionEncoding& encoding) {
    uint16_t modrm = 0x0000;

    // Bit 0: Continue Flag (bleibt vorerst 0)

    // Bits 1-5: Der Subcode aus der Gruppe!
    // Sicherheits-Check: Nur wenn es eine Gruppe ist UND der Vektor wirklich ein zweites Element hat
    if (encoding.isGroup && encoding.code.size() > 1) {
        uint16_t subCode = encoding.code[1];
        modrm |= (subCode & 0x1F) << 1;
    }

    // Bits 6-7: Additional Argument Count (0 bis 3)
    modrm |= (additionalArgs & 0x03) << 6;

    // ... Zukünftig Bits 8-15 für die Typen der Operanden ...

    return modrm;
}

ObjectCodeGenerator::InstructionEncoding ObjectCodeGenerator::getOpcode(const InstructionNode& node) {
    switch (node.mnemonic) {
        case Mnemonics::NOP:
            return {{0x00}, false};
        case Mnemonics::HLT:
            return {{0x01}, false};
        case Mnemonics::PUSH:
            return {{0x03}, false};
        case Mnemonics::POP:
            return {{0x04}, false};
        case Mnemonics::CFLAGS:
            return {{0x05}, false};
        case Mnemonics::PUSHF:
            return {{0x06}, false};
        case Mnemonics::POPF:
            return {{0x07}, false};
        case Mnemonics::MOV:
            if (node.operands.size() == 2) {
                if (node.operands[0]->isRegister() && node.operands[1]->isRegister())
                    return {{0x08}, false};
            } else
                return {{0x01, 0x00}, true};
            break;
        case Mnemonics::INC:
            return {{0x0D}, false};
        case Mnemonics::DEC:
            return {{0x0F}, false};

        case Mnemonics::ADD:
            return {{0x01, 0x01}, true};
        case Mnemonics::SUB:
            return {{0x01, 0x02}, true};
        case Mnemonics::SHL:
            return {{0x01, 0x03}, true};
        case Mnemonics::SHR:
            return {{0x01, 0x04}, true};
        case Mnemonics::AND:
            return {{0x01, 0x05}, true};
        case Mnemonics::OR:
            return {{0x01, 0x06}, true};
        case Mnemonics::XOR:
            return {{0x01, 0x07}, true};
        case Mnemonics::NOT:
            return {{0x01, 0x08}, true};
        
        case Mnemonics::CALL:
            return {{0x0A}, false};
        case Mnemonics::RET:
            return {{0x0B}, false};
        case Mnemonics::JMP:
            return {{0x0C}, false};
        case Mnemonics::CMP:
            return {{0x0D}, false};

        case Mnemonics::JE:
            return {{0x02, 0x01}, true};
        case Mnemonics::JNE:
            return {{0x02, 0x02}, true};
        case Mnemonics::JG:
            return {{0x02, 0x03}, true};
        case Mnemonics::JL:
            return {{0x02, 0x04}, true};
        case Mnemonics::JGE:
            return {{0x02, 0x05}, true};
        case Mnemonics::JLE:
            return {{0x02, 0x06}, true};
        default:
            return {{0x00}, false};
    }
}