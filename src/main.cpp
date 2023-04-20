#include "main.h"

//是否开启debug
#define DEBUG false

#if DEBUG
#include "test/bing/test_project.hpp"
#endif // DEBUG


int main(int argv, char** argc) {
	init();


#if DEBUG
	std::cout << "\033[44m--* DEBUG START *--\033[0m\n";
	
	//TestProject::testMethod1();
	//TestProject::testMethod2();
	//TestProject::testMethod3();
	//TestProject::testMethod4();


	std::cout << "\033[44m--*  DEBUG END  *--\033[0m\n";
#endif // DEBUG

	startServer();
	return 0;
}