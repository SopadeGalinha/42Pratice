#include "ASpell.hpp"

ASpell::ASpell() : _name("def"), _effect("def") {}

ASpell::ASpell(const ASpell& other) :
	_name(other.getName()), _effect(other.getEffects()) {}

ASpell& ASpell::operator=(const ASpell& other) {
	if (this != &other) {
		_name = other.getName();
		_effect = other.getEffects();
	}
	return *this;
}

ASpell::~ASpell() {}

ASpell::ASpell(const string& name, const string& effect) :
	_name(name), _effect(effect) {}

string const& ASpell::getName() const {
	return _name;
}

string const& ASpell::getEffects() const {
	return _effect;
}

void	ASpell::launch(const ATarget& target) {
	target.getHitBySpell(*this);
}	