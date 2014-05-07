#include "fonttexture.h"
#include <QColor>
#include <QDebug>
#include <QPixmap>
#include <QImageWriter>

FontTexture::FontTexture(const char* fileName, const char* format) :
    fileName(fileName)
{
    initialized = loadFile(fileName, format);
}

FontTexture::~FontTexture()
{
    while(characters.size())
    {
        FontCharacter* character = characters.front();

        if(character)
            delete character;

        characters.pop_front();
    }

    characters.clear();
}

FontCharacter* FontTexture::getCharacter(unsigned int index)
{
    if(index < characters.size())
        return characters.at(index);
    else
        return 0;
}

unsigned int FontTexture::getNumCharacters()
{
    return characters.size();
}

const std::deque<FontCharacter*>& FontTexture::getCharacters() const
{
    return characters;
}

const std::string& FontTexture::getFilename() const
{
    return fileName;
}

bool FontTexture::loadFile(const char* fileName, const char* format)
{
    fontImage = QImage(fileName, format);
    // QImage newImage = QImage(fontImage);
    // newImage.fill(QColor(255, 255, 255));
    // newImage.setColor(0, qRgb(0, 0, 0));

    if(fontImage.isNull())
    {
        qDebug() << "WARNING: cannot load image file: " << fileName;
        return false;
    }

    else
    {
        for(unsigned int i = 0; i < fontImage.size().width() / FontTexture::characterWidth; ++i)
        {
            QBitArray bitArray(FontTexture::characterWidth * FontTexture::characterHeight);

            for(unsigned int y = 0; y < FontTexture::characterHeight; ++y)
            {
                for(unsigned int x = 0; x < FontTexture::characterWidth; ++x)
                {
                    bool value = false;
                    QColor color = QColor::fromRgb(fontImage.pixel(x + (i * FontTexture::characterWidth), y));

                    if(color.red() > 0) // Picking an arbitrary color because it should only be Black or White
                    {
                        value = false;
                    }

                    else
                    {
                        value = true;
                        // newImage.setPixel(x + (i * 7) + 1, y, 0);
                    }

                    bitArray.setBit((y * FontTexture::characterWidth) + x, value);
                }
            }

            characters.push_back(new FontCharacter(bitArray, i));
        }

        qDebug() << "NUM CHARACTERS: " << characters.size();

        // QImageWriter writer("newFont.bmp");

        // if(!writer.write(newImage))
        //    qDebug() << "writing failed.";

        return true;
    }
}




