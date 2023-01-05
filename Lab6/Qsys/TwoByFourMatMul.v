module TwoByFourMatMul (
	input [63:0] a, b,
	output[63:0] c
);

wire [63:0] ca, cb;

TwoByTwoMatMul matmul1({a[63:48],a[31:16]},{b[63:48],b[31:16]},ca);
TwoByTwoMatMul matmul2({a[47:32],a[15:0]},{b[47:32],b[15:0]},cb);

genvar i;
generate
for(i=16; i<=64; i=i+16) begin: gen
	assign c[(i-1)-:16] = ca[(i-1)-:16] + cb[(i-1)-:16];
end
endgenerate

//assign c[63:48] = ca[0] + cb[0];
//assign c[47:32] = ca[1] + cb[1];
//assign c[31:16] = ca[2] + cb[2];
//assign c[15:0] = ca[3] + cb[3];

endmodule
