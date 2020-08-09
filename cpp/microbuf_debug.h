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

namespace microbuf {
    namespace debug{
        template<size_t N>
        void print_debug(const array<uint8_t,N>& bytes, const std::string& separator = " ") {
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

        // Print as hex with prefix and suffix, e.g.: "0x2a, 0x5e, 0x10"
        template<size_t N>
        void print_formatted_hex(const array <uint8_t, N> &bytes, const std::string &prefix = "0x",
                                 const std::string &suffix = ", ") {
            std::ios_base::fmtflags fmtflags( std::cout.flags() );
            for (const auto& byte : bytes) {
                int byte_int = static_cast<int>(byte);
                std::cout << prefix << std::hex << std::setfill('0') << std::setw(2) << byte_int << suffix;
            }
            std::cout.flags( fmtflags ); // restore formatting flags
            std::cout << std::endl;
        }
    }
}

#endif //MICROBUF_MICROBUF_DEBUG_H
