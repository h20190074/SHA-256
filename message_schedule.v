

module message_schedule(clk,msg_input,output_w,rst,done);
  
  input[23:0]msg_input;

  input clk;
  input rst;
  output reg[31:0]output_w;
  
  //32 bit words 
  reg [31:0]w[0:63];
 output reg done;
  reg [31:0]sigma0,sigma1;
  reg[6:0] i;
  reg [6:0]j;

	
always@(posedge clk)
begin
  if(rst)
    begin
      w[0]=32'b0;
      w[1]=32'b0;
      w[2]=32'b0;
      w[3]=32'b0;
      w[4]=32'b0;
      w[5]=32'b0;
      w[6]=32'b0;
      w[7]=32'b0;
      w[8]=32'b0;
      w[9]=32'b0;
      w[10]=32'b0;
      w[11]=32'b0;
      w[12]=32'b0;
      w[13]=32'b0;
      w[14]=32'b0;
      w[15]=32'b0;
      j=7'b0;
      done=1'b0;
    end
    
    //pre-processing: appending 1 to message,then 0s to get 448 bits,
    //then last 64 bits giving the length of the message. Divide these 512 bits into 32 bit words (w0 to w15)
  else
    begin
      w[0]={msg_input,1'b1,7'b0000000}; 
      w[1]=32'b0;
      w[2]=32'b0;
      w[3]=32'b0;
      w[4]=32'b0;
      w[5]=32'b0;
      w[6]=32'b0;
      w[7]=32'b0;
      w[8]=32'b0;
      w[9]=32'b0;
      w[10]=32'b0;
      w[11]=32'b0;
      w[12]=32'b0;
      w[13]=32'b0;
      w[14]=32'b0;
      w[15]=32'b00000000_00000000_00000000_00011000; //24 is the length of message signal
    end
  end
  
  
  
  
  
 //extending the message schedule to 64 words  
   always@(posedge clk)
   begin
     
     if(rst)
       begin
         i=16;
       end
       
     else
       begin
         if(i!=64)
           begin
             
           sigma0=({w[i-15][6:0],w[i-15][31:7]} ^ {w[i-15][17:0],w[i-15][31:18]} ^ (w[i-15]>>3));
           sigma1=({w[i-2][16:0], w[i-2][31:17]} ^ {w[i-2][18:0], w[i-2][31:19]} ^ (w[i-2] >> 10));
           w[i]=sigma0+sigma1+w[i-7]+w[i-16];
           i=i+1;
          
           end
           
         else if(i==7'd64)
           begin
            //  i=0;
           done=1'b1;
          
         end
           
       end
       
     end
       
       
       always@(posedge clk)
       begin
         if(done==1'b1 && j!=7'd64)
           begin
             output_w=w[j];
            j=j+1;
           end
           
           
           end
           
endmodule         
    
      

