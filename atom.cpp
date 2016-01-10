#include "prolog++.h"

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
	if(n.size() == 1 and n[0] == '_')
	{
		cout << "UNDERSCORE DETECTED" << endl;
		is_under_score = true;
	}
}

bool Atom::unifyWith(Atom * a)
{
  if(a->free_variable)
  {
    if(not free_variable)
    {
      a->possibilties.push_back(this);
    }
    else
    {
      cout << "not sure what to do here" << endl;
    }
    return true;
  }
  else if(*a == *this)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// check two atoms are equal
bool Atom::operator == (Atom &b)
{
  if(b.free_variable or free_variable)
  {
    return true;
  }
  if(this->name == b.name)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// check two atoms are not equal
bool Atom::operator != (Atom &b)
{
  if(b.free_variable or free_variable)
  {
    return false;
  }
  if(this->name != b.name)
  {
    return true;
  }
  else
  {
    return false;
  }
}
