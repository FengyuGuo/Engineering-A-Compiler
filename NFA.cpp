#include "stdafx.h"
#include "NFA.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <queue>
using namespace std;
NFA::NFA() :_start(-1)
{
}

NFA::NFA(istream& is)
{
	string str;
	getline(is, str);
	istringstream stm(str);
	stm >> _start;
	getline(is, str);
	stm.str(str);
	int st;
	stm.seekg(0);
	while (stm >> st)
	{
		addAccept(st);
	}
	getline(is, str);
	addCharSet(str);
	int i, f;
	char c;
	while (is >> i >> c >> f)
	{
		if (c == '@') c = '\0';
		addTrans(i, c, f);
	}
}

NFA::~NFA()
{
}

NFA::set_type NFA::getTrans(int istatus, char cond) const
{
	if (existMap(istatus, cond))
	{
		return _transMap.at(istatus).at(cond);
	}
	else
	{
		return{ -1 };
	}
}

void NFA::addTrans(int istatus, char cond, int fstatus)
{
	_statusSet.insert(istatus);
	_statusSet.insert(fstatus);
	if (existMap(istatus))
	{
		if (existMap(istatus, cond))
		{
			_transMap[istatus][cond].insert(fstatus);
		}
		else
		{
			_transMap[istatus][cond] = { fstatus };
		}
	}
	else
	{
		_transMap[istatus] = trans_type({ { cond, { fstatus } } });
	}
}

void NFA::print(ostream& os) const
{
	using std::endl;
	//using std::for_each;
	os << _start << endl;;
	for (auto s : _accept)
	{
		os << s << " ";
	}
	os << endl;
	os << getCharset() << endl;
	for (auto status : _transMap)
	{
		for (auto cond : status.second)
		{
			//os << status.first << " " << cond.first << " " << _transMap[status.first][cond.first] << endl;
			for (auto f : cond.second)
			{
				os << status.first << " " << cond.first << " " << f << endl;
			}
		}
	}
}

int NFA::getStart() const
{
	return _start;
}

bool NFA::setStart(int st)
{
	_start = st;
	return true;
}

NFA::set_type NFA::getAcceptSet() const
{
	return _accept;
}

bool NFA::acceptSet(set_type set) const
{
	for (auto i : set)
	{
		if (_accept.find(i) != _accept.end())
		{
			return true;
		}
	}
	return false;
}

bool NFA::addAccept(int st)
{
	_accept.insert(st);
	return true;
}

bool NFA::removeAccept(int st)
{
	_accept.erase(st);
	return true;
}

bool NFA::clearAccept()
{
	_accept.clear();
	return true;
}

int NFA::maxStatus() const
{
	return *_statusSet.rbegin();
}

bool NFA::testString(const string& str) const
{
	return acceptSet(processString(str));
}

NFA::set_type NFA::processString(const string& str) const
{
	set_type set = { _start };
	eClosure(set);
	for (auto c : str)
	{
		set = DTrans(set, c);
		if (set.empty())
		{
			return set;
		}
	}
	return set;
}

NFA::set_type NFA::eClosure(int status) const
{
	set_type ret = { status };
	eClosure(ret);
	return ret;
}

void NFA::eClosure(set_type& statusSet) const
{
	queue<int> q;
	for (auto st : statusSet)
	{
		q.push(st);
	}
	int status;
	set_type tempSet;
	while (!q.empty())
	{
		status = q.front();
		q.pop();
		tempSet = getTrans(status, '\0');
		for (auto st : tempSet)
		{
			if (st != -1 && statusSet.find(st) == statusSet.end())
			{
				q.push(st);
				statusSet.insert(st);
			}
		}
	}
}

NFA::set_type NFA::DTrans(set_type& statusSet, char c) const
{
	set_type ret, tempSet;
	queue<int> q;
	for (auto i : statusSet)
	{
		q.push(i);
	}
	int iStatus;
	while (!q.empty())
	{
		iStatus = q.front();
		q.pop();
		tempSet = getTrans(iStatus, c);
		if (tempSet.empty() || tempSet.find(-1) != tempSet.end())
		{
			continue;
		}
		eClosure(tempSet);
		for (auto i : tempSet)
		{
			if (ret.find(i) == ret.end())
			{
				ret.insert(i);
			}
		}
	}
	return ret;
}

void NFA::reset()
{
	_start = -1;
	_statusSet.clear();
	_transMap.clear();
	_accept.clear();
	_char.clear();
}

