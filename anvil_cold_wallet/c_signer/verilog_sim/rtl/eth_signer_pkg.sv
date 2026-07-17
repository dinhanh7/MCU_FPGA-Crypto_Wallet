package eth_signer_pkg;

  typedef logic [255:0] u256_t;
  typedef logic [511:0] u512_t;
  typedef logic [63:0]  u64_t;
  typedef logic [31:0]  u32_t;
  typedef byte unsigned byte_queue_t[$];

  localparam u256_t SECP_P = 256'hfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f;
  localparam u256_t SECP_N = 256'hfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141;
  localparam u256_t SECP_GX = 256'h79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798;
  localparam u256_t SECP_GY = 256'h483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8;
  localparam u256_t SECP_N_HALF = 256'h7fffffffffffffffffffffffffffffff5d576e7357a4501ddfe92f46681b20a0;

  typedef struct packed {
    logic infinity;
    u256_t x;
    u256_t y;
    u256_t z;
  } ec_point_t;

  function automatic int hex_nibble(input byte c);
    if (c >= "0" && c <= "9") return c - "0";
    if (c >= "a" && c <= "f") return c - "a" + 10;
    if (c >= "A" && c <= "F") return c - "A" + 10;
    return -1;
  endfunction

  task automatic parse_hex(input string text, output byte_queue_t bytes);
    int offset;
    int high;
    int low;
    bytes = {};
    offset = 0;
    if (text.len() >= 2 && text[0] == "0" && (text[1] == "x" || text[1] == "X")) offset = 2;
    if (((text.len() - offset) & 1) != 0) $fatal(1, "odd-length hexadecimal input");
    for (int i = offset; i < text.len(); i += 2) begin
      high = hex_nibble(text[i]);
      low = hex_nibble(text[i + 1]);
      if (high < 0 || low < 0) $fatal(1, "invalid hexadecimal input");
      bytes.push_back(byte'((high << 4) | low));
    end
  endtask

  task automatic parse_hex_u256(input string text, output u256_t value);
    byte_queue_t bytes;
    value = '0;
    parse_hex(text, bytes);
    if (bytes.size() != 32) $fatal(1, "private key must contain exactly 32 bytes");
    for (int i = 0; i < 32; i++) value[255 - i * 8 -: 8] = bytes[i];
  endtask

  task automatic parse_decimal_u256(input string text, output u256_t value);
    logic [259:0] extended;
    int digit;
    if (text.len() == 0) $fatal(1, "empty decimal integer");
    value = '0;
    for (int i = 0; i < text.len(); i++) begin
      if (text[i] < "0" || text[i] > "9") $fatal(1, "invalid decimal integer");
      digit = text[i] - "0";
      extended = {4'b0, value} * 10 + digit;
      if (extended[259:256] != 0) $fatal(1, "decimal integer exceeds uint256");
      value = extended[255:0];
    end
  endtask

  task automatic u256_to_bytes(input u256_t value, output byte_queue_t bytes);
    bytes = {};
    for (int i = 0; i < 32; i++) bytes.push_back(value[255 - i * 8 -: 8]);
  endtask

  task automatic u256_to_minimal_bytes(input u256_t value, output byte_queue_t bytes);
    bit found;
    byte b;
    bytes = {};
    found = 0;
    for (int i = 0; i < 32; i++) begin
      b = value[255 - i * 8 -: 8];
      if (b != 0 || found) begin
        bytes.push_back(b);
        found = 1;
      end
    end
  endtask

  task automatic bytes_to_hex(input byte_queue_t bytes, output string result);
    result = "0x";
    for (int i = 0; i < bytes.size(); i++) result = {result, $sformatf("%02x", bytes[i])};
  endtask

  function automatic string u256_hex(input u256_t value);
    return $sformatf("0x%064x", value);
  endfunction

  task automatic append_queue(inout byte_queue_t destination, input byte_queue_t source);
    for (int i = 0; i < source.size(); i++) destination.push_back(source[i]);
  endtask

  task automatic rlp_append_length(
    inout byte_queue_t output_bytes,
    input int length,
    input byte short_base,
    input byte long_base
  );
    byte_queue_t encoded;
    int remaining;
    if (length <= 55) begin
      output_bytes.push_back(short_base + length);
    end else begin
      encoded = {};
      remaining = length;
      while (remaining > 0) begin
        encoded.push_front(remaining[7:0]);
        remaining = remaining >> 8;
      end
      output_bytes.push_back(long_base + encoded.size());
      append_queue(output_bytes, encoded);
    end
  endtask

  task automatic rlp_append_bytes(inout byte_queue_t output_bytes, input byte_queue_t value);
    if (value.size() == 1 && value[0] < 8'h80) begin
      output_bytes.push_back(value[0]);
    end else begin
      rlp_append_length(output_bytes, value.size(), 8'h80, 8'hb7);
      append_queue(output_bytes, value);
    end
  endtask

  task automatic rlp_append_u256(inout byte_queue_t output_bytes, input u256_t value);
    byte_queue_t minimal;
    u256_to_minimal_bytes(value, minimal);
    rlp_append_bytes(output_bytes, minimal);
  endtask

  task automatic rlp_wrap_list(input byte_queue_t payload, output byte_queue_t encoded);
    encoded = {};
    rlp_append_length(encoded, payload.size(), 8'hc0, 8'hf7);
    append_queue(encoded, payload);
  endtask

  function automatic u64_t rol64(input u64_t value, input int amount);
    if (amount == 0) return value;
    return (value << amount) | (value >> (64 - amount));
  endfunction

  task automatic keccak_f1600(inout logic [1599:0] state_vector);
    u64_t state[0:24];
    u64_t round_constants[0:23];
    int rotations[0:23];
    int pi_lanes[0:23];
    u64_t column[0:4];
    u64_t row[0:4];
    u64_t theta;
    u64_t current;
    u64_t next_lane;
    int lane;
    for (int lane_index = 0; lane_index < 25; lane_index++)
      state[lane_index] = state_vector[lane_index * 64 +: 64];
    round_constants[0]=64'h0000000000000001; round_constants[1]=64'h0000000000008082;
    round_constants[2]=64'h800000000000808a; round_constants[3]=64'h8000000080008000;
    round_constants[4]=64'h000000000000808b; round_constants[5]=64'h0000000080000001;
    round_constants[6]=64'h8000000080008081; round_constants[7]=64'h8000000000008009;
    round_constants[8]=64'h000000000000008a; round_constants[9]=64'h0000000000000088;
    round_constants[10]=64'h0000000080008009; round_constants[11]=64'h000000008000000a;
    round_constants[12]=64'h000000008000808b; round_constants[13]=64'h800000000000008b;
    round_constants[14]=64'h8000000000008089; round_constants[15]=64'h8000000000008003;
    round_constants[16]=64'h8000000000008002; round_constants[17]=64'h8000000000000080;
    round_constants[18]=64'h000000000000800a; round_constants[19]=64'h800000008000000a;
    round_constants[20]=64'h8000000080008081; round_constants[21]=64'h8000000000008080;
    round_constants[22]=64'h0000000080000001; round_constants[23]=64'h8000000080008008;
    rotations[0]=1; rotations[1]=3; rotations[2]=6; rotations[3]=10; rotations[4]=15; rotations[5]=21;
    rotations[6]=28; rotations[7]=36; rotations[8]=45; rotations[9]=55; rotations[10]=2; rotations[11]=14;
    rotations[12]=27; rotations[13]=41; rotations[14]=56; rotations[15]=8; rotations[16]=25; rotations[17]=43;
    rotations[18]=62; rotations[19]=18; rotations[20]=39; rotations[21]=61; rotations[22]=20; rotations[23]=44;
    pi_lanes[0]=10; pi_lanes[1]=7; pi_lanes[2]=11; pi_lanes[3]=17; pi_lanes[4]=18; pi_lanes[5]=3;
    pi_lanes[6]=5; pi_lanes[7]=16; pi_lanes[8]=8; pi_lanes[9]=21; pi_lanes[10]=24; pi_lanes[11]=4;
    pi_lanes[12]=15; pi_lanes[13]=23; pi_lanes[14]=19; pi_lanes[15]=13; pi_lanes[16]=12; pi_lanes[17]=2;
    pi_lanes[18]=20; pi_lanes[19]=14; pi_lanes[20]=22; pi_lanes[21]=9; pi_lanes[22]=6; pi_lanes[23]=1;
    for (int round = 0; round < 24; round++) begin
      for (int x = 0; x < 5; x++)
        column[x] = state[x] ^ state[x + 5] ^ state[x + 10] ^ state[x + 15] ^ state[x + 20];
      for (int x = 0; x < 5; x++) begin
        theta = column[(x + 4) % 5] ^ rol64(column[(x + 1) % 5], 1);
        for (int y = 0; y < 25; y += 5) state[y + x] = state[y + x] ^ theta;
      end
      current = state[1];
      for (int i = 0; i < 24; i++) begin
        lane = pi_lanes[i];
        next_lane = state[lane];
        state[lane] = rol64(current, rotations[i]);
        current = next_lane;
      end
      for (int y = 0; y < 25; y += 5) begin
        for (int x = 0; x < 5; x++) row[x] = state[y + x];
        for (int x = 0; x < 5; x++)
          state[y + x] = row[x] ^ ((~row[(x + 1) % 5]) & row[(x + 2) % 5]);
      end
      state[0] = state[0] ^ round_constants[round];
    end
    for (int lane_index = 0; lane_index < 25; lane_index++)
      state_vector[lane_index * 64 +: 64] = state[lane_index];
  endtask

  task automatic keccak256(input byte_queue_t message, output u256_t digest);
    logic [1599:0] state_vector;
    byte unsigned final_block[0:135];
    u64_t lane;
    int offset;
    int remaining;
    state_vector = '0;
    offset = 0;
    while (message.size() - offset >= 136) begin
      for (int i = 0; i < 17; i++) begin
        lane = 0;
        for (int j = 0; j < 8; j++) lane = lane | (u64_t'(message[offset + i * 8 + j]) << (8 * j));
        state_vector[i * 64 +: 64] = state_vector[i * 64 +: 64] ^ lane;
      end
      keccak_f1600(state_vector);
      offset += 136;
    end
    for (int i = 0; i < 136; i++) final_block[i] = 0;
    remaining = message.size() - offset;
    for (int i = 0; i < remaining; i++) final_block[i] = message[offset + i];
    final_block[remaining] = final_block[remaining] ^ 8'h01;
    final_block[135] = final_block[135] ^ 8'h80;
    for (int i = 0; i < 17; i++) begin
      lane = 0;
      for (int j = 0; j < 8; j++) lane = lane | (u64_t'(final_block[i * 8 + j]) << (8 * j));
      state_vector[i * 64 +: 64] = state_vector[i * 64 +: 64] ^ lane;
    end
    keccak_f1600(state_vector);
    digest = '0;
    for (int i = 0; i < 32; i++)
      digest[255 - i * 8 -: 8] = state_vector[(i / 8) * 64 + (i % 8) * 8 +: 8];
  endtask

  function automatic u32_t rotr32(input u32_t value, input int amount);
    return (value >> amount) | (value << (32 - amount));
  endfunction

  task automatic sha256(input byte_queue_t message, output u256_t digest);
    u32_t k[0:63];
    byte_queue_t padded;
    longint unsigned bit_length;
    u32_t h[0:7];
    u32_t w[0:63];
    u32_t a,b,c,d,e,f,g,hh,s0,s1,ch,maj,t1,t2;
    k[0]=32'h428a2f98; k[1]=32'h71374491; k[2]=32'hb5c0fbcf; k[3]=32'he9b5dba5;
    k[4]=32'h3956c25b; k[5]=32'h59f111f1; k[6]=32'h923f82a4; k[7]=32'hab1c5ed5;
    k[8]=32'hd807aa98; k[9]=32'h12835b01; k[10]=32'h243185be; k[11]=32'h550c7dc3;
    k[12]=32'h72be5d74; k[13]=32'h80deb1fe; k[14]=32'h9bdc06a7; k[15]=32'hc19bf174;
    k[16]=32'he49b69c1; k[17]=32'hefbe4786; k[18]=32'h0fc19dc6; k[19]=32'h240ca1cc;
    k[20]=32'h2de92c6f; k[21]=32'h4a7484aa; k[22]=32'h5cb0a9dc; k[23]=32'h76f988da;
    k[24]=32'h983e5152; k[25]=32'ha831c66d; k[26]=32'hb00327c8; k[27]=32'hbf597fc7;
    k[28]=32'hc6e00bf3; k[29]=32'hd5a79147; k[30]=32'h06ca6351; k[31]=32'h14292967;
    k[32]=32'h27b70a85; k[33]=32'h2e1b2138; k[34]=32'h4d2c6dfc; k[35]=32'h53380d13;
    k[36]=32'h650a7354; k[37]=32'h766a0abb; k[38]=32'h81c2c92e; k[39]=32'h92722c85;
    k[40]=32'ha2bfe8a1; k[41]=32'ha81a664b; k[42]=32'hc24b8b70; k[43]=32'hc76c51a3;
    k[44]=32'hd192e819; k[45]=32'hd6990624; k[46]=32'hf40e3585; k[47]=32'h106aa070;
    k[48]=32'h19a4c116; k[49]=32'h1e376c08; k[50]=32'h2748774c; k[51]=32'h34b0bcb5;
    k[52]=32'h391c0cb3; k[53]=32'h4ed8aa4a; k[54]=32'h5b9cca4f; k[55]=32'h682e6ff3;
    k[56]=32'h748f82ee; k[57]=32'h78a5636f; k[58]=32'h84c87814; k[59]=32'h8cc70208;
    k[60]=32'h90befffa; k[61]=32'ha4506ceb; k[62]=32'hbef9a3f7; k[63]=32'hc67178f2;
    padded = message;
    bit_length = message.size() * 8;
    padded.push_back(8'h80);
    while ((padded.size() % 64) != 56) padded.push_back(8'h00);
    for (int i = 7; i >= 0; i--) padded.push_back(bit_length[i * 8 +: 8]);
    h[0]=32'h6a09e667; h[1]=32'hbb67ae85; h[2]=32'h3c6ef372; h[3]=32'ha54ff53a;
    h[4]=32'h510e527f; h[5]=32'h9b05688c; h[6]=32'h1f83d9ab; h[7]=32'h5be0cd19;
    for (int block = 0; block < padded.size() / 64; block++) begin
      for (int i = 0; i < 16; i++) begin
        w[i] = {padded[block*64+i*4], padded[block*64+i*4+1], padded[block*64+i*4+2], padded[block*64+i*4+3]};
      end
      for (int i = 16; i < 64; i++) begin
        s0 = rotr32(w[i-15],7) ^ rotr32(w[i-15],18) ^ (w[i-15] >> 3);
        s1 = rotr32(w[i-2],17) ^ rotr32(w[i-2],19) ^ (w[i-2] >> 10);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
      end
      a=h[0]; b=h[1]; c=h[2]; d=h[3]; e=h[4]; f=h[5]; g=h[6]; hh=h[7];
      for (int i = 0; i < 64; i++) begin
        s1 = rotr32(e,6) ^ rotr32(e,11) ^ rotr32(e,25);
        ch = (e & f) ^ ((~e) & g);
        t1 = hh + s1 + ch + k[i] + w[i];
        s0 = rotr32(a,2) ^ rotr32(a,13) ^ rotr32(a,22);
        maj = (a & b) ^ (a & c) ^ (b & c);
        t2 = s0 + maj;
        hh=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2;
      end
      h[0]=h[0]+a; h[1]=h[1]+b; h[2]=h[2]+c; h[3]=h[3]+d;
      h[4]=h[4]+e; h[5]=h[5]+f; h[6]=h[6]+g; h[7]=h[7]+hh;
    end
    digest = {h[0],h[1],h[2],h[3],h[4],h[5],h[6],h[7]};
  endtask

  task automatic hmac_sha256(input byte_queue_t key, input byte_queue_t message, output u256_t digest);
    byte_queue_t key_block;
    byte_queue_t inner;
    byte_queue_t outer;
    u256_t key_hash;
    u256_t inner_hash;
    key_block = {};
    if (key.size() > 64) begin
      sha256(key, key_hash);
      u256_to_bytes(key_hash, key_block);
    end else begin
      key_block = key;
    end
    while (key_block.size() < 64) key_block.push_back(0);
    inner = {};
    for (int i = 0; i < 64; i++) begin
      inner.push_back(key_block[i] ^ 8'h36);
    end
    append_queue(inner, message);
    sha256(inner, inner_hash);
`ifdef ETH_SIGNER_DEBUG
    $display("HMAC key=%0d message=%0d total=%0d bytes=%02x,%02x,%02x,%02x,%02x,%02x inner=%064x",
      key.size(), message.size(), inner.size(), inner[0], inner[19], inner[20], inner[63], inner[64], inner[71], inner_hash);
`endif
    // Construct the outer block only after SHA-256 has consumed `inner`.
    // Keeping two live dynamic queues across a task call triggers an optimizer
    // aliasing bug in some Verilator releases; Questa does not require this
    // workaround, but the ordering is also a simpler statement of HMAC.
    outer = {};
    for (int i = 0; i < 64; i++) outer.push_back(key_block[i] ^ 8'h5c);
    for (int i = 0; i < 32; i++) outer.push_back(inner_hash[255 - i * 8 -: 8]);
`ifdef ETH_SIGNER_DEBUG
    $display("HMAC outer total=%0d bytes=%02x,%02x,%02x,%02x,%02x,%02x",
      outer.size(), outer[0], outer[19], outer[20], outer[63], outer[64], outer[95]);
`endif
    sha256(outer, digest);
  endtask

  task automatic rfc6979_nonce(input u256_t private_key, input u256_t message_hash, output u256_t nonce);
    byte_queue_t k_bytes;
    byte_queue_t v_bytes;
    byte_queue_t key_data;
    byte_queue_t input_data;
    u256_t k_value;
    u256_t v_value;
    u256_t reduced_message;
    reduced_message = message_hash >= SECP_N ? message_hash - SECP_N : message_hash;
    k_value = 0;
    v_value = 256'h0101010101010101010101010101010101010101010101010101010101010101;
    u256_to_bytes(private_key, key_data);
    begin
      byte_queue_t temp;
      u256_to_bytes(reduced_message, temp);
      append_queue(key_data, temp);
    end
    u256_to_bytes(k_value, k_bytes);
    u256_to_bytes(v_value, v_bytes);
    input_data = v_bytes; input_data.push_back(8'h00); append_queue(input_data, key_data);
    hmac_sha256(k_bytes, input_data, k_value);
    u256_to_bytes(k_value, k_bytes);
    hmac_sha256(k_bytes, v_bytes, v_value);
    u256_to_bytes(v_value, v_bytes);
    input_data = v_bytes; input_data.push_back(8'h01); append_queue(input_data, key_data);
    hmac_sha256(k_bytes, input_data, k_value);
    u256_to_bytes(k_value, k_bytes);
    hmac_sha256(k_bytes, v_bytes, v_value);
    u256_to_bytes(v_value, v_bytes);
    begin : find_valid_nonce
      forever begin
        hmac_sha256(k_bytes, v_bytes, v_value);
        nonce = v_value;
        if (nonce > 0 && nonce < SECP_N) disable find_valid_nonce;
        u256_to_bytes(v_value, v_bytes);
        input_data = v_bytes; input_data.push_back(8'h00);
        hmac_sha256(k_bytes, input_data, k_value);
        u256_to_bytes(k_value, k_bytes);
        hmac_sha256(k_bytes, v_bytes, v_value);
        u256_to_bytes(v_value, v_bytes);
      end
    end
  endtask

  function automatic u256_t mod_add(input u256_t a, input u256_t b, input u256_t modulus);
    logic [256:0] sum;
    sum = {1'b0,a} + {1'b0,b};
    if (sum >= {1'b0,modulus}) sum = sum - modulus;
    return sum[255:0];
  endfunction

  function automatic u256_t mod_sub(input u256_t a, input u256_t b, input u256_t modulus);
    if (a >= b) return a - b;
    return modulus - (b - a);
  endfunction

  function automatic u256_t mod_mul(input u256_t a, input u256_t b, input u256_t modulus);
    u512_t product;
    product = a * b;
    return product % modulus;
  endfunction

  function automatic u256_t mod_small(input u256_t a, input int factor, input u256_t modulus);
    u512_t product;
    product = a * factor;
    return product % modulus;
  endfunction

  function automatic u256_t mod_pow(input u256_t base, input u256_t exponent, input u256_t modulus);
    u256_t result;
    u256_t current;
    result = 1;
    current = base;
    for (int i = 0; i < 256; i++) begin
      if (exponent[i]) result = mod_mul(result, current, modulus);
      current = mod_mul(current, current, modulus);
    end
    return result;
  endfunction

  function automatic u256_t mod_inv(input u256_t value, input u256_t modulus);
    return mod_pow(value, modulus - 2, modulus);
  endfunction

  function automatic ec_point_t point_infinity();
    ec_point_t result;
    result.infinity = 1;
    result.x = 0; result.y = 1; result.z = 0;
    return result;
  endfunction

  function automatic ec_point_t point_generator();
    ec_point_t result;
    result.infinity = 0;
    result.x = SECP_GX; result.y = SECP_GY; result.z = 1;
    return result;
  endfunction

  function automatic ec_point_t point_double(input ec_point_t p);
    ec_point_t r;
    u256_t yy, yyyy, s, m;
    if (p.infinity || p.y == 0) return point_infinity();
    yy = mod_mul(p.y, p.y, SECP_P);
    yyyy = mod_mul(yy, yy, SECP_P);
    s = mod_small(mod_mul(p.x, yy, SECP_P), 4, SECP_P);
    m = mod_small(mod_mul(p.x, p.x, SECP_P), 3, SECP_P);
    r.x = mod_sub(mod_mul(m, m, SECP_P), mod_small(s, 2, SECP_P), SECP_P);
    r.y = mod_sub(mod_mul(m, mod_sub(s, r.x, SECP_P), SECP_P), mod_small(yyyy, 8, SECP_P), SECP_P);
    r.z = mod_small(mod_mul(p.y, p.z, SECP_P), 2, SECP_P);
    r.infinity = 0;
    return r;
  endfunction

  function automatic ec_point_t point_add(input ec_point_t p, input ec_point_t q);
    ec_point_t r;
    u256_t z1z1,z2z2,u1,u2,s1,s2,h,i,j,rr,v,t;
    if (p.infinity) return q;
    if (q.infinity) return p;
    z1z1 = mod_mul(p.z,p.z,SECP_P);
    z2z2 = mod_mul(q.z,q.z,SECP_P);
    u1 = mod_mul(p.x,z2z2,SECP_P);
    u2 = mod_mul(q.x,z1z1,SECP_P);
    s1 = mod_mul(p.y,mod_mul(q.z,z2z2,SECP_P),SECP_P);
    s2 = mod_mul(q.y,mod_mul(p.z,z1z1,SECP_P),SECP_P);
    if (u1 == u2) begin
      if (s1 != s2) return point_infinity();
      return point_double(p);
    end
    h = mod_sub(u2,u1,SECP_P);
    i = mod_mul(mod_small(h,2,SECP_P),mod_small(h,2,SECP_P),SECP_P);
    j = mod_mul(h,i,SECP_P);
    rr = mod_small(mod_sub(s2,s1,SECP_P),2,SECP_P);
    v = mod_mul(u1,i,SECP_P);
    r.x = mod_sub(mod_sub(mod_mul(rr,rr,SECP_P),j,SECP_P),mod_small(v,2,SECP_P),SECP_P);
    r.y = mod_sub(mod_mul(rr,mod_sub(v,r.x,SECP_P),SECP_P),mod_small(mod_mul(s1,j,SECP_P),2,SECP_P),SECP_P);
    t = mod_sub(mod_sub(mod_mul(mod_add(p.z,q.z,SECP_P),mod_add(p.z,q.z,SECP_P),SECP_P),z1z1,SECP_P),z2z2,SECP_P);
    r.z = mod_mul(t,h,SECP_P);
    r.infinity = 0;
    return r;
  endfunction

  function automatic ec_point_t point_multiply(input u256_t scalar);
    ec_point_t result;
    ec_point_t generator;
    result = point_infinity();
    generator = point_generator();
    for (int i = 255; i >= 0; i--) begin
      result = point_double(result);
      if (scalar[i]) result = point_add(result, generator);
    end
    return result;
  endfunction

  function automatic ec_point_t point_affine(input ec_point_t p);
    ec_point_t result;
    u256_t z_inv,z2,z3;
    if (p.infinity) return p;
    z_inv = mod_inv(p.z,SECP_P);
    z2 = mod_mul(z_inv,z_inv,SECP_P);
    z3 = mod_mul(z2,z_inv,SECP_P);
    result.infinity = 0;
    result.x = mod_mul(p.x,z2,SECP_P);
    result.y = mod_mul(p.y,z3,SECP_P);
    result.z = 1;
    return result;
  endfunction

  task automatic derive_address(input u256_t private_key, output logic [159:0] address);
    ec_point_t public_key;
    byte_queue_t serialized;
    u256_t hash;
    public_key = point_affine(point_multiply(private_key));
    u256_to_bytes(public_key.x, serialized);
    begin
      byte_queue_t y_bytes;
      u256_to_bytes(public_key.y, y_bytes);
      append_queue(serialized, y_bytes);
    end
    keccak256(serialized, hash);
    address = hash[159:0];
  endtask

  task automatic ecdsa_sign(
    input u256_t private_key,
    input u256_t message_hash,
    output u256_t r,
    output u256_t s,
    output int recovery_id
  );
    u256_t nonce;
    u256_t z;
    u256_t numerator;
    ec_point_t nonce_point;
    rfc6979_nonce(private_key, message_hash, nonce);
    nonce_point = point_affine(point_multiply(nonce));
    recovery_id = (nonce_point.x >= SECP_N ? 2 : 0) | nonce_point.y[0];
    r = nonce_point.x % SECP_N;
    z = message_hash >= SECP_N ? message_hash - SECP_N : message_hash;
    numerator = mod_add(z, mod_mul(r, private_key, SECP_N), SECP_N);
    s = mod_mul(mod_inv(nonce,SECP_N), numerator, SECP_N);
    if (s > SECP_N_HALF) begin
      s = SECP_N - s;
      recovery_id = recovery_id ^ 1;
    end
    if (r == 0 || s == 0) $fatal(1, "cryptographically unreachable zero ECDSA scalar");
    if (recovery_id < 0 || recovery_id > 1) $fatal(1, "unexpected Ethereum recovery id");
  endtask

  task automatic append_transaction_fields(
    inout byte_queue_t payload,
    input string chain_id_text,
    input string nonce_text,
    input string max_priority_fee_text,
    input string max_fee_text,
    input string gas_limit_text,
    input string to_text,
    input string value_text,
    input string data_text
  );
    u256_t value;
    byte_queue_t bytes;
    parse_decimal_u256(chain_id_text,value); rlp_append_u256(payload,value);
    parse_decimal_u256(nonce_text,value); rlp_append_u256(payload,value);
    parse_decimal_u256(max_priority_fee_text,value); rlp_append_u256(payload,value);
    parse_decimal_u256(max_fee_text,value); rlp_append_u256(payload,value);
    parse_decimal_u256(gas_limit_text,value); rlp_append_u256(payload,value);
    parse_hex(to_text,bytes);
    if (bytes.size() != 20) $fatal(1, "recipient must be a 20-byte Ethereum address");
    rlp_append_bytes(payload,bytes);
    parse_decimal_u256(value_text,value); rlp_append_u256(payload,value);
    parse_hex(data_text,bytes);
    if (bytes.size() > 2048) $fatal(1, "data exceeds 2048 bytes");
    rlp_append_bytes(payload,bytes);
    payload.push_back(8'hc0);
  endtask

  task automatic make_typed_transaction(input byte_queue_t payload, output byte_queue_t typed);
    byte_queue_t list;
    rlp_wrap_list(payload,list);
    typed = {};
    typed.push_back(8'h02);
    append_queue(typed,list);
  endtask

endpackage
