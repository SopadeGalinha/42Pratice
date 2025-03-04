#ifndef WARLOCK_HPP
# define WARLOCK_HPP

# include <iostream>
# include <string>
# include <map>
# include "ATarget.hpp"
# include "ASpell.hpp"
# include "SpellBook.hpp"

using namespace std;

class Warlock {

	private:
		string _name;
		string _title;
		SpellBook _book;

		Warlock();
		Warlock(const Warlock& original);
		Warlock& operator=(const Warlock& original);
	public:
		Warlock(const string& name, const string& title);
		~Warlock();

		string const& getName() const;
		string const& getTitle() const;
		void	setTitle(const string& title);
		void	introduce() const;
		void	learnSpell(ASpell* spell);
		void	forgetSpell(const string &spell);
		void	launchSpell(const string &spell, const ATarget& target);
};

#endif