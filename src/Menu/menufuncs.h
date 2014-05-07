#pragma once

#include <boost/algorithm/string.hpp>
#include "string"
//#include "SDL2/SDL.h"
#include <iostream>
#include "Utility/ois2sdl.h"

using namespace utility;

static std::string wordWrap( std::string str, size_t width ) {
    size_t curWidth = width;
    while( curWidth < str.length() ) {
        std::string::size_type spacePos = str.rfind( ' ', curWidth );
        if( spacePos == std::string::npos )
            spacePos = str.find( ' ', curWidth );
        if( spacePos != std::string::npos ) {
            str[ spacePos ] = '\n';
            curWidth = spacePos + width + 1;
        }
    }
    return str;
}

static std::string characterWrap( std::string str, size_t width ) {
    if(str.size()>width) {
        std::string wrappedString;
        for(int i=0;i<=(str.size()/width);i++) {
            wrappedString += str.substr(i*width,width-1);
            wrappedString +="\n";
        }
        return wrappedString;
    } else {
        return str;
    }
}

static const std::string consBreak ="%&//^";

static const std::string reverseLines(std::string str, int numLines) {
    int numBreaks = 0;

    std::list<std::string> blockstrs;
    std::list<std::string> returnstrs;
    boost::split(returnstrs,str, boost::is_any_of("\n"));

    numBreaks = numLines - returnstrs.size();
    std::list<std::string>::iterator it;
    std::string finalString;
    int i=0;

    if(numBreaks>0) {
        boost::split(blockstrs,str, boost::is_any_of(consBreak));
        for(int i=0;i<numBreaks;i++) {
            blockstrs.push_front("\n");
        }
        for( it = blockstrs.begin() ; it != blockstrs.end(); ++it ) {
            finalString.append(*it);
            i++;
        }
    } else if(numBreaks<=0) {
        for(int i=0;i<numBreaks*-1;i++) {
            returnstrs.pop_front();
        }
        std::string postReturnStrs;
        for( it = returnstrs.begin() ; it != returnstrs.end(); ++it ) {
            postReturnStrs.append(*it);
            postReturnStrs.append("\n");
        }
        boost::split(blockstrs,postReturnStrs, boost::is_any_of(consBreak));
        for( it = blockstrs.begin() ; it != blockstrs.end(); ++it ) {
            finalString.append(*it);
            i++;
        }
    }
    return finalString;
}

static std::vector<std::string> pageWrap(std::string str, int pageSize)
{
    std::vector<std::string> pages;
    std::vector<std::string> strs;
    boost::split(strs,str, boost::is_any_of("\n"));
    int curPage = 0;
    int lineNum = 0;
    for(int i=0;i<strs.size();i++) {
        if(lineNum>=pageSize) {
            curPage++;
            lineNum = 0;
            std::string s("");
            pages.push_back(s);
        } else if(lineNum==0){
            std::string s("");
            pages.push_back(s);
            lineNum++;
        } else {
            lineNum++;
        }
        pages.at(curPage).append(strs.at(i));
        pages.at(curPage).append("\n");
    }

    return pages;
}


