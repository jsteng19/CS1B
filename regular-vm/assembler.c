#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include "opcodes.h"

unsigned char getRegister(char* token);

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Error: incorrect command line arguments");
        return 1;
    }
    char* filename = argv[1];
    

    struct stat st;
    stat(filename, &st);
    off_t size = st.st_size;

    unsigned char* mcode = (unsigned char*)calloc(size * 2, 1); // Todo: change this allocation size
    FILE *assembly = fopen(filename, "r");


    char line[16];
    char* token;
    unsigned char* instruction = mcode;
    int instructiontype;

    while(fgets(line, sizeof(line), assembly)) {
        token = strtok(line, " ");
        if(!strcmp(token, "nop")) {
            *instruction = NOP;
            instructiontype = 0;
        }
        else if(!strcmp(token, "add")) {
            *instruction = ADD;
            instructiontype = 3;
        }
        else if(!strcmp(token, "sub")) {
            *instruction = SUB;
            instructiontype = 3;
        }
        else if(!strcmp(token, "and")) {
            *instruction = AND;
            instructiontype = 3;
        }
        else if(!strcmp(token, "orr")) {
            *instruction = ORR;
            instructiontype = 3;
        }
        else if(!strcmp(token, "xor")) {
            *instruction = XOR;
            instructiontype = 3;
        }
        else if(!strcmp(token, "not")) {
            *instruction = NOT;
            instructiontype = 2;
        }
        else if(!strcmp(token, "lsh")) {
            *instruction = LSH;
            instructiontype = 3;
        }
        else if(!strcmp(token, "ash")) {
            *instruction = ASH;
            instructiontype = 3;
        }
        else if(!strcmp(token, "tcu")) {
            *instruction = TCU;
            instructiontype = 3;
        }
        else if(!strcmp(token, "tcs")) {
            *instruction = TCS;
            instructiontype = 3;
        }
        else if(!strcmp(token, "set")) {
            *instruction = SET;
            instructiontype = 1;
        }
        else if(!strcmp(token, "mov")) {
            *instruction = MOV;
            instructiontype = 2;
        }
        else if(!strcmp(token, "ldw")) {
            *instruction = LDW;
            instructiontype = 2;
        }
        else if(!strcmp(token, "stw")) {
            *instruction = STW;
            instructiontype = 2;
        }
        else if(!strcmp(token, "ldb")) {
            *instruction = LDB;
            instructiontype = 2;
        }
        else if(!strcmp(token, "stb")) {
            *instruction = STB;
            instructiontype = 2;
        }
        else if(!strcmp(token, "brk")) {
            *instruction = BRK;
            instructiontype = 0;
        }

        switch(instructiontype) {
        
            case 1:
                token = strtok(NULL, " ");
                instruction[1] = getRegister(token);
                token = strtok(NULL, " ");
                int16_t imm = (int16_t)atoi(token);
                memcpy(instruction + 2, &imm, 2);
                break;
                

            case 2:
                token = strtok(NULL, " ");
                instruction[1] = getRegister(token);
                token = strtok(NULL, " ");
                instruction[2] = getRegister(token);
                break;
                
            case 3:
                token = strtok(NULL, " ");
                instruction[1] = getRegister(token);
                token = strtok(NULL, " ");
                instruction[2] = getRegister(token);
                token = strtok(NULL, " ");
                instruction[3] = getRegister(token);
                break;
        }

        instruction += 4;

    }

    char* fileoutname = strtok(filename, ".");
    FILE* mcodefile = fopen(fileoutname, "wb");
    fwrite(mcode, 1, instruction - mcode, mcodefile);

    fclose(assembly);
    fclose(mcodefile);

    return 0;
}

unsigned char getRegister(char* token) {
    if(!strcmp(token, "pc")) {
        return 0;
    }
    if(!strcmp(token, "at")) {
        return 30;
    }
    if(!strcmp(token, "sp")) {
        return 31;
    }
    return (unsigned char)atoi(token + 1);
}