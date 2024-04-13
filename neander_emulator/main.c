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
    } else if(m[end] > 127) {
        ac = ac - ~m[end];
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
int NOT() {
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

bool verifyIfNeedSecondValue(unsigned char data);
void callNeanderFunctionByBinary(uint8_t firstValue, unsigned char data);

int main(void) {
    FILE *fptr;
    FILE *neanderBin;
    // Open a file in read mode
    fptr = fopen("neander.txt", "r"); 
    neanderBin = fopen("assembler.bin", "rb"); 
    
    int fileChoosen = -1;
    printf("Selecione <0> para ler o neander.txt e <1> para ler o neander.bin;\n");
    scanf("%d", &fileChoosen);
    while(fileChoosen != 0 && fileChoosen != 1) {
        printf("Input inválido, Selecione <0> para ler o neander.txt e <1> para ler o neander.bin;\n");
        scanf("%d", &fileChoosen); 
    }

    if(fileChoosen == 0) {
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
    } else {
        //Utilizado para comandos que precisam de dois binários
        bool comandNeedSecondValue = false;
        uint8_t firstValue;
        bool isInVarSection = false;
        bool isInExecutationSection = false;
        for(int i = 0; i < 256; i++) {
            unsigned char data;
            fread(&data, sizeof(unsigned char), 1, neanderBin);

            if(data == 0xFF) {
                isInVarSection = true;
            }
            if(data == 0xFE) {
                isInExecutationSection = true;
            }
            if(data == 0xFD) {
                isInVarSection = false;
                isInExecutationSection = false;
            }

            if(isInVarSection) {
                if(data != 0xFF) {
                    uint8_t uintData = (uint8_t)data;
                    if(comandNeedSecondValue) {
                        m[uintData] = firstValue;
                        printf("Armazenou a variável com valor inteiro %d no local de memória %d\n", firstValue, uintData);
                        comandNeedSecondValue = false;
                    } else {
                        firstValue = uintData;
                        comandNeedSecondValue = true;
                    }
                }
            } else if (isInExecutationSection) {
                if(data != 0xFE) {
                    uint8_t uintData = (uint8_t)data;
                    if(comandNeedSecondValue) {
                        callNeanderFunctionByBinary(firstValue, uintData);
                        comandNeedSecondValue = false;
                    } else {
                        if(verifyIfNeedSecondValue(data)) {
                            firstValue = uintData;
                            comandNeedSecondValue = true;
                        } else {
                            callNeanderFunctionByBinary(uintData, 0);
                        }
                    }
                }
            }
        }
    }

    printf("Valor do acumulador no final: %d\n", ac);
    fclose(fptr);
    fclose(neanderBin);
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
    } else if (strcmp(mychar, "NOT\n") == 0) {
        NOT();
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

bool verifyIfNeedSecondValue(unsigned char data) {
    switch(data) {
        case 0x06:
        return true;
        case 0x07:
        return true;
        case 0x08:
        return true;
        case 0x09:
        return true;
        case 0x0A:
        return true;
        case 0x0B:
        return true;
        case 0x0C:
        return true;
        case 0x0D:
        return true;
        case 0x0E:
        return true;
        case 0x0F:
        return true;
    }
    return false;
}

void callNeanderFunctionByBinary(uint8_t firstValue, uint8_t end) {
    switch(firstValue) {
        case 3:
        printf("NOP\n");
        NOP();
        break;
        case 4:
        printf("HLT\n");
        HLT();
        break;
        case 5:
        printf("NOT\n");
        NOT();
        break;
        case 6:
        printf("STA %d\n", end);
        STA(end);
        break;
        case 7:
         printf("LDA %d\n", end);
        LDA(end);
        break;
        case 8:
        printf("ADD %d\n", end);
        ADD(end);
        break;
        case 9:
        OR(end);
        break;
        case 10:
        AND(end);
        break;
        case 11:
        printf("JMP %d\n", end);
        JMP(end);
        break;
        case 12:
        JN(end);
        break;
        case 13:
        printf("JZ %d\n", end);
        JZ(end);
        break;
        case 14:
        JNZ(end);
        break;
    }
}