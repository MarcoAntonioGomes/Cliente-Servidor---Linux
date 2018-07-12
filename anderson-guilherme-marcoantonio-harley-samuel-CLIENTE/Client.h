#ifndef CLI_H_INCLUDED
#define CLI_H_INCLUDED
#include <netinet/in.h>

class Client{
	private:
		int 	clientSocket;
		int 	portNumber;
		int 	bufferSize;
		char* 	buffer;
		char* 	ip;
		struct 	sockaddr_in	serverAdress;

	public:
						Client 				();
		void 			SetClientSocket		(int _clientSocket);
		void 			SetPortNumber		(int _portNumber);
		void 			SetBufferSize		(int _bufferSize);
		void 			SetBuffer			(char* _buffer);
		void			SetBufferPosition	(int _index, char _char);
		void 			SetIp				(char* _ip);
		void			SetIpPosition		(int _index, char _char);
		int				GetClientSocket		();
		int				GetPortNumber		();
		int				GetBufferSize		();
		char*			GetBuffer 			();
		char			GetBufferPosition	(int _index);
		char*			GetIp				();
		char 			GetIpPosition		(int _index);
		sockaddr_in 	GetServerAdress	();
		sockaddr_in*	GetServerAdressPointer();
		bool			ConnectClient	();
		bool			ClientSocketCreated();
};

#endif
