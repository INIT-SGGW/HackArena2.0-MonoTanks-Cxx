#pragma once

#include "pch.h"
#include "handler.h"
#include "agent/agent.h"

class WebSocketClient {
 public:
	WebSocketClient(std::string  host, std::string  port, std::string nickname, std::string  code, int timeoutNumber);
	~WebSocketClient();

	std::future<bool> Connect();
	void Run();
	void Stop();
	std::string ConstructUrl(const std::string& code, const std::string& nickname, bool quickJoin);

 private:
	void DoConnect();
	void DoRead();
	void SendToProcessing();
	void DoWrite();
	void ProcessMessage(const std::string& message);
	void RespondToPing();
	void Reconnect();

	std::string host;
	std::string port;
	std::string nickname;
	std::string code;
	int timeoutNumber;

	boost::asio::io_context ioc;
	boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws;
	std::thread workThread;
	Handler handler;
	Agent agent;
	std::promise<bool> connectPromise;

	std::queue<std::string> messagesToSend;
	std::mutex mtx;
	std::condition_variable cv;
	std::queue<std::string> messagesReceived;
	std::mutex mtxR;
	std::condition_variable cvR;
	bool isPromiseSet = false;
	bool isReconnecting = false;
	std::chrono::steady_clock::time_point reconnectStartTime;
};