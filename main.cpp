#include "prolog++.h"


vector<Predicate *> predicates;
void consult_test(const char * s)
{
	cout << s << endl;
	predicates.push_back(	new Predicate(s, predicates));
}

void query_test(string s)
{
	if(s == "halt.")
	{
		exit(1);
	}
	PredicateRef p(s,predicates);
	bool unified = false;

	cout << "test" << endl;
	// Basically what happens here is that we try to unify the ref with all
	// the existing predicates, if one works set the unified flag to true
	for(int i = 0; i < predicates.size(); ++i)
	{
		if(predicates[i]->unifyWith(&p))
		{
			unified = true;
		}
	}
	// If unified, print out the variables that worked, these are stored inside the atoms
	// in a list of possibilites, this is potentially a really bad way to do this
	// I think this is going to need some kind of virtual/temporary variables to make this work
	if(unified)
	{
		for(int i = 0; i < p.args.size(); ++i)
		{
			char c;
			for(int j = 0; j < p.args[i]->possibilties.size(); ++j)
			{
				cout << *p.args[i] << "=" << *p.args[i]->possibilties[j] << endl;
			}
			cin >> c;
		}
		cout << endl << "yes" << endl;
	}
	else
	{
		cout << endl << "no" << endl;
	}
}


int main()
{
	cout << "Prolog++" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "Preloaded facts: " << endl << endl;
	consult_test("equal(X,X).");
	consult_test("basic(a,b,c).");
	consult_test("person(alice).");
	consult_test("person(bob).");
	cout << endl;

	while(not cin.eof())
	{
		string input;
		cout << "| ?- ";
		cin >> input;
		query_test(input);
	}
}
