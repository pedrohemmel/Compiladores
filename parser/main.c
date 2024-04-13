#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOREACH_TOKEN(TOKEN)                                                                         \
    TOKEN(Soma)                                                                                      \
    TOKEN(Subtracao)                                                                                 \
    TOKEN(Multiplicacao)                                                                             \
    TOKEN(Divisao)                                                                                   \
    TOKEN(Exponenciacao)                                                                             \
    TOKEN(Inteiros)                                                                                  \
    TOKEN(Reais)                                                                                     \
    TOKEN(Indefinido)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum Token { FOREACH_TOKEN(GENERATE_ENUM) };

static const char *TOKEN_STRING[] = {FOREACH_TOKEN(GENERATE_STRING)};

typedef struct TokensValues {
    enum Token token;
    void *value;
} TokenValue;

typedef struct HeadsLists {
    uint64_t size;
    TokenValue *list;
} HeadList;

//////////////
// FUNÇÕES PARA TOKENIZAR E ARMAZENAR TOKEN
//////////////

void addToken(HeadList *head, enum Token token, void *value, size_t valueSize) {
    head->size += 1;
    if (head->size == 1) {
        head->list = malloc(sizeof(TokenValue));
    } else {
        head->list = realloc(head->list, head->size * sizeof(TokenValue));
    }
    head->list[head->size - 1].token = token;
    head->list[head->size - 1].value = calloc(valueSize + 1, sizeof(char));
    size_t i;
    for (i = 0; i < valueSize; i++) {
        ((char *)(head->list[head->size - 1].value))[i] = ((char *)value)[i];
    }
}

void tokenize(HeadList *head, char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (isdigit(s[i])) {
            bool itHasPoint = false;
            bool isUndefined = false;
            char *numStr = calloc(sizeof(char), 25);
            int iStr = 0;

            numStr[iStr] = s[i];
            iStr++;
            while (isdigit(s[i + 1]) || s[i + 1] == '.') {
                i++;
                numStr[iStr] = s[i];
                iStr++;
                if (s[i] == '.') {
                    if (itHasPoint) {
                        isUndefined = true;
                    } else {
                        itHasPoint = true;
                    }
                }
            }
            if (!isUndefined) {
                if (itHasPoint) {
                    float num = atof(numStr);
                    addToken(head, Reais, &num, sizeof(float));
                } else {
                    unsigned int num = atoi(numStr);
                    addToken(head, Inteiros, &num, sizeof(unsigned int));
                }
            } else {
                addToken(head, Indefinido, numStr, strlen(numStr));
            }
            free(numStr);
        } else {
            switch (s[i]) {
            case '+':
                addToken(head, Soma, s + i, sizeof(char));
                break;
            case '-':
                addToken(head, Subtracao, s + i, sizeof(char));
                break;
            case '*':
                if (s[i + 1] == '*') {
                // **
                addToken(head, Exponenciacao, s + i, sizeof(char) * 2);
                i++;
                } else {
                // *
                addToken(head, Multiplicacao, s + i, sizeof(char));
                }
                break;
            case '/':
                addToken(head, Divisao, s + i, sizeof(char));
                break;
            default:
                if (!isspace(s[i]))
                addToken(head, Indefinido, s + i, sizeof(char));
                break;
            }
        }
    }
}

//////////////
// FUNÇÕES PARA LIBERAR MEMÓRIA
//////////////

void freeHeadList(HeadList *head) {
    for (uint64_t i = 0; i < head->size; i++) {
        free(head->list[i].value);
    }
    free(head->list);
    free(head);
}

//////////////
// GERANDO ASSEMBLER
//////////////

void generateSumAssembler(FILE *file, int num1, int num2) {
    fprintf(file, "var\n");
    fprintf(file, "valor1 %d 75\n", num1);
    fprintf(file, "valor2 %d 76\n", num2);
    fprintf(file, "end\n");
    fprintf(file, "code\n");
    fprintf(file, "LDA valor1\n");
    fprintf(file, "ADD valor2\n");
    fprintf(file, "HLT\n");
    fprintf(file, "end\n");
}

