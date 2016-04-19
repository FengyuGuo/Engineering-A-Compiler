#pragma once
#include <string>
#include "NFA.h"
using std::string;
/*!
 * \class Regex
 *
 * \brief 尚未完成编译功能！
 *
 * \author fengyu.guo
 * \date 四月 2016
 */
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

