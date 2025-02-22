#ifndef DUMMY
# define DUMMY

#include "ATarget.hpp"

class ATarget;

class Dummy : public ATarget {
	public:
		Dummy();
		virtual ~Dummy();
		Dummy *clone() const;
};

#endif