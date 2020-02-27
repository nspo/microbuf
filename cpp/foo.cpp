#include "microbuf.h"
#include <iostream>

int main() {
    std::cout <<"is big endian: "<< microbuf::is_big_endian()<<std::endl;
}