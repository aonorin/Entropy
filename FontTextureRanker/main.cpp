#include "fonttexture.h"
#include <fontanalyzer.h>

int main(int argc, char *argv[])
{
    FontTexture fontTexture(":/fontTextures/VisitorFontTexture.png", "PNG");
    std::vector<FontCharacter*> matches = fontanazlyer::analyze(fontTexture);
    fontanazlyer::saveMatches(matches);
    fontanazlyer::writeMatchesToTexture(matches, fontTexture.getNumCharacters());
    return 0;
}
