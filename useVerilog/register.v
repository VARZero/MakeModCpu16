module registers16x16(
    input wire clk,
    input wire reset,
    input wire op,
    input wire [3:0] selectreg,
    input wire [15:0] inimm,
    output wire [15:0] immout
)
    reg [15:0] registers [2:15];

    always @(posedge clk or negedge reset) begin
        if (!reset) begin
            registers[selectreg] <= 16'd0;
        end
        else
    end
endmodule