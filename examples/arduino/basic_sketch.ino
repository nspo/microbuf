#include "microbuf.h"

void setup()
{
  Serial.begin(9600);
}

template<size_t N>
void print_arr(const microbuf::array<uint8_t,N>& arr, const char* separator=" "){
  for(const uint8_t val : arr) {
    Serial.print(val, HEX);
    Serial.print(separator);
  }
  Serial.println("");
}

void loop() {
  Serial.println("--- Examples:");
  const auto byte_arr = microbuf::gen_float32(1.23);
  print_arr(byte_arr);
  
  float result;
  microbuf::parse_float32<0>(microbuf::array<uint8_t,5>{0xca, 0x3f, 0x9d, 0x70, 0xa4}, result);
  Serial.println(result);
  
  Serial.println("--- Checks: ");
  Serial.println(result==static_cast<float>(1.23));
  
  const auto ui64_bytes = microbuf::array<uint8_t,9>{0xcf, 0x11, 0x22, 0x10, 0xf4, 0x7D, 0xe9, 0x81, 0x15}; // 1234567890123456789U
  const uint64_t ui64_val = 1234567890123456789U;
  uint64_t ui64_result_val {};
  microbuf::parse_uint64<0>(ui64_bytes, ui64_result_val);
  Serial.println(bool(ui64_result_val == ui64_val));
  
  const auto ui64_result_bytes = microbuf::gen_uint64(ui64_val);
  Serial.println(bool(ui64_result_bytes == ui64_bytes));
  
  float f32_result_arr[3] {};
  microbuf::parse_multiple<3, 0>(
            microbuf::array<uint8_t, 15>{0xca, 0x3f, 0x9d, 0x70, 0xa4, // 1.23
                                         0xca, 0x40, 0x91, 0xeb, 0x85, // 4.56
                                         0xca, 0x43, 0x00, 0x00, 0x00 // 128
            },
            f32_result_arr, microbuf::parse_float32<0>);
  Serial.println(bool(f32_result_arr[0]==static_cast<float>(1.23)
    && f32_result_arr[1]==static_cast<float>(4.56) && f32_result_arr[2]==static_cast<float>(128.0)));
}

