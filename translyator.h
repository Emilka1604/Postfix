#pragma once
#include <exception>
#include <iostream>
#include <string>
#include <locale>
class elements {
protected:
    char sym;
	std::string view;
public:
	elements() {
		sym = '0';
		view = "0";
	}
	elements(char s, std::string v){
		sym = s;
		view = v;
	}
    char getsym() { return sym; }
	std::string getview()  { return view; }
	virtual int getpr() = 0{}
	virtual int getznach() = 0{}
};
class operations: public elements {
private:
	unsigned int pr;
	public:
	    int getpr() { return pr; }
		operations(char s, std::string v, unsigned int p): elements(s, v){
			pr = p;
		}
	     operations() : elements() {
			pr = 0;
		}
		 int getznach() {
			 std::exception ex("У класса cyfry нет метода getznach() ");
			 throw ex;
		 }
};
class cyfry : public elements {
private:
	int znach;

public:
	cyfry(char s, std::string v, int p): elements(s, v) {
		znach = p;
	}
	cyfry(): elements() {}
	int getpr() {
		std::exception ex("У класса cyfry нет метода getpr() ");
		throw ex;
	}
	int getznach() {
		return znach;
	}
};
class translyator {
private:
	elements** elem;
	int M;
	int K;
	std::string st;
	int faza;

public:
	 translyator(std::string str) {
		 M = 16;
		 K = 10;
		elem = new elements*[M];
		int i;
		for (i = 0; i < K; i++) {
			elem[i] = new cyfry((std::to_string(i))[0], "cyfra", i);
		}
		elem[K] = new operations('+', "oper", 1);
		elem[K + 1] = new operations('-', "oper", 1);
		elem[K + 2] = new operations('*', "oper", 2);
		elem[K + 3] = new operations('/', "oper", 2);
		elem[K + 4] = new operations('(', "skobka", 0);
		elem[K + 5] = new operations(')', "skobka", 0);
		st = str;
		faza = 0;
	}
	 elements* find(char ch) {
		 int m = -1;
		 for (int i = 0; i < M; i++)
			 if (elem[i]->getsym() == ch) {
				 m = i;
				 break;
			 }
		 if (m != -1)
			 return elem[m];
		 else {
			 std::exception ex("Not correct symbol");
				 throw ex;
		 }
	 }
	 void check() {
		 int i = 0;
		 int k = 0;
		 int sost = 0;
		 while (sost < 2) {
			 switch (sost) {
			 case 0:
				 if (st[i] == '\0')
					 sost = 2;
				 else
					 if ((find(st[i]))->getview() == "cyfra") {
						 sost = 1;
						 i++;
					 }
					 else
						 if ((find(st[i]))->getsym() == '(') {
							 sost = 0;
							 k++;
							 i++;
						 }
						 else
							 sost = 3;
				 break;
			 case 1:
				 if (st[i] == '\0')
					 sost = 2;
				 else
					 if ((find(st[i]))->getsym() == ')') {
						 k--;
						 if (k >= 0) {
							 i++;
							 sost = 1;
						 }
						 else
							 sost = 3;
					 }
					 else
						 if ((find(st[i]))->getview() == "oper") {
							 sost = 0;
							 i++;
						 }
						 else
							 sost = 3;
				 break;
			 }
		 }
		 if (sost == 2) {
			 if (k != 0)
				 sost = 3;
			 else
				 sost = 4;
		 }
		 if (sost == 3) {
			 std::exception ex("Not correct string");
			 throw ex;
		 }
		 else
			 if (sost == 4) {
				 faza = 1;
			 }
	 }
	 void pz() {
		 if (faza == 1) {
			 std::stack<elements*> stack;
			 std::string str;
			 for (int i = 0; i < st.size(); i++) {
				 if ((find(st[i]))->getview() == "cyfra") {
					 str += st[i];
				 }
				 else
					 if ((find(st[i]))->getview() == "oper") {
						 while ((stack.size() != 0) && ((find(st[i]))->getpr() <= (stack.top())->getpr())) {
							 str += (stack.top())->getsym();
							 stack.pop();
						 }
						 stack.push(find(st[i]));
					 }
					 else
						 if ((find(st[i]))->getsym() == ')') {
							 while ((stack.top())->getsym() != '(') {
								 str += (stack.top())->getsym();
								 stack.pop();
							 }
							 stack.pop();
						 }
						 else
							 if ((find(st[i]))->getsym() == '(')
								 stack.push(find(st[i]));
			 }
						 
			 if (!(stack.empty())) {
				 while (!(stack.empty())) {
					 str += (stack.top())->getsym();
					 stack.pop();
				 }
			 }

			 faza = 2;
			 st = str;
		 }
		 else {
			 std::exception ex("Wrong stage");
			 throw ex;
		 }
	 }
	  unsigned int result() {
		  if (faza == 2) {
			  int i;
			  std::stack<int> st1;
			  std::stack<char> st2;
			  for (i = 0; i < st.size(); i++) 
				  if ((find(st[i]))->getview() == "cyfra")
					  st1.push((find(st[i]))->getznach());
			  for (i = st.size() - 1; i > 0; i--)
				  if ((find(st[i]))->getview() == "oper")
					  st2.push((find(st[i]))->getsym());
			  if (st2.empty())
				  return st1.top();
			  else {
				  while (st2.size() != 0) {
					  int a;
					  int b;
					  if (st2.top() == '+') {
						  st2.pop();
						  b = st1.top();
						  st1.pop();
						  a = st1.top() + b;
						  st1.pop();
						  st1.push(a);
					  }
					  else
						  if (st2.top() == '-') {
							  st2.pop();
							  b = st1.top();
							  st1.pop();
							  a = st1.top() - b;
							  st1.pop();
							  st1.push(a);
						  }
						  else
							  if (st2.top() == '*') {
								  st2.pop();
								  b = st1.top();
								  st1.pop();
								  a = st1.top() * b;
								  st1.pop();
								  st1.push(a);
							  }
							  else {
								  st2.pop();
								  b = st1.top();
								  st1.pop();
								  a = st1.top() / b;
								  st1.pop();
								  st1.push(a);
							  }

				  }
				  std::cout<< "Result:" << st1.top();
			  }
		  }
		  else
		  {
			  std::exception ex("Wrong stage");
			  throw ex;
		  }

	 }


};
