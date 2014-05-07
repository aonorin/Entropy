#ifndef OIS2SDL_H
#define OIS2SDL_H


#include "boost/unordered_map.hpp"
#ifndef OIS
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#endif
#include <stdint.h>


// Prevent size_t redeclarations if the header define is slightly different
#ifndef _STDINT_H_
#define _STDINT_H_
#endif

#ifdef SDL
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_events.h>
#endif


namespace utility
{
// Translation map from OIS to SDL key codes
//extern boost::unordered::unordered_map<OIS::KeyCode, SDL_Keycode> keyMap;

// Translation map from OIS to SDL mouse codes
//extern boost::unordered::unordered_map<OIS::MouseButtonID, Uint8> mouseMap;

enum EntKeyCode {
    KC_UNASSIGNED,
    KC_ESCAPE,
    KC_1,
    KC_2,
    KC_3,
    KC_4,
    KC_5,
    KC_6,
    KC_7,
    KC_8,
    KC_9,
    KC_0,
    KC_MINUS,
    KC_EQUALS,
    KC_BACK,
    KC_TAB,
    KC_Q,
    KC_W,
    KC_E,
    KC_R,
    KC_T,
    KC_Y,
    KC_U,
    KC_I,
    KC_O,
    KC_P,
    KC_LBRACKET,
    KC_RBRACKET,
    KC_RETURN,
    KC_LCONTROL,
    KC_A,
    KC_S,
    KC_D,
    KC_F,
    KC_G,
    KC_H,
    KC_J,
    KC_K,
    KC_L,
    KC_SEMICOLON,
    KC_APOSTROPHE,
    KC_GRAVE,
    KC_LSHIFT,
    KC_BACKSLASH,
    KC_Z,
    KC_X,
    KC_C,
    KC_V,
    KC_B,
    KC_N,
    KC_M,
    KC_COMMA,
    KC_PERIOD,
    KC_SLASH,
    KC_RSHIFT,
    KC_MULTIPLY,
    KC_LMENU,
    KC_SPACE,
    KC_CAPITAL,
    KC_F1,
    KC_F2,
    KC_F3,
    KC_F4,
    KC_F5,
    KC_F6,
    KC_F7,
    KC_F8,
    KC_F9,
    KC_F10,
    KC_NUMLOCK,
    KC_SCROLL,
    KC_NUMPAD7,
    KC_NUMPAD8,
    KC_NUMPAD9,
    KC_SUBTRACT,
    KC_NUMPAD4,
    KC_NUMPAD5,
    KC_NUMPAD6,
    KC_ADD,
    KC_NUMPAD1,
    KC_NUMPAD2,
    KC_NUMPAD3,
    KC_NUMPAD0,
    KC_DECIMAL,
    KC_OEM_102,
    KC_F11,
    KC_F12,
    KC_F13,
    KC_F14,
    KC_F15,
    KC_ABNT_C1,
    KC_ABNT_C2,
    KC_NUMPADEQUALS,
    KC_PREVTRACK,
    KC_AT,
    KC_COLON,
    KC_UNDERLINE,
    KC_STOP,
    KC_NEXTTRACK,
    KC_NUMPADENTER,
    KC_RCONTROL,
    KC_MUTE,
    KC_CALCULATOR,
    KC_PLAYPAUSE,
    KC_MEDIASTOP,
    KC_VOLUMEDOWN,
    KC_VOLUMEUP,
    KC_WEBHOME,
    KC_NUMPADCOMMA,
    KC_DIVIDE,
    KC_SYSRQ,
    KC_RMENU,
    KC_PAUSE,
    KC_HOME,
    KC_UP,
    KC_PGUP,
    KC_LEFT,
    KC_RIGHT,
    KC_END,
    KC_DOWN,
    KC_PGDOWN,
    KC_INSERT,
    KC_DELETE,
    KC_LWIN,
    KC_RWIN,
    KC_APPS,
    KC_POWER,
    KC_SLEEP,
    KC_WEBSEARCH,
    KC_WEBFAVORITES,
    KC_WEBREFRESH,
    KC_WEBSTOP,
    KC_WEBFORWARD,
    KC_WEBBACK,
    KC_MYCOMPUTER,
    KC_MAIL,
    KC_MEDIASELECT,
    MB_Left,
    MB_Middle,
    MB_Right,
    MB_Button3,
    MB_Button4,
    KC_GREATER,
    KC_LESS
};

#ifndef OIS
extern boost::unordered::unordered_map<OIS::KeyCode, EntKeyCode> keyMap;
extern boost::unordered::unordered_map<OIS::MouseButtonID, EntKeyCode> mouseMap;
#else
extern boost::unordered::unordered_map<SDL_Keycode, EntKeyCode> keyMap;
extern boost::unordered::unordered_map<Uint8, EntKeyCode> mouseMap;
#endif

void initializeTranslationMaps();
}

#endif // OIS2SDL_H
