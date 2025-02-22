#include "ASpell.hpp"

ASpell::ASpell() : _name("default"), _effects("default") {}

ASpell::ASpell(const string &name, const string &effects) : _name(name), _effects(effects) {}

ASpell::ASpell(const ASpell &other)
{
	_name = other.getName();
	_effects = other.getEffects();
}

ASpell &ASpell::operator=(const ASpell &other)
{
	if (this != &other) {
		_name = other.getName();
		_effects = other.getEffects();
	}
	return *this;
}

const string &ASpell::getName() const {
	return _name;
}

const string &ASpell::getEffects() const {
	return _effects;
}

ASpell::~ASpell() {}


void ASpell::launch(const ATarget &target) const {
	target.getHitBySpell(*this);
}
