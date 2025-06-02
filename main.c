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
        // 0000 0 00 0
        // 0110 1 00 0
        // 0000 0 11 0
        RO0 = MBR & 0x07 >> 1 // Pega o byte menos significativo e move 1 bit para a direita
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