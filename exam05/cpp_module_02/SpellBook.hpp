#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

# include "ASpell.hpp"
# include <map>

class ASpell;

class SpellBook {
	private:
		map<string, ASpell*> _book;
		SpellBook(const SpellBook& original);
		SpellBook& operator=(const SpellBook& original);
	public:
		SpellBook();
		~SpellBook();
		
		void learnSpell(ASpell* spell);
		void forgetSpell(string const& spell);
		
		ASpell* createSpell(string const& spell);
};

#endif
