#include <stdio.h>

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

typedef struct instr{
    char opcode;
    char Rn;
    char R1;
    char R2
}InS;

void translateOpcode(){

}

void translateRegister(){
    
}

int main(){
    //
}