static char keyCodeToChar(EntKeyCode key, bool shift) {

    /*
    switch(event.key.keysym.scancode) {
    case SDL_SCANCODE_KP_GREATER:
        if(!shift) {
            return '>';
        } else {
            return '>';
        }
    case SDL_SCANCODE_NONUSBACKSLASH:
        if(!shift) {
            return '.';
        } else {
            return '>';
        }
    case SDL_SCANCODE_PERIOD:
        if(!shift) {
            return '.';
        } else {
            return '>';
        }
    case SDL_SCANCODE_APOSTROPHE:
        if(!shift) {
            return 39;
        } else {
            return 34;
        }
    case SDL_SCANCODE_GRAVE:
        if(!shift) {
            return 96;
        } else {
            return 126;
        }
    }*/

    switch(key) {
    case KC_0:
        if(!shift) {
            return '0';
        } else {
            return ')';
        }
    case KC_1:
        if(!shift) {
            return '1';
        } else {
            return '!';
        }
    case KC_2:
        if(!shift) {
            return '2';
        } else {
            return '@';
        }
    case KC_3:
        if(!shift) {
            return '3';
        } else {
            return '#';
        }
    case KC_4:
        if(!shift) {
            return '4';
        } else {
            return '$';
        }
    case KC_5:
        if(!shift) {
            return '5';
        } else {
            return '%';
        }
    case KC_6:
        if(!shift) {
            return '6';
        } else {
            return '^';
        }
    case KC_7:
        if(!shift) {
            return '7';
        } else {
            return '&';
        }
    case KC_8:
        if(!shift) {
            return '8';
        } else {
            return '*';
        }
    case KC_9:
        if(!shift) {
            return '9';
        } else {
            return '(';
        }
    case KC_A:
        if(!shift) {
            return 'a';
        } else {
            return 'A';
        }
    case KC_B:
        if(!shift) {
            return 'b';
        } else {
            return 'B';
        }
    case KC_C:
        if(!shift) {
            return 'c';
        } else {
            return 'C';
        }
    case KC_D:
        if(!shift) {
            return 'd';
        } else {
            return 'D';
        }
    case KC_E:
        if(!shift) {
            return 'e';
        } else {
            return 'E';
        }
    case KC_F:
        if(!shift) {
            return 'f';
        } else {
            return 'F';
        }
    case KC_G:
        if(!shift) {
            return 'g';
        } else {
            return 'G';
        }
    case KC_H:
        if(!shift) {
            return 'h';
        } else {
            return 'H';
        }
    case KC_I:
        if(!shift) {
            return 'i';
        } else {
            return 'I';
        }
    case KC_J:
        if(!shift) {
            return 'j';
        } else {
            return 'J';
        }
    case KC_K:
        if(!shift) {
            return 'k';
        } else {
            return 'K';
        }
    case KC_L:
        if(!shift) {
            return 'l';
        } else {
            return 'L';
        }
    case KC_M:
        if(!shift) {
            return 'm';
        } else {
            return 'M';
        }
    case KC_N:
        if(!shift) {
            return 'n';
        } else {
            return 'N';
        }
    case KC_O:
        if(!shift) {
            return 'o';
        } else {
            return 'O';
        }
    case KC_P:
        if(!shift) {
            return 'p';
        } else {
            return 'P';
        }
    case KC_Q:
        if(!shift) {
            return 'q';
        } else {
            return 'Q';
        }
    case KC_R:
        if(!shift) {
            return 'r';
        } else {
            return 'R';
        }
    case KC_S:
        if(!shift) {
            return 's';
        } else {
            return 'S';
        }
    case KC_T:
        if(!shift) {
            return 't';
        } else {
            return 'T';
        }
    case KC_U:
        if(!shift) {
            return 'u';
        } else {
            return 'U';
        }
    case KC_V:
        if(!shift) {
            return 'v';
        } else {
            return 'V';
        }
    case KC_W:
        if(!shift) {
            return 'w';
        } else {
            return 'W';
        }
    case KC_X:
        if(!shift) {
            return 'x';
        } else {
            return 'X';
        }
    case KC_Y:
        if(!shift) {
            return 'y';
        } else {
            return 'Y';
        }
    case KC_Z:
        if(!shift) {
            return 'z';
        } else {
            return 'Z';
        }
    case KC_BACKSLASH:
        if(!shift) {
            return 92;
        } else {
            return '|';
        }
    case KC_BACK:
        return 8;
    case KC_COMMA:
        if(!shift) {
            return ',';
        } else {
            return '<';
        }
    case KC_EQUALS:
        if(!shift) {
            return '=';
        } else {
            return '+';
        }
    case KC_NUMPAD1:
        if(!shift) {
            return 1;
        } else {
            return 1;
        }
    case KC_NUMPAD2:
        if(!shift) {
            return 2;
        } else {
            return 2;
        }
    case KC_NUMPAD3:
        if(!shift) {
            return 3;
        } else {
            return 3;
        }
    case KC_NUMPAD4:
        if(!shift) {
            return 4;
        } else {
            return 4;
        }
    case KC_NUMPAD5:
        if(!shift) {
            return 5;
        } else {
            return 5;
        }
    case KC_NUMPAD6:
        if(!shift) {
            return 6;
        } else {
            return 6;
        }
    case KC_NUMPAD7:
        if(!shift) {
            return 7;
        } else {
            return 7;
        }
    case KC_NUMPAD8:
        if(!shift) {
            return 8;
        } else {
            return 8;
        }
    case KC_NUMPAD9:
        if(!shift) {
            return 9;
        } else {
            return 9;
        }
//    case SDLK_KP_AMPERSAND:
//        if(!shift) {
//            return '&';
//        } else {
//            return '&';
//        }
    case KC_AT:
        if(!shift) {
            return '@';
        } else {
            return '@';
        }
    /*
    case SDLK_KP_COLON:
        if(!shift) {
            return ':';
        } else {
            return ':';
        }
    case SDLK_KP_COMMA:
        if(!shift) {
            return ',';
        } else {
            return ',';
        }
    case SDLK_KP_DIVIDE:
        if(!shift) {
            return 92;
        } else {
            return 92;
        }
    case SDLK_KP_EQUALS:
        if(!shift) {
            return '=';
        } else {
            return '=';
        }
    case SDLK_KP_EXCLAM:
        if(!shift) {
            return '!';
        } else {
            return '!';
        }
    case SDLK_KP_GREATER:
        if(!shift) {
            return '>';
        } else {
            return '>';
        }
    case SDLK_KP_HASH:
        if(!shift) {
            return '#';
        } else {
            return '#';
        }
    case SDLK_KP_LEFTBRACE:
        if(!shift) {
            return '{';
        } else {
            return '{';
        }
    case SDLK_KP_LEFTPAREN:
        if(!shift) {
            return '(';
        } else {
            return '(';
        }
    case SDLK_KP_LESS:
        if(!shift) {
            return '<';
        } else {
            return '<';
        }
    case SDLK_KP_MINUS:
        if(!shift) {
            return '-';
        } else {
            return '-';
        }
    case SDLK_KP_MULTIPLY:
        if(!shift) {
            return '*';
        } else {
            return '*';
        }
    case SDLK_KP_PERCENT:
        if(!shift) {
            return '%';
        } else {
            return '%';
        }
    case SDLK_KP_PERIOD:
        if(!shift) {
            return '.';
        } else {
            return '>';
        }
    case SDLK_KP_PLUS:
        if(!shift) {
            return '+';
        } else {
            return '+';
        }
    case SDLK_KP_POWER:
        if(!shift) {
            return '^';
        } else {
            return '^';
        }
    case SDLK_KP_RIGHTBRACE:
        if(!shift) {
            return '}';
        } else {
            return '}';
        }
    case SDLK_KP_RIGHTPAREN:
        if(!shift) {
            return ')';
        } else {
            return ')';
        }
    case SDLK_KP_SPACE:
        if(!shift) {
            return 32;
        } else {
            return 32;
        }
    case SDLK_KP_TAB:
        if(!shift) {
            return '    ';
        } else {
            return '    ';
        }
    case SDLK_KP_VERTICALBAR:
        if(!shift) {
            return '|';
        } else {
            return '|';
        }
    */
    case KC_LBRACKET:
        if(!shift) {
            return '[';
        } else {
            return '{';
        }
    case KC_MINUS:
        if(!shift) {
            return '-';
        } else {
            return '_';
        }
    case KC_PERIOD:
        if(!shift) {
            return '.';
        } else {
            return '>';
        }
    case KC_RBRACKET:
        if(!shift) {
            return ']';
        } else {
            return '}';
        }
    case KC_SEMICOLON:
        if(!shift) {
            return ';';
        } else {
            return ':';
        }
    case KC_SLASH:
        if(!shift) {
            return '/';
        } else {
            return '?';
        }
    case KC_TAB:
        if(!shift) {
            return '    ';
        } else {
            return '    ';
        }
    case KC_APOSTROPHE:
        if(!shift) {
            return '\'';
        } else {
            return '\"';
        }
//    case SDLK_GREATER:
//        if(!shift) {
//            return '>';
//        } else {
//            return '>';
//        }
//    case SDLK_LESS:
//        if(!shift) {
//            return 44;
//        } else {
//            return 60;
//        }
    }
    return 32;
}
