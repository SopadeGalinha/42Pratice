#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

# include "ASpell.hpp"
# include <map>

class ASpell;

using namespace std;

class SpellBook {

	private:
		SpellBook(const SpellBook& original);
		SpellBook& operator=(const SpellBook& original);
		map<string, ASpell*> _book;
	public:
		SpellBook();
		~SpellBook();
		void	learnSpell(ASpell* spell);
		void	forgetSpell(string const& spell);
		ASpell*	createSpell(string const& spell);
};

#endif