void generateSubtractionAssembler(FILE *file, int num1, int num2) {
    fprintf(file, "var\n");
    fprintf(file, "valor1 %d 75\n", num1);
    fprintf(file, "valor2 %d 76\n", num2);
    fprintf(file, "end\n");
    fprintf(file, "code\n");
    fprintf(file, "LDA valor2\n");
    fprintf(file, "NOT\n");
    fprintf(file, "STA valor2\n");
    fprintf(file, "LDA valor1\n");
    fprintf(file, "ADD valor2\n");
    fprintf(file, "HLT\n");
    fprintf(file, "end\n");
}

void generateMultiplierAssembler(FILE *file, int num1, int num2) {
    fprintf(file, "var\n");
    fprintf(file, "valor1 %d 75\n", num1);
    fprintf(file, "valor2 %d 76\n", num2);
    fprintf(file, "aux %d 77\n", num1);
    fprintf(file, "decrementer 1 78\n");
    fprintf(file, "end\n");
    fprintf(file, "code\n");
    fprintf(file, "LDA decrementer\n");
    fprintf(file, "NOT\n");
    fprintf(file, "STA decrementer\n");
    fprintf(file, "LDA valor2\n");
    fprintf(file, "ADD decrementer\n");
    fprintf(file, "NOP\n");
    fprintf(file, "JZ 22\n");
    fprintf(file, "LDA valor1\n");
    fprintf(file, "ADD aux\n");
    fprintf(file, "STA valor1\n");
    fprintf(file, "JMP 10\n");
    fprintf(file, "HLT\n");
    fprintf(file, "end\n");
}


void generateAssemblerWithOperation(FILE *file, HeadList *head, int num1, int num2) {
    for (int iList = 0; iList < head->size; iList++) {
        switch(head->list[iList].token) {
            case Soma:
            printf("Soma feita!\n");
            generateSumAssembler(file, num1, num2);
            break;
            case Multiplicacao:
            printf("Multiplicação feita!\n");
            generateMultiplierAssembler(file, num1, num2);
            break;
            case Divisao:
            printf("Conta não implementada");
            break;
            case Subtracao:
            printf("Subtração feita!\n");
            generateSubtractionAssembler(file, num1, num2);
            break;
            default:
            printf("Operação inválida\n");
            break;
        }
    }
}

void generateAssembler(FILE *file, HeadList *head) {
    int num1, num2;
    bool foundNumber = false;
    for (uint64_t i = 0; i < head->size; i++) {
        if (head->list[i].token == Inteiros) {
            if (!foundNumber) {
                num1 = *((int *)(head->list[i].value));
                foundNumber = true;
            } else {
                num2 = *((int *)(head->list[i].value));
                break;
            }
        }
    }

    generateAssemblerWithOperation(file, head, num1, num2);
}


int main() {
    HeadList *head = malloc(sizeof(HeadList));
    head->size = 0;

    //Abrir arquivo
    FILE *file = fopen("teste.txt", "r");
    if (file == NULL)
        printf("Error: Opening file");

    //Determinar quantidade de bytes do arquivo
    fseek(file, 0, SEEK_END);
    uint16_t fileSize = ftell(file);
    rewind(file);

    //Criar variavel com tamanho de memoria (buffer)
    char *program = (char *)calloc(sizeof(char), (fileSize + 1));

    //Lendo o arquivo e alocando no program conforme as especificações do arquivo
    size_t resultSize = fread(program, 1, fileSize, file);
    if (resultSize != fileSize)
        printf("Error: Reading file");
    fclose(file);

    //Chama a função que tokeniza o arquivo aberto
    tokenize(head, program);

    // Gerar o código assembly correspondente à operação
    FILE *assemblyFile = fopen("assembler.txt", "w");
    if (assemblyFile == NULL) {
        printf("Error: Opening assembler file");
        return 1;
    }
    generateAssembler(assemblyFile, head);

    fclose(assemblyFile);
    free(program);
    freeHeadList(head);
    return 0;
}
