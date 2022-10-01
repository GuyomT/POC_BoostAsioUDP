#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <mutex>
#include <queue>
#include <list>

using boost::asio::ip::udp;

// Simple mutex-guarded queue
template <typename _T>
class LockedQueue {
   private:
    std::mutex mutex;

   public:
    std::queue<_T> queue; //todo: make private
    void push(_T value) {
        std::unique_lock<std::mutex> lock(mutex);
        queue.push(value);
    };

    // Get top message in the queue
    // Note: not exception-safe (will lose the message)
    _T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        _T value;
        std::swap(value, queue.front());
        queue.pop();
        return value;
    };

    bool empty() {
        std::unique_lock<std::mutex> lock(mutex);
        return queue.empty();
    }
};

class UdpServer {
   public:
    UdpServer(boost::asio::io_context &io_context)
        : _socket(io_context, udp::endpoint(udp::v4(), 8000)) {
        start_receive();
    }

   private:
    void start_receive() {
        std::cout << "Start receiving\n";
        _socket.async_receive_from(
            boost::asio::buffer(_recv_buffer), _remote_endpoint,
            boost::bind(&UdpServer::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code &error,
                        std::size_t /*bytes_transferred*/) {
        if (!error) {
            boost::shared_ptr<std::string> message(
                new std::string("Hello from server"));
            _socket.async_send_to(boost::asio::buffer(*message), _remote_endpoint,
                                  boost::bind(&UdpServer::handle_send, this, message,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));

            start_receive();
            std::cout << "Received: " << _recv_buffer.data() << std::endl;
            _queue_recv.push(_recv_buffer.data());
            std::cout << "Queue size: " << _queue_recv.queue.size() << std::endl;

        }
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code & /*error*/,
                     std::size_t /*bytes_transferred*/) {
    }

    udp::socket _socket;
    udp::endpoint _remote_endpoint;
    boost::array<char, 4> _recv_buffer;
    LockedQueue<std::string> _queue_recv;
};

int main() {
    try {
        boost::asio::io_context io_context;
        UdpServer server(io_context);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}