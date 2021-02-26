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
    reg [15:0] registers[13]; // sp ~ r11
    always @(posedge clk) begin
        // decoder
        ALU = instrBus[3];
        
    end
endmodule