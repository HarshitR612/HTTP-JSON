#pragma once

#include <atomic>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <thread>

#include <utils.hpp>

namespace http {

    class https_server {
        public:
            https_server();
            https_server(const https_server&) = delete;
            https_server& operator=(const https_server&) = delete;

            void run(httputils::ipaddr addr);
            void stop();
        private:
            void start_server(httputils::ipaddr addr);
            bool is_server_running();
        private:
            std::shared_ptr<boost::asio::io_service> io_service_;
            std::unique_ptr<std::thread> thread_;
            std::atomic<bool> shutdown_;
    };

}