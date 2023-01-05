module MatMul (
	input clk,
	input rw, cs, comdat,
	input[7:0] data_in,
	output[7:0] data_out
);

reg[7:0] ram_a[0:63], ram_b[0:63];
reg[7:0] a[0:15], b[0:15];
wire[63:0] c;
reg[7:0] ab[0:127];
reg[6:0] addr = 0, ab_addr = 0;

//initial begin
//  $readmemh("test.mem", ram_a);
//  $readmemh("test.mem", ram_b);
//end

TwoByEightMatMul mm({a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11],a[12],a[13],a[14],a[15]},
					  {b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],b[11],b[12],b[13],b[14],b[15]},
						c);

assign data_out = ab[addr];

localparam S0 = 4'd0, S1 = 4'd1, S2 = 4'd3, S3 = 4'd2, S4 = 4'd4, S5 = 4'd5, S6 = 4'd7, S7 = 4'd6, S8 = 4'd8, S9 = 4'd9, S10 = 4'd11, S11 = 4'd10,
			  S12 = 4'd12, S13 = 4'd13, S14 = 4'd15, S15 
			  = 4'd14;

reg[3:0] state = S0, nextstate = S0;

integer i;
always @(posedge clk) begin
	if(cs) begin
		if(rw == 1) begin								// READ
			addr <= addr + 7'b0000001;
		end else begin									// WRITE
			if(comdat == 0) begin					// 	DATA
				addr <= addr + 7'b0000001;
				if(addr[6] == 0)
					ram_a[addr[5:0]] <= data_in;
				else
					ram_b[addr[5:0]] <= data_in;
			end else begin								//		COMMAND
				addr <= data_in[6:0];
			end
		end
	end
end


always @(posedge clk) begin
	state <= nextstate;
//	{ab[ab_addr],ab[ab_addr+1],ab[ab_addr+2],ab[ab_addr+3],ab[ab_addr+16],ab[ab_addr+17],ab[ab_addr+18],ab[ab_addr+19]} <= c;
	ab[ab_addr] <= c[63-:8];
	ab[ab_addr+1] <= c[55-:8];
	ab[ab_addr+2] <= c[47-:8];
	ab[ab_addr+3] <= c[39-:8];
	ab[ab_addr+16] <= c[31-:8];
	ab[ab_addr+17] <= c[23-:8];
	ab[ab_addr+18] <= c[15-:8];
	ab[ab_addr+19] <= c[7:0]; 
end

always @(state)begin
	nextstate <= state + 4'b1;
	case(state)
		S0: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i];
				b[i] <= ram_b[8*i];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[8+i];
				b[8+i] <= ram_b[8*i+1];
			end
			ab_addr <= 0; 
		end

		S1: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i];
				b[i] <= ram_b[8*i+2];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[8+i];
				b[8+i] <= ram_b[8*i+3];
			end
			ab_addr <= 4;
		end

		S2: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i];
				b[i] <= ram_b[8*i+4];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[8+i];
				b[8+i] <= ram_b[8*i+5];
			end
			ab_addr <= 8;
		end

		S3: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i];
				b[i] <= ram_b[8*i+6];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[8+i];
				b[8+i] <= ram_b[8*i+7];
			end
			ab_addr <= 12;
		end

		//
		S4: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+16];
				b[i] <= ram_b[8*i];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+24];
				b[8+i] <= ram_b[8*i+1];
			end
			ab_addr <= 32;
		end

		S5: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+16];
				b[i] <= ram_b[8*i+2];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+24];
				b[8+i] <= ram_b[8*i+3];
			end
			ab_addr <= 36;
		end

		S6: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+16];
				b[i] <= ram_b[8*i+4];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+24];
				b[8+i] <= ram_b[8*i+5];
			end
			ab_addr <= 40;
		end

		S7: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+16];
				b[i] <= ram_b[8*i+6];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+24];
				b[8+i] <= ram_b[8*i+7];
			end
			ab_addr <= 44;
		end

		//
		S8: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+32];
				b[i] <= ram_b[8*i];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+40];
				b[8+i] <= ram_b[8*i+1];
			end
			ab_addr <= 64;
		end

		S9: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+32];
				b[i] <= ram_b[8*i+2];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+40];
				b[8+i] <= ram_b[8*i+3];
			end
			ab_addr <= 68;
		end

		S10: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+32];
				b[i] <= ram_b[8*i+4];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+40];
				b[8+i] <= ram_b[8*i+5];
			end
			ab_addr <= 72;
		end

		S11: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+32];
				b[i] <= ram_b[8*i+6];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+40];
				b[8+i] <= ram_b[8*i+7];
			end
			ab_addr <= 76;
		end

		//
		S12: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+48];
				b[i] <= ram_b[8*i];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+56];
				b[8+i] <= ram_b[8*i+1];
			end
			ab_addr <= 96;
		end

		S13: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+48];
				b[i] <= ram_b[8*i+2];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+56];
				b[8+i] <= ram_b[8*i+3];
			end
			ab_addr <= 100;
		end

		S14: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+48];
				b[i] <= ram_b[8*i+4];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+56];
				b[8+i] <= ram_b[8*i+5];
			end
			ab_addr <= 104;
		end

		S15: begin
			for(i=0; i<8; i=i+1)begin
				a[i] <= ram_a[i+48];
				b[i] <= ram_b[8*i+6];
			end
			for(i=0; i<8; i=i+1)begin
				a[8+i] <= ram_a[i+56];
				b[8+i] <= ram_b[8*i+7];
			end
			ab_addr <= 108;
		end

		default: begin
			for(i=0; i<16; i=i+1)begin
				a[i] <= 8'd0;
				b[i] <= 8'd0;
			end
			ab_addr <= 0;
		end

	endcase
end

endmodule
