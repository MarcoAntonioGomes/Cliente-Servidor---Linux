#include "Client.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

Client::Client(){
	this->SetClientSocket(socket(AF_INET, SOCK_STREAM, 0));
	this->ClientSocketCreated();
	this->SetPortNumber(5555);
	this->SetBufferSize(1024);
	this->buffer = (char*)malloc(this->GetBufferSize()*sizeof(char));
	this->ip = (char*)malloc(9*sizeof(char));

	for(int i = 0;i < this->GetBufferSize(); i++){
		this->SetBufferPosition(i, '\0');
	}

	for(int i = 0; i < 9; i++){
		this->SetIpPosition(i, '\0');
	}

	this->SetIp("127.0.0.1");
	this->serverAdress.sin_family = AF_INET;
    this->serverAdress.sin_port = htons(this->GetPortNumber());
}

void Client::SetClientSocket(int _clientSocket){
	this->clientSocket = _clientSocket;
}

void Client::SetPortNumber(int _portNumber){
	this->portNumber = _portNumber;
}

void Client::SetBufferSize(int _bufferSize){
	this->bufferSize = _bufferSize;
}

void Client::SetBuffer(char* _buffer){
	this->buffer = _buffer;
}

void Client::SetBufferPosition(int _index, char _char){
	this->buffer[_index] = _char;
}

void Client::SetIp(char* _ip){
	this->ip = _ip;
}

void Client::SetIpPosition(int _index, char _char){
	this->ip[_index] = _char;
}

int	Client::GetClientSocket(){
	return this->clientSocket;
}

int	Client::GetPortNumber(){
	return this->portNumber;
}

int	Client::GetBufferSize(){
	return this->bufferSize;
}

char* Client::GetBuffer(){
	return this->buffer;
}

char Client::GetBufferPosition(int _index){
	return this->buffer[_index];
}

char* Client::GetIp(){
	return this->ip;
}

char Client::GetIpPosition(int _index){
	return this->ip[_index];
}

sockaddr_in Client::GetServerAdress(){
	return this->serverAdress;
}

sockaddr_in* Client::GetServerAdressPointer(){
	return &this->serverAdress;
}

bool Client::ConnectClient(){
	return (connect(this->GetClientSocket(), (struct sockaddr *)this->GetServerAdressPointer(), sizeof(this->GetServerAdress())) == 0);
}

bool Client::ClientSocketCreated(){
	bool created = (this->GetClientSocket() >= 0);

	return (created);
}
