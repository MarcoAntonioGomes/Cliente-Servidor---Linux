#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include "base64.h"
#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
#include "sha1.h"
#include "Server.h"
#include "ServerApp.h"

using namespace std;

ServerApp::ServerApp(){
    this->SetAuthorized(false);
	Server* _server = new Server();
    this->server = _server;
    this->service = new thread[10];
}

void ServerApp::SetAuthorized(bool _authorized){
    this->authorized = _authorized;
}

bool ServerApp::GetAuthorized(){
    return this->authorized;
}

Server* ServerApp::GetServer(){
	return this->server;
}

string ServerApp::GenerateBase64(string _password){
	return base64_encode(reinterpret_cast<const unsigned char*>(_password.c_str()), _password.length());
}

string ServerApp::DegenerateBase64(string _encoded){
	return base64_decode(_encoded);
}

void ServerApp::InitializeBuffer(char* _buffer, int _size){
	for(int i=0;i<_size;i++){
    	_buffer[i] = '\0';
    }
}

void ServerApp::ConstructBuffer(char* _buffer, string _auxBuffer){
	for(int i = 0; i < _auxBuffer.size(); i++){
    	_buffer[i] =  _auxBuffer[i];
    }
}

vector<string> ServerApp::Split(string _string, char _character){
	vector<string> internal;
  	stringstream ss(_string);
  	string tok;

  	while(getline(ss, tok, _character)) {
		internal.push_back(tok);
  	}

  	return internal;
}

void ServerApp::BufferTreatment(char* _buffer, string _answer, int _bufferSize){
	vector<string> sep1,sep2,sep_arq;
    ifstream fileReader ("pass.txt");
    string senha, novasenha, line,senhaantiga, novobanco="";
    char flagUsuario, flagUsuario1 =0;
    int flagcontrole, flagcontrole1=0;

    sep1=this->Split(_buffer, '\n');

    if(sep1[0] == "auth"){
        sep2=this->Split(sep1[2], ':'); //quebra a linha aonde tem a senha
        senha = this->DegenerateBase64(sep2[1]); // pega a senha e volta ela para o normal
        sep2 = this->Split(sep1[1],':'); //pega usuario no no buffer
        if(fileReader.is_open()){
            while (getline(fileReader,line)){
                sep_arq=this->Split(line, ':');


               if(sep_arq[0] == sep2[1]){ //usuario do arquivo = a usuario do buffer

                    flagUsuario1 = 1;

                    if(sep_arq[1] == sha1(senha)){ //verifica a senha do arquivo e a senha do buffer
                        flagcontrole1 = 1;
                    }
                }
            }

        }

        fileReader.close(); // fim do arquivo

        if(flagUsuario1 == 1){
            _answer = "401 Not authorized\n";
        }

        if(flagcontrole1 == 1){
            _answer = "200 Auth OK\n";
            this->SetAuthorized(true);

        }else{
            _answer = "450 User doesn't exist\n";
        }

        this->InitializeBuffer(_buffer, _bufferSize);
        this->ConstructBuffer(_buffer, _answer);

    }

        sep1=this->Split(_buffer, '\n');

     if(sep1[0] == "changepass"){

        sep2=this->Split(sep1[2], ' '); //pega a senha antiga
        senha = this->DegenerateBase64(sep2[1]); //pega a senha antiga e tira do base
        sep2=this->Split(sep1[3],' '); //pega a novasenha
        novasenha = this->DegenerateBase64(sep2[1]); //decode newpassw
        sep2 = this->Split(sep1[1],' '); //usuario do buffer
        if(fileReader.is_open()){

            while (getline(fileReader,line)){
                sep_arq=this->Split(line, ':'); //recupera user e senha do arquivo
                if(sep_arq[0] ==sep2[1]) {
                    flagUsuario = 1;

                    senha = sha1(senha);

                   if(sep_arq[1]==senha){
                        string aux;
                        aux = sep2[1]+":"+sha1(novasenha)+"\n";
                        novobanco = novobanco + aux;
                        flagcontrole =1;
                    }

            } else{
                    novobanco = novobanco + line+"\n";
            }
        }

        }
        fileReader.close(); // fim do arquivo

        if(flagcontrole == 1){
            ofstream f_out;
            f_out.open("pass.txt");

            if(f_out.is_open()){
                _answer = "250 Pass changed\n";
                this->InitializeBuffer(_buffer, _bufferSize);

                this->ConstructBuffer(_buffer, _answer);

                f_out<<novobanco;
            }
        f_out.close();


        }else if(flagUsuario ==1){
            _answer = "401 Not authorized\n";
            this->InitializeBuffer(_buffer, _bufferSize);
            this->ConstructBuffer(_buffer, _answer);

        }else{
            _answer = "450 User doesn't exist\n";
            this->InitializeBuffer(_buffer, _bufferSize);
            this->ConstructBuffer(_buffer, _answer);

        }


    }
}

void ServerApp::Application(){
    cout << "===> Waiting for clients..." << endl << endl;

    while(1){
        int _socket = accept(this->GetServer()->GetClientSocket(), (struct sockaddr*)this->GetServer()->GetServerAdressPointer(), this->GetServer()->GetAdressSize());
        this->GetServer()->SetServerSocket(_socket, this->GetServer()->GetClientsNumber());

        if(this->GetServer()->GetServerSocket(this->GetServer()->GetClientsNumber()) > 0){
            cout << "===> Server received connection." << endl;
            this->service[this->GetServer()->GetClientsNumber()] = thread(&ServerApp::Online, this, this->GetServer()->GetClientsNumber());
            this->GetServer()->SetClientsNumber(this->GetServer()->GetClientsNumber() + 1);
        }
    }
}

void ServerApp::Online(int _index){
	string resposta;
	bool isExit = false;
    bool isExistClient = false;
    bool isExitClientResponse = false;

	while(1){
		strcpy(this->GetServer()->GetBuffer(_index), "===> Server connected...\n");
        send(this->GetServer()->GetServerSocket(_index), this->GetServer()->GetBuffer(_index), this->GetServer()->GetBufferSize(), 0);
        cout << "===> Connected with the client #" << _index << endl;

        do {
            recv(this->GetServer()->GetServerSocket(_index), this->GetServer()->GetBuffer(_index), this->GetServer()->GetBufferSize(), 0);
            this->BufferTreatment(this->GetServer()->GetBuffer(_index), resposta, this->GetServer()->GetBufferSize());
            send(this->GetServer()->GetServerSocket(_index), this->GetServer()->GetBuffer(_index), this->GetServer()->GetBufferSize(), 0);

            if (this->GetAuthorized()) {
                this->SetAuthorized(false);
                this->Application();

                isExit = true;
                isExitClientResponse = true;
            }

        }while(!isExitClientResponse);
    }
}
