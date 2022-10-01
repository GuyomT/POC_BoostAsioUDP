#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <time.h>

using boost::asio::ip::udp;

int main(int argc, char* argv[]) {
    srand(time(NULL));
    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;

        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), argv[1], "8000");
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        udp::socket socket(io_service);
        socket.open(udp::v4());

        boost::array<char, 4> send_buf = {0};
        send_buf[0] = 64 & 0xff;
        send_buf[1] = rand() % 256 & 0xff;
        send_buf[2] = rand() % 256 & 0xff;
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
        std::cout << "Sended\n";

        boost::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        std::cout << "Len:\n";
        size_t len = socket.receive_from(
            boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout << len << std::endl;
        std::cout << "Received:\n";
        std::cout.write(recv_buf.data(), len);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}