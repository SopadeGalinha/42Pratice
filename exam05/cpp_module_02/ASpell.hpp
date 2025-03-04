#ifndef ASPELL
#define ASPELL

#include <string>
#include <iostream>
#include "ATarget.hpp"

using namespace std;
class ATarget;

class ASpell
{
protected:
	string _name;
	string _effects;

public:
	ASpell();
	ASpell(const ASpell &other);
	ASpell &operator=(const ASpell &other);
	ASpell(const string &name, const string &effects);

	const string &getName() const;
	const string &getEffects() const;

	virtual ~ASpell();
	virtual ASpell *clone() const = 0;

	void launch(const ATarget &target) const;
};

#endif