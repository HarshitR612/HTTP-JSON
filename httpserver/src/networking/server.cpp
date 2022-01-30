#include <server.hpp>
#include <string>

using namespace std;
using namespace httputils;

namespace http {

    https_server::https_server(): shutdown_(false) {
        io_service_.reset(new boost::asio::io_service());
    }

    void https_server::run(ipaddr addr) {
        thread_.reset(new thread(
            [this, addr]() {
                start_server(addr);
            }
        ));
    }

    void https_server::stop() {
        shutdown_.store(true);
        thread_->join();
    }

    void https_server::start_server(ipaddr addr) {
        //tcp_connection conn(io_service_, addr);
        // while(is_server_running()) {
        //     conn.start();
        // }
    }

    bool https_server::is_server_running() {
        return !shutdown_.load();
    }

}