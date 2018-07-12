#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "ServerApp.h"

using namespace std;

int main(){
	ServerApp* app = new ServerApp();
	app->Application();

	return 0;
}
