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
    reg [15:0] REGs[13]; // sp ~ r11
    reg [2:0] ALUo;
    reg option;
    reg ALU, LS, STACK, Shift;

    // 옵코드 구분
    reg [3:0] Rn;
    reg [3:0] R1;
    reg [3:0] R2;

    // 가져온 레지스터
    reg [4:0] VRn;
    reg [3:0] VR1;
    reg [3:0] VR2;
    
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

        // ALU
        if (ALU) begin
            VRn <= 4'b0000;
            VR1 <= 4'b0000;
            if (R1 > 2) begin
                VR1 <= REGs[R1 - 2];
            end
            else if (R1 == 1) begin
                VR1 <= status;
            end
            VR2 <= 4'b0000;
            if (R2 > 2) begin
                VR2 <= REGs[R2 - 2];
            end
            else if (R2 == 1) begin
                VR2 <= status;
            end

            case (ALUo) 
                3'b000: VRn <= VR1 + VR2;
                3'b001: VRn <= VR1 - VR2;
                3'b010: VRn <= VR1 * VR2;
                3'b100: VRn <= VR1 & VR2;
                3'b101: VRn <= VR1 | VR2;
                3'b110: VRn <= VR1 ^ VR2;
            endcase
            if (ALUo == 3'b011) begin
                VRn <= VR1 / VR2;
                REGs[12] <= VR1 % VR2;
            end
            else if (ALUo == 3'b111 && Rn != 4'b0001) begin
                VRn <= VRn ! VR1;
            end

            // status 적용
            status[14:12] <= 3'b000;
            if (VRn == 5'b00000) begin
                status[13] <= 1'b1; // Zero비트
            end
            else if (VRn[4] == 1'b1) begin
                if (status[15] == 1'b0) begin
                    status[14] <= 1'b1;
                end
                else if (status[15] == 1'b1) begin
                    status[12] <= 1'b1;
                end
            end

            // Not에 status 사용시
            else if (ALUo == 3'b111 && Rn == 4'b0001) begin
                status[15] <= R1[0]; // sign 여부 비트 설정
            end
        end

        // Shift
        if (Shift) begin
            
        end

    end
endmodule