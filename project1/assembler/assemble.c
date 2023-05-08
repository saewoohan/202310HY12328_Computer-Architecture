/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000
#define MAXLENGTH 51

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int check_duplicate(char *);
int find_label(char *);

typedef struct { 
    char label[MAXLINELENGTH]; 
    char opcode[MAXLINELENGTH]; 
    char arg0[MAXLINELENGTH];
    char arg1[MAXLINELENGTH]; 
    char arg2[MAXLINELENGTH];
} ins;

ins arr[MAXLENGTH];
int lines = 0;
int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}
	/* here is an example for how to use readAndParse to read a line from
		 inFilePtr */
	while(1){
		if (readAndParse(inFilePtr, arr[lines].label, arr[lines].opcode, arr[lines].arg0, arr[lines].arg1, arr[lines].arg2) == 1) {
			if(check_duplicate(arr[lines].label) == 0) {
				printf("error duplicate definition of labels %s\n", arr[lines].label);
				exit(1);
			}
			
			char temp[2]; 
			temp[0] = arr[lines].label[0];
			temp[1] = '\0';
			
			//number error
			if(isNumber(temp)) {
				printf("error undefined labels %s\n", arr[lines].label);
				exit(1);
			}

			if(strlen(arr[lines].label) > 6 ) {
				printf("error undefined labels %s\n", arr[lines].label);
				exit(1);
			}
			lines++;
		}
		else{
			break;
		}
	}

	for(int i = 0 ; i<lines; i++){
		int opcode;
		if(!strcmp(arr[i].opcode, "add")) 
			opcode = 0;
		else if(!strcmp(arr[i].opcode, "nor"))
			opcode = 1;
		else if(!strcmp(arr[i].opcode, "lw"))
			opcode = 2;
		else if(!strcmp(arr[i].opcode, "sw"))
			opcode = 3;
		else if(!strcmp(arr[i].opcode, "beq"))
			opcode = 4;
		else if(!strcmp(arr[i].opcode, "jalr"))
			opcode = 5;
		else if(!strcmp(arr[i].opcode, "halt"))
			opcode = 6;
		else if(!strcmp(arr[i].opcode, "noop"))
			opcode = 7;
		else if(!strcmp(arr[i].opcode, ".fill"))
			opcode = 8;
		else{
            printf("error unrecognized opcodes %s\n", arr[i].opcode);
            exit(1); 
		}

		int A2M = 0;
		A2M = A2M | (opcode << 22);

		//O-type instructions (halt, noop)
		if(opcode == 6 || opcode == 7){

		}

		//.fill instruction
		else if(opcode == 8){
			if(isNumber(arr[i].arg0)){
				A2M = atoi(arr[i].arg0);
			}
			else{
				A2M = find_label(arr[i].arg0);
				if(A2M == -1){
					printf("error use of undefined label %s\n", arr[i].arg0);
					exit(1);
				}
			}
		}

		//R, I, J-type instructions
		else{
			//error check
			if(!isNumber(arr[i].arg0)) {
                printf("error non-integer register arguments %s\n", arr[i].arg0);
                exit(1);
            }
            if(!isNumber(arr[i].arg1)) {
                printf("error non-integer register arguments %s\n", arr[i].arg1);
                exit(1);
            }
	
			int regA = atoi(arr[i].arg0);
			int regB = atoi(arr[i].arg1);

			//error check
			if(regA < 0 || regA > 7) {
                printf("error register outside the range[0, 7] %d\n", regA);
                exit(1);
            }
            if(regB < 0 || regB > 7) {
                printf("error register outside the range[0, 7] %d\n", regB);
                exit(1);
            }

			A2M = A2M | (regA << 19);
			A2M = A2M | (regB << 16);
            
			//R-type instructions (add, nor)
			if(opcode == 0 || opcode == 1){

				if(!isNumber(arr[i].arg2)){
					printf("error non-integer register arguments %s\n", arr[i].arg0);
					exit(1);					
				}
				
				int des = atoi(arr[i].arg2);

				if(des < 0 || des > 7){
					printf("error register outside the range[0, 7] %d\n", regB);
                	exit(1);
				}

				A2M = A2M | des;
			}

			//I-type instructions (lw, sw, beq)
			else if(opcode == 2 || opcode == 3 || opcode == 4){
				int offset;

				if(isNumber(arr[i].arg2)){
					offset = atoi(arr[i].arg2);
				}

				else{
					offset = find_label(arr[i].arg2);
					if(offset == -1){
						printf("error use of undefined labels %s\n", arr[i].arg2);
						exit(1);
					}
					if(opcode == 4){
						offset = offset - (i+1);
					}
				}
                if(offset < -32768 || offset > 32767) {
					printf("error offsetFields that don't fit in 16 bits %d\n", offset);
					exit(1);
				}

				offset = offset & 0xffff;
				A2M = A2M | offset;
			}


		}
		fprintf(outFilePtr, "%d\n", A2M);
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	exit(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	float i;
	int temp = 0;
	temp = sscanf(string, "%f", &i);
	if(temp == 1){
		if(i - (int)i == 0){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

int check_duplicate(char * temp) {
    int i;

    for(i = 0; i < lines; i++) {
		if(arr[i].label[0] == 0){
			continue;
		}
        if(strcmp(arr[i].label, temp) == 0)
            return 0;
    }
    
    return 1;

}

int find_label(char * temp){
	int i;
	for(i = 0 ; i<lines; i++){
		if(arr[i].label[0] == 0){
			continue;
		}
		if(strcmp(arr[i].label, temp) == 0){
			return i;
		}
	}
	return -1;
}

