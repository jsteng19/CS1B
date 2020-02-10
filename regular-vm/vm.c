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
    fread(memory, 1, size, file);

    uint32_t registers[32] = {0};

    uint32_t* pc = &registers[0];

    while(memory[*pc] != 0xFF) {
        
        unsigned char* rA = memory + *pc + 1;
        unsigned char* rB = memory + *pc + 2;
        unsigned char* rC = memory + *pc + 3;
        printf("%x\n", memory[*pc]);
        
        switch(memory[*pc]) {
            case 0x00: //NOP
                printf("nop");
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
                if((int32_t)rC > 0) {
                    registers[*rA] = registers[*rB] << registers[*rC];
                }
                else {
                    registers[*rA] = registers[*rB] >> registers[*rC];
                }    
                break; 

            case 0x08:
                if((int32_t)rC > 0) {
                    registers[*rA] = registers[*rB] << registers[*rC];
                }
                else {
                    *rA = ((int32_t)registers[*rB]) >> registers[*rC];
                } 
                break;

            case 0x09: {
                uint32_t sub = registers[*rB] - registers[*rC];
                registers[*rA] = (sub > 0) - (sub < 0);
                break; 
            }

            case 0x0A: {
                uint32_t sub = ((int32_t)registers[*rB]) - ((int32_t)registers[*rC]);
                registers[*rA] = (sub > 0) - (sub < 0);
                break; 
            }

            case 0x0B: {
                printf("set");
                int16_t* imm;
                memcpy(imm, rB, 2);
                *rA = (int32_t)*imm;
                break; 
            }

            case 0x0C:
                registers[*rA] = registers[*rB];
                break; 

            case 0x0D:
                // current-implemenation-defined behavior: copy 32 bit word even if not word-aligned in memory
                memcpy(registers + *rA, memory + registers[*rB], 4);
                break;

            case 0x0E:
                // current-implemenation-defined behavior: copy 32 bit word even if not word-aligned in memory
                memcpy(memory + registers[*rA], registers + *rB, 4);
                break; 

            case 0x0F:
                memcpy(registers + *rA, memory + registers[*rB], 1);
                break; 

            case 0x10:
                memcpy(memory + registers[*rA], registers + *rB, 1);
        }
        
        *pc += 4;
    }

    for(int i = 0; i < 32; i++) {
        printf("register %d: %u\n", i, registers[i]);
    }


    return 0;
}