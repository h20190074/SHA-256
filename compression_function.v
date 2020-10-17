//for computing hash values of a to h

module compression_function(clk,rst,output_w,msg_output,done);
  
  input clk;
  input rst;
  input done;
  input[31:0] output_w;
  reg [31:0] a_in,b_in,c_in,d_in,e_in,f_in,g_in,h_in;
  reg [31:0] a_out,b_out,c_out,d_out,e_out,f_out,g_out,h_out;
  reg [31:0]T1,T2;
  reg[31:0]CapSigma0,CapSigma1;
  reg[31:0]choice,majority;
  reg [31:0]w[0:63];
  reg start;
  reg[6:0] j,m;
  output reg [255:0] msg_output;
 
   
localparam [31:0]k[0:63] =

'{
32'h428a2f98,32'h71374491,32'hb5c0fbcf,32'he9b5dba5,32'h3956c25b,32'h59f111f1,32'h923f82a4,32'hab1c5ed5,
32'hd807aa98,32'h12835b01,32'h243185be,32'h550c7dc3,32'h72be5d74,32'h80deb1fe,32'h9bdc06a7,32'hc19bf174,
32'he49b69c1,32'hefbe4786,32'h0fc19dc6,32'h240ca1cc,32'h2de92c6f,32'h4a7484aa,32'h5cb0a9dc,32'h76f988da,
32'h983e5152,32'ha831c66d,32'hb00327c8,32'hbf597fc7,32'hc6e00bf3,32'hd5a79147,32'h06ca6351,32'h14292967,
32'h27b70a85,32'h2e1b2138,32'h4d2c6dfc,32'h53380d13,32'h650a7354,32'h766a0abb,32'h81c2c92e,32'h92722c85,
32'ha2bfe8a1,32'ha81a664b,32'hc24b8b70,32'hc76c51a3,32'hd192e819,32'hd6990624,32'hf40e3585,32'h106aa070,
32'h19a4c116,32'h1e376c08,32'h2748774c,32'h34b0bcb5,32'h391c0cb3,32'h4ed8aa4a,32'h5b9cca4f,32'h682e6ff3,
32'h748f82ee,32'h78a5636f,32'h84c87814,32'h8cc70208,32'h90befffa,32'ha4506ceb,32'hbef9a3f7,32'hc67178f2
};
  
  
//Intial hash values (first 32 bits of fractional parts of square roots of 1st 8 prime numbers)
	
	localparam H0 = 32'h6a09e667;
	localparam H1 = 32'hbb67ae85;
	localparam H2 = 32'h3c6ef372;
	localparam H3 = 32'ha54ff53a;
	localparam H4 = 32'h510e527f;
	localparam H5 = 32'h9b05688c;
	localparam H6 = 32'h1f83d9ab;
	localparam H7 = 32'h5be0cd19;
	
	always@(negedge clk)
	begin
	  
	  if(done==1'b1)
	    begin
	      w[m]=output_w;
	      m=m+1;
	     end
	     
	     if(m==64)
	       begin
	       start=1;
	       m=0;
	       end
	      
	  end
	
	always@(posedge clk)
	begin
	  if(rst)
	    begin
	      a_in<=H0;
	      b_in<=H1;
	      c_in<=H2;
	      d_in<=H3;
	      e_in<=H4;
	      f_in<=H5;
	      g_in<=H6;
	      h_in<=H7;
	      
	      j=0;
	      m=0;
	      start=0;
	      
	      
	    end
	      
	      
	   else
	     begin
	       if(j!=64 && start==1)
	         begin
	           CapSigma0=({a_in[1:0],e_in[31:2]} ^ {e_in[12:0],e_in[31:13]} ^ {e_in[21:0],e_in[31:22]});
	           CapSigma1=({e_in[5:0],e_in[31:6]} ^ {e_in[10:0],e_in[31:11]} ^ {e_in[24:0],e_in[31:25]});
	           choice=(e_in & f_in) ^ ((~e_in) & g_in);
	           majority=(a_in & b_in) ^ (a_in & c_in) ^ (b_in & c_in);
	           T1= CapSigma1 + choice + h_in + k[j] +w[j];
	           T2= CapSigma0 +  majority;
	           
	           a_in<=T1 + T2;
	           b_in<=a_in;
	           c_in<=b_in;
	           d_in<=c_in;
	           e_in<=d_in + T1;
	           f_in<=e_in;
	           g_in<=f_in;
	           h_in<=g_in;
	           
	           j=j+1;
	           end
	           
	           else if(j==64)
	             begin
	               a_out<=H0+a_in;
	               b_out<=H1+b_in;
	               c_out<=H2+c_in;
	               d_out<=H3+d_in;
	               e_out<=H4+e_in;
	               f_out<=H5+f_in;
	               g_out<=H6+g_in;
	               h_out<=H7+h_in;
	               
	               msg_output={a_out,b_out,c_out,d_out,e_out,f_out,g_out,h_out};
	               
	               end
	       end
       
	    end
	    endmodule
	       
	        
	      
	      


