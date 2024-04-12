#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void translateInstruction(const char *instruction, FILE *outputFile) {
    char strippedInstruction[256];
    int j = 0;
    for (int i = 0; instruction[i] != '\0'; i++) {
        if (!isspace(instruction[i])) {
            strippedInstruction[j++] = instruction[i];
        }
    }
    strippedInstruction[j] = '\0';

    if (strncmp(strippedInstruction, "finit", 5) == 0) {
        fputc(0xDB, outputFile);
    } else if (strncmp(strippedInstruction, "fld", 3) == 0) {
        fputc(0xD9, outputFile);
        fputc(0xC0, outputFile);
    } else if (strncmp(strippedInstruction, "fsub", 4) == 0) {
        fputc(0xDE, outputFile);
        fputc(0xE8, outputFile);
    } else if (strncmp(strippedInstruction, "fadd", 4) == 0) {
        fputc(0xDE, outputFile);
        fputc(0xC1, outputFile);
    } else if (strncmp(strippedInstruction, "fmul", 4) == 0) {
        fputc(0xDE, outputFile);
        fputc(0xC9, outputFile);
    } else if (strncmp(strippedInstruction, "fdiv", 4) == 0) {
        fputc(0xDE, outputFile);
        fputc(0xF9, outputFile);
    } else if (strncmp(strippedInstruction, "fstp", 4) == 0) {
        fputc(0xDD, outputFile);
        fputc(0xD8, outputFile);
    } else if (strncmp(strippedInstruction, "mov", 3) == 0) {
        fputc(0xB8, outputFile);
    } else if (strncmp(strippedInstruction, "int0x80", 6) == 0) {
        fputc(0xCD, outputFile);
        fputc(0x80, outputFile);
    }
}

int main() {
    FILE *asmFile = fopen("assembly_code.asm", "r");
    FILE *binFile = fopen("output.bin", "wb");

    char line[256];
    while (fgets(line, sizeof(line), asmFile) != NULL) {
        strtok(line, "\n");
        translateInstruction(line, binFile);
    }

    printf("Sucesso ao converter para binário!\n");

    fclose(asmFile);
    fclose(binFile);

    return 0;
}
