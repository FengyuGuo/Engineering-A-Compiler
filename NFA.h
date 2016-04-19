#pragma once
#include <unordered_map>
#include <iostream>
#include <set>
#include <unordered_set>
using namespace std;
/*!
 * \class NFA
 *
 * \brief 不确定有限状态自动机
 *
 * \author fengyu.guo
 * \date 四月 2016
 */
class NFA
{
public:
	typedef unordered_set<int> set_type;
	typedef unordered_map<char, set_type> trans_type;
	typedef unordered_map<int, trans_type> map_type;
	typedef set<int> status_set;
	typedef set<char> char_set;

	NFA();
	~NFA();
	NFA(istream& is);

	set_type getTrans(int istatus, char cond) const;
	void addTrans(int istatus, char cond, int fstatus);

	void print(ostream& os) const;

	int getStart() const;
	bool setStart(int st);

	set_type getAcceptSet() const;
	bool acceptSet(set_type set) const;
	bool addAccept(int st);
	bool removeAccept(int st);

	int maxStatus() const;

	bool testString(const string& str) const;
	set_type processString(const string& str) const;

	set_type eClosure(int status) const;
	void eClosure(set_type& statusSet) const;
	set_type DTrans(set_type& statusSet, char c) const;

	void reset();
	bool empty() const;

	void addCharSet(char c);
	void addCharSet(const string& str);
	string getCharset()const;

	void add(const NFA& nfa);
	void multiple(const NFA& nfa);
	void star();
private:
	status_set _statusSet;
	map_type _transMap;
	int _start;
	set_type _accept;
	char_set _char;

	bool existMap(int status, char cond) const;
	bool existMap(int status) const;	
	void processAdder(string& str, const NFA& nfa);
	void processMulter(string& str, const NFA& nfa);

	bool clearAccept();

public:
	static void offsNFAStatus(string& str, int offset);
	static vector<int> parseAcc(const string& str);
	static void subsNFAStart(string& str, set_type statusSet);
	//status_set _status;
};