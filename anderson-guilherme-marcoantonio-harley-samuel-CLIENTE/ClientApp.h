#ifndef CLIAPP_H_INCLUDED
#define CLIAPP_H_INCLUDED
#include <vector>
#include <iostream>
#include "Client.h"

using namespace std;

class ClientApp{
	private:
		Client* client;

	public:
						ClientApp				();
        Client*			GetClient				();
		string 			GenerateBase64			(string _password);
		string 			SerializeData			(string _user, string _password);
		string 			SerializeDataChangePass	(string _user, string _newPassword);
		void			InitializeBuffer		(char* _buffer, int _size);
		void			ConstructBuffer			(char* _buffer, string _auxBuffer);
		bool			VerifyData				(string _user, string _password);
		vector<string> 	Slice					(string _string, char _character);
		void			Application				();
};

#endif
