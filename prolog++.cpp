#include "prolog++.h"

string strip_ws(string s)
{
	string new_s;
	for(int i = 0; i < s.size(); ++i)
	{
		if(s[i]!=' ')
		{
			new_s.push_back(s[i]);
		}
	}
	return new_s;
}

Atom::Atom(string n)
{
	name = n;
	if('A' <= n[0] and n[0] <= 'Z')
	{
		free_variable = true;
	}
	else
	{
		free_variable = false;
	}
}

PredicateRef::PredicateRef(string input, vector<Predicate *> &predicates)
{
	int commas 	= countChar(input,',');
	// left brackets
	int lb 		= countChar(input,'(');
	// right brackets
	int rb	    = countChar(input,')');
	// dots
	int dot 	= countChar(input,'.');
	name = cropString(input,'(');
	// Add the airty to the name
	name = name + "\\0";
	airty = commas+lb;
	name[name.size()-1] = airty+'0';
	//cout << "debug_info : " << name << endl;

	bool exists = false;
	for(int i = 0; i < predicates.size(); ++i)
	{
		if(predicates[i]->name == name and predicates[i]->airty == airty)
		{
			exists = true;
		}
	}
	if(not exists)
	{
		cout << "ERROR in locating " << name << endl;
	}

	for(int i = 0; i < airty-1; ++i)
	{
		Atom * atom_ptr = new Atom(cropString(input,','));
		// for free variables, check for existing instance and point to that
		if(atom_ptr->free_variable)
		{
			for(int i = 0; i < args.size(); ++i)
			{
				if((*atom_ptr)==(*args[i]))
				{
					atom_ptr=args[i];
					break;
				}
			}
		}
		args.push_back(atom_ptr);
	}

	args.push_back(new Atom(cropString(input,')')));

	//cout << "debug_info : ";
	// for(int i = 0; i < airty; ++i)
	// {
	// 	cout << *args[i] << " ";
	// } cout << endl;

}

Predicate::Predicate(string s, vector<Predicate *> &predicates)
{
	s = strip_ws(s);
	// Count up to ":-"
	// commas
	int commas 	= 0;
	// left brackets
	int lb 		= 0;
	// right brackets
	int rb	    = 0;
	// conditionals
	int cond 	= 0;
	// dots
	int dot 	= 0;
	for(int i = 0; i < s.size(); ++i)
	{
		if(s[i] == ':')
		{
			if(s[i+1]=='-')
			{
				break;
			}
			else
			{
				cout << "Error: : followed by " << s[i+1] << endl;
				return;
			}
			cond = i + 2;
		}
		switch(s[i])
		{
			case ',':
				++commas;
				break;

			case '(':
				++lb;
				break;

			case ')':
				++rb;
				break;

			case '.':
				++dot;
				break;
		}
	}
	if(lb > 0)
	{
		name = cropString(s,'(');	
	}
	else if(dot > 0)
	{
		name = cropString(s,'.');
	}
	else if(cond > 0)
	{
		name = cropString(s,':');
	}

	airty = commas + lb;

	// Add the airty to the name
	name = name + "\\0";
	name[name.size()-1] = '0'+airty;

	vector<string> raw_args;
	vector<string> raw_conditions;
	// Do the processing
	for(int i = 0; i < airty; ++i)
	{
		char delimiter;
		if(i != commas)
		{
			delimiter = ',';
		}
		else
		{
			delimiter = ')';
		}
		raw_args.push_back(cropString(s,delimiter));
	}
	cropString(s,'-');

	// cout << "name : " << name << endl; 
	// cout << "Arguments : ";
 //  	for (unsigned int i = 0; i < raw_args.size(); ++i)
	// {
	// 	cout << raw_args[i] << ' ';
	// }
	if(s.size()>0)
	{
		for(int i = 0; i < s.size(); ++i)
		{
			if(s[i]==')' and s[i+1]==',')
			{
				raw_conditions.push_back(cropString(s,i+1));
				connectives.push_back(AND);
			}
			else if(s[i]==')' and s[i+1]==';')
			{
				raw_conditions.push_back(cropString(s,';'));
				connectives.push_back(OR);
			}
			else if(s[i]=='-' and s[i+1]=='>')
			{
				raw_conditions.push_back(cropString(s,'-'));
				cropString(s,'>');
				connectives.push_back(IMPLIES);
			}
			else if(s[i] == ')' and s[i+1] == '.')
			{
				raw_conditions.push_back(cropString(s,'.'));
				connectives.push_back(END);
				break;
			}
			else if(s[i] == '.')
			{
				raw_conditions.push_back(cropString(s,'.'));
				connectives.push_back(END);
				break;
			}
		}
	// 	cout << endl << "Conditions : "; 
	//   	for (unsigned int i = 0; i < raw_conditions.size(); ++i)
	// 	{
	// 		cout << raw_conditions[i];
	// 		if(connectives[i] == AND)
	// 		{
	// 			cout << " and ";
	// 		}
	// 	}
	}
	// cout << endl;

	// Parsing finished.

	for(int i = 0; i < raw_args.size(); ++i)
	{
		Atom * a = new Atom(raw_args[i]);
		args.push_back(a);
	}

	for(int i = 0; i < raw_conditions.size(); ++i)
	{
		conditions.push_back(new PredicateRef(raw_conditions[i],predicates));
	}
}

