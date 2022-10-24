#include "client.h"

int main(int argc, char* argv[])
{
		if(argc != 3 || socketCreation(argv))
		{
				usage(argv);
				return -1;
		}

		return 0;
}
