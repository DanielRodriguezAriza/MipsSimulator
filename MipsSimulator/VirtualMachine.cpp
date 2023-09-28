#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VirtualMachine.h"


VirtualMachine::VirtualMachine()
:registers{0},programCounter{0},memory{0},debugEnabled{false}
{}

VirtualMachine::~VirtualMachine()
{}


void VirtualMachine::RunSystemCall()
{
    switch(registers[$v0])
    {
        default:
            printf("Error: Unknown system call!\n");
            exit(-1);
            break;
        case 1:
            if(debugEnabled)
            {
                printf("DEBUG: Syscall 1 (print integer)\n");
            }
            printf("%d",registers[$a0]);
            break;
        case 2:
            if(debugEnabled)
            {
                printf("DEBUG: Syscall 2 (print float)\n");
            }
            printf("%f",*(float *)(&registers[$a0]));
            break;
        case 3:
            if(debugEnabled)
            {
                printf("DEBUG: Syscall 3 (print double)\n");
            }
            printf("%lf",*(double *)(&registers[$a0]));
            break;
        case 4:
            if(debugEnabled)
            {
                printf("DEBUG: Syscall 4 (print NUL terminated string)\n");
            }
            printf("%s",memory + registers[$a0]);
            break;
		case 5:
			if(debugEnabled)
			{
				printf("DEBUG: Syscall 5 (read integer)\n");
			}
			scanf("%d",&registers[$v0]);
			break;
		case 10:
			if(debugEnabled)
			{
				printf("DEBUG: Syscall 10 (exit)\n");
			}
			exit(0); //replace in the future with my own exit impl for the VM so that it does not shut down the whole VM program.
			break;
        case 11:
            if(debugEnabled)
            {
                printf("DEBUG: Syscall 11 (print character)\n");
            }
            printf("%c",registers[$a0]);
            break;
		case 17:
			if(debugEnabled)
			{
				printf("DEBUG: Syscall 17 (exit with return code $a0 = %d)\n",registers[$a0]);
			}
			exit(registers[$a0]);
			break;
    }
}

void VirtualMachine::RunInstruction(Instruction instruction)
{
    switch(instruction.type)
    {
    default:
        printf("Error: Unknown instruction!\n");
        exit(-1);
        break;
    case NOP:
        //do nothing
        break;
    
    case LI:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = %d\n",instruction.r1, instruction.r2);
        }
        registers[instruction.r1] = instruction.r2;
        break;
    
    //addition instructions:
    case ADD:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d + ($r%d) %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3, registers[instruction.r3]);
        }
        registers[instruction.r1] = registers[instruction.r2] + registers[instruction.r3];
        break;
    case ADDI:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d + %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3);
        }
        registers[instruction.r1] = registers[instruction.r2] + instruction.r3;
        break;
    case ADDU:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d + ($r%d) %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3, registers[instruction.r3]);
        }
        *(unsigned int *)(&registers[instruction.r1]) = *(unsigned int *)(&registers[instruction.r2]) + *(unsigned int *)(&registers[instruction.r3]);
        break;
    
    case ADDIU:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d + %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3);
        }
        *(unsigned int *)(&registers[instruction.r1]) = *(unsigned int *)(&registers[instruction.r2]) + *(unsigned int *)(&instruction.r3);
        break;
    
    
    
    case SUB:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d - ($r%d) %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3, registers[instruction.r3]);
        }
        registers[instruction.r1] = registers[instruction.r2] - registers[instruction.r3];
        break;
    case MUL:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d * ($r%d) %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3, registers[instruction.r3]);
        }
        registers[instruction.r1] = registers[instruction.r2] * registers[instruction.r3];
        break;
    case DIV:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d / ($r%d) %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3, registers[instruction.r3]);
        }
        registers[instruction.r1] = registers[instruction.r2] / registers[instruction.r3];
        break;
    case MOVE:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d\n",instruction.r1, instruction.r2 , registers[instruction.r2]);
        }
        registers[instruction.r1] = registers[instruction.r2];
        break;
    
    case SUBI:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d - %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3);
        }
        registers[instruction.r1] = registers[instruction.r2] - instruction.r3;
        break;
    case MULI:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d * %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3);
        }
        registers[instruction.r1] = registers[instruction.r2] * instruction.r3;
        break;
    case DIVI:
        if(debugEnabled)
        {
            printf("DEBUG: $r%d = ($r%d) %d / %d\n",instruction.r1, instruction.r2, registers[instruction.r2], instruction.r3);
        }
        registers[instruction.r1] = registers[instruction.r2] / instruction.r3;
        break;
    
    //jump instructions:
    case JUMP: //jumps unconditionally to the specified memory address.
    case JMP:
    case J:
        programCounter = instruction.r1 - 1; //the minus one is done because the for loop that executes the instructions inside of "RunProgram(...)" causes an increment to the next instruction as soon as this instruction (the jump instruction) is performed, meaning that we goto the instruction that we want to execute, but then we inmediately skip it and execute the next one. This behavior does not happen in a real MIPS architecture computer, so im forced to use this simple trick to solve the issue.
        break;
    case JR: //jumps unconditionally to the memory address stored inside of the specified register.
        programCounter = registers[instruction.r1] - 1; //also requires the same trick as the regular jump.
        break;
    
	case JAL:
		registers[$ra] = programCounter + 1;
		programCounter = registers[instruction.r1] - 1;
		break;
	
    case SYSCALL:
        if(debugEnabled)
        {
            printf("DEBUG: Executing syscall with $v0 = %d and $a0 = %d",registers[$v0],registers[$a0]);
        }
        RunSystemCall();
        break;
    }
}

void VirtualMachine::RunProgram(Instruction *instructions, int num_instructions)
{
    for(programCounter = 0; programCounter < num_instructions; ++programCounter)
    {
        RunInstruction(instructions[programCounter]);
    }
}

void VirtualMachine::RunProgram(std::vector<Instruction> instructions)
{
	for(programCounter = 0; programCounter < instructions.size(); ++programCounter)
	{
		RunInstruction(instructions[programCounter]);
	}
}

void VirtualMachine::LoadBytesIntoMemory(int address, char *bytes, int len)
{
    memcpy(memory + address, bytes, len);
}

//TODO: implement the program as an array that is in the ROM of the VM. That way i can load and unload programs, as well as determine what address the tags correspond to when i want to implement jumps. Also i need to work on the implementation of the stack and the functionality of the stack pointer and such.

#include <iostream>

int getRegisterFromText(std::string const &str)
{
	for(size_t i = 0; i < NUM_REGISTERS; ++i)
	{
		std::string regname1 = RegisterNames[i];
		std::string regname2 = RegisterNamesAlt[i];
		if(str == regname1 || str == regname2)
		{
			std::cout<<"The register is : "<< regname1<< " and i = " << i <<"\n";
			return i;
		}
	}
	return -1;
}
