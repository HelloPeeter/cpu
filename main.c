#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

unsigned char memoria[154];
unsigned int MBR;
unsigned short int MAR, IMM, PC;
unsigned char IR, RO0, RO1, E, G, L;
unsigned short int REG[4];

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
        RO0 = (MBR & 0x07) >> 1 // Pega os 3 bits menos significativo e move 1 bit para a direita
    }
    else if (IR >= 2 && IR <= 11)
    {
        // op    |r0|r1 |
        // 0000 1|00|0 0|000 0000
        RO0 = (MBR & 0x0600) >> 9; // Move 9 bits para a direita apos zerar tudo menos o R0
        RO1 = (MBR & 0x0180) >> 7; // Move 7 bits para a direita apos zerar tudo menos o R1
    }
    else if (IR >= 14 && IR <= 19)
        // op    | 0 |imm
        // 0001 0|00|0 0000 0000 0000 0000
        MAR = MBR & 0x01FFFF; // Pega os 17 bits menos significativos

    {
        else if (IR >= 20 && IR <= 29) if (IR == 20 || IR == 21)
        {
            // op    | 0|MAR
            // 0001 0|00|0 0000 0000 0000 0000
            MAR = MBR & 0x01FFFF; // Pega os 17 bits menos significativos
        }
        else if (IR >= 22 && IR <= 29)
        {
            // op    |r0|IMM
            // 0001 0|00|0 0000 0000 0000 0000
            RO0 = (MBR & 0x060000) >> 17; // Move 17 bits para a direita apos zerar tudo menos o R0
            IMM = MBR & 0x01FFFF;         // Pega os 17 bits menos significativos
        }
    }
    else
    {
        printf("Instrução inválida: %d\n", IR);
        exit(1);
    }
}

// do
// {
//     busca();
//     decodifica();
//     executa();
// } while (true);

int main()
{

    return 0;
}