#include "Warlock.hpp"

Warlock::Warlock(const Warlock &other) {
	(void)other;
};

Warlock & Warlock::operator=(const Warlock &other) {
	(void)other;
	return *this;
}

Warlock::Warlock(const string &name, const string &title) : \
	_name(name), _title(title) {
		cout << getName() \
		<< ": This looks like another boring day." << endl;
}

Warlock::~Warlock() {
	cout << getName() \
	<< ": My job here is done!" << endl;
}

const string &Warlock::getName() const {
	return _name;
}

const string &Warlock::getTitle() const {
	return _title;
}

void Warlock::setTitle(const string & title) {
	_title = title;
}

void	Warlock::introduce() const {
	cout \
	<< getName() << ": I am " \
	<< getName()  << ", " 
	<< getTitle() << "!" << endl;
}

void	Warlock::learnSpell(ASpell* spell) {
	_slots.insert( \
		pair<string, ASpell*>( \
		spell->getName(), spell->clone())
	);
}

void	Warlock::forgetSpell(string spell) {
	map<string, ASpell*>::iterator it;
	it = _slots.find(spell);
	if (it == _slots.end())
		return ;
	delete it->second;
	_slots.erase(spell);
}

void	Warlock::launchSpell(string spell, const ATarget& target) {
	map<string, ASpell*>::iterator it;
	it = _slots.find(spell);
	if (it != _slots.end()) 
		it->second->launch(target);
}