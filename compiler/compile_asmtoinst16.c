#include <stdio.h>
#include <stdlib.h>
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
    char R2;
}InS;

char* trOpcode(char* opcode, char* out2){
    char* out1 = malloc(1);
    if(strcmp(opcode, "imm") == 0){
        *out1 = 0;
        *out2 = 2;
    }
    else if(strcmp(opcode, "jump") == 0){
        *out1 = 1;
        *out2 = 1;
    }
    else if(strcmp(opcode, "load") == 0){
        *out1 = 2;
    }
    else if(strcmp(opcode, "save") == 0){
        *out1 = 3;
    }
    else if(strcmp(opcode, "push") == 0){
        *out1 = 4;
    }
    else if(strcmp(opcode, "pop") == 0){
        *out1 = 5;
    }
    else if(strcmp(opcode, "sl") == 0){
        *out1 = 6;
        *out2 = 1;
    }
    else if(strcmp(opcode, "sr") == 0){
        *out1 = 7;
        *out2 = 1;
    }
    else if(strcmp(opcode, "add") == 0){
        *out1 = 8;
    }
    else if(strcmp(opcode, "sub") == 0){
        *out1 = 9;
    }
    else if(strcmp(opcode, "mul") == 0){
        *out1 = 10;
    }
    else if(strcmp(opcode, "div") == 0){
        *out1 = 11;
    }
    else if(strcmp(opcode, "and") == 0){
        *out1 = 12;
    }
    else if(strcmp(opcode, "or") == 0){
        *out1 = 13;
    }
    else if(strcmp(opcode, "xor") == 0){
        *out1 = 14;
    }
    else if(strcmp(opcode, "not") == 0){
        *out1 = 15;
        *out2 = 1;
    }
    else{
        printf("Compile Error! Unknown Opcode! %d line '%s' \n", line, opcode);
        exit(-1);
    }
    return out1;
}

char* trReg(char* reg){
    char* out1 = malloc(1);
    if(strcmp(reg, "zero") == 0){
        *out1 = 0;
    }
    else if(strcmp(reg, "csr") == 0){
        *out1 = 1;
    }
    else if(strcmp(reg, "pc") == 0){
        *out1 = 2;
    }
    else if(strcmp(reg, "sp") == 0){
        *out1 = 3;
    }
    else if(strcmp(reg, "back") == 0){
        *out1 = 4;
    }
    else if(strcmp(reg, "r0") == 0){
        *out1 = 5;
    }
    else if(strcmp(reg, "r1") == 0){
        *out1 = 6;
    }
    else if(strcmp(reg, "r2") == 0){
        *out1 = 7;
    }
    else if(strcmp(reg, "r3") == 0){
        *out1 = 8;
    }
    else if(strcmp(reg, "r4") == 0){
        *out1 = 9;
    }
    else if(strcmp(reg, "r5") == 0){
        *out1 = 10;
    }
    else if(strcmp(reg, "r6") == 0){
        *out1 = 11;
    }
    else if(strcmp(reg, "r7") == 0){
        *out1 = 12;
    }
    else if(strcmp(reg, "r8") == 0){
        *out1 = 13;
    }
    else if(strcmp(reg, "r9") == 0){
        *out1 = 14;
    }
    else if(strcmp(reg, "r10") == 0){
        *out1 = 15;
    }
    else{
        printf("Compile Error! Unknown Register! %d line, %s \n", line, reg);
        exit(-1);
    }
    return out1;
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Error!: no input file.\n");
        return -1;
    }
    printf("%s\n", argv[1]);
    
    long dataSec = 0;
    long curLoca;

    char oneline[40];
    FILE* ASMFILE = fopen(argv[1], "r");
    //char* filename = argv[1];
    char* filen = strtok(argv[1], ".");

    FILE* BINARYFILE = fopen(strcat(filen, ".bin"), "wb");
    
    while(!feof(ASMFILE)){
        line++;
        fgets(oneline, 39, ASMFILE);
        printf("%s\n", oneline);
        char* copcode = strtok(oneline, " ");
        printf("c");
        char* cRn = strtok(NULL, " ");
        printf("d");
        char* cR1 = strtok(NULL, " ");
        printf("e");
        int iR1 = atoi(cR1);
        char* cR2 = strtok(NULL, " ");
        printf("f");
        int iR2 = atoi(cR2);
        printf("g");

        short oneinstr;

        char out2 = 0;
        char* bOpcode = trOpcode(copcode, &out2);
        char* bRn = trReg(cRn);
        char* bR1;
        char* bR2; 
        if (out2 == 0 || out2 == 1){
            bR1 = trReg(cR1);
        }
        if (out2 == 0){
            bR2 = trReg(cR2);
        }

        if (out2 != 0){
            if (out2 == 2){
                oneinstr = (short)(*bOpcode << 12) + (short)(*bRn << 8) + (iR1 << 4) + iR2;
                free(bOpcode);free(bRn);
                continue;
            }
            oneinstr = (short)(*bOpcode << 12) + (short)(*bRn << 8) + (short)(*bR1 << 4) + iR2;
            free(bOpcode);free(bRn);free(bR1);
            fwrite(&oneinstr, 4, 1, BINARYFILE);
            continue;
        }
        oneinstr = (short)(*bOpcode << 12) + (short)(*bRn << 8) + (short)(*bR1 << 4) + (short)(*bR2);
        free(bOpcode);free(bRn);free(bR1);free(bR2);
        fwrite(&oneinstr, 4, 1, BINARYFILE);
    }
    fclose(ASMFILE);
    fclose(BINARYFILE);
}