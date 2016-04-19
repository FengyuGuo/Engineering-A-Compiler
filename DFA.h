#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
using namespace std;
class DFA
{
public:
	typedef unordered_map<char, int> trans_type;
	typedef unordered_map<int, trans_type> map_type;
	typedef set<int> status_set;
	typedef set<char> char_set;

	DFA();
	~DFA();
	DFA(istream& is);

	void print(ostream &os);

	bool setStart(int st);
	int getStart() const;

	bool addAccept(int st);
	bool removeAccept(int st);

	void addTrans(int i, char c, int f);
	int getTrans(int i, char c) const;

	int maxStatus() const;

	bool testString(const string& str);
	int processString(const string& str);

	void reset();

	string getCharSet()const;
	void addCharSet(const string& str);
	void addCharSet(char c);
private:
	int _start;
	status_set _accept;
	status_set _status;
	map_type _trans;
	char_set _char;
	bool accept(int status);
};
