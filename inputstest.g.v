//This module is derived from the booth.g.v module,
//created to test if the parser accepts an unspecified
//number if inputs, which it does,
//MODULE3X1 has 3 inputs and MODULE6X1 has 6 inputs
//This was used for testing in addition to the files already provided to us on BB
module testinput(mr, md, x, z);
  wire _0_;
  wire _1_;
  input [3:0] md;
  input [3:0] mr;
  output x;
  output z;
  INVX1 _2_ (
    .A(md[1]),
    .Y(_0_)
  );
  XOR2X1 _9_ (
    .A(_0_),
    .B(md[3]),
    .Y(_1_)
  );
  MODULE3X1 test (
    .A(md[2]),
    .B(_1_),
    .C(mr[0]),
    .Y(x)
  );

  MODULE6X1 test2 (
    .A(md[2]),
    .B(md[3]),
    .C(mr[0]),
    .D(mr[1]),
    .E(mr[2]),
    .F(mr[3]),
    .Y(z)
  );

endmodule
