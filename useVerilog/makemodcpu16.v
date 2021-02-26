module vz16 (
    input clk,
    input reset,
    
    input [15:0] instrBus,
    output [4:0] ControlBus,

    input [15:0] dataInBus,
    output [15:0] dataOutBus,
    output [15:0] addrBus
)
    reg [15:0] pc;
    reg [15:0] status;
    reg [15:0] registers[13]; // sp ~ r11
    reg [2:0] ALUo;
    reg option;
    reg ALU, LS, STACK, Shift;
    always @(posedge clk) begin
        // decoder
        ALU <= instrBus[3];
        ALUo <= (ALU) ? instrBus[2:0] : 0;
        
        option <= instrBus[0];
        LS <= (ALU) ? 0 : ~instrBus[2] & instrBus[1];
        STACK <= (ALU) ? 0 : instrBus[2] & ~instrBus[1];
        Shift <= (ALU) ? 0 : instrBus[2] & instrBus[1];
        
        Rn <= instrBus[7:4];
        R1 <= instrBus[8:11];
        R2 <= instrBus[15:12];

        
    end
endmodule