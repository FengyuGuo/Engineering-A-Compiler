#pragma once
#include <map>
#include "NFA.h"
#include "DFA.h"
#include <vector>
#include <iostream>
using namespace std;
class NFAtoDFA
{
public:
	typedef vector<unsigned> index_type;
	typedef map<index_type, int> index_map_type;
	typedef map<int, index_type> status_map_type;

	NFAtoDFA();
	~NFAtoDFA();
	bool convert(DFA& dfa, NFA& nfa);

	bool reset();

	void printStatus(ostream& os, int status) const;
	void printDFA(ostream& os) ;
private:
	void setLen(int l);
	int getLen();
	int _Len;

	NFA* _nfa;
	DFA* _dfa;

	index_type setToIndex(const NFA::set_type& set);
	NFA::set_type indexToSet(const index_type& index);
	index_map_type _indexMap;
	status_map_type _statusMap;

	bool insertIndexStatus(const index_type& index, int status);
	bool alreadyFound(const index_type& index);

	int getStatus(const index_type& index) const;

};

