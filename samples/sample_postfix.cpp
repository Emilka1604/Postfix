#include <stack>
#include <string>
#include "postfix.h"


int main() {
	translyator trans("(2.3+8.98)*4.5-1");
	try {
		trans.check();
		trans.pz();
		std::cout << trans.result();
	}
	catch (std::exception ex) {
		std::cout << ex.what();
	}
	return 0;
}