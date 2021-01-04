#include <stdio.h>
#include <stdlib.h>

unsigned short reg[16]; //zero, status, PC, SP, BACK, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10;

enum Opcodes{
    MOV = 1,
    LOAD,
    SAVE,
    PUSH,
    POP,
    SL,
    SR,
    ADD = 9,
    SUB,
    MUL,
    DIV,
    AND,
    OR,
    XOR,
    NOT
}Opc;

enum movOpcOption{
    NONE,
    SAME,
    LSAME,
    L,
    RSAME,
    R,
    BACK = 9
}MOO;

short* Ram;

typedef struct instr{
    char opcode;
    char Rn;
    char R1;
    char R2
}InS;

int get_Bit(short val, int n){
    // http://mwultong.blogspot.com/2007/06/c-1-get-bit-read-bit.html
    return (val & (1 << n)) >> n;
}

void set1_Bit(short* val, int n){
    *val = *val | (1 << n) >> n;
}

void decoder(InS One){
    Opc = One.opcode;
    if (One.Rn == 1){return;} // status 레지스터는 직접 접근할 수 없습니다. 아키텍쳐 상에서 인터럽트 바로 걸어버림
    switch(Opc){
        case MOV:
            MOO = One.R2;
            if (!(MOO == NONE) || // 조건이 아예 없을때
            (!(MOO == SAME && get_Bit(reg[1], 13) == 1)) || // = 
            (!(MOO == LSAME && (get_Bit(reg[1], 13) == 1 || get_Bit(reg[1], 15) != 1))) || // >=
            (!(MOO == L && get_Bit(reg[1], 13) != 1 && get_Bit(reg[1], 15) != 1)) || // >
            (!(MOO == RSAME && (get_Bit(reg[1], 13) == 1 || get_Bit(reg[1], 15) == 1))) || // <=
            (!(MOO == R && get_Bit(reg[1], 13) != 1 && get_Bit(reg[1], 15) == 1))) // <
            {break;}
            else if (MOO == BACK){
                reg[2] = reg[4];
                break;
            }
            reg[4] = reg[2] + 2;
            reg[2] = One.R1;
        break;
        case LOAD:
            reg[One.Rn] = Ram[(One.R1)/2];
        break;
        case SAVE:
            Ram[(One.Rn)/2] = reg[One.R1];
        break;
        case PUSH:
            Ram[reg[3]] = reg[One.R1];
            reg[3] += 2;
        break;
        case POP:
            reg[One.Rn] = Ram[reg[3]];
            reg[3] -= 2;
        break;
        case SL:
            reg[One.Rn] << One.R1;
        break;
        case SR:
            reg[One.Rn] >> One.R1;
        break;
        case ADD:
            reg[One.Rn] = reg[One.R1] + reg[One.R2];
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
            if ((unsigned int)((unsigned int)reg[One.R1] + (unsigned int)reg[One.R2]) > sizeof(unsigned short) && get_Bit(reg[1], 15) == 0){
                // 부호없는 연산의 캐리 여부 확인
                set1_Bit(&reg[1], 14);
            }
            if ((int)((int)reg[One.R1] + (int)reg[One.R2]) > sizeof(short) && get_Bit(reg[1], 15) == 1){
                // 부호있는 연산의 오버플로우 여부 확인
                set1_Bit(&reg[1], 12);
            }
        break;
        case SUB:
            reg[One.Rn] = reg[One.R1] - reg[One.R2];
        break;
        case MUL:
            reg[One.Rn] = reg[One.R1] * reg[One.R2];
        break;
        case DIV:
            reg[One.Rn] = reg[One.R1] / reg[One.R2];
            reg[15] = reg[One.R1] % reg[One.R2];
        break;
        case AND:
            reg[One.Rn] = reg[One.R1] & reg[One.R2];
        break;
        case OR:
            reg[One.Rn] = reg[One.R1] | reg[One.R2];
        break;
        case XOR:
            reg[One.Rn] = reg[One.R1] ^ reg[One.R2];
        break;
        case NOT:
            reg[One.Rn] = !reg[One.R1];
        break;
    }
    reg[0] = 0; // zero 레지스터는 항상 0을 유지하도록 돌아갑니다.
}

int main(){
    reg[0] = 0;
    Ram = malloc(2048);
}
