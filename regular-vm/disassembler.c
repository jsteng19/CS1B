#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

char* getRegisterName(char byte);

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Error: incorrect command line arguments");
        return 1;
    }
    const char* filename = argv[1];
    

    struct stat st;
    stat(filename, &st);
    off_t size = st.st_size;

    unsigned char* mcode = (unsigned char*)malloc(sizeof(unsigned char*) * size);
    // memset((void*)mcode, 0x00, size);
    FILE *file = fopen(filename, "rb");

    fread(mcode, 1, size, file);
    char assembly[size * 4];
    
    char* pos = assembly;
    for(int i = 0; i < size; i+=4) {

        char* token;
        int instructiontype;
        char* r1 = NULL;
        char* r2 = NULL;
        char* r3 = NULL;
        char* imm = NULL;

        switch(mcode[i]) {
            case 0x00: 
            instructiontype = 0;
                token = "nop ";
                break;
                
            case 0x01: 
                token = "add ";
                instructiontype = 3;
                break;

            case 0x02:
                token = "sub ";  
                instructiontype = 3;
                break;

            case 0x03:
                token = "and ";
                instructiontype = 3;
                break;

            case 0x04:
                token = "orr "; 
                instructiontype = 3;
                break; 

            case 0x05:
                token = "xor ";
                instructiontype = 3;
                break; 

            case 0x06:
                token = "not ";  
                instructiontype = 2;
                break; 

            case 0x07:
                token = "lsh ";
                instructiontype = 3;
                break; 

            case 0x08:
                token = "ash ";
                instructiontype = 3;
                break;

            case 0x09: {
                token = "tcu ";
                instructiontype = 3;
                break;
            }

            case 0x0A: {
                token = "tcs ";
                instructiontype = 3;
                break;
            }

            case 0x0B: { 
                token = "set ";
                instructiontype = 1;
                break; 
            }

            case 0x0C:
                token = "mov ";
                instructiontype = 2;
                break; 

            case 0x0D:
                token = "ldw ";
                instructiontype = 2;
                break;

            case 0x0E:
                token = "stw ";
                instructiontype = 2;
                break; 

            case 0x0F:
                token = "ldb ";
                instructiontype = 2;
                break; 

            case 0x10:
                token = "stb ";
                instructiontype = 2;

            case 0xff:
                token = "brk ";
                instructiontype = 0;

            default:
                break;
        }

        strcat(assembly, token);

        switch(instructiontype) {
        
            case 1:
                r1 = getRegisterName(mcode[i + 1]);
                // int16_t imm_int;
                // memcpy(&imm_int, mcode + 2, 2);
                imm = (char*)malloc(sizeof(char*) * 4);
                sprintf(imm, "%d", (int16_t)mcode[i + 2]);
                strcat(assembly, r1);
                strcat(assembly, imm);
                break;
                

            case 2:
                r1 = getRegisterName(mcode[i + 1]);
                r2 = getRegisterName(mcode[i + 2]);
                strcat(assembly, r1);
                strcat(assembly, r2);
                break;
                
            case 3:
                r1 = getRegisterName(mcode[i + 1]);
                r2 = getRegisterName(mcode[i + 2]);
                r3 = getRegisterName(mcode[i + 3]);
                strcat(assembly, r1);
                strcat(assembly, r2);
                strcat(assembly, r3);
                break;
        }

        strcat(assembly, "\n");
    }

    printf("%s\n", assembly);

    return 0;
}

 char* getRegisterName(char byte) {
        
        switch(byte) {
            case 0:
                return "pc  ";
            case 31:
                return "sp  ";
            case 30:
                return "at  ";
        }

        char *reg = (char*)malloc(sizeof(char*) * 4);
        reg[0] = 'r';
        sprintf(reg + 1, "%d", byte);
        if(byte <= 9) {
            reg[2] = ' ';
        }
        reg[3] = ' ';
        return reg;

}