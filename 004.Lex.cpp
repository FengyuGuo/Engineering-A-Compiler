// 004.Lex.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>

#include "NFA.h"
#include "DFA.h"
#include "NFAtoDFA.h"
#include "MinimizeDFA.h"
using namespace std;

void print_set(ostream& os, NFA::set_type set)
{
	os << "{ ";
	for (auto i:set)
	{
		os << i << " ";
	}
	os << "}" << endl;
}

void debugNFA(void)
{
	ifstream in;
	in.open("NFA.txt");
	NFA nfa(in);
	nfa.print(cout);
	cout << "**************" << endl;
	cout << nfa.maxStatus() << endl;
	cout << "**************" << endl;
	DFA dfa;
	NFAtoDFA con;
	con.convert(dfa, nfa);
	con.printDFA(cout);
	string testString;
	while (getline(cin, testString))
	{
		print_set(cout, nfa.processString(testString));
	}
}

void debugDFA(void)
{
	ifstream in;
	in.open("DFA.txt");
	DFA dfa(in);
	dfa.print(cout);
	cout << "****************" << endl;
	MinimizeDFA md;
	DFA nDfa = md.minimize(dfa);
	nDfa.print(cout);
	cout << "****************" << endl;
	string testString;
	while (getline(cin, testString))
	{
		cout << dfa.processString(testString) << endl;
	}
}

void bitSetTest(void)
{
	int i = 32;
	//bitset<i> bs;
	bitset<3> bs2;

}

int _tmain(int argc, _TCHAR* argv[])
{
	debugDFA();
	system("pause");
	return 0;
}

