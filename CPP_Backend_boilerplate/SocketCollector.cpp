#include "SocketCollector.hpp"

//using namespace std;

//void client(SOCKET clientSock, SOCKADDR_IN clientAddr, vector<thread*>* clientlist)
//{
//	// 접속 정보를 콘솔에 출력한다.
//	cout << "Client connected IP address = " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;
//	// client로 메시지를 보낸다.
//	const char* message = "Welcome server!\r\n>\0";
//	send(clientSock, message, strlen(message) + 1, 0);
//	// telent은 한글자씩 데이터가 오기 때문에 글자를 모을 buffer가 필요하다.
//	vector<char> buffer;
//	// 수신 데이터
//	char x;
//	int iResult = 1;
//	while (iResult > 0)
//	{
//		iResult = recv(clientSock, &x, sizeof(char), 0);
//		
//		if (iResult == 0) {
//			Logger::Info("Socket connection closed");
//		}
//		
//		// 만약 buffer의 끝자리가 개행일 경우
//		if (buffer.size() > 0 && *(buffer.end() - 1) == '\r' && x == '\n')
//		{
//			// 메시지가 exit일 경우는 수신대기를 멈춘다.
//			if (*buffer.begin() == 'e' && *(buffer.begin() + 1) == 'x' && *(buffer.begin() + 2) == 'i' && *(buffer.begin() + 3) == 't') {
//				break;
//			}
//			// 콘솔에 출력하고 에코 메시지를 받는다.
//			const char* echo = print(&buffer);
//			// client로 에코 메시지 보낸다.
//			send(clientSock, echo, buffer.size() + 10, 0);
//			// 에코 메시지를 힙(new을 사용한 선언)에 선언했기 때문에 메모리 해지한다.
//			delete echo;
//			// 버퍼를 비운다.
//			buffer.clear();
//			// 다음 메시지 수신 대기
//			continue;
//		}
//		// 버퍼에 글자를 하나 넣는다.
//		buffer.push_back(x);
//	}
//	closesocket(clientSock);
//	cout << "Client disconnected IP address = " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;
//	for (auto ptr = clientlist->begin(); ptr < clientlist->end(); ptr++)
//	{
//		if ((*ptr)->get_id() == this_thread::get_id())
//		{
//			(*ptr)->detach();
//			clientlist->erase(ptr);
//			break;
//		}
//	}
//}
//
//char* print(vector<char>* str)
//{
//	// 포인트 위치
//	int p = 0;
//	// 버퍼 설정. +1은 \0를 넣기 위한 크기
//	char out[BUFFER_SIZE + 1];
//	// return을 하기 위해서는 힙에 데이터를 선언 해야 한다.
//	char* ret = new char[str->size() + 10];
//	// 메모리 복사 "echo - "를 붙힌다.
//	memcpy(ret, "echo - ", 7);
//	// 콘솔 출력
//	cout << "From Client message : ";
//	// buffer사이지를 넘어서는 데이터일 경우 반복을 통해서 받는다.
//	for (int n = 0; n < (str->size() / BUFFER_SIZE) + 1; n++)
//	{
//		// 버퍼 사이즈 설정
//		int size = str->size();
//		// 수신 데이터가 버퍼 사이즈를 넘었을 경우.
//		if (size > BUFFER_SIZE) {
//			if (str->size() < (n + 1) * BUFFER_SIZE)
//			{
//				size = str->size() % BUFFER_SIZE;
//			}
//			else
//			{
//				size = BUFFER_SIZE;
//			}
//		}
//		// echo 메시지와 콘솔 메시지를 작성한다.
//		for (int i = 0; i < size; i++, p++)
//		{
//			out[i] = *(str->begin() + p);
//			if (out[i] == '\0')
//			{
//				out[i] = ' ';
//			}
//			*(ret + p + 7) = out[i];
//		}
//		out[size] = '\0';
//		// 콘솔 메시지 콘솔 출력.
//		cout << out;
//	}
//	cout << endl;
//	// 에코 메시지는 끝에 개행 + ">"를 넣는다.
//	memcpy(ret + p + 7, "\n>\0", 3);
//	return ret;
//}
//
//
//SocketCollector::SocketCollector() {
//	
//}
//
//SocketCollector::~SocketCollector() {
//
//}
//
//
//bool SocketCollector::Initialize(bool* status) {
//	vector<thread*> clientList;
//	WSADATA wsaData;
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		return false;
//	}
//
//	SOCKET serverSocket = socket(PF_INET, SOCK_STREAM, 0);
//	
//	SOCKADDR_IN addr;
//	memset(&addr, 0, sizeof(addr));
//	addr.sin_family = AF_INET;
//	addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	addr.sin_port = htons(PORT + 1);
//
//	if (bind(serverSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
//		Logger::Error("SOCKET BINDING FAILED");
//		return false;
//	}
//
//	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
//		Logger::Error("SOCKET LISTENING FAILED");
//		return false;
//	}
//
//	*status = true;
//
//	Logger::Debug("SocketCollector STARTED");
//
//	while (true) {
//		int len = sizeof(SOCKADDR_IN);
//		SOCKADDR_IN clientAddr;
//		SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &len);
//		clientList.push_back(new thread(client, clientSocket, clientAddr, &clientList));
//	}
//
//	return true;
//}
//


void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
	Logger::Info(msg->get_payload());
}

bool SocketCollector::Initialize(bool* result) {
	server print_server;

	print_server.set_message_handler(&on_message);

	print_server.init_asio();
	print_server.listen(PORT + 1);
	print_server.start_accept();
	Logger::Debug("SocketCollector STARTED");
	*result = true;
	print_server.run();
	return true;
}