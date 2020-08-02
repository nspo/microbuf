#include "boost/asio.hpp"
#include "../../output/A.h" // include the automatically generated header file

/**
 * Before compiling, make sure that boost-system is installed!
 * On Ubuntu/Debian:
 * $ sudo apt install libboost-system-dev
 *
 * When compiling, include microbuf cpp directory and
 * link against boost-system and pthread (only needed for network functions), e.g.:
 * $ g++ udp_sender.cpp -lboost_system -pthread -I ../../cpp -o udp_sender
 */

int main(int argc, char **argv)
{
    using namespace boost::asio;
    io_service io_service;
    ip::udp::socket socket(io_service);
    ip::udp::endpoint remote_endpoint;

    socket.open(ip::udp::v4());

    // set target IP and port for UDP packet
    remote_endpoint = ip::udp::endpoint(ip::address::from_string("127.0.0.1"), 25000);

    // START of microbuf-specific code
    A_struct_t a_obj;
    // fill in data of A msg
    a_obj.foo = 123;
    a_obj.bar = 22123;
    a_obj.my_vector[0] = 10;
    a_obj.my_vector[1] = 20;
    a_obj.my_vector[2] = 30;

    const auto bytes = a_obj.as_bytes(); // convert to bytes
    // END of microbuf-specific code

    boost::system::error_code err;
    socket.send_to(buffer(bytes, bytes.size()), remote_endpoint, 0, err);

    socket.close();
}
