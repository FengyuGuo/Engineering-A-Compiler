#pragma once
#include <string>
#include "NFA.h"
using std::string;
/*!
 * \class Regex
 *
 * \ingroup GroupName
 *
 * \brief 正则表达式，尚未完成！
 *
 * TODO: 完成正则表达式的编译功能！
 *
 * \note 
 *
 * \author fengyu.guo
 *
 * \version 1.0
 *
 * \date 四月 2016
 *
 * Contact: user@company.com
 *
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

