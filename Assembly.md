# Design and Syntax
The syntax and semantics are heavily inspired by the X86 Assembly Language, more precisly the Intel Syntax.<br>
Most of the instrucions follow the x86 design and argument possibilities. <br>

## Registers
### General Purpose Registers
| 16 bits | 8 high bits | 8 low bits | Description        |
|---------|-------------|------------|--------------------|
| AX      | AH          | AL         | Accumulator        |
| BX      | BH          | BL         | Base               |
| CX      | CH          | CL         | Counter            |
| DX      | DH          | DL         | Data               |
| SI      | N/A         | SIL        | Source             |
| DI      | N/A         | DIL        | Destination        |
| SP      | N/A         | N/A        | Stack Pointer      |
| BP      | N/A         | N/A        | Stack Base Pointer |

### Pointer Registers
| 16 bits | 8 high bits | 8 low bits | Description         |
|---------|-------------|------------|---------------------|
| IP      | N/A         | N/A        | Instruction Pointer |

### Flags Register
| Bit | Label | Description   |
|-----|-------|---------------|
| 0   | CF    | Carry Flag    |
| 1   | ZF    | Zero Flag     |
| 2   | SF    | Sign Flag     |
| 3   | OF    | Overflow Flag |

unlisted Bits are reserved and give back 0

### Feature Status Register (FSR)
| Bit | Label | Description          |
|-----|-------|----------------------|
| 0   | IA    | Interrupts Available |
| 1   | PA    | Paging Available     |

unlisted Bits are reserved and give back 0

## Source / Destination
| Source / Destination | Immediates | Register | Pointer | Direct Memory |
|----------------------|------------|----------|---------|---------------|
| Immediates           | X          | ✓        | ✓       | ✓            |
| Register             | X          | ✓        | ✓       | ✓            |
| Pointer              | X          | ✓        | ✓       | ✓            |
| Direct Memory        | X          | ✓        | ✓       | ✓            |

X -> Not allowed <br>
✓ -> Allowed <br>

## Examples for each type Source / Destination
### Immediates
`mov ax, 6`
This will move the Imideate value 6 to the register ax. <br>

### Register
`mov ax, bx `
This will move the register bx to the register ax. <br>

### Pointer
`mov ax, [bx]`
This will move the Pointer value of bx to the register ax. <br>

### Direct Memory
`mov ax, [100h]`
This will move the Value of the Memory at location 0x100 to the register ax. <br>

