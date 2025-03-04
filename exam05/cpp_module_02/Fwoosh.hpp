#ifndef FWOOSH
# define FWOOSH

#include "ASpell.hpp"

class ASpell;

class Fwoosh : public ASpell {
	public:
		Fwoosh();
		virtual ~Fwoosh();
		Fwoosh *clone() const;
};

#endif