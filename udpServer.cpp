#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <stack>

using boost::asio::ip::udp;

class UdpServer
{
public:
    UdpServer(boost::asio::io_context &io_context)
        : _socket(io_context, udp::endpoint(udp::v4(), 8000))
    {
        start_receive();
    }

private:
    void start_receive()
    {
        std::cout << "Start receiving\n";
        _socket.async_receive_from(
            boost::asio::buffer(_recv_buffer), _remote_endpoint,
            boost::bind(&UdpServer::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code &error,
                        std::size_t /*bytes_transferred*/)
    {
        if (!error)
        {
            boost::shared_ptr<std::string> message(
                new std::string("Hello from server"));
            _socket.async_send_to(boost::asio::buffer(*message), _remote_endpoint,
                                  boost::bind(&UdpServer::handle_send, this, message,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));

            start_receive();
            _stack_recv.push(_recv_buffer.data());
            std::cout << _recv_buffer.data() << std::endl;
            std::cout << "Stack :";
            for (int i = 0; i < _stack_recv.size(); i++)
            {
                std::cout << _stack_recv.top();
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code & /*error*/,
                     std::size_t /*bytes_transferred*/)
    {
    }

    udp::socket _socket;
    udp::endpoint _remote_endpoint;
    boost::array<char, 4> _recv_buffer;
    std::stack<std::string> _stack_recv;
};

int main()
{
    try
    {
        boost::asio::io_context io_context;
        UdpServer server(io_context);
        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}