bool NFA::empty() const
{
	return _transMap.empty();
}

void NFA::addCharSet(char c)
{
	_char.insert(c);
}

void NFA::addCharSet(const string& str)
{
	for (auto c : str)
	{
		_char.insert(c);
	}
}

string NFA::getCharset() const
{
	ostringstream stm;
	for (auto c : _char)
	{
		stm << c;
	}
	return stm.str();
}

void NFA::add(const NFA& nfa)
{
	if (nfa.empty())return;
	ostringstream os;
	nfa.print(os);
	string nfaStr = os.str();
	processAdder(nfaStr, nfa);
	istringstream is(nfaStr);
	string tempStr;
	getline(is, tempStr);//start status. No special usage.
	getline(is, tempStr);//accept status
	vector<int> acc = parseAcc(tempStr);
	clearAccept();
	for (auto i : acc)
	{
		addAccept(i);
	}
	getline(is, tempStr);//char set
	for (auto c : tempStr)
	{
		addCharSet(c);
	}
	int i, f;
	char c;
	while (is >> i >> c >> f)
	{
		addTrans(i, c, f);
	}
}

void NFA::multiple(const NFA& nfa)
{
	if (nfa.empty()) return;
	ostringstream os;
	nfa.print(os);
	string strNFA = os.str();
	processMulter(strNFA, nfa);
	istringstream is(strNFA);
	string strTemp;
	int iStart;
	is >> iStart;
	addTrans(getStart(), '\0', iStart);
	getline(is, strTemp);//line after start
	getline(is, strTemp);//accept line
	istringstream is2;
	is2.str(strTemp);
	int iFinal;
	while (is2 >> iFinal)
	{
		addAccept(iFinal);
	}
	getline(is, strTemp);
	for (auto c : strTemp)
	{
		addCharSet(c);
	}
	int i, f;
	char c;
	while (is >> i >> c >> f)
	{
		addTrans(i, c, f);
	}
}

void NFA::star()
{
	int iStart = getStart();
	for (auto ac : getAcceptSet())
	{
		addTrans(ac, '\0', iStart);
		addTrans(iStart, '\0', ac);
	}
}

bool NFA::existMap(int status, char cond) const
{
	if (_transMap.find(status) == _transMap.end())
	{
		return false;
	}
	else
	{
		if (_transMap.at(status).find(cond) == _transMap.at(status).end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool NFA::existMap(int status) const
{
	if (_transMap.find(status) == _transMap.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void NFA::offsNFAStatus(string& str, int offset)
{
	istringstream is1(str), is2;
	ostringstream os;
	int start, final;
	int i, f;
	char c;
	is1 >> start;
	os << start + offset << endl;
	string temp;
	getline(is1, temp);//line after start
	getline(is1, temp);//accept line
	is2.str(temp);
	while (is2 >> final)
	{
		os << final + offset << " ";
	}
	os << endl;

	getline(is1, temp);//char set line
	os << temp << endl;

	while (is1 >> i >> c >> f)
	{
		os << i + offset << " " << c << " " << f + offset << endl;
	}
	str = os.str();
}

vector<int> NFA::parseAcc(const string& str)
{
	istringstream is(str);
	int i;
	vector<int> ret;
	while (is >> i)
	{
		ret.push_back(i);
	}
	return ret;
}

void NFA::subsNFAStart(string& str, set_type statusSet)
{
	istringstream is(str);
	ostringstream os;
	int start;
	is >> start;
	os << start << endl;
	string strTemp;
	getline(is, strTemp);//line after start
	getline(is, strTemp);//accept line
	//os << strTemp << endl;
	istringstream is2(strTemp);
	int ifinal;
	while (is2 >> ifinal)
	{
		os << ifinal - 1 << " ";
	}
	os << endl;
	getline(is, strTemp);//char set line
	os << strTemp << endl;
	int i, f;
	char c;
	while (is >> i >> c >> f)
	{
		if (i == start)
		{
			for (auto st : statusSet)
			{
				os << st << " " << c << " " << f - 1 << endl;
			}
		}
		else
		{
			os << i - 1 << " " << c << " " << f - 1 << endl;
		}
	}
	str = os.str();
}

void NFA::processAdder(string& str, const NFA& nfa)
{
	offsNFAStatus(str, maxStatus() + 1);
	subsNFAStart(str, getAcceptSet());
}

void NFA::processMulter(string& str, const NFA& nfa)
{
	offsNFAStatus(str, maxStatus() + 1);
}