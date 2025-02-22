#ifndef WARLOCK
#define WARLOCK

#include <string>
#include <iostream>
#include "ASpell.hpp"
#include "ATarget.hpp"
#include <map>

using namespace std;

class Warlock
{
	string _name;
	string _title;
	map<string, ASpell *> _book;

	Warlock();
	Warlock(const Warlock &other);
	Warlock &operator=(const Warlock &other);

public:
	~Warlock();
	Warlock(const string &name, const string &title);

	const string &getName() const;
	const string &getTitle() const;
	void setTitle(const string &title);
	void introduce() const;

	void learnSpell(ASpell *spell);
	void forgetSpell(string spell);
	void launchSpell(string spell, ATarget &target);
};

#endif