Na pasta "CPU", você vai encontrar o arquivo main.c, que possui o código do software, além de dois diretórios: input e output.

O diretório input inclui o arquivo input.txt, onde você deve colocar o código que quer rodar. Por sua vez, o diretório output contém o arquivo executável main.exe, que é usado para processar o programa.

Para usar o software, edite o arquivo input.txt na pasta input e insira o código que deseja executar. Depois, rode o arquivo main.exe para começar o processamento.

O arquivo input.txt tem que seguir essa formatação:

![image](https://github.com/user-attachments/assets/8e4d4930-e9a1-465f-84e6-545815508245)

O primeiro campo se refere ao endereço(em hexadecimal), o campo do meio se refere a DADO(d) ou INSTRUÇÃO(i), o campo final se refere a instrução, operando ou imediato(em hexadecimal).

O tamanho das instruções seguem essa formatação:
![image](https://github.com/user-attachments/assets/921188e4-4226-4f82-abec-69fe839b91c1)
![image](https://github.com/user-attachments/assets/325bcf36-ec47-4a2b-9d0c-e44c1f131153)

DETALHES IMPORTANTES:

🔹A "memoria" da cpu possui 154 posicoes de 8 bits, logo o endereçamento vai de 0x00 até 0x99 (hexadecimal);
🔹O imediato(IMM*) deve possuir 16 bits, logo deve se dedicar 2 posicoes da memoria para guardar o IMM*, o primeiro endereço contem os 8 bits mais significativos, e o segundo endereço os 8 bits menos significativos;
🔹A CPU possui 4 registradores, de r0 a r3;
🔹A CPU possui 2 operandos que podem ser manipulados, ro0 e ro1;

------------------------//----------------------
A ideia do projeto foi desenvolvida pelo Professor Hugo Vinicius Leao e Silva, doutor e professor no IFG - Campus Anápolis;

Código desenvolvido por:
@HelloPeeter
@guitaveira
