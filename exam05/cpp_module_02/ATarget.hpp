#ifndef ATARGET
#define ATARGET

#include <string>
#include <iostream>
#include "ASpell.hpp"

using namespace std;

class ASpell;

class ATarget
{
protected:
	string _type;
public:
	ATarget();
	ATarget(const ATarget &other);
	ATarget &operator=(const ATarget &other);
	ATarget(const string &type);

	const string &getType() const;

	virtual ~ATarget();
	virtual ATarget *clone() const = 0;

	void getHitBySpell(const ASpell &spell) const;
};

#endif