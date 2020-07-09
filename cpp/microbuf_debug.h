//
// Created by nicolai on 09.07.20.
//

#ifndef MICROBUF_MICROBUF_DEBUG_H
#define MICROBUF_MICROBUF_DEBUG_H

#include "microbuf.h"
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ios>

namespace microbuf::debug {

void print_debug(const std::vector<uint8_t>& bytes, const std::string& separator = " ") {
    std::cout << "DEC: ";
    for (const auto& byte : bytes) {
        int byte_int = static_cast<int>(byte);
        std::cout << static_cast<int>(byte) << separator;
        if(byte_int < 10) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    std::cout << "HEX: ";
    std::ios_base::fmtflags fmtflags( std::cout.flags() );
    for (const auto& byte : bytes) {
        int byte_int = static_cast<int>(byte);
        std::cout << std::hex << std::setfill('0') << std::setw(2) << byte_int << separator;
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

}


#endif //MICROBUF_MICROBUF_DEBUG_H
