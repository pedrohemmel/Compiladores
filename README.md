[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/VoX6-nVZ)
Escreva um README.md com informações sobre seu projeto:

### Todas pastas ja tem arquivos prontos (TXTs, BINs) que podem ser utilizados em cada uma das ferramentas (LEXER, PARSER, ASSEMBLER TO BINARY E NEANDER EMULATOR).

## LEXER
### Instruções para rodar o lexer
- Necessariamente tem que ter um arquivo txt na raiz da pasta lexer para que o main pegue o arquivo e comece a tokenizar.
- O lexer tokeniza números inteiros ou reais e alguns operadores matemáticos como soma (+), multiplicação (*), divisão (/), subtração (-) e exponenciação (**). Ou seja, um exemplo válido de arquivo txt para o lexer entender é < * + - / ** 10 1.1 1.1.1 1..1 >.

## PARSER
### Instruções para rodar o parser
- Necessariamente tem que ter um arquivo txt na raiz da pasta parser para que o main pegue o arquivo e comece a tokenizar com base no que foi lido. Também precisa ter um arquivo do tipo txt para escrever o código assembler.
- Exemplos de operações que podem ser escritas no txt que o parser vai ler: <5 + 5>, <5 * 5>, <5 - 5>.
- O parser não gera assembler para divisão nem exponenciação pois envolve calculo de valores com ponto flutuante.

## CONVERSOR DO ASSEMBLER PARA BINÁRIO
### Instruções para rodar o conversor
- Necessariamente tem que ter um arquivo txt na raiz da pasta assembler_to_binary para que o main pegue o arquivo e comece a converter para binário. Também precisa ter um arquivo do tipo bin para escrever o código convertido.
- O txt na raiz tem que ser o txt assembler que foi gerado pelo parser.

## NEANDER EMULATOR
### Instruções para rodar o emulador
- Necessariamente tem que ter um arquivo txt assembler (que o parser gera) ou um bin assembler (que o assembler_to_binary gera) na raiz da pasta neander_emulator para que o main pegue o arquivo e comece a interpretar. 
- Ao iniciar o projeto, é perguntado se você quer interpretar o txt (digite <0>) ou bin (digite <1>).

