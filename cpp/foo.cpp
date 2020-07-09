#include "microbuf.h"
#include "microbuf_debug.h"
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ios>


struct A_struct_t {
    float a {1.23};
    double b {4.56};

    std::vector<uint8_t> as_bytes() const {
        std::vector<uint8_t> bytes;
        microbuf::append_array(bytes, 2);
        microbuf::append_float32(bytes, a);
        microbuf::append_float64(bytes, b);

        return bytes;
    }
};

int main() {
    using namespace microbuf::debug;
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

    std::cout<<"bool test\n";

    microbuf::append_array(bytes, 4);
    microbuf::append_bool(bytes, true);
    microbuf::append_bool(bytes, false);
    microbuf::append_bool(bytes, false);
    microbuf::append_bool(bytes, true);
    print_debug(bytes); bytes.clear();

    std::cout<<"---\n";
//    microbuf::internal::bytes_union<uint16_t> foo {};
//    foo.val = 300;
//    print_debug(std::vector<uint8_t>(foo.bytes, foo.bytes+2));

    std::cout<<"A\n";
    A_struct_t A;
    print_debug(A.as_bytes());

}