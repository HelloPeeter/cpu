#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

unsigned char memoria[154];
unsigned int MBR;
unsigned short int MAR, IMM, PC;
unsigned char IR, RO0, RO1, E, G, L;
unsigned short int reg[4];

// 0000 0000 0000 0000 0000 0000 1001 0000
void busca()
{
    printf("Buscando instrução...\n");
    MAR = PC;
    MBR = memoria[MAR];
    IR = MBR >> 3; // Move o MBR 3 bits para a direita
    if (IR == 0 || IR == 1 || IR == 13)
    {
        // Nao faz nada
    }
    else if (IR >= 2 && IR <= 12)
    {
        // Coloco o resto dos bits da palavra no MBR
        MAR++;
        MBR = (MBR << 8) + memoria[MAR]; // Movo o MBR 8 bits para a esquerda e adiciono o byte seguinte
    }
    else if (IR >= 14 && IR <= 29)
    {
        // Coloco o resto dos bits da palavra no MBR
        MAR++;
        MBR = (MBR << 8) + memoria[MAR]; // Movo o MBR 8 bits para a esquerda e adiciono o byte seguinte
        MAR++;
        MBR = (MBR << 8) + memoria[MAR]; // Movo o MBR 8 bits para a esquerda e adiciono o byte seguinte
    }
    else
    {
        printf("Instrução inválida: %d\n", IR);
        exit(1);
    }
}

void decodifica()
{
    printf("Decodificando instrução...\n");
    if (IR == 0 || IR == 1)
    {
        // Nao faz nada
    }
    else if (IR == 12)
    {
        // op     |r0|
        // 0000 0|00|0
        RO0 = (MBR & 0x07) >> 1; // Pega os 3 bits menos significativo e move 1 bit para a direita
    }
    else if (IR >= 2 && IR <= 11)
    {
        // op    |r0|r1 |
        // 0000 1|00|0 0|000 0000
        RO0 = (MBR & 0x0600) >> 9; // Move 9 bits para a direita apos zerar tudo menos o R0
        RO1 = (MBR & 0x0180) >> 7; // Move 7 bits para a direita apos zerar tudo menos o R1
    }
    else if (IR >= 14 && IR <= 19)
    {
        // op    | 0  |imm
        // 0001 0|000 |0000 0000 0000 0000
        MAR = MBR & 0x00FFFF; // Pega os 16 bits menos significativos
    }
    else if (IR >= 20 && IR <= 29)
    {
        if (IR == 20 || IR == 21)
        {
            // op    |r0| |MAR
            // 0001 0|00|0 |0000 0000 0000 0000
            RO0 = (MBR & 0x060000) >> 17; // Move 17 bits para a direita apos zerar tudo menos o R0
            MAR = MBR & 0x00FFFF;         // Pega os 16 bits menos significativos
        }
        else if (IR >= 22 && IR <= 29)
        {
            // op    |r0|  |IMM
            // 0001 0|00|0 |0000 0000 0000 0000
            RO0 = (MBR & 0x060000) >> 17; // Move 17 bits para a direita apos zerar tudo menos o R0
            IMM = MBR & 0x00FFFF;         // Pega os 16 bits menos significativos
        }
    }
    else
    {
        printf("Instrução inválida: %d\n", IR);
        exit(1);
    }
}

void executa()
{
    printf("Executando instrução...\n");
    // Teste instrucao LD r0, 1E

    // mar = 1E
    // RO0 = 00
    if (IR == 21)
    {
        reg[RO0] = memoria[MAR];
        PC += 3;
    }
}

void imprime_estado()
{
    setlocale(LC_ALL, "Portuguese Brazilian");
    printf("\nCPU:\n");
    printf("R0: 0x%04X R1: 0x%04X R2: 0x%04X R3: 0x%04X\n", reg[0], reg[1], reg[2], reg[3]);
    printf("MBR: 0x%06X MAR: 0x%04X IMM: 0x%04X PC: 0x%04X\n", MBR, MAR, IMM, PC);
    printf("IR: 0x%02X RO0: 0x%X RO1: 0x%X\n", IR, RO0, RO1);
    printf("E: 0x%X L: 0x%X G: 0x%X\n", E, L, G);

    printf("\nMemoria:\n");
    for (int i = 0; i < 16; i++)
    {
        printf("%d: 0x%02X ", i, memoria[i]);
        if ((i + 1) % 4 == 0)
            printf("\n");
    }
    printf("...\n%d: 0x%02X\n", 99, memoria[99]);
}
// do
// {
//     busca();
//     decodifica();
//     executa();
// } while (true);

int main()
{
    // Instrução: LD R0, 0x001E (IR = 21, R0 = 0b00, IMM = 0x001E)
    // Codificação: 00010101 00000000 00011110
    memoria[0] = 0xA8; // 10101000 → IR = 21 (0x15)
    memoria[1] = 0x00;
    memoria[2] = 0x1E;
    memoria[0x1E] = 10;

    PC = 0x0000;
    do
    {
        busca();
        decodifica();
        executa();
        imprime_estado();
    } while (PC <= 2);

    return 0;
}