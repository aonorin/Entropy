#ifndef FONTCHARACTER_H
#define FONTCHARACTER_H

#include <QBitArray>

/**
 * @brief The FontCharacter class contains the bit array description of a single character, described using white/black pixels
 */
class FontCharacter
{
public:
    FontCharacter(QBitArray& bitArray, unsigned int id);

    double match(QBitArray character);

    QBitArray bitArray, compressedBitArray;
    unsigned int bitKey, compressedBitKey, numberOfMatches, id;

private:

    void compress();
    bool compressCorner(unsigned int index);
    bool compressCenter(unsigned int index);
    bool compressInner(unsigned int index);
};

#endif // FONTCHARACTER_H
