module tb;
  reg clk;
  wire [31:0]out;
  wire data_valid;
  ascii a1(clk,out,data_valid);
  
  initial
    begin
      $dumpvars;
      $dumpfile("tb.vcd");
    end
  
  initial
    begin
      clk=0;
    #1000  $finish;
    end
  
  initial
    begin
      $monitor("%h %b",out,data_valid);
    end
  
  always #5 clk=~clk;
endmodule
