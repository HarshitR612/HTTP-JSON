#include <boost/asio.hpp>
#include <memory>
#include <tcpsocket.hpp>
#include <thread>

#include <exceptions.hpp>
#include <httpconnhandler.hpp>
#include <utils.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace httputils;
using namespace utils;
using namespace std;

namespace http {

    tcp_connection::tcp_connection(const std::shared_ptr<io_service>& ioservice,
                                    httputils::ipaddr addr): addr_(addr), io_service_(ioservice), shutdown_(false)
    {
        switch(addr_.domain) {
            case IPV4:
                initialize_acceptor_ipv4();
            case IPV6:
                initialize_acceptor_ipv6();
                break;
            default:
                throw unsupported_domain_exception("only ipv4 and ipv6 domains are supported");
        }
    }

    void tcp_connection::connect() {
        if(is_not_null(acceptor_)) {
            acceptor_->listen();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            accept_connection();
        }
    }

    void tcp_connection::close() {
        shutdown_.store(true);
    }

    void tcp_connection::accept_connection() {
        while(is_server_running() && is_not_null(io_service_)) {
            shared_ptr<tcp::socket> socket(new tcp::socket(*io_service_));
            acceptor_->async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
                if(is_success(ec)) {
                    (new http_connection_handler(socket))->handle_http_request();
                }
                else {
                    throw socket_exception(ec.message());
                }
            });
        }
    }

    void tcp_connection::initialize_acceptor_ipv4() {
        if(is_not_null(io_service_)) {
            acceptor_.reset(new tcp::acceptor(*io_service_, tcp::endpoint(address_v4::any(), addr_.port)));
        }
    }

    void tcp_connection::initialize_acceptor_ipv6() {
        if(is_not_null(io_service_)) {
            acceptor_.reset(new tcp::acceptor(*io_service_, tcp::endpoint(address_v6::any(), addr_.port)));
        }
    }

    bool tcp_connection::is_server_running() {
        return !shutdown_.load();
    }

    bool tcp_connection::is_success(const boost::system::error_code& ec) {
        return ec.value() == ERR_CODE_NO_ERR;
    }

}