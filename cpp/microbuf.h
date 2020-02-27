#ifndef MICROBUF_MICROBUF_H
#define MICROBUF_MICROBUF_H

#include <cstdint>

namespace microbuf {
    bool is_big_endian() {
        uint16_t num = 1;
        if(*(uint8_t *)&num == 1)
        {
            // Little Endian
            return false;
        } else {
            // Big Endian
            return true;
        }
    }
}

#endif //MICROBUF_MICROBUF_H
