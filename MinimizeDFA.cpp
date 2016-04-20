#include "stdafx.h"
#include "MinimizeDFA.h"
#include <map>
using namespace std;
MinimizeDFA::MinimizeDFA()
{
}


MinimizeDFA::~MinimizeDFA()
{
}

DFA MinimizeDFA::minimize(const DFA& dfa)
{
	DFA ret;
	_dfa = dfa;
	_cSet = dfa.getCharSet();
	DFA::status_set statusSet1 = dfa.getAccept();
	DFA::status_set statusSet2 = _minusSet(dfa.getStatusSet(), statusSet1);
	set_set_type T;
	_nDFASet.clear();
	T.insert(statusSet1); T.insert(statusSet2);
	while (T != _nDFASet)
	{
		_nDFASet = T;
		T.clear();
		for (auto st:_nDFASet)
		{
			if (_splitSet(st, statusSet1, statusSet2))
			{
				T.insert(statusSet1);
				T.insert(statusSet2);
			}
			else
			{
				T.insert(st);
			}
		}
	}
	_setSetToDFA(_nDFASet, ret);
	return ret;
}

void MinimizeDFA::printConversion(ostream& os)
{
	if (_nDFASet.empty())
	{
		os << "Empty conversion!" << endl;
	}
	for (auto &st:_nDFASet)
	{
		os << "{ ";
		for (auto &status:st)
		{
			os << status << " ";
		}
		os << "} -> ";
		os << _inToNDFAMap.at(*(st.begin())) << endl;
	}
}

bool MinimizeDFA::_splitSet(const DFA::status_set& in, DFA::status_set& out1, DFA::status_set& out2)
{
	if (in.size()<=1)
	{
		return false;
	}
	out1.clear();
	out2.clear();
	bool split = false;
	DFA::status_type status, tempStatus;
	char ch;
	for (auto c:_cSet)
	{
		status = _dfa.getTrans(*(in.begin()), c);
		for (auto st:in)
		{
			if (_dfa.getTrans(st, c) != status)
			{
				split = true;
				ch = c;
				break;
			}
		}
		if (split)
		{
			break;
		}
	}
	if (split)
	{
		for (auto i:in)
		{
			tempStatus = _dfa.getTrans(i, ch);
			if (tempStatus == status)
			{
				out1.insert(i);
			}
			else
			{
				out2.insert(i);
			}
		}
		return true;
	}
	return false;
}

DFA::status_set MinimizeDFA::_minusSet(const DFA::status_set& lhs, const DFA::status_set& rhs)
{
	DFA::status_set ret;
	for (auto i:lhs)
	{
		if (rhs.find(i)==rhs.end())
		{
			ret.insert(i);
		}
	}
	return ret;
}

bool MinimizeDFA::_setSetToDFA(const set_set_type& st, DFA& out)
{
	if (st.empty()) return false;
	out.reset();
	int newStart = 0;
	int newFinal = 0;
	DFA::status_type start, final;
	_inToNDFAMap.clear();
	int i = 1;
	for (auto &statusSet:st)
	{
		_inToNDFAMap[*(statusSet.begin())] = i++;
	}
	for (auto &statusSet:st)
	{
		start = *(statusSet.begin());
		newStart = _inToNDFAMap[start];
		for (auto c:_cSet)
		{
			final = _dfa.getTrans(start, c);
			if (final!= -1)
			{
				newFinal = _inToNDFAMap.at(*(_findStatus(final, st)->begin()));
				out.addTrans(newStart, c, newFinal);
			}
		}
	}
	DFA::status_type newAcc, acc;
	acc = *(_dfa.getAccept().begin());
	newAcc = _inToNDFAMap[*(_findStatus(acc,st)->begin())];
	out.addAccept(newAcc);
	DFA::status_type sta = _dfa.getStart();
	DFA::status_type newSta = _inToNDFAMap[*(_findStatus(sta, st)->begin())];
	out.setStart(newSta);
	out.addCharSet(_cSet);
	return true;
}

MinimizeDFA::set_iterator MinimizeDFA::_findStatus(DFA::status_type stts, const set_set_type& st)
{
	set_set_type::iterator ret = st.begin();
	set_set_type::iterator end = st.end();
	while (ret != end)
	{
		if (ret->find(stts) != ret->end())
		{
			return ret;
		}
		ret++;
	}
	return end;
}
