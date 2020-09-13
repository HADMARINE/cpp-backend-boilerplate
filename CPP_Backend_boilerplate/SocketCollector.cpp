#include "SocketCollector.hpp"


void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
	CLogger::Info(msg->get_payload());
}

bool SocketCollector::Initialize(bool* result) {
	server print_server;

	print_server.set_message_handler(&on_message);

	print_server.init_asio();
	print_server.listen(PORT + 1);
	print_server.start_accept();
	CLogger::Debug("SocketCollector STARTED");
	*result = true;
	print_server.run();
	return true;
}