#include "stdafx.h"
#include "DFA.h"
#include <queue>
#include <exception>
#include <sstream>
using namespace std;

DFA::DFA() :_start(-1)
{
}


DFA::DFA(istream& is)
{
	string str;
	getline(is, str);
	istringstream stm(str);
	stm >> _start;
	getline(is, str);
	stm.str(str);
	stm.seekg(0);
	int acc;
	while (stm>>acc)
	{
		addAccept(acc);
	}
	getline(is, str);
	addCharSet(str);
	int i, f;
	char c;
	while (is>>i>>c>>f)
	{
		if (c=='@')
		{
			c = '\0';
		}
		addTrans(i, c, f);
	}
}

void DFA::print(ostream &os)
{
	os << _start << endl;
	for (auto i : _accept)
	{
		os << i << " ";
	}
	os << endl;
	os << getCharSet() << endl;
	for (auto status:_trans)
	{
		for (auto ch: status.second)
		{
			os << status.first << " " << ch.first << " " << ch.second << endl;
		}
	}
}

DFA::~DFA()
{

}

bool DFA::setStart(int st)
{
	_start = st;
	return true;
}

int DFA::getStart() const
{
	return _start;
}

bool DFA::addAccept(int st)
{
	_accept.insert(st);
	return true;
}

bool DFA::removeAccept(int st)
{
	_accept.erase(st);
	return true;
}

void DFA::addTrans(int i, char c, int f)
{
	_status.insert(i);
	_status.insert(f);
	if (_trans.find(i)!=_trans.end())
	{
		if (_trans.at(i).find(c)==_trans.at(i).end())
		{
			_trans[i][c] = f;
		}
		else
		{
			string str;
			ostringstream stm;
			stm << "Redefinition of status " << i << " char " << c << ".";
			str = stm.str();
			throw exception(str.c_str());
		}
	}
	else
	{
		_trans[i] = trans_type( { { c, f } });
	}
}

int DFA::getTrans(int i, char c) const
{
	if (_trans.find(i)==_trans.end())
	{
		return -1;
	}
	else
	{
		if (_trans.at(i).find(c)==_trans.at(i).end())
		{
			return -1;
		}
		else
		{
			return _trans.at(i).at(c);
		}
	}
}

int DFA::maxStatus() const
{
	return *_status.rbegin();
}

bool DFA::testString(const string& str)
{
	return accept(processString(str));
}

int DFA::processString(const string& str)
{
	int status = _start;
	for (auto c : str)
	{
		status = getTrans(status, c);
		if (status < 0)
		{
			return status;
		}
	}
	return status;
}

void DFA::reset()
{
	_start = -1;
	_accept.clear();
	_trans.clear();
	_char.clear();
}

string DFA::getCharSet() const
{
	ostringstream stm;
	for (auto c:_char)
	{
		stm << c;
	}
	return stm.str();
}

bool DFA::accept(int status)
{
	return _accept.find(status)!= _accept.end();
}

void DFA::addCharSet(const string& str)
{
	for (auto c:str)
	{
		_char.insert(c);
	}
}

void DFA::addCharSet(char c)
{
	_char.insert(c);
}
