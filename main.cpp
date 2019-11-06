
#include <stack>
#include <string>
#include "translyator.h"


int main() {
	setlocale(LC_CTYPE, " Rus");
	translyator trans("2*(5-6*(9-1*4))");
	try {
		trans.check();
		trans.pz();
		trans.result();
	}
	catch (std::exception ex) {
	   std::cout<< ex.what();
	}
}