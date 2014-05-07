#include "fontcharacter.h"
#include "fonttexture.h"
#include <iostream>

FontCharacter::FontCharacter(QBitArray &bitArray, unsigned int id) :
    bitArray(bitArray),
    id(id),
    numberOfMatches(0)
{
    for(unsigned int y = 0; y < FontTexture::characterHeight; ++y)
    {
        for(unsigned int x = 0; x < FontTexture::characterWidth; ++x)
        {
            if(bitArray.at((y * FontTexture::characterWidth) + x))
                std::cout << "*";
            else
                std::cout << " ";
        }

        std::cout << std::endl;
    }

    bitKey = 0;
    for(int i = 0; i < bitArray.size(); ++i)
    {
        bitKey += bitArray.at(i) << i;
    }

    std::cout << bitKey << std::endl;

    compress();
}

double FontCharacter::match(QBitArray character)
{
    double percentage = 0;

    for(int i = 0; i < compressedBitArray.size(); ++i)
    {
        if(character.at(i) == compressedBitArray.at(i))
            percentage += 100.0 / (double) compressedBitArray.size();
    }

    std::cout << "Current Match Key: "<< bitKey << " Match Percentage: " << percentage << std::endl;
    return percentage;
}

void FontCharacter::compress() // Currently 5 to 3 compression.
{
    compressedBitArray = QBitArray(FontTexture::compressedCharacterWidth * FontTexture::compressedCharacterHeight);

    for(int i = 0; i < compressedBitArray.size(); ++i)
    {
        switch(i)
        {
        case 0:
        case 2:
        case 6:
        case 8:
            compressedBitArray.setBit(i, compressCorner(i));
            break;

       case 4:
            compressedBitArray.setBit(i, compressCenter(i));
            break;

        default:
            compressedBitArray.setBit(i, compressInner(i));
            break;
        }
    }

    for(unsigned int y = 0; y < FontTexture::compressedCharacterHeight; ++y)
    {
        for(unsigned int x = 0; x < FontTexture::compressedCharacterWidth; ++x)
        {
            if(compressedBitArray.at((y * FontTexture::compressedCharacterWidth) + x))
                std::cout << "*";
            else
                std::cout << " ";
        }

        std::cout << std::endl;
    }

    compressedBitKey = 0;
    for(int i = 0; i < compressedBitArray.size(); ++i)
    {
        compressedBitKey += compressedBitArray.at(i) << i;
    }

    std::cout << compressedBitKey << std::endl;
}

bool FontCharacter::compressCorner(unsigned int index)
{
    double average = 0;
    unsigned int translatedIndex = 0;

    switch(index) // Translate to top left corner of 5x5 grid 2x2 corners
    {
    case 0:
        translatedIndex = 0;
        break;

    case 2:
        translatedIndex = 3;
        break;

    case 6:
        translatedIndex = 15;
        break;

    case 8:
        translatedIndex = 18;
        break;
    }

    if(bitArray.at(translatedIndex))
        average += 0.25;

    if(bitArray.at(translatedIndex + 1))
        average += 0.25;

    if(bitArray.at(translatedIndex + 5))
        average += 0.25;

    if(bitArray.at(translatedIndex + 6))
        average += 0.25;

    if(average >= 0.5)
        return true;
    else
        return false;
}

bool FontCharacter::compressCenter(unsigned int index)
{
    unsigned int translatedIndex = 12;
    return bitArray.at(translatedIndex);
}

bool FontCharacter::compressInner(unsigned int index)
{
    unsigned int translatedIndex = 0;

    switch(index) // Translate to top left corner of 5x5 grid 2x2 corners
    {
    case 1:
        translatedIndex = 2;

        if(bitArray.at(translatedIndex) || bitArray.at(translatedIndex + 5))
            return true;
        else
            return false;

        break;

    case 3:
        translatedIndex = 10;

        if(bitArray.at(translatedIndex) || bitArray.at(translatedIndex + 1))
            return true;
        else
            return false;

        break;

    case 5:
        translatedIndex = 14;

        if(bitArray.at(translatedIndex) || bitArray.at(translatedIndex - 1))
            return true;
        else
            return false;

        break;

    case 7:
        translatedIndex = 22;

        if(bitArray.at(translatedIndex) || bitArray.at(translatedIndex - 5))
            return true;
        else
            return false;

        break;

    default:
        std::cout << "WARNING. FontCharacter::compressInner default SHOULD NOT BE REACHED." << std::endl;
        return false;
    }

    return false;
}
