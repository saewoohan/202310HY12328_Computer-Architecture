/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    int opcode, regA, regB, des, offset;
    int instruction;
    int instruction_count = 0;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    for(int i = 0; i <NUMREGS; i++){
        state.reg[i] = 0;
        state.pc = 0;
    }

    while(1){
        printState(&state);
        instruction = state.mem[state.pc];

        state.pc++;
        instruction_count++;

        if(instruction_count > 200){
            printf("error enough instrcutions\n");
            exit(1);
        }

        opcode = instruction & (7 << 22);
        opcode = opcode >> 22;

        regA = instruction & (7 << 19);
        regA = regA >> 19;

        regB = instruction & (7 << 16);
        regB = regB >> 16;

        //add
        if(opcode == 0) {
            des = instruction & (7);

            state.reg[des] = state.reg[regA] + state.reg[regB];
        }

        //nor
        else if(opcode == 1){
            des = instruction & (7);

            state.reg[des] = ~(state.reg[regA] | state.reg[regB]);
        }

        //lw
        else if(opcode == 2){
            offset = instruction & 0xffff;

            offset = convertNum(offset);
            state.reg[regB] = state.mem[state.reg[regA] + offset];
        }


        //sw
        else if(opcode == 3){
            offset = instruction & 0xffff;

            offset = convertNum(offset);
            state.mem[state.reg[regA] + offset] = state.reg[regB];
        }


        //beq
        else if(opcode == 4){
            offset = instruction & 0xffff;

            offset = convertNum(offset);
            if(state.reg[regA] == state.reg[regB]){
                state.pc = state.pc + offset;
                if(state.pc < 0){
                    printf("error branch to negative address\n");
                    exit(1);
                }
            }
        }


        //jalr
        else if(opcode == 5){
            if(regA == regB){
                continue;
            }
            state.reg[regB] = state.pc;
            state.pc = state.reg[regA];
            printf("%d",state.pc);
            if(state.pc < 0){
                printf("error branch to negative address\n");
                exit(1);
            }
        }


        //halt
        else if(opcode == 6) {
            printf("machine halted\ntotal of %d instrcutions executed\nfinal state of machine:\n", instruction_count);
            printState(&state);
            break;
        }


        //noop
        else if(opcode == 7)
            continue;
        
    }

    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}
