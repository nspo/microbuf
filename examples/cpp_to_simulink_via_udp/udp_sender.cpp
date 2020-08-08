// for sending UDP packets
#include "boost/asio.hpp"

// include the automatically generated header file - microbuf must have been called before compiling!
#include "../../output/SensorData.h"

/**
 * Before compiling, make sure that boost-system (for easier UDP transmissions) is installed!
 * On Ubuntu/Debian:
 * $ sudo apt install libboost-system-dev
 *
 * When compiling, include microbuf cpp directory and
 * link against boost-system and pthread (only needed for network functions), e.g.:
 * $ g++ udp_sender.cpp -lboost_system -pthread -I ../../cpp -o udp_sender
 * or use the Makefile:
 * $ make
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
    SensorData_struct_t sensor_data;
    // fill example data into SensorData msg
    for(size_t i=0; i<10; ++i)
    {
        sensor_data.distance[i] = 2.5*static_cast<float>(i);
        sensor_data.angle[i] = 4.2*static_cast<float>(i);
    }
    sensor_data.robot_id = 42U;

    const auto bytes = sensor_data.as_bytes(); // convert to bytes
    // END of microbuf-specific code

    boost::system::error_code err;
    socket.send_to(buffer(bytes, bytes.size()), remote_endpoint, 0, err);

    socket.close();
}