# List of Instructions
- [General](#General)
    - [NOP](#-NOP)
    - [HLT](#HLT)
    - [PUSH](#PUSH)
    - [POP](#POP)
    - [CFLAGS](#CFLAGS)
    - [PUSHF](#PUSHF)
    - [POPF](#POPF)
    - [MOV](#MOV)
- [Arithmetic](#Arithmetic)
    - [INC](#INC)
    - [DEC](#DEC)
    - [ADD](#ADD)
    - [SUB](#SUB)
    - [SHL](#SHL)
    - [SHR](#SHR)
    - [AND](#AND)
    - [OR](#OR)
    - [XOR](#XOR)
    - [NOT](#NOT)
- [Controll Flow](#Controll-Flow)
    - [JMP](#JMP)
    - [CALL](#CALL)
    - [RET](#RET)
    - [CMP](#CMP)
    - [JCC](#JCC)

## General
- [nop](#NOP)
- [hlt](#HLT)
- [push](#PUSH)
- [pop](#POP)
- [cflags](#CFLAGS)
- [pushf](#PUSHF)
- [popf](#POPF)
- [mov](#MOV)

### NOP
#### OP-Code
Op-Code: 0x00 <br>
ModR/M: N/A

#### Description
Performs no action and advances the program counter to the next instruction.

### HLT
#### OP-Code
Op-Code: 0x01 <br>
ModR/M: N/A

#### Description
Stops the execution of the CPU. The processor enters a halted state and remains there until an external interrupt (or system reset) is received. This is essential for power-saving and clean system shutdown.

### PUSH [Source]
#### OP-Code
Op-Code: 0x03 <br>
ModR/M: N/A

#### Description
Decrements the stack pointer (SP) and copies the [Source] value onto the stack at the address pointed to by SP. <br> <br>
Valid sources: Registers, Immediates. <br>

#### Example
`push ax` -> pushes the value of ax to the stack <br>
`push 8` -> pushes 8 to the stack

### POP [Destination]
#### OP-Code
Op-Code: 0x04 <br>
ModR/M: N/A

#### Description
Copies the value from the top of the stack (pointed to by SP) into the [Destination] and then increments the stack pointer (SP). <br> <br>
Valid sources: Registers. <br>

#### Example
`pop ax` -> Pop the value of the stack and copies it to ax.

### CFLAGS
#### OP-Code
Op-Code: 0x05 <br>
ModR/M: N/A

#### Description
Clears flags register (sets all bits to 0).

#### Affected flags
All Flags (ZF, SF, OF, CF, etc.): Cleared (0).

#### Example
`cflags` -> clears all flags.

### PUSHF
#### OP-Code
Op-Code: 0x06 <br>
ModR/M: N/A

#### Description
Decrements the stack pointer (SP) and copies the 16-bit flags register onto the stack at the address pointed to by SP.

#### Example
`pushf` -> Pushes flags to the Stack for safe storage.

### POPF
#### OP-Code
Op-Code: 0x07 <br>
ModR/M: N/A

#### Description
Copies the 16-bit value from the top of the stack (pointed to by SP) directly into the flags register and then increments the stack pointer (SP).

#### Example
`popf` -> Restores the flags from the stack.

### MOV [Destination], [Source] 
#### OP-Code
Op-Code: 0x08(if no modR/M) or 0x00(with modR/M) <br>
Sub-Code: 0x01 <br>
ModR/M: only if extended registers are used.

#### Description
Copies the source value to the destination. <br> <br>
Valid sources: Registers, Immediates, Pointers, Direct Memory. <br>
Valid destinations: Registers, Immediates, Pointers, Direct Memory.

#### Example
`mov bx, ax` -> copies the register ax to the register bx. <br>
`mov [200h], ax` -> copies the register ax to the memory location 0x200. <br>


## Arithmetic
- [INC](#INC)
- [DEC](#DEC)
- [ADD](#ADD)
- [SUB](#SUB)
- [SHL](#SHL)
- [SHR](#SHR)
- [AND](#AND)
- [OR](#OR)
- [XOR](#XOR)
- [NOT](#NOT)

### INC [Value 1 / Destination]
#### OP-Code
Op-Code: 0x0E <br>
ModR/M: none

#### Description
Adds 1 to the provided register <br> <br>
Valid types Value 1: Registers.

#### Affected flags
ZF -> Is set if awnser is exacly 0. <br>
CF -> Set to 1 if the unsigned addition results in a carry out of the most significant bit (i.e., the result exceeds 16 bits / 65,535). <br>
SF -> Set to 1 if the result is mathematically negative (Bit 15 is 1). <br>
OF -> Set to 1 if there is a signed overflow (e.g., adding to a number results in a negative number, adding to a number negative numbers results in a positive number).

#### Example
`inc ax` -> Adds 1 to the register ax. <br>

### DEC [Value 1 / Destination]
#### OP-Code
Op-Code: 0x0F <br>
ModR/M: none

#### Description
Subtracts 1 from the provided register <br> <br>
Valid types Value 1: Registers.

#### Affected flags
ZF -> Is set if awnser is exacly 0. <br>
CF -> Set to 1 if the unsigned addition results in a carry out of the most significant bit (i.e., the result exceeds 16 bits / 65,535). <br>
SF -> Set to 1 if the result is mathematically negative (Bit 15 is 1). <br>
OF -> Set to 1 if there is a signed overflow (e.g., subtracting from a number results in a negative number, subtracting from a number negative numbers results in a positive number).

#### Example
`dec ax` -> Subtracts 1 from the register ax. <br>

### ADD [Value 1 / Destination], [Value 2]
#### OP-Code
Group: 0x01 <br>
Sub-Code: 0x01 <br>
ModR/M: required

#### Description
Adds value 1 to Value 2 and stores the result in first argument. <br> <br>
Valid types Value 1: Registers.
Valid types Value 2: Immediates, Registers.

#### Affected flags
ZF -> Is set if awnser is exacly 0. <br>
CF -> Set to 1 if the unsigned addition results in a carry out of the most significant bit (i.e., the result exceeds 16 bits / 65,535). <br>
SF -> Set to 1 if the result is mathematically negative (Bit 15 is 1). <br>
OF -> Set to 1 if there is a signed overflow (e.g., adding two positive numbers results in a negative number, or adding two negative numbers results in a positive number).

#### Example
`add ax, 5` -> Adds the value 5 to the register ax. <br>
`add bx, cx` -> Adds bx to cx and stores the result in bx. <br>

### SUB [Value 1 / Destination], [Value 2]
#### OP-Code
Group: 0x01 <br>
Sub-Code: 0x02 <br>
ModR/M: required

#### Description
Subtracts value 1 from Value 2 and stores the result in first argument. <br> <br>
Valid types Value 1: Registers.
Valid types Value 2: Immediates, Registers.

#### Affected flags
ZF -> Is set if awnser is exacly 0. <br>
CF -> Set to 1 if a borrow was required (i.e., treating both as unsigned numbers, <Source> was greater than <Destination>). <br>
SF -> Set to 1 if the result is negative. <br>
OF -> Set to 1 if the signed result cannot fit in 16 bits (signed overflow). <br>

#### Example
`sub ax, 5` -> Subtracts the value 5 from the register ax. <br>
`sub bx, cx` -> Subtracts bx to cx and stores the result in bx. <br>

### SHL [Source] [Amount]
#### OP-Code
Group: 0x01 <br>
Sub-Code: 0x03 <br>
ModR/M: required

#### Description
Shifts the bits in [Source] by n bits supplied in [Amount] left. <br> <br>
Valid types Source: Registers.
Valid types Amount: Immediates, Registers.

#### Affected flags
ZF: Is set if the answer is exactly 0. <br>
SF: Set to 1 if the result is negative (Bit 15 is 1). <br>
OF: Set to 1 if the signed result cannot fit in 16 bits (signed overflow). <br>
CF: Receives the last bit shifted out of the left side (Bit 15). <br>

#### Example
`shl ax, 5` -> Shifts the bits to the left by 5 positions.
`shl ax, bx` -> Shifts the bits to the left by the value stored in bx.

### SHR [Source] [Amount]
#### OP-Code
Group: 0x01 <br>
Sub-Code: 0x04 <br>
ModR/M: required

#### Description
Shifts the bits in [Source] by n bits supplied in [Amount] right. <br>
When Amount is not specified, it defaults to 1. <br>
Valid types Source: Registers.
Valid types Amount: Immediates, Registers.

#### Affected flags
ZF: Is set if the answer is exactly 0. <br>
SF: Set to 1 if the result is negative (Bit 15 is 1). <br>
OF: Set to 1 if the signed result cannot fit in 16 bits (signed overflow). <br>
CF: Receives the last bit shifted out of the right side (Bit 15). <br>

#### Example
`shr ax, 5` -> Shifts the bits to the right by 5 positions.
`shr ax, bx` -> Shifts the bits to the right by the value stored in bx.

### AND [Value 1 / Destination], [Value 2]
#### OP-Code
Group: 0x01 <br>
Sub-Code: 0x05 <br>
ModR/M: required

#### Description
Bitwise and Operation on Value 1 and Value 2. <br>
Valid types Value 1: Registers.
Valid types Value 2: Immediates, Registers.

#### Affected flags
ZF: Is set if the answer is exactly 0.
SF: Set to 1 if the result is negative (Bit 15 is 1).
OF: Cleared (0).
CF: Cleared (0).

#### Example
`and ax, 8h` -> Bit wise and on ax Register with 0x08
`and ax, bx` -> Bit wise and on ax Register with bx Register

### OR [Value 1 / Destination] [Value 2]
#### OP-Code
Group: 0x01 <br>
Sub-Code: 0x06 <br>
ModR/M: required

#### Description
Bitwise or Operation on Value 1 and Value 2. <br>
Valid types Value 1: Registers.
Valid types Value 2: Immediates, Registers.

#### Affected flags
ZF: Is set if the answer is exactly 0.
SF: Set to 1 if the result is negative (Bit 15 is 1).
OF: Cleared (0).
CF: Cleared (0).

#### Example
`or ax, 1` -> Sets bit 0 in ax, rest of bits unchanged 
`or ax, bx` -> Bit wise or on ax Register with bx Register

### XOR [Value 1 / Destination] [Value 2]
#### OP-Code
Group: 0x01 <br>
Sub-Code: 0x07 <br>
ModR/M: required

#### Description
Bitwise xor Operation on Value 1 and Value 2. <br>
Valid types Value 1: Registers.
Valid types Value 2: Immediates, Registers.

#### Affected flags
ZF: Is set if the answer is exactly 0.
SF: Set to 1 if the result is negative (Bit 15 is 1).
OF: Cleared (0).
CF: Cleared (0).

#### Example
`xor ax, 1` -> Clears the register ax (sets it to 0) and sets the Zero Flag. 
`xor ax, bx` -> Performs bitwise XOR on bx and cx, storing the result in bx.

### NOT [Value 1 / Destination]
#### OP-Code
Group: 0x01 <br>
Sub-Code: 0x08 <br>
ModR/M: required

#### Description
Inverts bits in Value 1. Result is stored back in Value 1. <br>
Valid types Value 1: Registers.

#### Affected flags
none

#### Example
`not ax` -> Inverts all bits in ax register.


## Controll Flow
Controll flow must be implemented using JMP or other equivalant instructions. Never directly modify the IP register!

- [JMP](#JMP)
- [CALL](#CALL)
- [RET](#RET)
- [CMP](#CMP)
- [JCC]](#JCC)


### CALL [Destination]
#### OP-Code
Op-Code: 0x0A <br>
ModR/M: N/A

#### Description
Calls a subroutine (function). First, the address of the next instruction (the return address) is pushed onto the stack to save it. Then, the Program Counter (PC) is overwritten with the specified target address to branch to the subroutine.
<br>
Valid types Target: Immediates (Addresses), Labels, Registers.

#### Affected flags
none

#### Examples
```assembly
call label       ; call to 'label'

mov ax, 5       ; code will not execute

label: 
    mov ax, 6   ; execution continues here
``` 
<br>
`call ax ; jumps to addres stored in ax`
`call 100h ; jumps to memmory location 0x100` 

### RET
#### OP-Code
Op-Code: 0x0B <br>
ModR/M: N/A

#### Description
Returns from a subroutine. Pops the 16-bit return address from the top of the stack (pointed to by SP) and loads it directly into the Program Counter (PC). The stack pointer (SP) is then incremented. This transfers control back to the instruction immediately following the original CALL.

#### Affected flags
none

#### Examples
```assembly
call label       ; call to 'label'

mov ax, 5       ; code will execute after call

label: 
    mov ax, 6   ; execution continues here
    ret         ; returns to caller
```

### JMP [Destination]
#### OP-Code
Op-Code: 0x0C <br>
ModR/M: Not available if only registers are used

#### Description
Unconditionally jumps to the specified label or memory address. The Program Counter (PC) is directly overwritten with the target address. <br>
Valid types Destination: Immediates (Addresses), Registers,  Labels.

#### Affected flags
none

#### Examples
```assembly
jmp label       ; jump to 'label'

mov ax, 5       ; code will not execute

label: 
    mov ax, 6   ; execution continues here
``` 
<br>
`jmp ax ; jumps to addres stored in ax`<br>
`jmp 100h ; jumps to memmory location 0x100` 


### CMP [Value 1], [Value 2]
#### OP-Code
Op-Code: 0x0D <br>
ModR/M: required 

#### Description
Compares Value 1 with Value 2 by performing a temporary subtraction (Value 1 - Value 2). The result is discarded, but the flags register is updated based on the outcome. <br>
Valid types Value 1: Registers. <br>
Valid types Value 2: Immediates, Registers.

#### Affected flags
ZF: Is set to 1 if Value 1 equals Value 2 (Result is exactly 0).<br>
SF: Set to 1 if the signed result is negative.<br>
OF: Set to 1 if a signed overflow occurred.<br>
CF: Set to 1 if an unsigned borrow occurred (Value 1 < Value 2 as unsigned).

#### Examples
`cmp ax, 5` -> Compares the contents of ax with 5.<br>
`cmp ax, bx` -> Compares the contents of ax with bx.

### JCC [Target]
#### OP-Code
Group: 0x02 <br>
ModR/M: required

#### Description
Jumps to the specified target address only if the corresponding hardware flag condition is met. If the condition is false, execution continues with the next instruction. <br>
Valid types Target: Immediates (Addresses), Labels, Registers.

#### Conditions Overview
(OpCode: 0x01) JE (Jump if Equal): Jumps if ZF == 1<br>
(OpCode: 0x02) JNE (Jump if Not Equal): Jumps if ZF == 0<br>
(OpCode: 0x03) JG (Jump if Greater Than, Signed): Jumps if ZF == 0 AND SF == OF<br>
(OpCode: 0x04) JL (Jump if Less Than, Signed): Jumps if SF != OF<br>
(OpCode: 0x05) JGE (Jump if Greater or Equal, Signed): Jumps if SF == OF<br>
(OpCode: 0x06) JLE (Jump if Less or Equal, Signed): Jumps if ZF == 1 OR SF != OF<br>

#### Examples
```assembly
cmp ax, bx      ; Compare ax and bx
je match        ; Jump to 'match' if ax == bx

mov cx, 0       ; Executes only if ax != bx
jmp end

match:
    mov cx, 1   ; Executes if ax == bx

end:
```