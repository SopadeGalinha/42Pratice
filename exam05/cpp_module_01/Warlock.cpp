# include "Warlock.hpp"

Warlock::Warlock(const string &name, const string &title) : _name(name), _title(title) {
	cout << getName() << ": This looks like another boring day." << endl;
}

Warlock::~Warlock() {
	cout << getName() << ": My job here is done!" << endl;
}

const string &Warlock::getName() const {
	return _name;
}

const string &Warlock::getTitle() const {
	return _title;
}

void Warlock::setTitle(const string &title) {
	_title = title;
}

void Warlock::introduce() const {
	cout << getName() << ": I am " << getName() << ", " << getTitle() << "!" << endl;
}

void Warlock::learnSpell(ASpell *spell) {
	_book[spell->getName()] = spell->clone();
}

void Warlock::launchSpell(string spell, ATarget &target) {
	if (_book.count(spell))
		_book[spell]->launch(target);
}

void Warlock::forgetSpell(string spell) {
	if (_book.count(spell)) {
		delete _book[spell];
		_book.erase(spell);
	}
}
