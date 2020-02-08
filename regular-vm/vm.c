#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


int main() {
    int memsize = 100000;
    

    int numInstructions;
    char* memory = (char*)malloc(sizeof(char*) * memsize); // = {"A"};  does memory need to be 0-initialized?
    memset((void*)memory, 0x00, memsize);
    //memcpy(*memory, *instructions, sizeof(instructions));
    
    const char* filename = "instructions";
    FILE *file = fopen(filename, "rb");

    struct stat st;
    stat(filename, &st);
    off_t size = st.st_size;
    fread(memory, 1, size, file);

    uint32_t registers[32] = {0};
    
    uint32_t instruction;
    uint32_t* pc = &registers[0];

    while(registers[0] > 0 && registers[0] < numInstructions) {
        instruction = memory[*pc];
        uint32_t* rA = &registers[memory[*pc + 1]];
        uint32_t* rB = &registers[memory[*pc + 2]];
        uint32_t* rC = &registers[memory[*pc + 3]];


                switch(instruction) {
            case 0x00:
                break;
                
            case 0x01:
                *rA = *rB + *rC;
                break;

            case 0x02:
                *rA = *rB - *rC;    
                break;

            case 0x03:
                *rA = *rB & *rC;    
                break;

            case 0x04:
                *rA = *rB | *rC;    
                break; 

            case 0x05:
                *rA = *rB ^ *rC;    
                break; 

            case 0x06:
                *rA = ~*rB;    
                break; 

            case 0x07:
                if((int32_t)rC > 0) {
                    *rA = *rB << *rC;
                }
                else {
                    *rA = *rB >> *rC;
                }    
                break; 

            case 0x08:
                if((int32_t)rC > 0) {
                    *rA = *rB << *rC;
                }
                else {
                    *rA = ((int32_t)*rB) >> *rC;
                } 
                break; 

            case 0x09:
                uint32_t sub = *rB - *rC;
                *rA = (sub > 0) - (sub < 0);
                break; 

            case 0x0A:
                uint32_t sub = ((int32_t)*rB) - ((int32_t)*rC);
                *rA = (sub > 0) - (sub < 0);
                break; 

            case 0x0B:
                int16_t* imm;
                memcpy(imm, rB, 2);
                rA = (int32_t)imm;
                break; 

            case 0x0C:
                *rA = *rB;
                break; 

            case 0x0D:
                // current-implemenation-defined behavior: copy 32 bit word even if not word-aligned in memory
                memcpy(rA, memory + *rB, 4);
                break;

            case 0x0E:
                // current-implemenation-defined behavior: copy 32 bit word even if not word-aligned in memory
                memcpy(memory + *rA, rB, 4);
                break; 

            case 0x0F:
                memcpy(rA, memory + *rB, 1);
                break; 

            case 0x10:
                memcpy(memory + *rA, rB, 1);
        }
        
        pc += 4;
    }
    return 0;

}