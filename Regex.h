#pragma once
#include <string>
#include "NFA.h"
using std::string;
class Regex
{
public:
	//Regex();
	~Regex();
	Regex();
	Regex(const string& reg);

	bool compile(const string& reg);
	bool match(const string& mtch);
private:
	bool _compile();
	string _regex;
	NFA _regexNFA;

	int _i;

};

