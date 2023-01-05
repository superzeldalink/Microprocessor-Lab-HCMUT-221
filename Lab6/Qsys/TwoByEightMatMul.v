module TwoByEightMatMul (
	input [127:0] a, b,
	output[63:0] c
);

wire [63:0] ca, cb, cc, cd;

TwoByTwoMatMul matmul1({a[127-:16],a[63-:16]},{b[127-:16],b[63-:16]},ca);
TwoByTwoMatMul matmul2({a[111-:16],a[47-:16]},{b[111-:16],b[47-:16]},cb);
TwoByTwoMatMul matmul3({a[95-:16],a[31-:16]},{b[95-:16],b[31-:16]},cc);
TwoByTwoMatMul matmul4({a[79-:16],a[15:0]},{b[79-:16],b[15:0]},cd);

genvar i;
generate
for(i=16; i<=64; i=i+16) begin: gen
	assign c[(i-1)-:16] = ca[(i-1)-:16] + cb[(i-1)-:16] + cc[(i-1)-:16] + cd[(i-1)-:16];
end
endgenerate

//assign c[63:48] = ca[0] + cb[0];
//assign c[47:32] = ca[1] + cb[1];
//assign c[31:16] = ca[2] + cb[2];
//assign c[15:0] = ca[3] + cb[3];

endmodule
