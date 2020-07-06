#ifndef MICROBUF_MICROBUF_H
#define MICROBUF_MICROBUF_H

#include <cstdint>
#include <cassert>
#include <vector>
#include <climits>

namespace microbuf {
    namespace internal {
        bool is_big_endian() {
            uint16_t num = 1;
            if (*(uint8_t *) &num == 1) {
                // Little Endian
                return false;
            } else {
                // Big Endian
                return true;
            }
        }

        template <typename T>
        union bytes_union {
            // makes data types accessible as byte array - used for Endian-correct serialization
            T val;
            uint8_t bytes[sizeof(T)];
        };

        template <typename T>
        void append_forward(std::vector<uint8_t>& bytes, const T& union_data) {
            // append data in same order as currently in memory (i.e. it's already big-endian in memory)
            for(size_t i=0; i<sizeof(T); ++i) {
                bytes.push_back(union_data.bytes[i]);
            }
        }

        template <typename T>
        void append_backward(std::vector<uint8_t>& bytes, const T& union_data) {
            // append data in reverse order (i.e. it's little-endian in memory)
            for(size_t i=0; i<sizeof(T); ++i) {
                bytes.push_back(union_data.bytes[sizeof(T)-1-i]);
            }
        }
    }

    void append_array(std::vector<uint8_t>& bytes, const size_t length) {
        // add array to end of data

        using namespace internal;

        assert(length > 0);
        // TODO: also check for max size?

        if (length <= 15) {
            // fixarray
            bytes.push_back(length | 0x90U);
        } else if (length <= 65535) {
            // array 16
            bytes.push_back(0xdc);

            bytes_union<uint16_t> length_union {};
            length_union.val = length;

            if(is_big_endian()) {
                append_forward(bytes, length_union);
            } else {
                append_backward(bytes, length_union);
            }
        } else {
            // array 32
            bytes.push_back(0xdd);

            bytes_union<uint32_t> length_union {};
            length_union.val = length;

            if(is_big_endian()) {
                append_forward(bytes, length_union);
            } else {
                append_backward(bytes, length_union);
            }
        }
    }

    void append_float32(std::vector<uint8_t>& bytes, const float f) {
        // add float32 to end of data
        using namespace internal;

        static_assert(sizeof(float) * CHAR_BIT == 32, "System must have 32-bit floats");

        bytes.push_back(0xca);

        bytes_union<float> float_union {};
        float_union.val = f;
        if(is_big_endian()) {
            append_forward(bytes, float_union);
        } else {
            append_backward(bytes, float_union);
        }
    }

    void append_float64(std::vector<uint8_t>& bytes, const double d) {
        // add float64 to end of data
        using namespace internal;

        static_assert(sizeof(double) * CHAR_BIT == 64, "System must have 64-bit doubles");

        bytes.push_back(0xcb);

        bytes_union<double> double_union {};
        double_union.val = d;
        if(is_big_endian()) {
            append_forward(bytes, double_union);
        } else {
            append_backward(bytes, double_union);
        }
    }

    void append_uint8(std::vector<uint8_t>& bytes, const uint8_t val) {
        // add uint8 to end of data
        using namespace internal;

        bytes.push_back(0xcc);
        bytes.push_back(val);
    }

    void append_uint16(std::vector<uint8_t>& bytes, const uint16_t val) {
        // add uint16 to end of data
        using namespace internal;

        bytes.push_back(0xcd);

        bytes_union<uint16_t> val_union {};
        val_union.val = val;
        if(is_big_endian()) {
            append_forward(bytes, val_union);
        } else {
            append_backward(bytes, val_union);
        }
    }

    void append_uint32(std::vector<uint8_t>& bytes, const uint32_t val) {
        // add uint32 to end of data
        using namespace internal;

        bytes.push_back(0xce);

        bytes_union<uint32_t> val_union {};
        val_union.val = val;
        if(is_big_endian()) {
            append_forward(bytes, val_union);
        } else {
            append_backward(bytes, val_union);
        }
    }

    void append_uint64(std::vector<uint8_t>& bytes, const uint64_t val) {
        // add uint64 to end of data
        using namespace internal;

        bytes.push_back(0xcf);

        bytes_union<uint64_t> val_union {};
        val_union.val = val;
        if(is_big_endian()) {
            append_forward(bytes, val_union);
        } else {
            append_backward(bytes, val_union);
        }
    }
}

#endif //MICROBUF_MICROBUF_H
