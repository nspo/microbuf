#include "microbuf.h"
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ios>

void print_debug(const std::vector<uint8_t>& bytes) {
    std::cout << "DEC: ";
    for (const auto& byte : bytes) {
        int byte_int = static_cast<int>(byte);
        std::cout << static_cast<int>(byte) << " ";
        if(byte_int < 10) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    std::cout << "HEX: ";
    std::ios_base::fmtflags fmtflags( std::cout.flags() );
    for (const auto& byte : bytes) {
        int byte_int = static_cast<int>(byte);
        std::cout << std::hex << std::setfill('0') << std::setw(2) << byte_int << " ";
        if(byte_int>99) {
            std::cout<<" ";
        }
    }
    std::cout.flags( fmtflags ); // restore formatting flags
    std::cout << std::endl;
}

void append_multiple_uint8(std::vector<uint8_t>& bytes, size_t num) {
    for(size_t i=0; i<num; ++i) {
        microbuf::append_uint8(bytes, i%255);
    }
}

int main() {
    std::cout <<"is big endian: "<< microbuf::internal::is_big_endian()<<std::endl;

    std::cout<<"array test\n";
    std::vector<uint8_t> bytes;
    microbuf::append_array(bytes, 1);
    append_multiple_uint8(bytes, 1);
    print_debug(bytes); bytes.clear();

    microbuf::append_array(bytes, 16);
    append_multiple_uint8(bytes, 16);
    print_debug(bytes); bytes.clear();

    std::cout<<"float test\n";
    microbuf::append_array(bytes, 2);
    microbuf::append_float64(bytes, 1.23);
    microbuf::append_float32(bytes, 4.56);
    print_debug(bytes); bytes.clear();

    std::cout<<"uint test\n";

    microbuf::append_array(bytes, 8);
    microbuf::append_uint8(bytes, 1);
    microbuf::append_uint8(bytes, 255);
    microbuf::append_uint16(bytes, 256);
    microbuf::append_uint16(bytes, 65535);
    microbuf::append_uint32(bytes, 65536);
    microbuf::append_uint32(bytes, 4294967295);
    microbuf::append_uint64(bytes, 4294967296);
    microbuf::append_uint64(bytes, 18446744073709551615U);
    print_debug(bytes); bytes.clear();

    std::cout<<"---\n";
//    microbuf::internal::bytes_union<uint16_t> foo {};
//    foo.val = 300;
//    print_debug(std::vector<uint8_t>(foo.bytes, foo.bytes+2));

}