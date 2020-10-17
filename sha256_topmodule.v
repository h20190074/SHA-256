

module top_module(clk,msg_input,msg_output,rst);
  
  input[23:0]msg_input;
  input clk;
  input rst;
  output[255:0]msg_output;
  
  wire [31:0]output_w;
  wire done;
  
  message_schedule ms(clk,msg_input,output_w,rst,done);
  
  compression_function cf(clk,rst,output_w,msg_output,done);

endmodule
