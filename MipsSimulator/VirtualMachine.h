#ifndef DRA_MIPS_VM_H
#define DRA_MIPS_VM_H

#include <ostream>
#include <vector>

enum InstructionType
{
    // DIVI : this one doesnt actually exist in mips but i want to create it for consistency sake.
    UNKNOWN_INSTRUCTION = 0,
	NOP,
    LI,
    ADD,ADDI,ADDU,ADDIU,
    SUB,SUBI,SUBU,SUBIU,
    MUL,MULI,
    DIV,DIVI,
    MOVE,
    SYSCALL,
    JUMP,JMP,J,
    JR,JAL,
    NUM_INSTRUCTIONS,
};

char const *const InstructionNames[] = {
	"UNKNOWN_INSTRUCTION",
	"NOP",
    "LI",
    "ADD","ADDI","ADDU","ADDIU",
    "SUB","SUBI","SUBU","SUBIU",
    "MUL","MULI",
    "DIV","DIVI",
    "MOVE",
    "SYSCALL",
    "JUMP","JMP","J",
    "JR","JAL",
    "NUM_INSTRUCTIONS",
};

//optimize this struct by adding a few functions that allows me to do bitwise operations to extract the type, values from registers, etc... so that i can use a single int to store everything. This comes with the added limitation that i will need multiple li instructions with the $at register when translating the interpreted mips code into actual instructions, because i can only have 16 bits worth of immediates if i actually do what the hardware does in real life.
struct Instruction
{
public:
    unsigned int type, r1, r2, r3;
	
	friend std::ostream &operator<<(std::ostream &os, Instruction const &inst)
	{
		os<<"Instruction : { " << InstructionNames[inst.type] << ", r1 = " << inst.r1 << ", r2 = "<< inst.r2 << ", r3 = " << inst.r3 << " }";
		return os;
	}
};

class VirtualMachine
{
public:
	
	VirtualMachine();
	
	inline void setDebugEnabled(bool b) {this->debugEnabled = b;}
	
	inline void setProgramCounter(unsigned int pc) {this->programCounter = pc;}
	
	inline void setRegister(size_t index, unsigned int value) {this->registers[index] = value;}
	
	inline unsigned int getRegister(size_t index) const {return this->registers[index];}
	
	inline unsigned int getProgramCounter() const {return this->programCounter;}
	
	inline bool isDebugEnabled() const {return this->debugEnabled;}
	
	inline char *getMemory() {return this->memory;}
	
	~VirtualMachine();

public:

	void RunSystemCall();
	void RunInstruction(Instruction instruction);
	void RunProgram(Instruction *instructions, int num_instructions);
	void LoadBytesIntoMemory(int address, char *bytes, int len);
	
	void RunProgram(std::vector<Instruction> instructions);
	
	//need to rework the run program stuff so that it reads it from wherever the program is loaded inside of the memory array.

public:
    unsigned int registers[32];
    unsigned int programCounter;
    char memory[32000];
    bool debugEnabled;
};

enum Registers
{
    $zero = 0,
    $at,
    $v0,$v1,
    $a0,$a1,$a2,$a3,
    $t0,$t1,$t2,$t3,$t4,$t5,$t6,$t7,
    $s0,$s1,$s2,$s3,$s4,$s5,$s6,$s7,
    $t8,$t9,
    $k0,$k1,
    $gp,
    $sp,
    $fp,
    $ra,
	NUM_REGISTERS //in mips its always 32
};

char const * const RegisterNames[] = {
	"zero",
    "at",
    "v0","v1",
    "a0","a1","a2","a3",
    "t0","t1","t2","t3","t4","t5","t6","t7",
    "s0","s1","s2","s3","s4","s5","s6","s7",
    "t8","t9",
    "k0","k1",
    "gp",
    "sp",
    "fp",
    "ra"
};

char const * const RegisterNamesAlt[] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15","r16","r17","r18","r19","r20","r21","r22","r23","r24","r25","r26","r27","r28","r29","r30","r31"};

int getRegisterFromText(std::string const &str);

#endif