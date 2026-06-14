# Encoding Design
Each instruction is at least 2 bytes. <br>
Instructions are grpuped as follows: <br>
- Group 0: General
- Group 1: Arithmetic
- Group 2: Controll Flow

All other Groups are reserved and should not be used!
<br>
The first byte in each instruction is made up as follows:

## Op-Code
| Bits  | Description                                                |
|-------|------------------------------------------------------------|
| 0     | modR/M Follows                                             |
| 1-5   | Group if bit 0 = 1; OpCode if bit 0 = 0                    |
| 6-8   | Source                                                     |
| 9-11  | Destination                                                |
| 12-15 | unused If bit 0 = 1; continuation of bits 1-5 if bit 0 = 0 |

<br>

## ModR/M
| Group | Description                                 |
|-------|---------------------------------------------|
| 0     | Continue                                    |
| 1-5   | OpCode if bit 0 = 0; Sub Group if bit 0 = 1 |
| 6-7   | additional argument count                   |
| 8-11  | additional argument 1 type                  |
| 12-15 | additional argument 2 type                  |

If opcode is 0, no sub group will be ignored and will be used to add arguments if needed. <br>
Stacking ModR/M Words is valid, as long as the last word, does not contain the Continue bit.
<br>

| Hex | Name      | Description                                                                                                                                        |
|-----|-----------|----------------------------------------------------------------------------------------------------------------------------------------------------|
| 0x0 | Register  | Selector as Register. Register is selected by first word bits 6-11.                                                                                |
| 0x1 | Immediate | Selects an Immediate value as an argument.                                                                                                         |
| 0x2 | Memory    | Next word is a Memory Address and loads it as an argument.                                                                                         |
| 0x3 | Pointer   | Treats register as Pointer and loads Memory value from address.                                                                                    |
| 0xF | Extension | Needed if more than 2 arguments are required. Type 2 will be treated as a Register index. <br> Only valid if applied on Type 1. Otherwise ignored. |

