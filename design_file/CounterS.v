module CounterS(
  CLK   , // 时钟，上升沿有效
  SCLK ,
  CNTVAL // 输出的计数值信号
  );// 计数溢出信号，计数值为最大值时该信号为1

input CLK,SCLK;
output [7:0] CNTVAL; 
reg [7:0] CNTVAL;
reg OV;
wire SCLK_l2h;
wire SCLK_h2l;
reg SCLK_0,SCLK_1;
assign SCLK_l2h=(SCLK_0 && ~SCLK_1);
assign SCLK_h2l=(~SCLK_0 && SCLK_1);

parameter CNT_MAX_VAL = 8'b11111111;

always@(posedge CLK)
begin
 SCLK_0<=SCLK;
 SCLK_1<=SCLK_0;
end
always @ (posedge CLK) begin
  if(SCLK_h2l) begin
      if(CNTVAL < CNT_MAX_VAL ) begin
         CNTVAL <=CNTVAL +1;
         OV <= 0;
      end
      else begin
         CNTVAL<=0;
      end
  end
  else begin
    CNTVAL <= CNTVAL;
  end
end
endmodule