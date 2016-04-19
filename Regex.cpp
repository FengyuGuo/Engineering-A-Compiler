#include "stdafx.h"
#include "Regex.h"
#include <string>
#include <locale>
using namespace std;

Regex::~Regex()
{
}

Regex::Regex() :_regex(""), _i(0)
{
}

Regex::Regex(const string& reg) : _regex(reg), _i(0)
{

}

bool Regex::compile(const string& reg)
{
	_regex = reg;
	return _compile();
}

bool Regex::match(const string& mtch)
{
	return _regexNFA.testString(mtch);
}

bool Regex::_compile()
{
	_regexNFA.reset();

	return true;
}

