#define enumtostring(x) #x
#include <stdio.h>
#include <string.h>

int line = 0;

enum movOpcOption{
    NONE,
    SAME,
    LSAME,
    L,
    RSAME,
    R,
    BACK = 9
}MOO;

typedef struct instr{
    char opcode;
    char Rn;
    char R1;
    char R2
}InS;

char* trOpcode(char* opcode, char* out2){
    char* out1;
    if(strcmp(opcode, "mov ") == 0){
        *out1 = 1;
    }
    else if(strcmp(opcode, "load ") == 0){
        *out1 = 2;
    }
    else if(strcmp(opcode, "save ") == 0){
        *out1 = 3;
    }
    else if(strcmp(opcode, "push ") == 0){
        *out1 = 4;
    }
    else if(strcmp(opcode, "pop ") == 0){
        *out1 = 5;
    }
    else if(strcmp(opcode, "sl ") == 0){
        *out1 = 6;
    }
    else if(strcmp(opcode, "sr ") == 0){
        *out1 = 7;
    }
    else if(strcmp(opcode, "add ") == 0){
        *out1 = 8;
    }
    else if(strcmp(opcode, "sub ") == 0){
        *out1 = 9;
    }
    else if(strcmp(opcode, "mul ") == 0){
        *out1 = 10;
    }
    else if(strcmp(opcode, "div ") == 0){
        *out1 = 11;
    }
    else if(strcmp(opcode, "and ") == 0){
        *out1 = 12;
    }
    else if(strcmp(opcode, "or ") == 0){
        *out1 = 13;
    }
    else if(strcmp(opcode, "xor ") == 0){
        *out1 = 14;
    }
    else if(strcmp(opcode, "not ") == 0){
        *out1 = 15;
    }
    else{
        printf("Compile Error! Unknown Opcode! %d line '%s' \n", line, *out1);
        exit(-1);
    }
    return out1;
}

char* trReg(char* reg){
    char* out1;
    if(strcmp(reg, "zero") == 0){
        *out1 = 0;
    }
    else if(strcmp(reg, "csr ") == 0){
        *out1 = 1;
    }
    else if(strcmp(reg, "pc ") == 0){
        *out1 = 2;
    }
    else if(strcmp(reg, "sp ") == 0){
        *out1 = 3;
    }
    else if(strcmp(reg, "back ") == 0){
        *out1 = 4;
    }
    else if(strcmp(reg, "r0 ") == 0){
        *out1 = 5;
    }
    else if(strcmp(reg, "r1 ") == 0){
        *out1 = 6;
    }
    else if(strcmp(reg, "r2 ") == 0){
        *out1 = 7;
    }
    else if(strcmp(reg, "r3 ") == 0){
        *out1 = 8;
    }
    else if(strcmp(reg, "r4 ") == 0){
        *out1 = 9;
    }
    else if(strcmp(reg, "r5 ") == 0){
        *out1 = 10;
    }
    else if(strcmp(reg, "r6 ") == 0){
        *out1 = 11;
    }
    else if(strcmp(reg, "r7 ") == 0){
        *out1 = 12;
    }
    else if(strcmp(reg, "r8 ") == 0){
        *out1 = 13;
    }
    else if(strcmp(reg, "r9 ") == 0){
        *out1 = 14;
    }
    else if(strcmp(reg, "r10 ") == 0){
        *out1 = 15;
    }
    else{
        printf("Compile Error! Unknown Register! %d line '%s' \n", line, *out1);
        exit(-1);
    }
    return out1;
}

int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Error!: no input file.\n");
    }

    char oneline[25];
    FILE* ASMFILE = fopen(argv[1], "rt");
    char filename[256] = argv[1];
    char* filen = strtok(filename, ".");

    FILE* BINARYFILE = fopen(strcat(filen, ".bin"), "wb");
    
    while(!feof(ASMFILE)){
        line++;
        fgets(&oneline, 24, ASMFILE);
        
        char* linesl = strtok(oneline, " ");
        char copcode[6] = linesl;
        
        linesl = strtok(NULL, ",");
        char cRn[6] = linesl;

        linesl = strtok(NULL, ",");
        char cR1[6] = linesl;

        linesl = strtok(NULL, ",");
        char cR2[6] = linesl;
        strcat(cR2, " ");

        int oneinstr;

        char* out2 = NULL;
        char* bOpcode = trOpcode(copcode, out2);
        char* bRn = trReg(cRn);
        char* bR1 = trReg(cR1);
        char* bR2 = trReg(cR2);
        
        if (out2 != NULL){
            // 수정바람. trOpcode부분도 수정해야됨
            break;
        }
        oneinstr = (int)(*bOpcode << 12) + (int)(*bRn << 8) + (int)(*bR1 << 4) + (int)(*bR2);
    }
    fclose(ASMFILE);
    fclose(BINARYFILE);
}