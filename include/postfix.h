#pragma once
#include <exception>
#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <stack>
class elements {
protected:
	char sym;
	std::string view;
public:
	elements() {
		sym = '0';
		view = "0";
	}
	elements(char s, std::string v) {
		sym = s;
		view = v;
	}
	char getsym() { return sym; }
	std::string getview() { return view; }
	virtual int getpr() = 0;
	virtual double getznach() = 0;
};
class operations : public elements {
private:
	int pr;
public:
	int getpr() { return pr; }
	operations(char s, std::string v, int p) : elements(s, v) {
		pr = p;
	}
	operations() : elements() {
		pr = 0;
	}
	double getznach() {
		std::exception ex("У класса operations нет метода getznach() ");
		throw ex;
	}
};
class cyfry : public elements {
private:
	double znach;

public:
	cyfry(char s, std::string v, double p) : elements(s, v) {
		znach = p;
	}
	cyfry() : elements() {
		znach = 0;
	}
	int getpr() {
		std::exception ex("У класса cyfry нет метода getpr() ");
		throw ex;
	}
	double getznach() {
		return znach;
	}
};
class translyator {
private:
	elements** elem;
	const int M = 16;
	const int K = 10;
	std::string st;
	int faza;
	std::vector<elements*> v;

public:
	translyator(std::string str) {
		elem = new elements * [M];
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
						size_t pos;
						double res = std::stod(&st[i], &pos);
						v.push_back(new cyfry('_', "cyfra", res));
						i += pos;
					}
					else
						if ((find(st[i]))->getsym() == '(') {
							sost = 0;
							v.push_back(find(st[i]));
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
						v.push_back(find(st[i]));
						if (k >= 0) {
							i++;
							sost = 1;
						}
						else
							sost = 3;
					}
					else
						if ((find(st[i]))->getview() == "oper") {
							v.push_back(find(st[i]));

							if (st[i + 1] == '\0') {
								sost = 3;
							}
							else {
								sost = 0;
								i++;
							}
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
			std::vector<elements*> v1;
			for (int i = 0; i < v.size(); i++) {
				if (v[i]->getview() == "cyfra") {
					v1.push_back(v[i]);
				}
				else
					if (v[i]->getview() == "oper") {
						while ((stack.size() != 0) && (v[i]->getpr() <= (stack.top())->getpr())) {
							v1.push_back(stack.top());
							stack.pop();
						}
						stack.push(v[i]);
					}
					else
						if (v[i]->getsym() == ')') {
							while ((stack.top())->getsym() != '(') {
								v1.push_back(stack.top());
								stack.pop();
							}
							stack.pop();
						}
						else
							if ((v[i])->getsym() == '(')
								stack.push(v[i]);
			}

			if (!(stack.empty())) {
				while (!(stack.empty())) {
					v1.push_back(stack.top());
					stack.pop();
				}
			}

			faza = 2;
			v = v1;

		}
		else {
			std::exception ex("Wrong stage");
			throw ex;
		}
	}
	double result() {
		if (faza == 2) {
			int i;
			std::stack<double> st1;
			for (i = 0; i < v.size(); i++) {
				if (v[i]->getview() == "cyfra")
					st1.push(v[i]->getznach());
				else
					if (v[i]->getview() == "oper") {
						double a;
						double b;
						if (v[i]->getsym() == '+') {
							b = st1.top();
							st1.pop();
							a = st1.top() + b;
							st1.pop();
							st1.push(a);
						}
						else
							if (v[i]->getsym() == '-') {
								b = st1.top();
								st1.pop();
								a = st1.top() - b;
								st1.pop();
								st1.push(a);
							}
							else
								if (v[i]->getsym() == '*') {
									b = st1.top();
									st1.pop();
									a = st1.top() * b;
									st1.pop();
									st1.push(a);
								}
								else
									if (v[i]->getsym() == '/') {
										if (st1.top() == 0) {
											std::exception ex("Cannot be divided by zero");
											throw ex;
										}
										else {
											b = st1.top();
											st1.pop();
											a = st1.top() / b;
											st1.pop();
											st1.push(a);
										}
									}

					}
			}
			return st1.top();
		}

		else
		{
			std::exception ex("Wrong stage");
			throw ex;
		}

	}


};
