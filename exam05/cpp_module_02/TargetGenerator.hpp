#ifndef TARGETGENERATOR_HPP
# define TARGETGENERATOR_HPP

# include <map>
# include "ATarget.hpp"

class ATarget;

class TargetGenerator {

	private:
		map<string, ATarget*> _targets;
	
		TargetGenerator(const TargetGenerator& original);
		TargetGenerator& operator=(const TargetGenerator& original);
	public:
		TargetGenerator();
		~TargetGenerator();
		
		void learnTargetType(ATarget* target);
		void forgetTargetType(string const& target);
		
		ATarget* createTarget(string const& target);
};

#endif