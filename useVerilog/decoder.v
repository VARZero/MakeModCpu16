module decoder(
    input wire [3:0] opcode,
    output wire IMMs,
    output wire Branch,
    output wire [1:0] ALUs,
    output wire PCs,
    output wire [1:0] STACKs,
    output wire [1:0] SHIFTs,
    output wire [1:0] Memory
);
    assign ALUs[0] = opcode[3];
    assign ALUs[1] = opcode[3] & opcode[2];
    assign IMMs = !opcode[3] & !opcode[2] & !opcode[1] & !opcode[0];
    assign Branch = !opcode[3] & !opcode[2] & !opcode[1] & opcode[0];
    assign Memory[0] = !opcode[3] & !opcode[2] & opcode[1];
    assign Memory[1] = !opcode[3] & !opcode[2] & opcode[1] & opcode[0];
    assign STACKs[0] = !opcode[3] & opcode[2] & !opcode[1];
    assign STACKs[1] = !opcode[3] & opcode[2] & !opcode[1] & opcode[0];
    assign SHIFTs[0] = !opcode[3] & opcode[2] & opcode[1];
    assign SHIFTs[1] = !opcode[3] & opcode[2] & opcode[1] & opcode[0];

endmodule