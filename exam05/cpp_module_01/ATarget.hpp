#ifndef ATARGET_HPP
# define ATARGET_HPP

# include <iostream>
# include <string>
# include "ASpell.hpp"

class ASpell;
using namespace std;

// Coplien's form class
class ATarget {

	protected:
		// Atribuites
		string	_type;

	public:
		// Constructor
		ATarget();

		// Copy constructor
		ATarget(const ATarget& other);

		// Constructor with parameters
		ATarget(const string& type);

		// Assigment operator
		ATarget& operator=(const ATarget& other);
		
		// Destructor
		virtual ~ATarget();
		
		// Getter
		const string &getType() const;
		
		// Methods
		void	getHitBySpell(const ASpell& spell) const;
		
		// Pure virtual method
		virtual ATarget* clone() const = 0;
};

#endif