#include "ATarget.hpp"

ATarget::ATarget() : _type("def") {}

ATarget::ATarget(const string& type) : _type(type) {}

ATarget::ATarget(const ATarget& other) : _type(other.getType()) {}

ATarget::~ATarget() {}

ATarget& ATarget::operator=(const ATarget& other) {
	if (this != &other)
		_type = other.getType();
	return *this;
}

string const& ATarget::getType() const {
	return _type;
}

void	ATarget::getHitBySpell(const ASpell& spell) const {
	cout << getType() << " has been " << spell.getEffects() << "!" << endl;
}