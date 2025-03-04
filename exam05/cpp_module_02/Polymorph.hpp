#ifndef POLYMORPH_HPP
# define POLYMORPH_HPP

# pragma once
# include "ASpell.hpp"

class Polymorph : public ASpell {

	public:
		Polymorph();
		virtual ~Polymorph();
		Polymorph* clone() const;
};

#endif