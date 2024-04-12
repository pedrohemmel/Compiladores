//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//modelos
struct constant {
    int value;
    uint8_t memory_local;
    char name[50];
};
 
uint8_t ac = 0;
int pc = 0;
bool n = false;
bool z = false;

uint8_t m[256];

struct constant constants[10];
int indexCount = 0;

// Função para verificar se o resultado é negativo ou zero
void verifyNOrZ() {
    n = (ac & 0x80) != 0; 
    z = ac == 0; 
}

// Função para converter o valor de ac para o formato de complemento de dois
int8_t twosComplement(uint8_t value) {
    if (value & 0x80) { 
        return -(256 - value); 
    } else {
        return value; 
    }
}

int HLT(){return -1;}
int NOP(){
    pc++;
    return 0;
}
int STA(unsigned int end) {
    m[end] = ac;
    pc += 2;
    return 0;
}
int LDA(unsigned int end) {
    ac = m[end];
    pc += 2;
    return 0;
}
int ADD(unsigned int end) {
    // Perform addition
    if(ac > 127) {
        ac = m[end] - ~ac;
    } else {
        ac = ac + m[end];
    }
    pc += 2;
    return 0;
}
int OR(unsigned int end) {
    if(ac || m[end]) {
        pc += 2;
    }
    return 0;
}
int AND(unsigned int end) {
    if(ac && m[end]) {
        pc += 2;
    }
    return 0;
}
int NOT(unsigned int end) {
    ac = ~ac; 
    pc++;
    verifyNOrZ();
    return 0;
}

int JMP(unsigned int end) {
    pc = end;
    return 0;
}
int JN(unsigned int end) {
    verifyNOrZ();
    if(n == true) {
        pc = end;
    }
    return 0;
}
int JZ(unsigned int end) {
    verifyNOrZ();
    if(z == true) {
        pc = end;
    }
    return 0;
}
int JNZ(unsigned int end) {
    if (ac > 0) {
        pc = end;
    }
    return 0;
}

//Main functions
struct constant storeConstant(char mychar[100], struct constant constants[10], int indexCount);
struct constant *findConstantByName(char constantName[50]);
void callNeanderFunction(char mychar[100]);

int main(void) {
    FILE *fptr;
    // Open a file in read mode
    fptr = fopen("neander.txt", "r"); 

    char mychar[100];
    

    bool isInVarSection = false;
    bool isInExecutationSection = false;

    while (fgets(mychar, 100, fptr) != NULL) {
        if(strcmp(mychar, "var\n") == 0) {
            isInVarSection = true;
        }
        if(strcmp(mychar, "code\n") == 0) {
            isInExecutationSection = true;
        }
        if(strcmp(mychar, "end\n") == 0) {
            isInVarSection = false;
            isInExecutationSection = false;
        }

        if(isInVarSection) {
            if(strcmp(mychar, "var\n") != 0) {
                constants[indexCount] = storeConstant(mychar, constants, indexCount);
                indexCount++;
            }
        } else if (isInExecutationSection) {
            if(strcmp(mychar, "code\n") != 0) {
                callNeanderFunction(mychar);
            }
        }
    }

    printf("Valor do acumulador no final: %d\n", ac);
    fclose(fptr);
    return 0;
}
    
struct constant storeConstant(char mychar[100], struct constant constants[10], int indexCount) {
    int value, memoryLocal;
    char name[50];
    
    // Analisando a string de entrada
    sscanf(mychar, "%s %d %d", name, &value, &memoryLocal);
    printf("Armazenou a variável %s com valor inteiro %d no local de memória %d\n", name, value, memoryLocal);
    
    constants[indexCount].value = value;
    constants[indexCount].memory_local = memoryLocal;
    strcpy(constants[indexCount].name, name);

    m[memoryLocal] = value;
    
    return constants[indexCount];
}

struct constant *findConstantByName(char constantName[50]) {
    for(int i = 0; i < indexCount; i++) {
        if((strcmp(constants[i].name, constantName) == 0)) {
            return &constants[i];
        }
    }
    return NULL;
}

void callNeanderFunction(char mychar[100]) {
    char funcName[4];
    char constant[50];
    //switch of mychar here
    if (strcmp(mychar, "NOP\n") == 0) {
        NOP();
    } else if (strcmp(mychar, "HLT\n") == 0) {
        HLT();
    } else if (sscanf(mychar, "%s %s", funcName, constant)) {
        int constantInteger;
        unsigned int end;

        struct constant *variable = findConstantByName(constant);
        if(variable == NULL) {
            end = atoi(constant); 
        } else {
            end = variable->memory_local;
        }

        if (strcmp(funcName, "STA") == 0) {
            STA(end);
        } else if (strcmp(funcName, "LDA") == 0) {
            LDA(end);
        } else if (strcmp(funcName, "ADD") == 0) {
            ADD(end);
        } else if (strcmp(funcName, "OR") == 0) {
            OR(end);
        } else if (strcmp(funcName, "AND") == 0) {
            AND(end);
        } else if (strcmp(funcName, "NOT") == 0) {
            NOT(end);
        } else if (strcmp(funcName, "JMP") == 0) {
            JMP(end);
        } else if (strcmp(funcName, "JN") == 0) {
            JN(end);
        } else if (strcmp(funcName, "JZ") == 0) {
            JZ(end);
        } else if (strcmp(funcName, "JNZ") == 0) {
            JNZ(end);
        } else {
            printf("Invalid instruction: %s", mychar);
        }   
    } else {
        printf("Invalid instruction: %s", mychar);
    }
}