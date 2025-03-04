#include "Warlock.hpp"

Warlock::Warlock() : _name("default"), _title("default") {}

Warlock::~Warlock() {
	cout << getName() << ": My job here is done!" << endl;
}

Warlock::Warlock(const string &name, const string &title) : _name(name), _title(title) {
	cout << getName() << ": This looks like another boring day." << endl;
}

string const &Warlock::getName() const {
	return _name;
}

string const &Warlock::getTitle() const {
	return _title;
}

void Warlock::setTitle(const string &title) {
	_title = title;
}

void Warlock::introduce() const {
	cout << getName() << ": I am " << getName() << ", " << getTitle() << "!" << endl;
}

void Warlock::learnSpell(ASpell *spell) {
	if (spell)
		_book.learnSpell(spell);
}

void Warlock::forgetSpell(const string &spell) {
	_book.forgetSpell(spell);
}

void Warlock::launchSpell(const string &spell, const ATarget &target) {
	if (ASpell *s = _book.createSpell(spell))
		s->launch(target);
}
