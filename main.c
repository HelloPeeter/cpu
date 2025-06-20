#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

// Registradores e Memoria
unsigned char memoria[154] = {0};
unsigned int mbr = 0;
unsigned short int mar = 0, imm = 0, pc = 0;
unsigned char ir = 0, ro0 = 0, ro1 = 0, e = 0, g = 0, l = 0;
unsigned short int reg[4] = {0};

// 0000 0000 0000 0000 0000 0000 1001 0000
// Busca a instrução na memória
void busca()
{

    mar = pc;
    mbr = memoria[mar];

    ir = mbr >> 3; // Move o mbr 3 bits para a direita
    if (ir == 0 || ir == 1 || ir == 13)
    {
        // Nao faz nada
    }
    else if (ir >= 2 && ir <= 12)
    {
        // Coloco o resto dos bits da palavra no mbr
        mar++;
        mbr = (mbr << 8) + memoria[mar]; // Movo o mbr 8 bits para a esquerda e adiciono o byte seguinte
    }
    else if (ir >= 14 && ir <= 29)
    {
        // Coloco o resto dos bits da palavra no mbr
        mar++;
        mbr = (mbr << 8) + memoria[mar]; // Movo o mbr 8 bits para a esquerda e adiciono o byte seguinte
        mar++;
        mbr = (mbr << 8) + memoria[mar]; // Movo o mbr 8 bits para a esquerda e adiciono o byte seguinte
    }
}

// Decodifica a instrucao
void decodifica()
{
    if (ir == 0 || ir == 1)
    {
        // Nao faz nada
    }
    else if (ir == 13)
    {
        // op     |r0|
        // 0000 0100
        // 0000 0110
        ro0 = (mbr & 0x06) >> 1; // Pega os 3 bits menos significativo e move 1 bit para a direita
    }
    else if (ir >= 2 && ir <= 12)
    {
        // op    |r0|r1 |
        // 0000 1|00|0 0|000 0000
        // 0000 0000 0110 0000
        ro0 = (mbr & 0x0600) >> 9; // Move 9 bits para a direita apos zerar tudo menos o R0
        ro1 = (mbr & 0x0180) >> 7; // Move 7 bits para a direita apos zerar tudo menos o R1
    }
    else if (ir >= 14 && ir <= 20)
    {
        // op    | 0  |imm
        // 0001 0|000 |0000 0000 0000 0000
        mar = mbr & 0x00FFFF; // Pega os 16 bits menos significativos
    }
    else if (ir >= 21 && ir <= 29)
    {
        if (ir == 21 || ir == 22)
        {

            // op    |r0|  |mar
            // 0001 0|00|0 |0000 0000 0000 0000
            ro0 = (mbr & 0x060000) >> 17; // Move 17 bits para a direita apos zerar tudo menos o R0
            mar = mbr & 0x00FFFF;         // Pega os 16 bits menos significativos
        }
        else if (ir >= 23 && ir <= 29)
        {
            // op    |r0|  |imm
            // 0001 0|00|0 |0000 0000 0000 0000
            ro0 = (mbr & 0x060000) >> 17; // Move 17 bits para a direita apos zerar tudo menos o R0
            imm = mbr & 0x00FFFF;         // Pega os 16 bits menos significativos
        }
    }
}

