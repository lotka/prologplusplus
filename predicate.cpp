#include "prolog++.h"

PredicateRef::PredicateRef(string input, vector<Predicate *> &predicates)
{
	int commas 	= countChar(input,',');
	// left brackets
	int lb 			= countChar(input,'(');
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
				if((*atom_ptr).name==(*args[i]).name)
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
			if(not args[i]->unifyWith(ref->args[i]))
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
