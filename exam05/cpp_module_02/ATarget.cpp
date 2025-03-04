#include "ATarget.hpp"

ATarget::ATarget() : _type("default") {}

ATarget::ATarget(const string &type) : _type(type) {}

ATarget::ATarget(const ATarget &other)
{
	_type = other.getType();
}

ATarget &ATarget::operator=(const ATarget &other)
{
	if (this != &other)
		_type = other.getType();
	return *this;
}

const string &ATarget::getType() const {
	return _type;
}

ATarget::~ATarget() {}

void ATarget::getHitBySpell(const ASpell &spell) const {
	cout << getType() << " has been " << spell.getEffects() << "!" << endl;
}