#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Error: incorrect command line arguments");
        return 1;
    }
    const char* filename = argv[1];
    int memsize = 100000;
    unsigned char* memory = (unsigned char*)malloc(sizeof(unsigned char*) * memsize);
    memset((void*)memory, 0x00, memsize);
    
    FILE *file = fopen(filename, "rb");

    struct stat st;
    stat(filename, &st);
    off_t size = st.st_size;
    size_t sizet = (size_t)size;
    printf("size: %u\n", sizet);
    fread(memory, 1, size, file);

    uint32_t registers[32] = {0};

    uint32_t* pc = &registers[0];

    while(memory[*pc] != 0xFF) {
        
        unsigned char* rA = memory + *pc + 1;
        unsigned char* rB = memory + *pc + 2;
        unsigned char* rC = memory + *pc + 3;
        
        switch(memory[*pc]) {
            case 0x00: //NOP
                break;
                
            case 0x01: 
                registers[*rA] = registers[*rB] + registers[*rC];
                break;

            case 0x02:
                registers[*rA] = registers[*rB] - registers[*rC];    
                break;

            case 0x03:
                registers[*rA] = registers[*rB] & registers[*rC];
                break;

            case 0x04:
                registers[*rA] = registers[*rB] | registers[*rC]; 
                break; 

            case 0x05:
                registers[*rA] = registers[*rB] ^ registers[*rC]; 
                break; 

            case 0x06:
                registers[*rA] = ~registers[*rB];    
                break; 

            case 0x07:
                if((int32_t)registers[*rC] > 0) {
                    registers[*rA] = registers[*rB] << (int32_t)registers[*rC];
                }
                else {
                    registers[*rA] = registers[*rB] >> abs((int32_t)registers[*rC]);
                }    
                break; 

            case 0x08:
                if((int32_t)rC > 0) {
                    registers[*rA] = (int32_t)registers[*rB] << (int32_t)registers[*rC];
                }
                else {
                    *rA = ((int32_t)registers[*rB]) >> abs((int32_t)registers[*rC]);
                } 
                break;

            case 0x09: {
                int32_t sub = registers[*rB] - registers[*rC];
                registers[*rA] = (sub > 0) - (sub < 0);
                break; 
            }

            case 0x0A: {
                int32_t sub = ((int32_t)registers[*rB]) - ((int32_t)registers[*rC]);
                registers[*rA] = (sub > 0) - (sub < 0);
                break; 
            }

            case 0x0B: { //set
                memcpy(registers + *rA, rB, 2);
                int16_t imm = registers[*rA];
                // printf("setting %d to %d\n", *rA, imm);
                registers[*rA] = (int32_t)imm;
                break; 
            }

            case 0x0C:
                registers[*rA] = registers[*rB];
                break; 

            case 0x0D:
                // current-implementation-defined behavior: copy 32 bit word even if not word-aligned in memory
                memcpy(registers + *rA, memory + registers[*rB], 4);
                break;

            case 0x0E:
                // current-implementation-defined behavior: copy 32 bit word even if not word-aligned in memory
                memcpy(memory + registers[*rA], registers + *rB, 4);
                break; 

            case 0x0F:
                memcpy(registers + *rA, memory + registers[*rB], 1);
                break; 

            case 0x10:
                memcpy(memory + registers[*rA], registers + *rB, 1);

            default:
                break;
        }
        
        *pc += 4;
    }

    for(int i = 0; i < 32; i++) {
        printf("register %d: %d\n", i, (int32_t)registers[i]);
    }




    return 0;
}