bool Predicate::unifyWith(PredicateRef * ref)
{
	if(ref->name == name and ref->airty == airty)
	{
		for(int i = 0; i < airty; ++i)
		{
			if(args[i]->free_variable)
			{
				cout << "panic!" << endl;
			}
			if(not (*args[i] == *ref->args[i]))
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

string cropString(string &input_string, int n)
{
	string crop;
	int i = 0;
	for(i = 0; i < input_string.size(); ++i)
	{
		if(i == n)
		{
			break;
		}
		else
		{
			crop.push_back(input_string[i]);
		}
	}
	input_string.erase(0,i+1);
	return crop;
}

string cropString(string &input_string, char delimiter)
{
	string crop;
	int i = 0;
	for(i = 0;i < input_string.size(); ++i)
	{
		if(input_string[i] == delimiter)
		{
			break;
		}
		else
		{
			crop.push_back(input_string[i]);
		}
	}
	input_string.erase(0,i+1);
	return crop;
}

int countChar(string s, char c)
{
	int count = 0;
	for(int i = 0; i < s.size(); ++i)
	{
		if(s[i]==c)
		{
			++count;
		}
	}
	return count;
}

int countChar(string s, char c, char delimiter)
{
	int count = 0;
	for(int i = 0; i < s.size(); ++i)
	{
		if(s[i]==delimiter)
		{
			return count;
		}
		if(s[i]==c)
		{
			++count;
		}
	}
	return count;
}

vector<Predicate *> predicates;
void consult_test(const char * s)
{
	// cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << s << endl;
	// cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	predicates.push_back(	new Predicate(s, predicates));
	// cout << endl << endl;
}

void query_test(string s)
{
	PredicateRef p(s,predicates);
	bool unified = false;
	for(int i = 0; i < predicates.size(); ++i)
	{
		if(predicates[i]->unifyWith(&p))
		{
			unified = true;
		}
	}
	if(unified)
	{
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
	consult_test("| equal(theThing,theThing).");
	consult_test("| basic(a,b,c).");
	cout << endl;
	// consult_test("good(X,Y) :- equal(X,X),equal(Y,Y).");
	// consult_test("equal_test(X,Y) :- equal(X,Y).");

	// cout << "| ?- ";
	// query_test("equal(theThing,theThing).");


	while(not cin.eof())
	{
		string input;
		cout << "| ?- ";
		cin >> input;
		query_test(input);
	}
}