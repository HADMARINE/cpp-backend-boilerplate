#include "SocketManager.hpp"

using namespace restbed;


namespace Socket {
    map<string, shared_ptr<WebSocket>> SocketCollector::sockets = {};


    string base64_encode(const unsigned char* input, int length)
    {
        BIO* bmem, * b64;
        BUF_MEM* bptr;

        b64 = BIO_new(BIO_f_base64());
        bmem = BIO_new(BIO_s_mem());
        b64 = BIO_push(b64, bmem);
        BIO_write(b64, input, length);
        (void)BIO_flush(b64);
        BIO_get_mem_ptr(b64, &bptr);

        char* buff = (char*)malloc(bptr->length);
        memcpy(buff, bptr->data, bptr->length - 1);
        buff[bptr->length - 1] = 0;

        BIO_free_all(b64);

        return buff;
    }

    multimap<string, string> build_websocket_handshake_response_headers(const shared_ptr<const Request>& request)
    {
        auto key = request->get_header("Sec-WebSocket-Key");
        key.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

        Byte hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char*>(key.data()), key.length(), hash);

        multimap< string, string > headers;
        headers.insert(make_pair("Upgrade", "websocket"));
        headers.insert(make_pair("Connection", "Upgrade"));
        headers.insert(make_pair("Sec-WebSocket-Accept", base64_encode(hash, SHA_DIGEST_LENGTH)));

        return headers;
    }

	bool SocketCollector::Initialize(bool &result, Service &service, shared_ptr<Session> session) {

		const auto request = session->get_request();
		const auto connection_header = request->get_header("connection", String::lowercase);

		if (connection_header.find("upgrade") != string::npos) {
            if (request->get_header("upgrade", String::lowercase) == "websocket") {

                const auto headers = build_websocket_handshake_response_headers(request);

                session->upgrade(SWITCHING_PROTOCOLS, headers, [](const shared_ptr< WebSocket > socket) {
                        if (socket->is_open()) {
                            //socket->set_close_handler(close_handler);
                            //socket->set_error_handler(error_handler);
                            //socket->set_message_handler(message_handler);

                            socket->send("Welcome to Corvusoft Chat!", [](const shared_ptr< WebSocket > socket) {
                                const auto key = socket->get_key();
                                SocketCollector::sockets.insert(make_pair(key, socket));

                                fprintf(stderr, "Sent welcome message to %s.\n", key.data());
                                });
                        }
                        else
                        {
                            fprintf(stderr, "WebSocket Negotiation Failed: Client closed connection.\n");
                        }
                    });
            }
		}

		return true;
	}
}
