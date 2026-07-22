  localparam logic [255:0] SECP256K1_P =
    256'hfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f;
  localparam logic [255:0] SECP256K1_N =
    256'hfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141;
  localparam logic [255:0] SECP256K1_N_HALF =
    256'h7fffffffffffffffffffffffffffffff5d576e7357a4501ddfe92f46681b20a0;
  localparam logic [255:0] SECP256K1_GX =
    256'h79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798;
  localparam logic [255:0] SECP256K1_GY =
    256'h483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8;

  function automatic logic [255:0] mod_add256(
    input logic [255:0] a,
    input logic [255:0] b,
    input logic [255:0] modulus
  );
    logic [256:0] sum;
    begin
      sum = {1'b0, a} + {1'b0, b};
      if (sum >= {1'b0, modulus})
        sum = sum - {1'b0, modulus};
      mod_add256 = sum[255:0];
    end
  endfunction

  function automatic logic [255:0] mod_sub256(
    input logic [255:0] a,
    input logic [255:0] b,
    input logic [255:0] modulus
  );
    begin
      if (a >= b)
        mod_sub256 = a - b;
      else
        mod_sub256 = modulus - (b - a);
    end
  endfunction

  function automatic logic [255:0] mod_double256(
    input logic [255:0] a,
    input logic [255:0] modulus
  );
    mod_double256 = mod_add256(a, a, modulus);
  endfunction

  function automatic logic [255:0] mod_triple256(
    input logic [255:0] a,
    input logic [255:0] modulus
  );
    mod_triple256 = mod_add256(mod_add256(a, a, modulus), a, modulus);
  endfunction

  function automatic logic [5:0] uint256_min_bytes(input logic [255:0] number);
    integer common_index;
    logic common_found;
    begin
      uint256_min_bytes = 0;
      common_found = 1'b0;
      for (common_index = 31; common_index >= 0; common_index = common_index - 1) begin
        if (!common_found && number[common_index*8 +: 8] != 8'h00) begin
          uint256_min_bytes = common_index + 1;
          common_found = 1'b1;
        end
      end
    end
  endfunction

  function automatic logic [2:0] length_of_length(input logic [12:0] length);
    begin
      if (length <= 13'h00ff) length_of_length = 1;
      else length_of_length = 2;
    end
  endfunction
