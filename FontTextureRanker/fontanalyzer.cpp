#include "fontanalyzer.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <QImage>
#include <QImageWriter>

namespace fontanazlyer {

QBitArray intToBitArray(unsigned long long num)
{
    unsigned long long size = FontTexture::compressedCharacterWidth * FontTexture::compressedCharacterHeight;
    QBitArray bitArray(size);

    for(unsigned long long i = 0; i < size; ++i)
    {
        bitArray.setBit(size - 1ULL - i, (num >> (size - i - 1ULL)) & 1ULL);
    }

    return bitArray;
}

std::vector<FontCharacter*> analyze(FontTexture& fontTexture)
{
    const std::deque<FontCharacter*>& characters = fontTexture.getCharacters();
    std::vector<FontCharacter*> characterMatches;
    unsigned long long limit = std::pow(2, FontTexture::compressedCharacterWidth * FontTexture::compressedCharacterHeight);

    for(unsigned long long i = 0; i < limit; ++i)
    {
        FontCharacter* bestMatch = characters.front();
        double bestMatchPercentage = 0;

        std::cout << "============================================================" << std::endl;
        std::cout << "============================================================" << std::endl;

        std::deque<FontCharacter*>::const_iterator iter = characters.begin();

        while(iter != characters.end())
        {
            FontCharacter* currentCharacter = *iter;
            double currentPercentage = currentCharacter->match(intToBitArray(i));

            if(currentPercentage > bestMatchPercentage)
            {
                bestMatch = currentCharacter;
                bestMatchPercentage = currentPercentage;
            }

            else if(currentPercentage == bestMatchPercentage) // In case of a tie, check which has less matches and choose that
            {
                if(currentCharacter->numberOfMatches <= bestMatch->numberOfMatches)
                {
                    bestMatch = currentCharacter;
                }
            }

            ++iter;
        }

        characterMatches.push_back(bestMatch);
        bestMatch->numberOfMatches = bestMatch->numberOfMatches + 1;
        std::cout << "__________________________________" << std::endl;
        std::cout << "Int to match: " << i << std::endl;
        std::cout << "Best Match percentage: " << bestMatchPercentage << std::endl;
        std::cout << "Best Match Key: " << bestMatch->compressedBitKey << std::endl;

        // std::cout << ((double) i / (double) limit) << std::endl;
    }

    return characterMatches;
}

bool saveMatches(const std::vector<FontCharacter*>& characterMatches)
{
    std::ofstream output("FontLookup.txt");

    if(!output.is_open())
        return false;

    for(unsigned int i = 0; i < characterMatches.size(); ++i)
    {
        output << characterMatches.at(i)->id << std::endl;
    }

    std::cout << "FontLookup saved. Total number of character matches: " << characterMatches.size() << std::endl;
    return true;
}

bool writeMatchesToTexture(const std::vector<FontCharacter*>& characterMatches, unsigned int numCharacters)
{
    QImage texture(256, 2, QImage::Format_Indexed8);
    unsigned int x = 0;
    unsigned int y = 0;

    texture.setColorCount(numCharacters);
    for(int i = 0; i < numCharacters; ++i)
    {
        texture.setColor(i, qRgb(i, i, i));
    }

    for(unsigned int i = 0; i < characterMatches.size(); ++i)
    {
        texture.setPixel(x, y, characterMatches.at(i)->id);
        ++x;

        if(x >= 256)
        {
            x = 0;
            y = 1;
        }
    }

    QImageWriter writer("FontLookup.bmp");

    if(!writer.write(texture))
    {
        std::cout << "fonatanalyzer::writeMatchesToTexture writing failed." << std::endl;
        return false;
    }

    std::cout << "FontLookupTexture saved. " << std::endl;
    return true;
}

} // fontanalyzer
