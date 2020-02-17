#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "opcodes.h"


int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Error: incorrect command line arguments");
        return 1;
    }
    const char* filename = argv[1];
    int memsize = 100000;
    unsigned char* memory = (unsigned char*)calloc(memsize, 1);
    
    FILE *file = fopen(filename, "rb");

    struct stat st; // or lseek
    stat(filename, &st);
    off_t size = st.st_size;
    fread(memory, 1, size, file);

    uint32_t registers[32] = {};
    uint32_t* pc = registers;
    uint32_t* sp = registers + 31; 
    *sp = size; // Todo: find a way to avoid this


    while(memory[*pc] != 0xFF) {
        
        unsigned char* rA = memory + *pc + 1;
        unsigned char* rB = memory + *pc + 2;
        unsigned char* rC = memory + *pc + 3;
        
        switch(memory[*pc]) {
            case NOP: 
                break;
                
            case ADD: 
                registers[*rA] = registers[*rB] + registers[*rC];
                break;

            case SUB:
                registers[*rA] = registers[*rB] - registers[*rC];    
                break;

            case AND:
                registers[*rA] = registers[*rB] & registers[*rC];
                break;

            case ORR:
                registers[*rA] = registers[*rB] | registers[*rC]; 
                break; 

            case XOR:
                registers[*rA] = registers[*rB] ^ registers[*rC]; 
                break; 

            case NOT:
                registers[*rA] = ~registers[*rB];    
                break; 

            case LSH:
                if((int32_t)registers[*rC] > 0) {
                    registers[*rA] = registers[*rB] << (int32_t)registers[*rC];
                }
                else {
                    registers[*rA] = registers[*rB] >> abs((int32_t)registers[*rC]);
                }    
                break; 

            case ASH:
                if((int32_t)rC > 0) {
                    registers[*rA] = (int32_t)registers[*rB] << (int32_t)registers[*rC];
                }
                else {
                    *rA = ((int32_t)registers[*rB]) >> abs((int32_t)registers[*rC]);  // or negative
                } 
                break;

            case TCU: {
                int32_t sub = registers[*rB] - registers[*rC];
                registers[*rA] = (sub > 0) - (sub < 0);
                break; 
            }

            case TCS: {
                int32_t sub = ((int32_t)registers[*rB]) - ((int32_t)registers[*rC]);
                registers[*rA] = (sub > 0) - (sub < 0);
                break; 
            }

            case SET: { 
                memcpy(registers + *rA, rB, 2); // or rB and rC
                int16_t imm = registers[*rA];
                registers[*rA] = (int32_t)imm;
                
                uint16_t left = ((uint16_t)(*rB)) << 8;
                uint16_t right = (uint16_t)(*rC);
                registers[*rA] = (uint32_t)(left | right);
                break; 
            }

            case MOV:
                registers[*rA] = registers[*rB];
                break; 

            case LDW:
                // current-implementation-defined behavior: copy 32 bit word even if not word-aligned in memory
                memcpy(registers + *rA, memory + registers[*rB], 4);
                break;

            case STW:
                // current-implementation-defined behavior: copy 32 bit word even if not word-aligned in memory
                memcpy(memory + registers[*rA], registers + *rB, 4);
                break; 

            case LDB:
                memcpy(registers + *rA, memory + registers[*rB], 1);
                break; 

            case STB:
                memcpy(memory + registers[*rA], registers + *rB, 1);

            default:
                break;
        }
        
        *pc += 4;
    }

    // printf("%d\n", *sp);
    for(int i = 0; i < 32; i++) {
        printf("register %d: %d\n", i, (int32_t)registers[i]);
    }




    return 0;
}