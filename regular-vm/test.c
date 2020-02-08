#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


int main() {
    char c = 0x00;
    uint32_t registers[32] = {0}; 
    char* memory = (char*)malloc(sizeof(char*) * 100);
    memset((void*)memory, 0x00, 100);

    registers[0] = 0x1000001;
    uint32_t* r = &registers[0];
    memcpy(memory + 1, r, 1);

    printf("%u\n", memory[1]);

    // uint32_t uint = -5;
    // int32_t sum = 1 + uint;
    // printf("sum: %d\n", sum);
    // int32_t sint = (int32_t)uint;
    // printf("%u", sint);
    // printf("%d", (int32_t)uint);

    // unsigned int i = 1; 
    // char *c = (char*)&i; 
    // if (*c)     
    //     printf("Little endian"); 
    // else
    //     printf("Big endian"); 
    // return 0;

    // int16_t uint = 135;
    // printf("%d\n", uint);
    // int32_t res = (int32_t)uint;
    // printf("%d\n", res);

    // int sub = 1324334;
    // int res = (sub > 0) - (sub < 0);    
    // printf("%d",res);
}