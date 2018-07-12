#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sstream>
#include <vector>
#include "ClientApp.h"
#include "base64.h"
#include "Client.h"

using namespace std;

ClientApp::ClientApp(){
	Client* _client = new Client();

	this->client = _client;
}

Client* ClientApp::GetClient(){
	return this->client;
}

string ClientApp::GenerateBase64(string _password){
	return base64_encode(reinterpret_cast<const unsigned char*>(_password.c_str()), _password.length());
}


string ClientApp::SerializeData(string _user, string _password){
	string auxBuffer;

    auxBuffer = "auth\nuser:";
    _password = this->GenerateBase64(_password);
    auxBuffer +=_user+"\npass:"+_password+"\n\n";

    return auxBuffer;
}


string ClientApp::SerializeDataChangePass(string _user, string _newPassword){
	string auxBuffer,pass, newpass;
    vector<string> sep1,sep2;
    sep2 = this->Slice(_user, '*');
    _user = sep2[1];
    auxBuffer = "changepass\nuser: ";
    sep1 = this->Slice(_newPassword, '*');
    pass = sep1[0];
    newpass = sep1[1];
    pass = this->GenerateBase64(pass);
    newpass = this->GenerateBase64(newpass);
    auxBuffer += _user + "\npass: "+pass+"\nnewpass: "+newpass+"\n\n";

    return auxBuffer;
}

void ClientApp::InitializeBuffer(char* _buffer, int _size){
	for(int i=0;i<_size;i++){
        _buffer[i] = '\0';
    }
}

void ClientApp::ConstructBuffer(char* _buffer, string _auxBuffer){
	for(int i = 0; i < _auxBuffer.size(); i++){
        if(_auxBuffer[i] != '\0'){
            _buffer[i] =  _auxBuffer[i];
        }
    }
}

bool ClientApp::VerifyData(string _user, string _password){
	vector<string> sep1;
    bool notFind = true;

     if(_user[0] == '*'){
        if(_password[0] == '*'){
            cout << "\n===> Error: wrong password format." << endl << endl;
            return true;
         }

      for(int i = 1; i < _password.length();i++){
        if(_password[i] == '*' ){
          notFind = false;
        }
      }

      if(notFind){
            cout << "\n===> Error: wrong password format." << endl << endl;
            return notFind;
         }
    }

    return false;
}


vector<string> ClientApp::Slice(string _string, char _character){
	vector<string> internal;
  	stringstream ss(_string);
  	string tok;

  	while(getline(ss, tok, _character)) {
    	internal.push_back(tok);
  	}

  	return internal;
}

void ClientApp::Application(){
	string user, pass, auxBuffer;
	char* pEnd;
	long int status;
	bool isExit = false;
    bool isExitServerResponse = false;

    if(this->GetClient()->ConnectClient()){
        recv(this->GetClient()->GetClientSocket(), this->GetClient()->GetBuffer(), this->GetClient()->GetBufferSize(), 0);

        do{
            do{
                cout << "auth" << endl;
                cout << "user: ";
                cin>> user;
                cout << "pass: ";
                cin >> pass;

            }while(this->VerifyData(user,pass));

            this->InitializeBuffer(this->GetClient()->GetBuffer(), this->GetClient()->GetBufferSize());

            if(user[0] == '*'){
                auxBuffer = this->SerializeDataChangePass(user,pass);
            }else{
                auxBuffer = this->SerializeData(user,pass);
            }


            this->ConstructBuffer(this->GetClient()->GetBuffer(), auxBuffer);

            send(this->GetClient()->GetClientSocket(), this->GetClient()->GetBuffer(), this->GetClient()->GetBufferSize(), 0);

            do{
                recv(this->GetClient()->GetClientSocket(), this->GetClient()->GetBuffer(), this->GetClient()->GetBufferSize(), 0);
                cout << endl;
                cout << this->GetClient()->GetBuffer() <<endl ;
                status = strtol (this->GetClient()->GetBuffer(),&pEnd,10);

                if (status == 200) {
                    isExitServerResponse = true;
                    isExit = true;
                    cout << "RELEASED LOCK" <<endl;
                }

                if(status == 250 || status == 401 || status == 450){
                    isExitServerResponse = true;
                }
            }while(!isExitServerResponse);
                cout << endl;


        }while(!isExit);
    }else{
        cout << "===> Couldn't connect to the server..." << endl;
    }



}