// Executa a instrucao decodificada
void executa()
{
    if (ir == 0b00000) // 0 - hlt
    {
        // Não realiza nenhuma operação.
    }

    else if (ir == 0b00001) // 1 - nop
    {
        pc += 1; // Não realiza nenhuma operação, apenas avança o contador de programa (PC).
    }
    else if (ir == 0b00010) // 2 - ldr rx, ry -> rX =∗ rY
    {
        // 0000 0000 0111 1000 ->120
        reg[ro0] = memoria[reg[ro1]];                       // Carrega o byte mais significativo da memória no endereço especificado pelo registrador rY e armazena em rX.
        reg[ro0] = (reg[ro0] << 8) + memoria[reg[ro1] + 1]; // Carrega o byte menos significativo da memória no endereço especificado pelo registrador rY e armazena em rX.
        pc += 2;                                            // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b00011) // 3 -> str rx, ry -> ∗rY = rX
    {
        memoria[reg[ro1]] = (reg[ro0] & 0xFF00) >> 8; // Armazena os 8 bits mais significativos de rX na memória.
        memoria[reg[ro1] + 1] = (reg[ro0] & 0x00FF);  // Armazena os 8 bits menos significativos de rX na memória.
        pc += 2;                                      // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b00100) // 4 -> add rX, rY ->> rX = rX + rY
    {
        reg[ro0] = reg[ro0] + reg[ro1]; // Soma o valor do registrador rY ao valor do registrador rX e armazena o resultado em rX.
        pc += 2;                        // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b00101) // 5 -> sub rX, rY -> rX = rX−rY
    {
        reg[ro0] = reg[ro0] - reg[ro1]; // Subtrai o valor do registrador rY do valor do registrador rX e armazena o resultado em rX.
        pc += 2;                        // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b00110) // 6 -> mul rX, rY -> rX = rX*rY
    {
        reg[ro0] = reg[ro0] * reg[ro1]; // Multiplica o valor do registrador rX pelo valor do registrador rY e armazena o resultado em rX.
        pc += 2;                        // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b00111) // 7 -> div rX, rY -> rX = rX/rY
    {
        reg[ro0] = reg[ro0] / reg[ro1]; // Divide o valor do registrador rX pelo valor do registrador rY e armazena o resultado em rX.
        pc += 2;                        // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b01000) // 8 -> cmp rX, rY
    {
        // Compara os valores dos registradores rX e rY
        if (reg[ro0] == reg[ro1])
        {
            e = 1; // Define o sinal de igualdade (E) se os valores forem iguais
            l = 0; // Limpa o sinal de menor que (L)
            g = 0; // Limpa o sinal de maior que (G)
        }
        else if (reg[ro0] < reg[ro1])
        {
            e = 0; // Limpa o sinal de igualdade (E)
            l = 1; // Define o sinal de menor que (L)
            g = 0; // Limpa o sinal de maior que (G)
        }
        else
        {
            e = 0; // Limpa o sinal de igualdade (E)
            l = 0; // Limpa o sinal de menor que (L)
            g = 1; // Define o sinal de maior que (G)
        }
        pc += 2; // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b01001) // 9 -> movr rX, rY -> rX = rY
    {
        reg[ro0] = reg[ro1];
    }
    else if (ir == 0b01010) // 10 -> and rX,rY ->  rX = rX & rY
    {
        reg[ro0] = reg[ro0] & reg[ro1]; // Realiza a operação lógica AND entre os valores dos registradores rX e rY e armazena o resultado em rX.
        pc += 2;                        // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b01011) // 11 -> or rX, rY -> rX = rX | rY
    {
        reg[ro0] = reg[ro0] | reg[ro1]; // Realiza a operação lógica OR entre os valores dos registradores rX e rY e armazena o resultado em rX.
        pc += 2;                        // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b01100) // 12 ->xor rX, rY -> rX = rX ^ rY
    {
        reg[ro0] = reg[ro0] ^ reg[ro1]; // Realiza a operação lógica XOR entre os valores dos registradores rX e rY e armazena o resultado em rX.
        pc += 2;                        // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b01101) // 13 -> not rX -> rX = ~rX
    {
        reg[ro0] = ~reg[ro0]; // Realiza a negação bit a bit do valor do registrador rX e armazena o resultado em rX.
        pc += 1;              // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b01110) // 14 -> je Z
    {
        if (e == 1) // Verifica se o sinal de igualdade (E) está definido
        {
            pc = mar; // Se for verdadeiro, salta para o endereço especificado por Z (mar).
        }
        else
        {
            pc += 3; // Caso contrário, avança o contador de programa (PC) para a próxima instrução.
        }
    }
    else if (ir == 0b01111) // 15 -> jne Z
    {
        if (e == 0) // Verifica se o sinal de igualdade (E) não está definido
        {
            pc = mar; // Se for verdadeiro, salta para o endereço especificado por Z (mar).
        }
        else
        {
            pc += 3; // Caso contrário, avança o contador de programa (PC) para a próxima instrução.
        }
    }
    else if (ir == 0b10000) // 16 - > jl Z
    {
        if (l == 1) // Verifica se o sinal de menor que (L) está definido
        {
            pc = mar; // Se for verdadeiro, salta para o endereço especificado por Z (mar).
        }
        else
        {
            pc += 3; // Caso contrário, avança o contador de programa (PC) para a próxima instrução.
        }
    }
    else if (ir == 0b10001) // 17 - jle Z
    {
        if (l == 1 || e == 1) // Verifica se o sinal de menor que (L) ou o sinal de igualdade (E) estão definidos
        {
            pc = mar; // Se for verdadeiro, salta para o endereço especificado por Z (mar).
        }
        else
        {
            pc += 3; // Caso contrário, avança o contador de programa (PC) para a próxima instrução.
        }
    }
    else if (ir == 0b10010) // 18 -> jg Z
    {
        if (g == 1) // Verifica se o sinal de maior que (G) está definido
        {
            pc = mar; // Se for verdadeiro, salta para o endereço especificado por Z (mar).
        }
        else
        {
            pc += 3; // Caso contrário, avança o contador de programa (PC) para a próxima instrução.
        }
    }
    else if (ir == 0b10011) // 19 - jge
    {
        if (g == 1 || e == 1) // Verifica se o sinal de maior que (G) ou o sinal de igualdade (E) estão definidos
        {
            pc = mar; // Se for verdadeiro, salta para o endereço especificado por Z (mar).
        }
        else
        {
            pc += 3; // Caso contrário, avança o contador de programa (PC) para a próxima instrução.
        }
    }
    else if (ir == 0b10100) // 20 - jmp Z
    {
        pc = mar; // Salta incondicionalmente para o endereço especificado por Z (mar).
    }
    else if (ir == 0b10101) // 21 - ld rX, Z -> rX = *Z
    {
        reg[ro0] = memoria[mar];                       // Armazena o byte mais significativo da memória no endereço especificado por Z (mar) e armazena em rX.
        reg[ro0] = (reg[ro0] << 8) + memoria[mar + 1]; // Armazena o byte menos significativo da memória ao valor de rX.
        pc += 3;                                       // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b10110) // 22 -> st rX, Z -> *Z = rX
    {
        memoria[mar] = (reg[ro0] >> 8);         // Armazena o byte mais significativo do registrador rX na memória no endereço especificado por Z (mar).
        memoria[mar + 1] = (reg[ro0] & 0x00FF); // Armazena o byte menos significativo do registrador rX na memória.
        pc += 3;                                // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b10111) // 23 -> movi rX, IMM -> rX = IMM
    {
        reg[ro0] = imm; // Move o valor imediato (IMM) para o registrador rX.
        pc += 3;        // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b11000) // 24 -> addi rX, IMM -> rX = rX + IMM
    {
        reg[ro0] = reg[ro0] + imm; // Adiciona o valor imediato (IMM) ao valor do registrador rX e armazena o resultado em rX.
        pc += 3;                   // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b11001) // 25 -> subi rX, IMM -> rX = rX - IMM
    {
        reg[ro0] = reg[ro0] - imm; // Subtrai o valor imediato (IMM) do valor do registrador rX e armazena o resultado em rX.
        pc += 3;                   // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b11010) // 26 -> muli rX, IMM -> rX = rX * IMM
    {
        reg[ro0] = reg[ro0] * imm; // Multiplica o valor do registrador rX pelo valor imediato (IMM) e armazena o resultado em rX.
        pc += 3;                   // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b11011) // 27 -> divi rX, IMM -> rX = rX / IMM
    {
        reg[ro0] = reg[ro0] / imm; // Divide o valor do registrador rX pelo valor imediato (IMM) e armazena o resultado em rX.
        pc += 3;                   // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b11100) // 28 -> lsh rX, IMM -> rX = rX << IMM
    {
        reg[ro0] = reg[ro0] << imm; // Realiza um deslocamento à esquerda no valor do registrador rX pelo valor imediato (IMM) e armazena o resultado em rX.
        pc += 3;                    // Avança o contador de programa (PC) para a próxima instrução.
    }
    else if (ir == 0b11101) // 29 -> rsh rX, IMM -> rX = rX >> IMM
    {
        reg[ro0] = reg[ro0] >> imm; // Realiza um deslocamento à direita no valor do registrador rX pelo valor imediato (IMM) e armazena o resultado em rX.
        pc += 3;                    // Avança o contador de programa (PC) para a próxima instrução.
    }
}

// Imprime estado da memoria
void imprime_estado()
{
    printf("\nCPU:\n");
    printf("R0: 0x%04X R1: 0x%04X R2: 0x%04X R3: 0x%04X\n", reg[0], reg[1], reg[2], reg[3]);
    printf("MBR: 0x%06X MAR: 0x%04X IMM: 0x%04X PC: 0x%04X\n", mbr, mar, imm, pc);
    printf("IR: 0x%02X RO0: 0x%X RO1: 0x%X\n", ir, ro0, ro1);
    printf("E: 0x%X L: 0x%X G: 0x%X\n", e, l, g);

    printf("\nMemoria:\n");
    for (int i = 0; i < 154; i++)
    {
        printf("%02X: 0x%02X ", i, memoria[i]);
        if ((i + 1) % 7 == 0)
            printf("\n");
    }
    printf("\n\nPressione a tecla enter para iniciar o proximo ciclo de maquina ou aperte CTRL+C para finalizar a execucao do trabalho.");
}

void carregar_arquivo(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (linha[0] == '\n' || linha[0] == '#' || linha[0] == ';')
            continue;

        char endereco_str[10], tipo[10], conteudo[100];
        sscanf(linha, "%[^;];%[^;];%[^\n]", endereco_str, tipo, conteudo);

        int endereco = (int)strtol(endereco_str, NULL, 16);

        if (strcmp(tipo, "d") == 0)
        {
            unsigned char valor = (unsigned char)strtol(conteudo, NULL, 16);
            memoria[endereco] = valor >> 8;
            memoria[endereco + 1] = valor & 0x00FF;
        }
        else if (strcmp(tipo, "i") == 0)
        {
            codifica_instrucao(endereco, conteudo);
        }
    }

    fclose(arquivo);
}

void codifica_instrucao(int endereco, const char *instrucao)
{
    char op[10], arg1[10] = "", arg2[10] = "";
    unsigned char opcode = 0;
    unsigned char ro0 = 0, ro1 = 0;
    unsigned short imm = 0;
    // int num_args;

    sscanf(instrucao, "%s %[^,], %s", op, arg1, arg2);

    // ---------- Determinar opcode ----------
    if (strcmp(op, "hlt") == 0)
        opcode = 0b00000;
    else if (strcmp(op, "nop") == 0)
        opcode = 0b00001;
    else if (strcmp(op, "ldr") == 0)
        opcode = 0b00010;
    else if (strcmp(op, "str") == 0)
        opcode = 0b00011;
    else if (strcmp(op, "add") == 0)
        opcode = 0b00100;
    else if (strcmp(op, "sub") == 0)
        opcode = 0b00101;
    else if (strcmp(op, "mul") == 0)
        opcode = 0b00110;
    else if (strcmp(op, "div") == 0)
        opcode = 0b00111;
    else if (strcmp(op, "cmp") == 0)
        opcode = 0b01000;
    else if (strcmp(op, "movr") == 0)
        opcode = 0b01001;
    else if (strcmp(op, "and") == 0)
        opcode = 0b01010;
    else if (strcmp(op, "or") == 0)
        opcode = 0b01011;
    else if (strcmp(op, "xor") == 0)
        opcode = 0b01100;
    else if (strcmp(op, "not") == 0)
        opcode = 0b01101;
    else if (strcmp(op, "je") == 0)
        opcode = 0b01110;
    else if (strcmp(op, "jne") == 0)
        opcode = 0b01111;
    else if (strcmp(op, "jl") == 0)
        opcode = 0b10000;
    else if (strcmp(op, "jle") == 0)
        opcode = 0b10001;
    else if (strcmp(op, "jg") == 0)
        opcode = 0b10010;
    else if (strcmp(op, "jge") == 0)
        opcode = 0b10011;
    else if (strcmp(op, "jmp") == 0)
        opcode = 0b10100;
    else if (strcmp(op, "ld") == 0)
        opcode = 0b10101;
    else if (strcmp(op, "st") == 0)
        opcode = 0b10110;
    else if (strcmp(op, "movi") == 0)
        opcode = 0b10111;
    else if (strcmp(op, "addi") == 0)
        opcode = 0b11000;
    else if (strcmp(op, "subi") == 0)
        opcode = 0b11001;
    else if (strcmp(op, "muli") == 0)
        opcode = 0b11010;
    else if (strcmp(op, "divi") == 0)
        opcode = 0b11011;
    else if (strcmp(op, "lsh") == 0)
        opcode = 0b11100;
    else if (strcmp(op, "rsh") == 0)
        opcode = 0b11101;

    // ---------- Determinar registradores e imediato ----------
    if (arg1[0] == 'r')
        ro0 = arg1[1] - '0';
    if (arg2[0] == 'r')
        ro1 = arg2[1] - '0';
    else if (strlen(arg2) > 0)
        imm = (unsigned short)strtol(arg2, NULL, 16);
    else if (strlen(arg1) > 0 && arg1[0] != 'r')
        imm = (unsigned short)strtol(arg1, NULL, 16);

    // ---------- Codificar na memória ----------
    if (opcode == 0b00000 || opcode == 0b00001) // 0-2  hlt e nop
    {
        memoria[endereco] = (opcode << 3);
    }
    else if (opcode == 0b01101) // 13 - not
    {
        memoria[endereco] = (opcode << 3) | (ro0 << 1);
    }
    else if (opcode >= 0b00010 && opcode <= 0b01100) // 2-12 ldr, str, add, sub, mul, div, cmp, movr, and, or, xor
    {
        memoria[endereco] = (opcode << 3) | (ro0 << 1) | ((ro1 >> 1) & 0x01);
        memoria[endereco + 1] = (ro1 << 7);
    }
    else if ((opcode >= 0b01110 && opcode <= 0b10100)) // je, jne, jl, jle, jg, jge, jmp
    {
        memoria[endereco] = (opcode << 3);
        memoria[endereco + 1] = (imm >> 8) & 0xFF;
        memoria[endereco + 2] = imm & 0xFF;
    }
    else if ((opcode == 0b10101) || (opcode == 0b10110)) // ld, st
    {
        memoria[endereco] = (opcode << 3) | (ro0 << 1);
        memoria[endereco + 1] = (imm >> 8) & 0xFF;
        memoria[endereco + 2] = imm & 0xFF;
    }
    else if (opcode >= 0b10111 && opcode <= 0b11101) // imediatos
    {

        memoria[endereco] = (opcode << 3) | (ro0 << 1);
        memoria[endereco + 1] = (imm >> 8) & 0xFF;
        memoria[endereco + 2] = imm & 0xFF;
    }
    else
    {
        printf("Instrução não tratada: %s\n", instrucao);
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    carregar_arquivo("../input/input.txt");

    char word;
    do
    {
        system("cls");
        busca();
        decodifica();
        executa();
        imprime_estado();
        word = getchar();
    } while (word == '\n');

    return 0;
}
