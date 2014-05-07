#include <vector>
#include <iostream>
#include <fstream>
#include "src/utility/util.h"

#include "abilityicongenerator.h"


namespace neuro
{

std::vector<std::string> biology, cryptography, cybernetics, hackerJargon, viral;

void loadCategoryStrings(std::vector<std::string>* categoryStrings, const char* fileName)
{
    std::fstream filestream(fileName);
    if(filestream.is_open())
    {
        while(!filestream.eof())
        {
            char str[200];
            filestream.getline(str, 200);
            categoryStrings->push_back(std::string(str));
        }
    }
}

void loadAbilityNameResources()
{
    loadCategoryStrings(&biology, "../Resources/resources/TextFiles/Biology.txt");
    loadCategoryStrings(&cryptography, "../Resources/resources/TextFiles/Cryptography.txt");
    loadCategoryStrings(&cybernetics, "../Resources/resources/TextFiles/Cybernetics.txt");
    loadCategoryStrings(&hackerJargon, "../Resources/resources/TextFiles/HackerJargon.txt");
    loadCategoryStrings(&viral, "../Resources/resources/TextFiles/VirusList.txt");
}

std::string generateInfectionName()
{
    return viral.at(neuro::util::rand_int(0, viral.size()));
}

std::string generateCryptographyName()
{
    std::string name = cryptography.at(neuro::util::rand_int(0, cryptography.size()));

    if(name.length() < 15)
        name.append(" ").append(hackerJargon.at(neuro::util::rand_int(0, hackerJargon.size())));

    return name;
}

std::string generateCyberneticsName()
{
    std::string name = cybernetics.at(neuro::util::rand_int(0, cybernetics.size()));
    name.append(" ").append(biology.at(neuro::util::rand_int(0, biology.size())));

    return name;
}

std::string generateAbilityName(Ability::Type type)
{
    switch(type)
    {
    case Ability::INFECTION:
        return generateInfectionName();
        break;

    case Ability::CRYPTOGRAPHY:
        return generateCryptographyName();
        break;

    case Ability::CYBERNETICS:
    default:
        return generateCyberneticsName();
        break;
    }
}

} // neuro namespace
