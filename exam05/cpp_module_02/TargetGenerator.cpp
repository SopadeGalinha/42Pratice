#include "TargetGenerator.hpp"

TargetGenerator::TargetGenerator() {}

TargetGenerator::~TargetGenerator() {
    while (!_targets.empty()) {
        delete _targets.begin()->second;
        _targets.erase(_targets.begin());
    }
}


void	TargetGenerator::learnTargetType(ATarget* target) {
	if (target)
		_targets[target->getType()] = target->clone();
}

void	TargetGenerator::forgetTargetType(string const& target) {
	if (_targets.find(target) != _targets.end()) {
		delete _targets[target];
		_targets.erase(target);
	}
}

ATarget*	TargetGenerator::createTarget(string const& target) {
	if (_targets.count(target))
		return _targets[target];
	return NULL;
}
