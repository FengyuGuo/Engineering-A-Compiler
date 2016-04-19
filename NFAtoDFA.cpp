#include "stdafx.h"
#include "NFAtoDFA.h"
#include <unordered_map>
#include <string>
#include <queue>
#include <sstream>
#include "DFA.h"
using namespace std;

NFAtoDFA::NFAtoDFA() :_nfa(nullptr), _dfa(nullptr)
{
}

NFAtoDFA::~NFAtoDFA()
{
}

bool NFAtoDFA::convert(DFA& dfa, NFA& nfa)
{
	_nfa = &nfa; _dfa = &dfa;
	dfa.reset(); reset();
	if (nfa.empty()) return true;
	dfa.addCharSet(nfa.getCharset());
	string chSet = dfa.getCharSet();
	int iStatus = 0;
	setLen(nfa.maxStatus());
	int startStatus, finalStatus;
	queue<index_type> q;
	NFA::set_type startSet = { nfa.getStart() }, finalSet;
	nfa.eClosure(startSet);
	index_type startIndex = setToIndex(startSet), finalIndex;
	q.push(startIndex);
	dfa.setStart(iStatus);
	insertIndexStatus(startIndex, iStatus++);
	while (!q.empty())
	{
		startIndex = q.front();
		q.pop();
		startSet = indexToSet(startIndex);
		startStatus = getStatus(startIndex);
		for (auto c : chSet)
		{
			finalSet = nfa.DTrans(startSet, c);
			if (finalSet.empty()) continue;
			finalIndex = setToIndex(finalSet);
			if (!alreadyFound(finalIndex))
			{
				insertIndexStatus(finalIndex, iStatus++);
				q.push(finalIndex);
			}
			finalStatus = getStatus(finalIndex);
			dfa.addTrans(startStatus, c, finalStatus);
			if (nfa.acceptSet(finalSet))dfa.addAccept(finalStatus);
		}
	}
	return true;
}

bool NFAtoDFA::reset()
{
	_Len = 0;
	_indexMap.clear();
	_statusMap.clear();
	_nfa = nullptr;
	_dfa = nullptr;
	return true;
}

void NFAtoDFA::printStatus(ostream& os, int status) const
{
	const index_type& id = _statusMap.at(status);
	size_t sz = 8 * sizeof(index_type::value_type);
	size_t j, i;
	size_t len = id.size();
	os << "{" << " ";
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < sz; j++)
		{
			if ((id[i] & (0x01 << j)) != 0)
			{
				os << i * 8 + j << " ";
			}
		}
	}
	os << "}";
}

void NFAtoDFA::printDFA(ostream& os)
{
	ostringstream sos;
	_dfa->print(sos);
	string str = sos.str();
	string strTemp;
	istringstream is(str);
	getline(is, strTemp);
	os << strTemp << endl;
	getline(is, strTemp);
	os << strTemp << endl;
	getline(is, strTemp);
	os << strTemp << endl;
	int i, f;
	char c;
	while (is >> i >> c >> f)
	{
		os << i << " ";
		printStatus(os, i);
		os << " --> " << c << " --> " << f << " ";
		printStatus(os, f);
		os << endl;
	}
}

void NFAtoDFA::setLen(int l)
{
	_Len = l;
}

int NFAtoDFA::getLen()
{
	return _Len;
}

NFAtoDFA::index_type NFAtoDFA::setToIndex(const NFA::set_type& set)
{
	index_type ret;
	ret.resize(getLen() / 32 + 1);
	for (auto st : set)
	{
		ret[st / 32] |= (0x01 << (st % 32));
	}
	return ret;
}

NFA::set_type NFAtoDFA::indexToSet(const index_type& index)
{
	NFA::set_type ret; int len = index.size();
	int sz = 8 * sizeof(index_type::value_type);
	int i, j;
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < sz; j++)
		{
			if ((index[i] & (0x01 << j)) != 0)
			{
				ret.insert(i * 8 + j);
			}
		}
	}
	return ret;
}

bool NFAtoDFA::insertIndexStatus(const index_type& index, int status)
{
	_indexMap[index] = status;
	_statusMap[status] = index;
	return true;
}

bool NFAtoDFA::alreadyFound(const index_type& index)
{
	if (_indexMap.find(index) == _indexMap.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

int NFAtoDFA::getStatus(const index_type& index) const
{
	return _indexMap.at(index);
}