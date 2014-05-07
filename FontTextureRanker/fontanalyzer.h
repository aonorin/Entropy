#ifndef FONTANALYZER_H
#define FONTANALYZER_H

#include <utility>
#include <vector>
#include "fonttexture.h"

namespace fontanazlyer {

QBitArray intToBitArray(unsigned long long num);

// Takes a texture and returns a vector whose indexes representing the binary representation of a shadergrid struct
std::vector<FontCharacter*> analyze(FontTexture& fontTexture);
bool saveMatches(const std::vector<FontCharacter*>& characterMatches);
bool writeMatchesToTexture(const std::vector<FontCharacter*>& characterMatches, unsigned int numCharacters);

} // fontanalyzer

#endif // FONTANALYZER_H
