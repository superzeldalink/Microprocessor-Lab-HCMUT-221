module TwoByTwoMatMul(
	input [31:0] a, b,
	output [63:0] c
);

wire[7:0] a0, a1, a2, a3, b0, b1, b2, b3;

assign a0 = a[31:24];
assign a1 = a[23:16];
assign a2 = a[15:8];
assign a3 = a[7:0];

assign b0 = b[31:24];
assign b1 = b[23:16];
assign b2 = b[15:8];
assign b3 = b[7:0];
//assign c[63:48] = a[31:24]*b[31:24]+a[23:16]*b[15:8];
//assign c[47:32] = a[31:24]*b[23:16]+a[23:16]*b[7:0];
//assign c[31:16] = a[15:8]*b[31:24]+a[7:0]*b[15:8];
//assign c[15:0]  = a[15:8]*b[23:16]+a[7:0]*b[7:0];

assign c[63:48] = a0*b0+a1*b1;
assign c[47:32] = a0*b2+a1*b3;
assign c[31:16] = a2*b0+a3*b1;
assign c[15:0]  = a2*b2+a3*b3;

endmodule

// a b | e f
// c d | g h

// a b | e g
// c d | f h