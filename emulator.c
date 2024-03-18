//Libraries
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t ac = 0;
int pc = 0;
bool n = false;
bool z = false;

uint8_t m[256];

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
    ac = ac + m[end];
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

int main(void) {
    printf("oi mundo\n");

    while(true) {
        break;
    }
    return 0;
}