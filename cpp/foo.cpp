#include "microbuf.h"
#include <cstdint>
#include <iostream>
#include <iomanip>

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
    for (const auto& byte : bytes) {
        int byte_int = static_cast<int>(byte);
        std::cout << std::hex << std::setfill('0') << std::setw(2) << byte_int << " ";
        if(byte_int>99) {
            std::cout<<" ";
        }
    }
    std::cout << std::endl;
}

int main() {
    std::cout <<"is big endian: "<< microbuf::internal::is_big_endian()<<std::endl;

    std::vector<uint8_t> bytes;
    microbuf::append_array(bytes, 2);
    microbuf::append_float64(bytes, 1.23);
    microbuf::append_float32(bytes, 1.23);
    print_debug(bytes);

    std::cout<<"---\n";
    microbuf::internal::bytes_union<uint16_t> foo;
    foo.val = 300;
    print_debug(std::vector<uint8_t>(foo.bytes, foo.bytes+2));

}