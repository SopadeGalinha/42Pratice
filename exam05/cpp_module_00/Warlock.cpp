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
