#ifndef MICROBUF_MICROBUF_H
#define MICROBUF_MICROBUF_H

#include <stdint.h>
#include <limits.h>

namespace microbuf {

    template <class T, size_t N>
    struct array {
        // Statically-sized array - basically like std::array, but does not need the STL
        // Compare https://arduino.stackexchange.com/a/69178

        T data[N];

        static size_t size() { return N; }
        using type = T;

        T &operator[](size_t index) { return data[index]; }
        const T &operator[](size_t index) const { return data[index]; }

        T *begin() { return &data[0]; }
        const T *begin() const { return &data[0]; }
        T *end() { return &data[N]; }
        const T *end() const { return &data[N]; }

        bool operator==(const array<T, N> &rhs) const {
            if (this == &rhs) {
                return true;
            }
            for (size_t i = 0; i < N; ++i) {
                if ((*this)[i] != rhs[i]) {
                    return false;
                }
            }
            return true;
        }
        bool operator!=(const array<T, N> &rhs) const {
            return !(*this == rhs);
        }
    };

    // insert all bytes in source into dest at index - checked at compile time
    template<size_t index, size_t dest_length, size_t source_length>
    void insert_bytes(array<uint8_t,dest_length>& dest, const array<uint8_t,source_length>& source)
    {
        static_assert(index+source_length <= dest_length, "Destination too small");
        memcpy(dest.begin()+index, source.begin(), source_length);
    }

    namespace internal {
        inline bool is_big_endian() {
            const uint16_t num = 1;
            if (*(uint8_t *) &num == 1) { // could be rewritten with bytes_union
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
            array<uint8_t,sizeof(T)> bytes;
        };

        // Swap the input's byte order if the system is Little Endian
        template<size_t N>
        inline array<uint8_t, N> swap_if_little_endian(const array<uint8_t,N>& bytes)
        {
            if(is_big_endian()) {
                return bytes;
            } else {
                // Little Endian - reverse order of bytes
                array<uint8_t, N> reversed_bytes {};
                for(size_t i=0; i<bytes.size(); ++i) {
                    reversed_bytes[bytes.size()-1-i] = bytes[i];
                }
                return reversed_bytes;
            }
        }

        // Convert POD type to Big Endian representation with msgpack prefix
        template<typename T>
        inline array<uint8_t, sizeof(T)+1> to_msgpack_data(const T& data, const uint8_t prefix) {
            array<uint8_t, sizeof(T)+1> bytes {}; // storage for prefix+data
            bytes[0] = prefix;

            bytes_union<T> val_union {};
            val_union.val = data;
            insert_bytes<1>(bytes, swap_if_little_endian(val_union.bytes)); // insert Big Endian bytes after prefix
            return bytes;
        }

        // Get an array of the data in bytes from index_start to index_end (NOT including it)
        template<size_t index_start, size_t index_end, size_t N>
        inline array<uint8_t,index_end-index_start> get_partial_array(const array<uint8_t,N>& bytes) {
            static_assert(index_end > index_start, "index_end must be after index_start");
            array<uint8_t,index_end-index_start> result {};
            for(size_t i=0; i<index_end-index_start; ++i) {
                result[i] = bytes[index_start+i];
            }
            return result;
        }

        // Convert serialized msgpack data to POD type
        // Start reading at index in bytes
        template<size_t index,typename T,size_t N>
        inline bool from_msgpack_data(const array<uint8_t,N>& bytes, const uint8_t prefix, T& result) {
            static_assert(index+1+sizeof(T) <= N, "bytes is too small to contain a prefix and T at index");

            if(bytes[index] != prefix) {
                return false;
            }

            bytes_union<T> val_union {};
            val_union.bytes = swap_if_little_endian(get_partial_array<index+1,index+1+sizeof(T)>(bytes));
            result = val_union.val;

            return true;
        }


        inline uint16_t crc16_aug_ccitt(const uint8_t *ptr, uint32_t count) {
            // Return CRC16/AUG-CCITT (https://reveng.sourceforge.io/crc-catalogue/16.htm)
            // width=16 poly=0x1021 init=0x1d0f refin=false refout=false xorout=0x0000
            // check=0xe5cc residue=0x0000 name="CRC-16/SPI-FUJITSU"
            //
            // Compare http://srecord.sourceforge.net/crc16-ccitt.html and
            // https://hg.ulukai.org/ecm/crc16-t/file/f5262db9f5e0/test2.c#l40
            uint16_t crc;
            uint8_t i, v, ch;
            crc = 0xFFFF;
            while (count) {
                ch = *ptr++;
                i = 8;
                v = 0x80;
                do {
                    if (crc & 0x8000)
                        crc = (crc << 1) ^ 0x1021;
                    else
                        crc = (crc << 1);
                    if (ch & v)
                        crc ^= 1;
                    v >>= 1;
                } while (--i);
                --count;
            }
            for (i = 0; i < 16; ++i) {
                if (crc & 0x8000)
                    crc = (crc << 1) ^ 0x1021;
                else
                    crc = (crc << 1);
            }
            return (crc);
        }

    // Generate multiple plain microbuf fields after each other from an array
    // MultipleGeneratorClass is needed b/c function templates cannot be partially specialized but classes can
    template<size_t num_elements, size_t each_length, typename T>
    struct MultipleGeneratorClass {
        static array<uint8_t,num_elements*each_length> gen_multiple(const T* source_array, array<uint8_t,each_length> (*gen_element)(T))
        {
            array<uint8_t,num_elements*each_length> bytes;
            // fill current element and then recurse
            insert_bytes<0>(bytes, gen_element(source_array[0]));
            insert_bytes<0+each_length>(bytes, MultipleGeneratorClass<num_elements-1,each_length,T>::gen_multiple(source_array+1, gen_element));
            return bytes;
        }
    };

    // Partial specialization to stop the recursion
    template<size_t each_length, typename T>
    struct MultipleGeneratorClass<1,each_length,T> {
        static array<uint8_t,each_length> gen_multiple(const T* source_array, array<uint8_t,each_length> (*gen_element)(T))
        {
            array<uint8_t,each_length> bytes;
            // fill in only 1 element and return
            insert_bytes<0>(bytes, gen_element(source_array[0]));
            return bytes;
        }
    };

    } // namespace microbuf::internal

