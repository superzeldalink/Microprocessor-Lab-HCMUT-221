module MatMul_top (
	input clk, read, resetn, cs,
	input[31:0] writedata,
	output[31:0] readdata,
	output[7:0] conduit
);

wire[7:0] from_mm, to_mm;

assign conduit = from_mm;

assign readdata = {24'b0, from_mm};
assign to_mm = writedata[7:0];

MatMul mm(.clk(clk), .cs(cs), .rw(read), .comdat(to_mm[7]), .data_in({1'b0, to_mm[6:0]}), .data_out(from_mm));

endmodule 
