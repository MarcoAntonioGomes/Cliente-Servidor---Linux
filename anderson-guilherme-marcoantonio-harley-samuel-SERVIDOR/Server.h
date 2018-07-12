#ifndef SERV_H_INCLUDED
#define SERV_H_INCLUDED
#include <netinet/in.h>

class Server{
	private:
		int 				clientSocket;
		int* 				serverSocket;
		int 				portNumber;
		int 				bufferSize;
		int					clientsNumber;
		int					maxClients;
		char** 				buffer;
		socklen_t 			adressSize;
		struct  sockaddr_in serverAdress;

	public:
						Server 					();
		void 			SetClientSocket			(int _clientSocket);
		void 			SetServerSocket			(int _serverSocket, int _index);
		void 			SetPortNumber			(int _portNumber);
		void 			SetBufferSize			(int _bufferSize);
		void			SetClientsNumber		(int _clientsNumber);
		void			SetMaxClients 			(int _max);
		void 			SetBuffer 				(char* _buffer, int _index);
		void 			SetBufferPosition		(int _index, int _index2, char _char);
		void			SetAdressSize			(socklen_t _adressSize);
		int				GetClientSocket			();
		int				GetServerSocket			(int _index);
		int				GetPortNumber			();
		int				GetBufferSize			();
		int				GetClientsNumber		();
		int				GetMaxClients			();
		char*			GetBuffer 				(int _index);
		char 			GetBufferPosition		(int _index, int _index2);
		socklen_t*		GetAdressSize			();
		sockaddr_in 	GetServerAdress			();
		sockaddr_in*	GetServerAdressPointer	();
		bool			BindSocket				();
};

#endif
