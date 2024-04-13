//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

struct constant {
    uint8_t value;
    uint8_t memory_local;
    char name[50];
};
struct constant constants[10];
int indexCount = 0;

//Main functions
void storeConstant(char mychar[100], int indexCount, FILE *binFile);
struct constant *findConstantByName(char constantName[50]);
void callNeanderFunction(char mychar[100], FILE *binFile);

int main(void) {

    FILE *assembler = fopen("assembler.txt", "r");
    FILE *binFile = fopen("assembler.bin", "wb");

    uint8_t constants[10];

    char mychar[100];

    bool isInVarSection = false;
    bool isInExecutationSection = false;
    bool didSetUpBinaryVar = false;
    bool didSetupBinaryCode = false;
    
    while (fgets(mychar, 100, assembler) != NULL) {
        if(strcmp(mychar, "var\n") == 0) {
            isInVarSection = true;
            if(didSetUpBinaryVar == false) {
                uint8_t value = 0xFF;
                printf("%d\n", value);
                fwrite(&value, sizeof(uint8_t), 1, binFile);
                didSetUpBinaryVar = true;
            }
        }
        if(strcmp(mychar, "code\n") == 0) {
            isInExecutationSection = true;
            if(didSetupBinaryCode == false) {
                uint8_t value = 0xFE;
                printf("%d\n", value);
                fwrite(&value, sizeof(uint8_t), 1, binFile);
                didSetupBinaryCode = true;
            }
        }
        if(strcmp(mychar, "end\n") == 0 || strcmp(mychar, "end") == 0) {
            uint8_t value = 0xFD;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            isInVarSection = false;
            isInExecutationSection = false;
        }

        if(isInVarSection) {
            if(strcmp(mychar, "var\n") != 0) {
                storeConstant(mychar, indexCount, binFile);
                indexCount++;
            }
        } else if (isInExecutationSection) {
            if(strcmp(mychar, "code\n") != 0) {
                callNeanderFunction(mychar, binFile);
            }
        }
    }

 
    fclose(binFile);
    fclose(assembler);
    return 0;
}
    
void storeConstant(char mychar[100], int indexCount, FILE *binFile) {
    int intValue, intMemoryLocal;
    char name[50];
    
    // Analisando a string de entrada
    sscanf(mychar, "%s %d %d", name, &intValue, &intMemoryLocal);
    // printf("Armazenou a variável %s com valor inteiro %d no local de memória %d\n", name, intValue, intMemoryLocal);
    uint8_t value, memoryLocal;
    value = intValue;
    memoryLocal = intMemoryLocal;
    constants[indexCount].value = value;
    constants[indexCount].memory_local = memoryLocal;
    strcpy(constants[indexCount].name, name);

    fwrite(&value, sizeof(uint8_t), 1, binFile);
    printf("%d\n", value);
    fwrite(&memoryLocal, sizeof(uint8_t), 1, binFile);
    printf("%d\n", memoryLocal);
}

struct constant *findConstantByName(char constantName[50]) {
    for(int i = 0; i < indexCount; i++) {
        if((strcmp(constants[i].name, constantName) == 0)) {
            return &constants[i];
        }
    }
    return NULL;
}

void callNeanderFunction(char mychar[100], FILE *binFile) {
    char funcName[4];
    char constant[50];
    if (strcmp(mychar, "NOP\n") == 0) {
        uint8_t value = 0X03;
        printf("%d\n", value);
        fwrite(&value, sizeof(uint8_t), 1, binFile);
    } else if (strcmp(mychar, "HLT\n") == 0) {
        uint8_t value = 0X04;
        printf("%d\n", value);
        fwrite(&value, sizeof(uint8_t), 1, binFile);
    } else if (strcmp(mychar, "NOT\n") == 0) {
        uint8_t value = 0X05;
        printf("%d\n", value);
        fwrite(&value, sizeof(uint8_t), 1, binFile);
    } else if (sscanf(mychar, "%s %s", funcName, constant)) {
        int constantInteger;
        uint8_t end;

        struct constant *variable = findConstantByName(constant);
        if(variable == NULL) {
            end = atoi(constant); 
        } else {
            end = variable->memory_local;
        }

        if (strcmp(funcName, "STA") == 0) {
            uint8_t value = 0X06;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);
        } else if (strcmp(funcName, "LDA") == 0) {
            uint8_t value = 0X07;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);   
        } else if (strcmp(funcName, "ADD") == 0) {
            uint8_t value = 0X08;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);
        } else if (strcmp(funcName, "OR") == 0) {
            uint8_t value = 0X09;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);
        } else if (strcmp(funcName, "AND") == 0) {
            uint8_t value = 0X0A;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);
        } else if (strcmp(funcName, "JMP") == 0) {
            uint8_t value = 0X0B;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);
        } else if (strcmp(funcName, "JN") == 0) {
            uint8_t value = 0X0C;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);
        } else if (strcmp(funcName, "JZ") == 0) {
            uint8_t value = 0X0D;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);
        } else if (strcmp(funcName, "JNZ") == 0) {
            uint8_t value = 0X0E;
            printf("%d\n", value);
            fwrite(&value, sizeof(uint8_t), 1, binFile);
            fwrite(&end, sizeof(uint8_t), 1, binFile);
        } else {
            printf("Invalid instruction: %s\n", mychar);
        }   
    } else {
        printf("Invalid instruction: %s\n", mychar);
    }
}
