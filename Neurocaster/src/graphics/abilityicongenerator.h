#ifndef ABILITYICONGENERATOR_H
#define ABILITYICONGENERATOR_H

#include "src/gameplay/abilities/ability.h"

namespace neuro
{

void loadAbilityNameResources();
std::string generateAbilityName(Ability::Type type);

} // neuro namespace

#endif // ABILITYICONGENERATOR_H