    // Generate multiple plain microbuf fields after each other from an array
    // This will not put a msgpack array around the result
    // Note that it is NOT automatically checked at compile-time that source_array actually has num_elements!
    template<size_t num_elements, size_t each_length, typename T>
    inline array<uint8_t,num_elements*each_length> gen_multiple(const T* source_array, array<uint8_t,each_length> (*gen_element)(T)) {
        return internal::MultipleGeneratorClass<num_elements,each_length,T>::gen_multiple(source_array, gen_element);
    }

    // Generate fixarray
    // 0 < length <= 15
    inline array<uint8_t, 1> gen_fixarray(const uint8_t length) {
        return { static_cast<uint8_t>(length | 0x90U) };
    }

    // Generate array16
    // 0 < length <= 65535
    inline array<uint8_t, 3> gen_array16(const uint16_t length) {
        using namespace internal;
        return to_msgpack_data(length, 0xdc);
    }

    // Generate array32
    // 0 < length <= 4294967295
    inline array<uint8_t, 5> gen_array32(const uint32_t length) {
        using namespace internal;
        return to_msgpack_data(length, 0xdd);
    }

    // TODO: __SIZEOF_{FLOAT,DOUBLE}__ is non-standard
    #if !defined __SIZEOF_FLOAT__ || __SIZEOF_FLOAT__ == 4
    inline array<uint8_t,5> gen_float32(const float f) {
        using namespace internal;
        static_assert(sizeof(float) * CHAR_BIT == 32, "System must have 32-bit floats");
        return to_msgpack_data(f, 0xca);
    }
    #endif

    // E.g. some Arduino platforms have 4-byte doubles
    #if !defined __SIZEOF_DOUBLE__ || __SIZEOF_DOUBLE__ == 8
    inline array<uint8_t, 9>  gen_float64(const double d) {
        using namespace internal;
        static_assert(sizeof(double) * CHAR_BIT == 64, "System must have 64-bit doubles");
        return to_msgpack_data(d, 0xcb);
    }
    #endif

    inline array<uint8_t, 2> gen_uint8(const uint8_t val) {
        return {0xcc, val};
    }

    inline array<uint8_t, 3>  gen_uint16(const uint16_t val) {
        using namespace internal;
        return to_msgpack_data(val, 0xcd);
    }

    inline array<uint8_t, 5> gen_uint32(const uint32_t val) {
        using namespace internal;
        return to_msgpack_data(val, 0xce);
    }

    inline array<uint8_t, 9>  gen_uint64(const uint64_t val) {
        using namespace internal;
        return to_msgpack_data(val, 0xcf);
    }

    inline array<uint8_t, 1> gen_bool(const bool val) {
        if(val) {
            return { 0xc3 };
        } else {
            return { 0xc2 };
        }
    }

    // Add CRC16 checksum to the end of bytes
    // Will ignore the last three bytes for CRC calculation and put the uint16 CRC there
    template<size_t N>
    inline void append_crc(array<uint8_t,N>& bytes) {
        using namespace internal;

        const uint16_t crc = internal::crc16_aug_ccitt(&bytes[0], N-3);
        insert_bytes<N-3>(bytes, gen_uint16(crc));
    }

    // Check if bytes1 at index1 contains bytes2
    template<size_t index1, size_t N1, size_t N2>
    inline bool bytes_equal(const array<uint8_t,N1>& bytes1, const array<uint8_t,N2> bytes2) {
        static_assert(index1+N2 <= N1, "bytes2 does not fit into bytes1 when starting at index1");
        for(size_t i=0; i<bytes2.size(); ++i) {
            if(bytes1[index1+i] != bytes2[i]) {
                return false;
            }
        }
        return true;
    }

    template<size_t index,size_t N>
    inline bool check_fixarray(const array<uint8_t,N>& bytes, const uint8_t length) {
        return bytes_equal<index>(bytes, gen_fixarray(length));
    }

    template<size_t index,size_t N>
    inline bool check_array16(const array<uint8_t,N>& bytes, const uint16_t length) {
        return bytes_equal<index>(bytes, gen_array16(length));
    }

    template<size_t index,size_t N>
    inline bool check_array32(const array<uint8_t,N>& bytes, const uint32_t length) {
        return bytes_equal<index>(bytes, gen_array32(length));
    }

    template<size_t index,size_t N>
    inline bool parse_uint8(const array<uint8_t,N>& bytes, uint8_t& result) {
        using namespace internal;
        return from_msgpack_data<index>(bytes, 0xcc, result);
    }

    template<size_t index,size_t N>
    inline bool parse_uint16(const array<uint8_t,N>& bytes, uint16_t& result) {
        using namespace internal;
        return from_msgpack_data<index>(bytes, 0xcd, result);
    }

    template<size_t index,size_t N>
    inline bool parse_uint32(const array<uint8_t,N>& bytes, uint32_t& result) {
        using namespace internal;
        return from_msgpack_data<index>(bytes, 0xce, result);
    }

    template<size_t index,size_t N>
    inline bool parse_uint64(const array<uint8_t,N>& bytes, uint64_t& result) {
        using namespace internal;
        return from_msgpack_data<index>(bytes, 0xcf, result);
    }
}

#endif //MICROBUF_MICROBUF_H
