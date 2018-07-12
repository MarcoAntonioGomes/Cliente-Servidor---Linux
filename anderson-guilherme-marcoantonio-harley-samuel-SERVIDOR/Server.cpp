#include "Server.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

Server::Server(){
	this->SetClientSocket(socket(AF_INET, SOCK_STREAM, 0));
	this->SetPortNumber(5555);
	this->SetBufferSize(1024);
	this->SetMaxClients(10);
	this->SetClientsNumber(0);
	this->buffer = (char**)malloc(this->GetMaxClients()*sizeof(char*));
	this->serverSocket = (int*)malloc(this->GetMaxClients()*sizeof(int*));

	for(int i = 0; i < this->GetMaxClients(); i++){
		this->buffer[i] = (char*)malloc(this->GetBufferSize()*sizeof(char));
	}

	for(int j = 0; j < this->GetMaxClients(); j++){
		for(int i = 0;i < this->GetBufferSize(); i++){
			this->SetBufferPosition(j, i, '\0');
		}
	}



	this->serverAdress.sin_family = AF_INET;
	this->serverAdress.sin_addr.s_addr = htons(INADDR_ANY);
    this->serverAdress.sin_port = htons(this->GetPortNumber());

    if(this->BindSocket()){
    	cout << "===> Error: couldn't bind socket." << endl;
    }

    this->SetAdressSize(sizeof(this->GetServerAdress()));
    listen(this->GetClientSocket(), 5);
}

void Server::SetClientSocket(int _clientSocket){
	this->clientSocket = _clientSocket;
}

void Server::SetServerSocket(int _serverSocket, int _index){
	this->serverSocket[_index] = _serverSocket;
}

void Server::SetPortNumber(int _portNumber){
	this->portNumber = _portNumber;
}

void Server::SetBufferSize(int _bufferSize){
	this->bufferSize = _bufferSize;
}

void Server::SetClientsNumber(int _clientsNumber){
	this->clientsNumber = _clientsNumber;
}

void Server::SetMaxClients(int _max){
	this->maxClients = _max;
}

void Server::SetBuffer(char* _buffer, int _index){
	this->buffer[_index] = _buffer;
}

void Server::SetBufferPosition(int _index, int _index2, char _char){
	this->buffer[_index][_index2] = _char;
}

void Server::SetAdressSize(socklen_t _adressSize){
	this->adressSize = _adressSize;
}

int	Server::GetClientSocket(){
	return this->clientSocket;
}

int Server::GetServerSocket(int _index){
	return this->serverSocket[_index];
}

int	Server::GetPortNumber(){
	return this->portNumber;
}

int	Server::GetBufferSize(){
	return this->bufferSize;
}

int Server::GetClientsNumber(){
	return this->clientsNumber;
}

int Server::GetMaxClients(){
	return this->maxClients;
}

char* Server::GetBuffer(int _index){
	return this->buffer[_index];
}

char Server::GetBufferPosition(int _index, int _index2){
	return this->buffer[_index][_index2];
}

socklen_t* Server::GetAdressSize(){
	return &this->adressSize;
}

sockaddr_in Server::GetServerAdress(){
	return this->serverAdress;
}

sockaddr_in* Server::GetServerAdressPointer(){
	return &this->serverAdress;
}

bool Server::BindSocket(){
	return (bind(this->GetClientSocket(), (struct sockaddr*)this->GetServerAdressPointer(), sizeof(this->GetServerAdress())) < 0);
}
