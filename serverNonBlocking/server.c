#include "server.h"

int main(int argc, char *argv[])
{
		if(argc != 2 || socketCreation(argv))
		{
				usage(argv);
				return -1; 
		}

		return 0;
}
