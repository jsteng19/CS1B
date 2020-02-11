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
    

    struct stat st;
    stat(filename, &st);
    off_t size = st.st_size;

    unsigned char* mcode = (unsigned char*)malloc(sizeof(unsigned char*) * size);
    // memset((void*)mcode, 0x00, size);
    FILE *file = fopen(filename, "rb");

    fread(mcode, 1, size, file);
    char* assembly = (char*)malloc(sizeof(char*) * size * 4);
    
    char* pos = assembly;
    for(int i = 0; i < size; i+=4) {

        char* token;
        switch(mcode[i]) {
            case 0x00: 
                token = "nop ";
                break;
                
            case 0x01: 
                token = "add ";
                break;

            case 0x02:
                token = "sub ";  
                break;

            case 0x03:
                token = "and ";
                break;

            case 0x04:
                token = "orr "; 
                break; 

            case 0x05:
                token = "xor ";
                break; 

            case 0x06:
                token = "not ";  
                break; 

            case 0x07:
                token = "lsh ";
                break; 

            case 0x08:
                token = "ash ";
                break;

            case 0x09: {
                token = "tcu ";
                break;
            }

            case 0x0A: {
                token = "tcs ";
                break;
            }

            case 0x0B: { 
                token = "set ";
                break; 
            }

            case 0x0C:
                token = "mov ";
                break; 

            case 0x0D:
                token = "ldw ";
                break;

            case 0x0E:
                token = "stw ";
                break; 

            case 0x0F:
                token = "ldb ";
                break; 

            case 0x10:
                token = "stb ";

            default:
                break;
        }
        

        memcpy(pos, token, 4);
        pos+=4;

        *pos = '\n';
        pos++;
    }

    printf("%s\n", assembly);
    // for(int i = 0; i < 32; i++) {
    //     printf("register %d: %d\n", i, (int32_t)registers[i]);
    // }




    return 0;
}

 char* getRegisterName(char* byte) {
        if(byte == 0) {
            return "pc";
        }
        return "rr";
}