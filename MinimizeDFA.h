#pragma once
#include "DFA.h"
#include <set>
#include <string>
#include <map>
#include <iostream>
using namespace std;
class MinimizeDFA
{
public:
	typedef set<DFA::status_set> set_set_type;
	typedef set<DFA::status_set>::iterator set_iterator;
	typedef DFA::status_set status_set;

	MinimizeDFA();
	~MinimizeDFA();

	DFA minimize(const DFA& dfa);

	void printConversion(ostream& os);
private:
	DFA _dfa;
	string _cSet;
	bool _splitSet(const DFA::status_set& in, DFA::status_set& out1, DFA::status_set& out2);
	DFA::status_set _minusSet(const DFA::status_set& lhs, const DFA::status_set& rhs);
	bool _setSetToDFA(const set_set_type& st, DFA& out);
	set_iterator _findStatus(DFA::status_type stts, const set_set_type& st);

	set_set_type _nDFASet;
	map<int, int> _inToNDFAMap;
};