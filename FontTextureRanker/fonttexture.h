#ifndef FONTTEXTURE_H
#define FONTTEXTURE_H

#include <deque>
#include <string>
#include <QImage>
#include "fontcharacter.h"

/**
 * @brief The FontTexture class is the collection of font characters that describes a font using only white/black pixels as bit flags
 */
class FontTexture
{
public:

    FontTexture(const char* fileName, const char* format);
    ~FontTexture();

    FontCharacter* getCharacter(unsigned int index);
    const std::deque<FontCharacter*>& getCharacters() const;
    unsigned int getNumCharacters();
    const std::string& getFilename() const;

    static const unsigned int characterWidth = 5;
    static const unsigned int characterHeight = 5;
    static const unsigned int compressedCharacterWidth = 3;
    static const unsigned int compressedCharacterHeight = 3;

private:

    bool loadFile(const char* fileName, const char *format);

    std::deque<FontCharacter*> characters;
    std::string fileName;
    bool initialized;
    QImage fontImage;
};

#endif // FONTTEXTURE_H
