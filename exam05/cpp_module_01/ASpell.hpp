#ifndef ASPELL_HPP
# define ASPELL_HPP

# include <iostream>
# include <string>
# include "ATarget.hpp"

class ATarget;
using namespace std;

// Coplien's form class 
class ASpell {

	protected:
		// Atribuites
		string	_name;
		string	_effect;

	public:
		// Constructor
		ASpell();

		// Copy constructor
		ASpell(const ASpell& other);

		// Constructor with parameters
		ASpell(const string& name, const string& effect);

		// Destructor
		virtual ~ASpell();

		// Assigment operator
		ASpell& operator=(const ASpell& other);
		
		// Getters
		const string &getName() const;
		const string &getEffects() const;
		
		// Methods
		void	launch(const ATarget& target);
		virtual ASpell* clone() const = 0;
};

#endif