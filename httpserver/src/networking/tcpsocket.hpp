#pragma once

#include <atomic>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <utils.hpp>

namespace http {

    class tcp_connection {
        public:
            tcp_connection(const std::shared_ptr<boost::asio::io_service>& service, httputils::ipaddr addr);
            void connect();
            void close();
        private:
            void accept_connection();
            void initialize_acceptor_ipv4();
            void initialize_acceptor_ipv6();
            bool is_server_running();
            bool is_success(const boost::system::error_code& ec);
        private:
            httputils::ipaddr addr_;
            std::shared_ptr<boost::asio::io_service> io_service_;
            std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
            std::atomic<bool> shutdown_;
            enum { ERR_CODE_NO_ERR };
    };

}