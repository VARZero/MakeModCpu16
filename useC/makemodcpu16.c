#define RAM_SIZE 2048

#include <stdio.h>
#include <stdlib.h>

unsigned short reg[16] = 0; //zero, status, PC, SP, BACK, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10;

enum Opcodes{
    MOV = 1,
    LOAD,
    SAVE,
    PUSH,
    POP,
    SL,
    SR,
    ADD,
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
    BACK = 8
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

void set0_Bit(short* val, int n){
    *val = *val & (0 << n) >> n;
}

void set1_Bit(short* val, int n){
    *val = *val | (1 << n) >> n;
}

void decoder(InS One){
    Opc = One.opcode;
    char temp = reg[2]; // pc레지스터 변화 여부 확인 (PC레지스터는 일반 레지스터와 다르게 동작합니다.)
    if (One.Rn == 1){
        if (Opc == NOT){
            switch(One.R2){
                case 0:
                    set0_Bit(&reg[1], 15);
                break;
                case 1:
                    set1_Bit(&reg[1], 15);
                break;
            }
        }
        return;
    } // status 레지스터는 NOT 명령어에세만 동작합니다 (양-음수 여부 변경만 가능)
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
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
        break;
        case SR:
            reg[One.Rn] >> One.R1;
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
        break;
        case ADD:
            reg[1] = 0;
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
            reg[1] = 0;
            reg[One.Rn] = reg[One.R1] - reg[One.R2];
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
            if ((unsigned int)((unsigned int)reg[One.R1] - (unsigned int)reg[One.R2]) > sizeof(unsigned short) && get_Bit(reg[1], 15) == 0){
                // 부호없는 연산의 캐리 여부 확인
                set1_Bit(&reg[1], 14);
            }
            if ((int)((int)reg[One.R1] - (int)reg[One.R2]) > sizeof(short) && get_Bit(reg[1], 15) == 1){
                // 부호있는 연산의 오버플로우 여부 확인
                set1_Bit(&reg[1], 12);
            }
        break;
        case MUL:
            reg[1] = 0;
            reg[One.Rn] = reg[One.R1] * reg[One.R2];
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
            if ((unsigned int)((unsigned int)reg[One.R1] * (unsigned int)reg[One.R2]) > sizeof(unsigned short) && get_Bit(reg[1], 15) == 0){
                // 부호없는 연산의 캐리 여부 확인
                set1_Bit(&reg[1], 14);
            }
            if ((int)((int)reg[One.R1] * (int)reg[One.R2]) > sizeof(short) && get_Bit(reg[1], 15) == 1){
                // 부호있는 연산의 오버플로우 여부 확인
                set1_Bit(&reg[1], 12);
            }
        break;
        case DIV:
            reg[1] = 0;
            if (reg[One.R1] == 0 || reg[One.R2] == 0){break;}
            reg[One.Rn] = reg[One.R1] / reg[One.R2];
            reg[15] = reg[One.R1] % reg[One.R2];
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
        break;
        case AND:
            reg[1] = 0;
            reg[One.Rn] = reg[One.R1] & reg[One.R2];
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
        break;
        case OR:
            reg[1] = 0;
            reg[One.Rn] = reg[One.R1] | reg[One.R2];
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
        break;
        case XOR:
            reg[1] = 0;
            reg[One.Rn] = reg[One.R1] ^ reg[One.R2];
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
        break;
        case NOT:
            reg[1] = 0;
            reg[One.Rn] = !reg[One.R1];
            if (reg[One.Rn] == 0){
                set1_Bit(&reg[1], 13);
            }
        break;
    }
    reg[0] = 0; // zero 레지스터는 항상 0을 유지하도록 돌아갑니다.
    if (reg[2] == temp){reg[2] += 2;} // PC레지스터가 변화하지 않았으면 +2하도록 함
}

int main(){
    reg[0] = 0;
    int i = 0;
    Ram = malloc(RAM_SIZE);
    unsigned short oneline;
    FILE* Rom = fopen("Rom", "rb");
    printf("start\n");

    // 롬에서 램으로 데이터 갖고 오자
    while(feof(Rom) == 0){
        if (i == RAM_SIZE){
            break;
        }
        fread(Ram[i], 2, 1, Rom);
        i++;
    }
    fclose(Rom);
    
    reg[14] = 1536; //최초 실행시 데이터 영역의 주소를 저장
    while(1)
    {
        oneline = Ram[reg[2]];
        InS IST;
        printf("%x\n", oneline);
        IST.opcode = (char)((oneline & 61440) >> 8);
        IST.Rn = (char)((oneline & 3840) >> 4);
        IST.R1 = (char)(oneline & 240);
        IST.R2 = (char)(oneline & 15);
        printf("line %x %x %x %x\n", IST.opcode, IST.Rn, IST.R1, IST.R2);
        decoder(IST);
    }
}
