# 🧠 Simulador de CPU

[![C](https://img.shields.io/badge/Linguagem-C-00599C?logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))  
![Status](https://img.shields.io/badge/Status-%20Concluido-green)  

Simulador de CPU simples com 154 posições de memória, registradores básicos e suporte a dados e instruções em formato hexadecimal.

---

## 📁 Estrutura do Projeto
```text
CPU/
├── main.c           # Código-fonte principal
├── input/
│   └── input.txt    # Arquivo com código a ser executado
└── output/
    └── main.exe     # Executável para executar o programa
```
---

## ▶️ Como Usar

1. Abra o arquivo `input/input.txt`.
2. Insira seu código/programa conforme o **formato**.
3. Execute `main.exe` dentro da pasta `output`.

---

## 📝 Formato do `input.txt`

Cada linha segue:
[ENDEREÇO_HEX];[TIPO];[INSTRUÇÃO/DADO]
- `ENDEREÇO_HEX`: de `00` até `99` (total de 154 posições).
- `TIPO`:  
  - `i` → instrução  
  - `d` → dado  
- `INSTRUÇÃO/DADO`: instrução ou dado a ser inserido na mémoria  (ex: `A3`, `ld ro0, 10`).

**Exemplo**:

![Exemplo de Instruçoes](https://github.com/user-attachments/assets/5c63adcf-e6cb-4302-9ec7-a61da90f9d81)
---

## 📏 Tamanho das Instruções

Cada instrução ocupa **1, 2 ou 3 bytes**, dependendo da operação e dos operandos. Abaixo está a tabela com base no formato de instrução da CPU.

---

### 🔹 1 Byte (8 bits)

| Instruções         | Operandos       | Descrição                         |
|--------------------|------------------|-----------------------------------|
| `HLT`, `NOP`       | nenhum           | Instruções simples, sem operandos |
| `NOT rX`           | 1 registrador    | Opera diretamente sobre um registrador |

---

### 🔹 2 Bytes (16 bits)

| Instruções                       | Operandos         | Descrição                                    |
|----------------------------------|--------------------|----------------------------------------------|
| `ADD rX, rY`, `SUB`, `MUL`, etc | 2 registradores    | Aritmética/lógica entre registradores        |

---

### 🔹 3 Bytes (24 bits)

| Instruções                         | Operandos                    | Descrição                                                |
|-----------------------------------|-------------------------------|------------------------------------------------------------|
| `LD rX, Z`, `ST rX, Z`, etc       | 1 registrador + 16 bits end. | Carrega/salva dado de/para endereço de memória            |
| `JE Z`, `JNE Z`, `JMP Z`, etc     | endereço de 16 bits (MAR)    | Todos os jumps usam 2 bytes de endereço                   |
| `MOVI rX, IMM`, `ADDI rX, IMM`, etc| registrador + imediato (16b) | Operações com valores imediatos                           |

---

### 🧠 Observações

- Endereços ou imediatos de **16 bits** ocupam **2 posições consecutivas** na memória:
  - **Byte 1**: bits mais significativos (MSB)
  - **Byte 2**: bits menos significativos (LSB)

---

### ✅ Tabela Resumo

| Tipo de Instrução                   | Tamanho |
|------------------------------------|---------|
| `NOP`, `HLT`, `NOT`                | 1 byte  |
| `ADD`, `SUB`, `CMP`, `OR`, etc     | 2 bytes |
| `LD`, `ST`, `JMP`, `JE`, `MOVI`, etc | 3 bytes (com 16 bits de endereço ou imediato) |



Formato das instruções:

![Formato das instruções-1](https://github.com/user-attachments/assets/06311c77-7ac5-4fba-b5cd-54c56332d8fe)
![Formato das instruçoes-2](https://github.com/user-attachments/assets/e61a460e-bc60-4f1c-abff-77b0e2a702d7)


---

## ⚙️ Especificações da CPU

- **Memória**: 154 × 8 bits (0x00–0x99)
- **Registradores**: `r0`, `r1`, `r2`, `r3`
- **Operandos**: `ro0`, `ro1`
- **Imediato**:
  - Ocupa 16 bits (2 bytes)
  - Primeiro byte = MSB, segundo = LSB

---

## 🔄 Fluxo de Execução

1. O `main.exe` lê `input.txt`, carregando dados e instruções na memória.
2. A CPU executa a partir do endereço `0x00`, interpretando os bytes conforme o formato.
3. Dados (`d`) são armazenados na memória; instruções (`i`) são lidas e processadas.

---

## 🧑‍🏫 Créditos

- 🔹 Idealização: **Prof. Dr. Hugo Vinicius Leão e Silva** (IFG – Campus Anápolis)  
- 👨‍💻 Desenvolvimento por: [@HelloPeeter](https://github.com/HelloPeeter), [@guitaveira](https://github.com/guitaveira)

---

*Este projeto foi configurado para facilitar o aprendizado de arquiteturas de CPU e sistemas de controle em baixo nível.*  
