#include "SocketCollector.hpp"

//using namespace std;

//void client(SOCKET clientSock, SOCKADDR_IN clientAddr, vector<thread*>* clientlist)
//{
//	// ���� ������ �ֿܼ� ����Ѵ�.
//	cout << "Client connected IP address = " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;
//	// client�� �޽����� ������.
//	const char* message = "Welcome server!\r\n>\0";
//	send(clientSock, message, strlen(message) + 1, 0);
//	// telent�� �ѱ��ھ� �����Ͱ� ���� ������ ���ڸ� ���� buffer�� �ʿ��ϴ�.
//	vector<char> buffer;
//	// ���� ������
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
//		// ���� buffer�� ���ڸ��� ������ ���
//		if (buffer.size() > 0 && *(buffer.end() - 1) == '\r' && x == '\n')
//		{
//			// �޽����� exit�� ���� ���Ŵ�⸦ �����.
//			if (*buffer.begin() == 'e' && *(buffer.begin() + 1) == 'x' && *(buffer.begin() + 2) == 'i' && *(buffer.begin() + 3) == 't') {
//				break;
//			}
//			// �ֿܼ� ����ϰ� ���� �޽����� �޴´�.
//			const char* echo = print(&buffer);
//			// client�� ���� �޽��� ������.
//			send(clientSock, echo, buffer.size() + 10, 0);
//			// ���� �޽����� ��(new�� ����� ����)�� �����߱� ������ �޸� �����Ѵ�.
//			delete echo;
//			// ���۸� ����.
//			buffer.clear();
//			// ���� �޽��� ���� ���
//			continue;
//		}
//		// ���ۿ� ���ڸ� �ϳ� �ִ´�.
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
//	// ����Ʈ ��ġ
//	int p = 0;
//	// ���� ����. +1�� \0�� �ֱ� ���� ũ��
//	char out[BUFFER_SIZE + 1];
//	// return�� �ϱ� ���ؼ��� ���� �����͸� ���� �ؾ� �Ѵ�.
//	char* ret = new char[str->size() + 10];
//	// �޸� ���� "echo - "�� ������.
//	memcpy(ret, "echo - ", 7);
//	// �ܼ� ���
//	cout << "From Client message : ";
//	// buffer�������� �Ѿ�� �������� ��� �ݺ��� ���ؼ� �޴´�.
//	for (int n = 0; n < (str->size() / BUFFER_SIZE) + 1; n++)
//	{
//		// ���� ������ ����
//		int size = str->size();
//		// ���� �����Ͱ� ���� ����� �Ѿ��� ���.
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
//		// echo �޽����� �ܼ� �޽����� �ۼ��Ѵ�.
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
//		// �ܼ� �޽��� �ܼ� ���.
//		cout << out;
//	}
//	cout << endl;
//	// ���� �޽����� ���� ���� + ">"�� �ִ´�.
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