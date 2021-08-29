#pragma once
#include <exception>
#include <iostream>
using namespace std;
class MyException : public exception
{
public:
	explicit MyException(const char* _reason):cReason(new char[strlen(_reason) + 1]),sReason(_reason)
	{
		strcpy_s(cReason, strlen(_reason) + 1, _reason);
		cout << cReason << endl;
	}
	explicit MyException(string& _reason):cReason(new char[_reason.size()+1]),sReason(_reason)
	{
		for (int i = 0; i < _reason.size(); i++)
			cReason[i] = _reason[i];
		cReason[_reason.size()] = '\0';
		cout << sReason << endl;
	}
	const char* what() const throw()
	{
		return cReason;
	}

private:
	char* cReason;
	string sReason;
};

