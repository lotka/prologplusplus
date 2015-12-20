#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>

using namespace std;
string cropString(string &input_string, int n);
string cropString(string &input_string, char delimiter);
int countChar(string s, char c);
int countChar(string s, char c, char delimiter);

class Atom;
class PredicateReference;
class Predicate;


class Atom
{
public:
	Atom(string n);
	string name;
	bool free_variable;

	friend ostream &operator<<(ostream &out, Atom &c)     //output
	{
	        cout << c.name;
	}

	bool operator == (Atom &b)
	{
		if(this->name == b.name and this->free_variable == b.free_variable)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

class PredicateRef
{
public:
	string name;
	PredicateRef(string input, vector<Predicate *> &predicates);
	vector<Atom *> args;
	int airty;
	friend ostream &operator<<(ostream &out, PredicateRef &c)
	{
	        cout << c.name;
	}
};

class Predicate
{
public:
	Predicate(string input, vector<Predicate *> &predicates);
	bool unifyWith(PredicateRef * ref);
	string name;
	int airty;
	vector<Atom *> args;
	vector<PredicateRef *> conditions;

	#define AND 1
	#define OR 	2
	#define IMPLIES 3
	#define END 4
	vector<int> connectives;
	Predicate * next;

};