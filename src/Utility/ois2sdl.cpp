#include "ois2sdl.h"

namespace utility
{
//    boost::unordered::unordered_map<OIS::KeyCode, SDL_Keycode> keyMap;
//    boost::unordered::unordered_map<OIS::MouseButtonID, Uint8> mouseMap;

#ifndef SDL
boost::unordered::unordered_map<OIS::KeyCode, EntKeyCode> keyMap;
boost::unordered::unordered_map<OIS::MouseButtonID, EntKeyCode> mouseMap;

void initializeTranslationMaps()
{
    keyMap.clear();
    keyMap[OIS::KC_UNASSIGNED] = KC_UNASSIGNED;
    keyMap[OIS::KC_ESCAPE] = KC_ESCAPE;
    keyMap[OIS::KC_1] = KC_1;
    keyMap[OIS::KC_2] = KC_2;
    keyMap[OIS::KC_3] = KC_3;
    keyMap[OIS::KC_4] = KC_4;
    keyMap[OIS::KC_5] = KC_5;
    keyMap[OIS::KC_6] = KC_6;
    keyMap[OIS::KC_7] = KC_7;
    keyMap[OIS::KC_8] = KC_8;
    keyMap[OIS::KC_9] = KC_9;
    keyMap[OIS::KC_0] = KC_0;
    keyMap[OIS::KC_MINUS] = KC_MINUS;	// - on main keyboard
    keyMap[OIS::KC_EQUALS] = KC_EQUALS;
    keyMap[OIS::KC_BACK] = KC_BACK;	// backspace
    keyMap[OIS::KC_TAB] = KC_TAB;
    keyMap[OIS::KC_Q] = KC_Q;
    keyMap[OIS::KC_W] = KC_W;
    keyMap[OIS::KC_E] = KC_E;
    keyMap[OIS::KC_R] = KC_R;
    keyMap[OIS::KC_T] = KC_T;
    keyMap[OIS::KC_Y] = KC_Y;
    keyMap[OIS::KC_U] = KC_U;
    keyMap[OIS::KC_I] = KC_I;
    keyMap[OIS::KC_O] = KC_O;
    keyMap[OIS::KC_P] = KC_P;
    keyMap[OIS::KC_LBRACKET] = KC_LBRACKET;
    keyMap[OIS::KC_RBRACKET] = KC_RBRACKET;
    keyMap[OIS::KC_RETURN] = KC_RETURN;	// Enter on main keyboard
#ifdef __APPLE__
    keyMap[OIS::KC_LSHIFT] = KC_LCONTROL;
    keyMap[OIS::KC_LCONTROL] = KC_LSHIFT;
#else
    keyMap[OIS::KC_LSHIFT] = KC_LSHIFT;
    keyMap[OIS::KC_LCONTROL] = KC_LCONTROL;
#endif
    keyMap[OIS::KC_A] = KC_A;
    keyMap[OIS::KC_S] = KC_S;
    keyMap[OIS::KC_D] = KC_D;
    keyMap[OIS::KC_F] = KC_F;
    keyMap[OIS::KC_G] = KC_G;
    keyMap[OIS::KC_H] = KC_H;
    keyMap[OIS::KC_J] = KC_J;
    keyMap[OIS::KC_K] = KC_K;
    keyMap[OIS::KC_L] = KC_L;
    keyMap[OIS::KC_SEMICOLON] = KC_SEMICOLON;
    keyMap[OIS::KC_APOSTROPHE] = KC_APOSTROPHE;
    keyMap[OIS::KC_GRAVE] = KC_GRAVE;	// accent
    keyMap[OIS::KC_BACKSLASH] = KC_BACKSLASH;
    keyMap[OIS::KC_Z] = KC_Z;
    keyMap[OIS::KC_X] = KC_X;
    keyMap[OIS::KC_C] = KC_C;
    keyMap[OIS::KC_V] = KC_V;
    keyMap[OIS::KC_B] = KC_B;
    keyMap[OIS::KC_N] = KC_N;
    keyMap[OIS::KC_M] = KC_M;
    keyMap[OIS::KC_COMMA] = KC_COMMA;
    keyMap[OIS::KC_PERIOD] = KC_PERIOD;	//. on main keyboard
    keyMap[OIS::KC_SLASH] = KC_SLASH;	// / on main keyboard
    keyMap[OIS::KC_RSHIFT] = KC_RSHIFT;
    keyMap[OIS::KC_MULTIPLY] = KC_MULTIPLY;	// * on numeric keypad
    keyMap[OIS::KC_LMENU] = KC_LMENU;	// left Alt
    keyMap[OIS::KC_SPACE] = KC_SPACE;
    keyMap[OIS::KC_CAPITAL] = KC_CAPITAL;
    keyMap[OIS::KC_F1] = KC_F1;
    keyMap[OIS::KC_F2] = KC_F2;
    keyMap[OIS::KC_F3] = KC_F3;
    keyMap[OIS::KC_F4] = KC_F4;
    keyMap[OIS::KC_F5] = KC_F5;
    keyMap[OIS::KC_F6] = KC_F6;
    keyMap[OIS::KC_F7] = KC_F7;
    keyMap[OIS::KC_F8] = KC_F8;
    keyMap[OIS::KC_F9] = KC_F9;
    keyMap[OIS::KC_F10] = KC_F10;
    keyMap[OIS::KC_NUMLOCK] = KC_NUMLOCK;
    keyMap[OIS::KC_SCROLL] = KC_SCROLL;	// Scroll Lock
    keyMap[OIS::KC_NUMPAD7] = KC_NUMPAD7;
    keyMap[OIS::KC_NUMPAD8] = KC_NUMPAD8;
    keyMap[OIS::KC_NUMPAD9] = KC_NUMPAD9;
    keyMap[OIS::KC_SUBTRACT] = KC_SUBTRACT;	// - on numeric keypad
    keyMap[OIS::KC_NUMPAD4] = KC_NUMPAD4;
    keyMap[OIS::KC_NUMPAD5] = KC_NUMPAD5;
    keyMap[OIS::KC_NUMPAD6] = KC_NUMPAD6;
    keyMap[OIS::KC_ADD] = KC_ADD;	// + on numeric keypad
    keyMap[OIS::KC_NUMPAD1] = KC_NUMPAD1;
    keyMap[OIS::KC_NUMPAD2] = KC_NUMPAD2;
    keyMap[OIS::KC_NUMPAD3] = KC_NUMPAD3;
    keyMap[OIS::KC_NUMPAD0] = KC_NUMPAD0;
    keyMap[OIS::KC_DECIMAL] = KC_DECIMAL;	//. on numeric keypad
    keyMap[OIS::KC_OEM_102] = KC_OEM_102;	//<>|on UK/Germany // keyboards
    keyMap[OIS::KC_F11] = KC_F11;
    keyMap[OIS::KC_F12] = KC_F12;
    keyMap[OIS::KC_F13] = KC_F13;	// (NEC PC98
    keyMap[OIS::KC_F14] = KC_F14;	// (NEC PC98
    keyMap[OIS::KC_F15] = KC_F15;	// (NEC PC98
    // keyMap[OIS::KC_KANA] = SDLK_KANA;	// (Japanese keyboard
    keyMap[OIS::KC_ABNT_C1] = KC_ABNT_C1;	// / ? on Portugese (Brazilian // keyboards
    // keyMap[OIS::KC_CONVERT] = SDLK_79;	// (Japanese keyboard
    // keyMap[OIS::KC_NOCONVERT] = SDLK_7B;	// (Japanese keyboard
    // keyMap[OIS::KC_YEN] = SDLK_7D;	// (Japanese keyboard
    keyMap[OIS::KC_ABNT_C2] = KC_ABNT_C2;	// Numpad. on Portugese // (Brazilian keyboards
    keyMap[OIS::KC_NUMPADEQUALS] = KC_NUMPADEQUALS;	// = on numeric keypad // (NEC PC98
    keyMap[OIS::KC_PREVTRACK] = KC_PREVTRACK;
    keyMap[OIS::KC_AT] = KC_AT;	// (NEC PC98
    keyMap[OIS::KC_COLON] = KC_COLON;	// (NEC PC98
    keyMap[OIS::KC_UNDERLINE] = KC_UNDERLINE;	// (NEC PC98
    // keyMap[OIS::KC_KANJI] = SDLK_94;	// (Japanese keyboard
    keyMap[OIS::KC_STOP] = KC_STOP;	// (NEC PC98
    // keyMap[OIS::KC_AX] = SDLK_96;	// (Japan AX
    // keyMap[OIS::KC_UNLABELED] = SDLK_97;	// (J3100
    keyMap[OIS::KC_NEXTTRACK] = KC_NEXTTRACK;	// Next Track
    keyMap[OIS::KC_NUMPADENTER] = KC_NUMPADENTER;	// Enter on numeric keypad
    keyMap[OIS::KC_RCONTROL] = KC_RCONTROL;
    keyMap[OIS::KC_MUTE] = KC_MUTE;	// Mute
    keyMap[OIS::KC_CALCULATOR] = KC_CALCULATOR;	// Calculator
    keyMap[OIS::KC_PLAYPAUSE] = KC_PLAYPAUSE;	// Play/Pause
    keyMap[OIS::KC_MEDIASTOP] = KC_MEDIASTOP;	// Media Stop
    keyMap[OIS::KC_VOLUMEDOWN] = KC_VOLUMEDOWN;	// Volume −
    keyMap[OIS::KC_VOLUMEUP] = KC_VOLUMEUP;	// Volume +
    keyMap[OIS::KC_WEBHOME] = KC_WEBHOME;	// Web home
    keyMap[OIS::KC_NUMPADCOMMA] = KC_NUMPADCOMMA;	// on numeric keypad // (NEC PC98
    keyMap[OIS::KC_DIVIDE] = KC_DIVIDE;	// / on numeric keypad
    keyMap[OIS::KC_SYSRQ] = KC_SYSRQ;
    keyMap[OIS::KC_RMENU] = KC_RMENU;	// right Alt
    keyMap[OIS::KC_PAUSE] = KC_PAUSE;	// Pause
    keyMap[OIS::KC_HOME] = KC_HOME;	// Home on arrow keypad
    keyMap[OIS::KC_UP] = KC_UP;	// UpArrow on arrow keypad
    keyMap[OIS::KC_PGUP] = KC_PGUP;	// PgUp on arrow keypad
    keyMap[OIS::KC_LEFT] = KC_LEFT;	// LeftArrow on arrow keypad
    keyMap[OIS::KC_RIGHT] = KC_RIGHT;	// RightArrow on arrow // keypad
    keyMap[OIS::KC_END] = KC_END;	// End on arrow keypad
    keyMap[OIS::KC_DOWN] = KC_DOWN;	// DownArrow on arrow // keypad
    keyMap[OIS::KC_PGDOWN] = KC_PGDOWN;	// PgDn on arrow keypad
    keyMap[OIS::KC_INSERT] = KC_INSERT;	// Insert on arrow keypad
    keyMap[OIS::KC_DELETE] = KC_DELETE;	// Delete on arrow keypad
    keyMap[OIS::KC_LWIN] = KC_LWIN;	// Left Windows key
    keyMap[OIS::KC_RWIN] = KC_RWIN;	// Right Windows key
    keyMap[OIS::KC_APPS] = KC_APPS;	// AppMenu key
    keyMap[OIS::KC_POWER] = KC_POWER;	// System Power
    keyMap[OIS::KC_SLEEP] = KC_SLEEP;	// System Sleep
    // keyMap[OIS::KC_WAKE] = SDLK_WAKE;	// System Wake
    keyMap[OIS::KC_WEBSEARCH] = KC_WEBSEARCH;	// Web Search
    keyMap[OIS::KC_WEBFAVORITES] = KC_WEBFAVORITES;	// Web Favorites
    keyMap[OIS::KC_WEBREFRESH] = KC_WEBREFRESH;	// Web Refresh
    keyMap[OIS::KC_WEBSTOP] = KC_WEBSTOP;	// Web Stop
    keyMap[OIS::KC_WEBFORWARD] = KC_WEBFORWARD;	// Web Forward
    keyMap[OIS::KC_WEBBACK] = KC_WEBBACK;	// Web Back
    keyMap[OIS::KC_MYCOMPUTER] = KC_MYCOMPUTER;	// My Computer
    keyMap[OIS::KC_MAIL] = KC_MAIL;	// Mail
    keyMap[OIS::KC_MEDIASELECT] = KC_MEDIASELECT;	// Media Select

    mouseMap.clear();
    mouseMap[OIS::MB_Left] = MB_Left;
    mouseMap[OIS::MB_Left] = MB_Left;
    mouseMap[OIS::MB_Right] = MB_Right;
    mouseMap[OIS::MB_Button3] = MB_Button3;
    mouseMap[OIS::MB_Button4] = MB_Button4;
}

#else
boost::unordered::unordered_map<SDL_Keycode,EntKeyCode> keyMap;
boost::unordered::unordered_map<Uint8, EntKeyCode> mouseMap;

void initializeTranslationMaps()
{
    keyMap.clear();
    keyMap[SDLK_UNKNOWN] = KC_UNASSIGNED;
    keyMap[SDLK_ESCAPE] = KC_ESCAPE;
    keyMap[SDLK_1] = KC_1;
    keyMap[SDLK_2] = KC_2;
    keyMap[SDLK_3] = KC_3;
    keyMap[SDLK_4] = KC_4;
    keyMap[SDLK_5] = KC_5;
    keyMap[SDLK_6] = KC_6;
    keyMap[SDLK_7] = KC_7;
    keyMap[SDLK_8] = KC_8;
    keyMap[SDLK_9] = KC_9;
    keyMap[SDLK_0] = KC_0;
    keyMap[SDLK_MINUS] = KC_MINUS;	// - on main keyboard
    keyMap[SDLK_EQUALS] = KC_EQUALS;
    keyMap[SDLK_BACKSPACE] = KC_BACK;	// backspace
    keyMap[SDLK_TAB] = KC_TAB;
    keyMap[SDLK_q] = KC_Q;
    keyMap[SDLK_w] = KC_W;
    keyMap[SDLK_e] = KC_E;
    keyMap[SDLK_r] = KC_R;
    keyMap[SDLK_t] = KC_T;
    keyMap[SDLK_y] = KC_Y;
    keyMap[SDLK_u] = KC_U;
    keyMap[SDLK_i] = KC_I;
    keyMap[SDLK_o] = KC_O;
    keyMap[SDLK_p] = KC_P;
    keyMap[SDLK_LEFTBRACKET] = KC_LBRACKET;
    keyMap[SDLK_RIGHTBRACKET] = KC_RBRACKET;
    keyMap[SDLK_RETURN] = KC_RETURN;	// Enter on main keyboard
    keyMap[SDLK_LCTRL] = KC_LCONTROL;
    keyMap[SDLK_a] = KC_A;
    keyMap[SDLK_s] = KC_S;
    keyMap[SDLK_d] = KC_D;
    keyMap[SDLK_f] = KC_F;
    keyMap[SDLK_g] = KC_G;
    keyMap[SDLK_h] = KC_H;
    keyMap[SDLK_j] = KC_J;
    keyMap[SDLK_k] = KC_K;
    keyMap[SDLK_l] = KC_L;
    keyMap[SDLK_SEMICOLON] = KC_SEMICOLON;
    keyMap[SDLK_QUOTEDBL] = KC_APOSTROPHE;
    keyMap[SDLK_BACKQUOTE] = KC_GRAVE;	// accent
    keyMap[SDLK_LSHIFT] = KC_LSHIFT;
    keyMap[SDLK_BACKSLASH] = KC_BACKSLASH;
    keyMap[SDLK_z] = KC_Z;
    keyMap[SDLK_x] = KC_X;
    keyMap[SDLK_c] = KC_C;
    keyMap[SDLK_v] = KC_V;
    keyMap[SDLK_b] = KC_B;
    keyMap[SDLK_n] = KC_N;
    keyMap[SDLK_m] = KC_M;
    keyMap[SDLK_COMMA] = KC_COMMA;
    keyMap[SDLK_PERIOD] = KC_PERIOD;	//. on main keyboard
    keyMap[SDLK_SLASH] = KC_BACKSLASH;	// / on main keyboard
    keyMap[SDLK_RSHIFT] = KC_RSHIFT;
    keyMap[SDLK_ASTERISK] = KC_MULTIPLY;	// * on numeric keypad
    keyMap[SDLK_LALT] = KC_LMENU;	// left Alt
    keyMap[SDLK_SPACE] = KC_SPACE;
    keyMap[SDLK_CAPSLOCK] = KC_CAPITAL;
    keyMap[SDLK_F1] = KC_F1;
    keyMap[SDLK_F2] = KC_F2;
    keyMap[SDLK_F3] = KC_F3;
    keyMap[SDLK_F4] = KC_F4;
    keyMap[SDLK_F5] = KC_F5;
    keyMap[SDLK_F6] = KC_F6;
    keyMap[SDLK_F7] = KC_F7;
    keyMap[SDLK_F8] = KC_F8;
    keyMap[SDLK_F9] = KC_F9;
    keyMap[SDLK_F10] = KC_F10;
    keyMap[SDLK_NUMLOCKCLEAR] = KC_NUMLOCK;
    keyMap[SDLK_SCROLLLOCK] = KC_SCROLL;	// Scroll Lock
    keyMap[SDLK_KP_7] = KC_NUMPAD7;
    keyMap[SDLK_KP_8] = KC_NUMPAD8;
    keyMap[SDLK_KP_9] = KC_NUMPAD9;
    keyMap[SDLK_MINUS] = KC_SUBTRACT;	// - on numeric keypad
    keyMap[SDLK_KP_4] = KC_NUMPAD4;
    keyMap[SDLK_KP_5] = KC_NUMPAD5;
    keyMap[SDLK_KP_6] = KC_NUMPAD6;
    keyMap[SDLK_PLUS] = KC_ADD;	// + on numeric keypad
    keyMap[SDLK_KP_1] = KC_NUMPAD1;
    keyMap[SDLK_KP_2] = KC_NUMPAD2;
    keyMap[SDLK_KP_3] = KC_NUMPAD3;
    keyMap[SDLK_KP_0] = KC_NUMPAD0;
    keyMap[SDLK_DECIMALSEPARATOR] = KC_DECIMAL;	//. on numeric keypad
    keyMap[SDLK_LESS] = KC_OEM_102;	//<>|on UK/Germany // keyboards
    keyMap[SDLK_F11] = KC_F11;
    keyMap[SDLK_F12] = KC_F12;
    keyMap[SDLK_F13] = KC_F13;	// (NEC PC98
    keyMap[SDLK_F14] = KC_F14;	// (NEC PC98
    keyMap[SDLK_F15] = KC_F15;	// (NEC PC98
    // keyMap[SDLK_KANA] = SDLK_KANA;	// (Japanese keyboard
    keyMap[SDLK_SLASH] = KC_ABNT_C1;	// / ? on Portugese (Brazilian // keyboards
    // keyMap[KC_CONVERT] = SDLK_79;	// (Japanese keyboard
    // keyMap[KC_NOCONVERT] = SDLK_7B;	// (Japanese keyboard
    // keyMap[KC_YEN] = SDLK_7D;	// (Japanese keyboard
    keyMap[SDLK_DECIMALSEPARATOR] = KC_ABNT_C2;	// Numpad. on Portugese // (Brazilian keyboards
    keyMap[SDLK_KP_EQUALS] = KC_NUMPADEQUALS;	// = on numeric keypad // (NEC PC98
    keyMap[SDLK_AUDIOPREV] = KC_PREVTRACK;
    keyMap[SDLK_AT] = KC_AT;	// (NEC PC98
    keyMap[SDLK_COLON] = KC_COLON;	// (NEC PC98
    keyMap[SDLK_UNDERSCORE] = KC_UNDERLINE;	// (NEC PC98
    // keyMap[KC_KANJI] = SDLK_94;	// (Japanese keyboard
    keyMap[SDLK_STOP] = KC_STOP;	// (NEC PC98
    // keyMap[KC_AX] = SDLK_96;	// (Japan AX
    // keyMap[KC_UNLABELED] = SDLK_97;	// (J3100
    keyMap[SDLK_AUDIONEXT] = KC_NEXTTRACK;	// Next Track
    keyMap[SDLK_KP_ENTER] = KC_NUMPADENTER;	// Enter on numeric keypad
    keyMap[SDLK_RCTRL] = KC_RCONTROL;
    keyMap[SDLK_MUTE] = KC_MUTE;	// Mute
    keyMap[SDLK_CALCULATOR] = KC_CALCULATOR;	// Calculator
    keyMap[SDLK_AUDIOPLAY] = KC_PLAYPAUSE;	// Play/Pause
    keyMap[SDLK_AUDIOSTOP] = KC_MEDIASTOP;	// Media Stop
    keyMap[SDLK_VOLUMEDOWN] = KC_VOLUMEDOWN;	// Volume −
    keyMap[SDLK_VOLUMEUP] = KC_VOLUMEUP;	// Volume +
    keyMap[SDLK_AC_HOME] = KC_WEBHOME;	// Web home
    keyMap[SDLK_KP_COMMA] = KC_COMMA;	// on numeric keypad // (NEC PC98
    keyMap[SDLK_KP_DIVIDE] = KC_NUMPADCOMMA;	// / on numeric keypad
    keyMap[SDLK_SYSREQ] = KC_SYSRQ;
    keyMap[SDLK_RALT] = KC_RMENU;	// right Alt
    keyMap[SDLK_PAUSE] = KC_PAUSE;	// Pause
    keyMap[SDLK_HOME] = KC_HOME;	// Home on arrow keypad
    keyMap[SDLK_UP] = KC_UP;	// UpArrow on arrow keypad
    keyMap[SDLK_PAGEUP] = KC_PGUP;	// PgUp on arrow keypad
    keyMap[SDLK_LEFT] = KC_LEFT;	// LeftArrow on arrow keypad
    keyMap[SDLK_RIGHT] = KC_RIGHT;	// RightArrow on arrow // keypad
    keyMap[SDLK_END] = KC_END;	// End on arrow keypad
    keyMap[SDLK_DOWN] = KC_DOWN;	// DownArrow on arrow // keypad
    keyMap[SDLK_PAGEDOWN] = KC_PGDOWN;	// PgDn on arrow keypad
    keyMap[SDLK_INSERT] = KC_INSERT;	// Insert on arrow keypad
    keyMap[SDLK_DELETE] = KC_DELETE;	// Delete on arrow keypad
    keyMap[SDLK_LGUI] = KC_LWIN;	// Left Windows key
    keyMap[SDLK_RGUI] = KC_RWIN;	// Right Windows key
    keyMap[SDLK_MENU] = KC_APPS;	// AppMenu key
    keyMap[SDLK_POWER] = KC_POWER;	// System Power
    keyMap[SDLK_SLEEP] = KC_SLEEP;	// System Sleep
    // keyMap[KC_WAKE] = SDLK_WAKE;	// System Wake
    keyMap[SDLK_AC_SEARCH] = KC_WEBSEARCH;	// Web Search
    keyMap[SDLK_AC_BOOKMARKS] = KC_WEBFAVORITES;	// Web Favorites
    keyMap[SDLK_AC_REFRESH] = KC_WEBFAVORITES;	// Web Refresh
    keyMap[SDLK_AC_STOP] = KC_WEBSTOP;	// Web Stop
    keyMap[SDLK_AC_FORWARD] = KC_WEBFORWARD;	// Web Forward
    keyMap[SDLK_AC_BACK] = KC_WEBBACK;	// Web Back
    keyMap[SDLK_COMPUTER] = KC_MYCOMPUTER;	// My Computer
    keyMap[SDLK_MAIL] = KC_MAIL;	// Mail
    keyMap[SDLK_MEDIASELECT] = KC_MEDIASELECT;	// Media Select

    mouseMap.clear();
    mouseMap[SDL_BUTTON_LEFT] = MB_Left;
    mouseMap[SDL_BUTTON_MIDDLE] = MB_Middle;
    mouseMap[SDL_BUTTON_RIGHT] = MB_Right;
    mouseMap[SDL_BUTTON_X1] = MB_Button3;
    mouseMap[SDL_BUTTON_X2] = MB_Button3;
}

#endif
/*
    void initializeTranslationMaps()
    {
        keyMap.clear();
        keyMap[OIS::KC_UNASSIGNED] = SDLK_UNKNOWN;
        keyMap[OIS::KC_ESCAPE] = SDLK_ESCAPE;
        keyMap[OIS::KC_1] = SDLK_1;
        keyMap[OIS::KC_2] = SDLK_2;
        keyMap[OIS::KC_3] = SDLK_3;
        keyMap[OIS::KC_4] = SDLK_4;
        keyMap[OIS::KC_5] = SDLK_5;
        keyMap[OIS::KC_6] = SDLK_6;
        keyMap[OIS::KC_7] = SDLK_7;
        keyMap[OIS::KC_8] = SDLK_8;
        keyMap[OIS::KC_9] = SDLK_9;
        keyMap[OIS::KC_0] = SDLK_0;
        keyMap[OIS::KC_MINUS] = SDLK_MINUS;	// - on main keyboard
        keyMap[OIS::KC_EQUALS] = SDLK_EQUALS;
        keyMap[OIS::KC_BACK] = SDLK_BACKSPACE;	// backspace
        keyMap[OIS::KC_TAB] = SDLK_TAB;
        keyMap[OIS::KC_Q] = SDLK_q;
        keyMap[OIS::KC_W] = SDLK_w;
        keyMap[OIS::KC_E] = SDLK_e;
        keyMap[OIS::KC_R] = SDLK_r;
        keyMap[OIS::KC_T] = SDLK_t;
        keyMap[OIS::KC_Y] = SDLK_y;
        keyMap[OIS::KC_U] = SDLK_u;
        keyMap[OIS::KC_I] = SDLK_i;
        keyMap[OIS::KC_O] = SDLK_o;
        keyMap[OIS::KC_P] = SDLK_p;
        keyMap[OIS::KC_LBRACKET] = SDLK_LEFTBRACKET;
        keyMap[OIS::KC_RBRACKET] = SDLK_RIGHTBRACKET;
        keyMap[OIS::KC_RETURN] = SDLK_RETURN;	// Enter on main keyboard
        keyMap[OIS::KC_LCONTROL] = SDLK_LCTRL;
        keyMap[OIS::KC_A] = SDLK_a;
        keyMap[OIS::KC_S] = SDLK_s;
        keyMap[OIS::KC_D] = SDLK_d;
        keyMap[OIS::KC_F] = SDLK_f;
        keyMap[OIS::KC_G] = SDLK_g;
        keyMap[OIS::KC_H] = SDLK_h;
        keyMap[OIS::KC_J] = SDLK_j;
        keyMap[OIS::KC_K] = SDLK_k;
        keyMap[OIS::KC_L] = SDLK_l;
        keyMap[OIS::KC_SEMICOLON] = SDLK_SEMICOLON;
        keyMap[OIS::KC_APOSTROPHE] = SDLK_QUOTEDBL;
        keyMap[OIS::KC_GRAVE] = SDLK_BACKQUOTE;	// accent
        keyMap[OIS::KC_LSHIFT] = SDLK_LSHIFT;
        keyMap[OIS::KC_BACKSLASH] = SDLK_BACKSLASH;
        keyMap[OIS::KC_Z] = SDLK_z;
        keyMap[OIS::KC_X] = SDLK_x;
        keyMap[OIS::KC_C] = SDLK_c;
        keyMap[OIS::KC_V] = SDLK_v;
        keyMap[OIS::KC_B] = SDLK_b;
        keyMap[OIS::KC_N] = SDLK_n;
        keyMap[OIS::KC_M] = SDLK_m;
        keyMap[OIS::KC_COMMA] = SDLK_COMMA;
        keyMap[OIS::KC_PERIOD] = SDLK_PERIOD;	//. on main keyboard
        keyMap[OIS::KC_SLASH] = SDLK_SLASH;	// / on main keyboard
        keyMap[OIS::KC_RSHIFT] = SDLK_RSHIFT;
        keyMap[OIS::KC_MULTIPLY] = SDLK_ASTERISK;	// * on numeric keypad
        keyMap[OIS::KC_LMENU] = SDLK_LALT;	// left Alt
        keyMap[OIS::KC_SPACE] = SDLK_SPACE;
        keyMap[OIS::KC_CAPITAL] = SDLK_CAPSLOCK;
        keyMap[OIS::KC_F1] = SDLK_F1;
        keyMap[OIS::KC_F2] = SDLK_F2;
        keyMap[OIS::KC_F3] = SDLK_F3;
        keyMap[OIS::KC_F4] = SDLK_F4;
        keyMap[OIS::KC_F5] = SDLK_F5;
        keyMap[OIS::KC_F6] = SDLK_F6;
        keyMap[OIS::KC_F7] = SDLK_F7;
        keyMap[OIS::KC_F8] = SDLK_F8;
        keyMap[OIS::KC_F9] = SDLK_F9;
        keyMap[OIS::KC_F10] = SDLK_F10;
        keyMap[OIS::KC_NUMLOCK] = SDLK_NUMLOCKCLEAR;
        keyMap[OIS::KC_SCROLL] = SDLK_SCROLLLOCK;	// Scroll Lock
        keyMap[OIS::KC_NUMPAD7] = SDLK_KP_7;
        keyMap[OIS::KC_NUMPAD8] = SDLK_KP_8;
        keyMap[OIS::KC_NUMPAD9] = SDLK_KP_9;
        keyMap[OIS::KC_SUBTRACT] = SDLK_MINUS;	// - on numeric keypad
        keyMap[OIS::KC_NUMPAD4] = SDLK_KP_4;
        keyMap[OIS::KC_NUMPAD5] = SDLK_KP_5;
        keyMap[OIS::KC_NUMPAD6] = SDLK_KP_6;
        keyMap[OIS::KC_ADD] = SDLK_PLUS;	// + on numeric keypad
        keyMap[OIS::KC_NUMPAD1] = SDLK_KP_1;
        keyMap[OIS::KC_NUMPAD2] = SDLK_KP_2;
        keyMap[OIS::KC_NUMPAD3] = SDLK_KP_3;
        keyMap[OIS::KC_NUMPAD0] = SDLK_KP_0;
        keyMap[OIS::KC_DECIMAL] = SDLK_DECIMALSEPARATOR;	//. on numeric keypad
        keyMap[OIS::KC_OEM_102] = SDLK_LESS;	//<>|on UK/Germany // keyboards
        keyMap[OIS::KC_F11] = SDLK_F11;
        keyMap[OIS::KC_F12] = SDLK_F12;
        keyMap[OIS::KC_F13] = SDLK_F13;	// (NEC PC98
        keyMap[OIS::KC_F14] = SDLK_F14;	// (NEC PC98
        keyMap[OIS::KC_F15] = SDLK_F15;	// (NEC PC98
        // keyMap[OIS::KC_KANA] = SDLK_KANA;	// (Japanese keyboard
        keyMap[OIS::KC_ABNT_C1] = SDLK_SLASH;	// / ? on Portugese (Brazilian // keyboards
        // keyMap[OIS::KC_CONVERT] = SDLK_79;	// (Japanese keyboard
        // keyMap[OIS::KC_NOCONVERT] = SDLK_7B;	// (Japanese keyboard
        // keyMap[OIS::KC_YEN] = SDLK_7D;	// (Japanese keyboard
        keyMap[OIS::KC_ABNT_C2] = SDLK_DECIMALSEPARATOR;	// Numpad. on Portugese // (Brazilian keyboards
        keyMap[OIS::KC_NUMPADEQUALS] = SDLK_KP_EQUALS;	// = on numeric keypad // (NEC PC98
        keyMap[OIS::KC_PREVTRACK] = SDLK_AUDIOPREV;
        keyMap[OIS::KC_AT] = SDLK_AT;	// (NEC PC98
        keyMap[OIS::KC_COLON] = SDLK_COLON;	// (NEC PC98
        keyMap[OIS::KC_UNDERLINE] = SDLK_UNDERSCORE;	// (NEC PC98
        // keyMap[OIS::KC_KANJI] = SDLK_94;	// (Japanese keyboard
        keyMap[OIS::KC_STOP] = SDLK_STOP;	// (NEC PC98
        // keyMap[OIS::KC_AX] = SDLK_96;	// (Japan AX
        // keyMap[OIS::KC_UNLABELED] = SDLK_97;	// (J3100
        keyMap[OIS::KC_NEXTTRACK] = SDLK_AUDIONEXT;	// Next Track
        keyMap[OIS::KC_NUMPADENTER] = SDLK_KP_ENTER;	// Enter on numeric keypad
        keyMap[OIS::KC_RCONTROL] = SDLK_RCTRL;
        keyMap[OIS::KC_MUTE] = SDLK_MUTE;	// Mute
        keyMap[OIS::KC_CALCULATOR] = SDLK_CALCULATOR;	// Calculator
        keyMap[OIS::KC_PLAYPAUSE] = SDLK_AUDIOPLAY;	// Play/Pause
        keyMap[OIS::KC_MEDIASTOP] = SDLK_AUDIOSTOP;	// Media Stop
        keyMap[OIS::KC_VOLUMEDOWN] = SDLK_VOLUMEDOWN;	// Volume −
        keyMap[OIS::KC_VOLUMEUP] = SDLK_VOLUMEUP;	// Volume +
        keyMap[OIS::KC_WEBHOME] = SDLK_AC_HOME;	// Web home
        keyMap[OIS::KC_NUMPADCOMMA] = SDLK_KP_COMMA;	// on numeric keypad // (NEC PC98
        keyMap[OIS::KC_DIVIDE] = SDLK_KP_DIVIDE;	// / on numeric keypad
        keyMap[OIS::KC_SYSRQ] = SDLK_SYSREQ;
        keyMap[OIS::KC_RMENU] = SDLK_RALT;	// right Alt
        keyMap[OIS::KC_PAUSE] = SDLK_PAUSE;	// Pause
        keyMap[OIS::KC_HOME] = SDLK_HOME;	// Home on arrow keypad
        keyMap[OIS::KC_UP] = SDLK_UP;	// UpArrow on arrow keypad
        keyMap[OIS::KC_PGUP] = SDLK_PAGEUP;	// PgUp on arrow keypad
        keyMap[OIS::KC_LEFT] = SDLK_LEFT;	// LeftArrow on arrow keypad
        keyMap[OIS::KC_RIGHT] = SDLK_RIGHT;	// RightArrow on arrow // keypad
        keyMap[OIS::KC_END] = SDLK_END;	// End on arrow keypad
        keyMap[OIS::KC_DOWN] = SDLK_DOWN;	// DownArrow on arrow // keypad
        keyMap[OIS::KC_PGDOWN] = SDLK_PAGEDOWN;	// PgDn on arrow keypad
        keyMap[OIS::KC_INSERT] = SDLK_INSERT;	// Insert on arrow keypad
        keyMap[OIS::KC_DELETE] = SDLK_DELETE;	// Delete on arrow keypad
        keyMap[OIS::KC_LWIN] = SDLK_LGUI;	// Left Windows key
        keyMap[OIS::KC_RWIN] = SDLK_RGUI;	// Right Windows key
        keyMap[OIS::KC_APPS] = SDLK_MENU;	// AppMenu key
        keyMap[OIS::KC_POWER] = SDLK_POWER;	// System Power
        keyMap[OIS::KC_SLEEP] = SDLK_SLEEP;	// System Sleep
        // keyMap[OIS::KC_WAKE] = SDLK_WAKE;	// System Wake
        keyMap[OIS::KC_WEBSEARCH] = SDLK_AC_SEARCH;	// Web Search
        keyMap[OIS::KC_WEBFAVORITES] = SDLK_AC_BOOKMARKS;	// Web Favorites
        keyMap[OIS::KC_WEBREFRESH] = SDLK_AC_REFRESH;	// Web Refresh
        keyMap[OIS::KC_WEBSTOP] = SDLK_AC_STOP;	// Web Stop
        keyMap[OIS::KC_WEBFORWARD] = SDLK_AC_FORWARD;	// Web Forward
        keyMap[OIS::KC_WEBBACK] = SDLK_AC_BACK;	// Web Back
        keyMap[OIS::KC_MYCOMPUTER] = SDLK_COMPUTER;	// My Computer
        keyMap[OIS::KC_MAIL] = SDLK_MAIL;	// Mail
        keyMap[OIS::KC_MEDIASELECT] = SDLK_MEDIASELECT;	// Media Select

        mouseMap.clear();
        mouseMap[OIS::MB_Left] = SDL_BUTTON_LEFT;
        mouseMap[OIS::MB_Middle] = SDL_BUTTON_MIDDLE;
        mouseMap[OIS::MB_Right] = SDL_BUTTON_RIGHT;
        mouseMap[OIS::MB_Button3] = SDL_BUTTON_X1;
        mouseMap[OIS::MB_Button4] = SDL_BUTTON_X2;
    }
    */
}