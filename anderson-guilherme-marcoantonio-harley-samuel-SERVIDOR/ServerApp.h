#ifndef SERVAPP_H_INCLUDED
#define SERVAPP_H_INCLUDED
#include <iostream>
#include <vector>
#include <thread>
#include "Server.h"

using namespace std;

class ServerApp{
	private:
		Server* server;
		thread* service;
		bool    authorized;

	public:
						ServerApp				();
        void 			SetAuthorized           (bool _authorized);
        bool            GetAuthorized           ();
		Server*			GetServer				();
		string			GenerateBase64			(string _password);
		string			DegenerateBase64		(string _encoded);
		void			InitializeBuffer		(char* _buffer, int _size);
		void			ConstructBuffer			(char* _buffer, string _auxBuffer);
		vector<string>	Split					(string _string, char _character);
		void			BufferTreatment			(char* _buffer, string _answer, int _bufferSize);
		void			Application				();
		void			Online					(int _index);
};

#endif
