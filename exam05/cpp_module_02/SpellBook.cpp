#include "SpellBook.hpp"

SpellBook::SpellBook() {}

SpellBook::~SpellBook() {
    while (!_book.empty()) {
        delete _book.begin()->second;
        _book.erase(_book.begin());
    }
}

void SpellBook::learnSpell(ASpell *spell) {
	if (spell)
		_book[spell->getName()] = spell->clone();
}

void SpellBook::forgetSpell(string const &spell) {
    if (_book.count(spell)) {
        delete _book[spell];
        _book.erase(spell);
    }
}

ASpell *SpellBook::createSpell(string const &spell) {
    if (_book.count(spell))
        return _book[spell]->clone();
    return NULL;
}
