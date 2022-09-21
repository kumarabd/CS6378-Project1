#include <iostream>
#include <cstdlib>
#include <fstream>
#include "readConfig.cpp"
using namespace std;

int main()
{
	///* A list of possible environment variables*/
	//const char *env_var[5] = {"NODE_COUNT","minPerActive","maxPerActive","minSendDelay","maxNumber"};
	//char *env_val[5];

	//for(int i=0; i<5; i++)
	//{
	//	/* Getting environment value if exists */
	//	env_val[i] = getenv(env_var[i]);
	//	if (env_val[i] != NULL)
	//		cout << env_var[i] << "= " << env_val[i] << endl;
	//}

	read_config();

    //create_nodes();

    //create_network(nodes);

}