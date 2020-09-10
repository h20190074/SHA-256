
module ascii(clk,out,data_valid);
  reg [31:0]a;
  output reg data_valid;
  reg [31:0]mem[500:0];
  input clk;
  output reg [31:0]out;
  integer file1,char,i,k;
  
  initial
  begin
    i=0;
    file1=$fopen("test.txt","r");
    while(!$feof(file1))
    begin
      char=$fgetc(file1);
      mem[i]=char;
      i=i+1;
     end
  end
  
  
  initial
    begin
      k=0;
    end
  
  always @(posedge clk)
    begin
      if(k!=i)
        begin
      out=mem[k];
      k=k+1;
          data_valid=1'b1;
        end
      else
        data_valid=1'b0;
    end
endmodule
      
