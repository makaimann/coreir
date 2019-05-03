/* External Modules
module SB_RAM40_4K (
  input [15:0] MASK,
  input [10:0] RADDR,
  input  RCLK,
  input  RCLKE,
  output [15:0] RDATA,
  input  RE,
  input [10:0] WADDR,
  input  WCLK,
  input  WCLKE,
  input [15:0] WDATA,
  input  WE
);

endmodule  // SB_RAM40_4K

*/
module top (
  input [15:0] MASK,
  input [10:0] RADDR,
  input  RCLK,
  input  RCLKE,
  output [15:0] RDATA,
  input  RE,
  input [10:0] WADDR,
  input  WCLK,
  input  WCLKE,
  input [15:0] WDATA,
  input  WE
);


  wire [15:0] SB_RAM40_4K_inst0__MASK;
  wire [10:0] SB_RAM40_4K_inst0__RADDR;
  wire  SB_RAM40_4K_inst0__RCLK;
  wire  SB_RAM40_4K_inst0__RCLKE;
  wire [15:0] SB_RAM40_4K_inst0__RDATA;
  wire  SB_RAM40_4K_inst0__RE;
  wire [10:0] SB_RAM40_4K_inst0__WADDR;
  wire  SB_RAM40_4K_inst0__WCLK;
  wire  SB_RAM40_4K_inst0__WCLKE;
  wire [15:0] SB_RAM40_4K_inst0__WDATA;
  wire  SB_RAM40_4K_inst0__WE;
  SB_RAM40_4K #(.INIT_0(256'h0000000000000000000000000000000000000000000000000000000000ff00fe),.INIT_1(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_2(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_3(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_4(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_5(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_6(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_7(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_8(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_9(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_A(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_B(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_C(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_D(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_E(256'h0000000000000000000000000000000000000000000000000000000000000000),.INIT_F(256'h0000000000000000000000000000000000000000000000000000000000000000),.READ_MODE(0),.WRITE_MODE(0)) SB_RAM40_4K_inst0(
    .MASK(SB_RAM40_4K_inst0__MASK),
    .RADDR(SB_RAM40_4K_inst0__RADDR),
    .RCLK(SB_RAM40_4K_inst0__RCLK),
    .RCLKE(SB_RAM40_4K_inst0__RCLKE),
    .RDATA(SB_RAM40_4K_inst0__RDATA),
    .RE(SB_RAM40_4K_inst0__RE),
    .WADDR(SB_RAM40_4K_inst0__WADDR),
    .WCLK(SB_RAM40_4K_inst0__WCLK),
    .WCLKE(SB_RAM40_4K_inst0__WCLKE),
    .WDATA(SB_RAM40_4K_inst0__WDATA),
    .WE(SB_RAM40_4K_inst0__WE)
  );

  assign SB_RAM40_4K_inst0__MASK[15:0] = MASK[15:0];

  assign SB_RAM40_4K_inst0__RADDR[10:0] = RADDR[10:0];

  assign SB_RAM40_4K_inst0__RCLK = RCLK;

  assign SB_RAM40_4K_inst0__RCLKE = RCLKE;

  assign RDATA[15:0] = SB_RAM40_4K_inst0__RDATA[15:0];

  assign SB_RAM40_4K_inst0__RE = RE;

  assign SB_RAM40_4K_inst0__WADDR[10:0] = WADDR[10:0];

  assign SB_RAM40_4K_inst0__WCLK = WCLK;

  assign SB_RAM40_4K_inst0__WCLKE = WCLKE;

  assign SB_RAM40_4K_inst0__WDATA[15:0] = WDATA[15:0];

  assign SB_RAM40_4K_inst0__WE = WE;


endmodule  // top
