#pragma once

#include <cstdint>

namespace engine {

using KeyState = uint32_t;
namespace KeyStates {

inline constexpr KeyState Up = 0;
inline constexpr KeyState Pressed = 1;
inline constexpr KeyState Down = 2;
inline constexpr KeyState Released = 3;

}  // namespace KeyStates

using Key = uint32_t;
namespace KeyCodes {

inline constexpr Key Unknown = 0;

// Letters
inline constexpr Key A = 4;
inline constexpr Key B = 5;
inline constexpr Key C = 6;
inline constexpr Key D = 7;
inline constexpr Key E = 8;
inline constexpr Key F = 9;
inline constexpr Key G = 10;
inline constexpr Key H = 11;
inline constexpr Key I = 12;
inline constexpr Key J = 13;
inline constexpr Key K = 14;
inline constexpr Key L = 15;
inline constexpr Key M = 16;
inline constexpr Key N = 17;
inline constexpr Key O = 18;
inline constexpr Key P = 19;
inline constexpr Key Q = 20;
inline constexpr Key R = 21;
inline constexpr Key S = 22;
inline constexpr Key T = 23;
inline constexpr Key U = 24;
inline constexpr Key V = 25;
inline constexpr Key W = 26;
inline constexpr Key X = 27;
inline constexpr Key Y = 28;
inline constexpr Key Z = 29;

// Numbers
inline constexpr Key Num1 = 30;
inline constexpr Key Num2 = 31;
inline constexpr Key Num3 = 32;
inline constexpr Key Num4 = 33;
inline constexpr Key Num5 = 34;
inline constexpr Key Num6 = 35;
inline constexpr Key Num7 = 36;
inline constexpr Key Num8 = 37;
inline constexpr Key Num9 = 38;
inline constexpr Key Num0 = 39;

// UI / Controls
inline constexpr Key Return = 40;
inline constexpr Key Escape = 41;
inline constexpr Key Backspace = 42;
inline constexpr Key Tab = 43;
inline constexpr Key Space = 44;
inline constexpr Key Minus = 45;
inline constexpr Key Equals = 46;
inline constexpr Key LeftBracket = 47;
inline constexpr Key RightBracket = 48;
inline constexpr Key Backslash = 49;
inline constexpr Key Semicolon = 51;
inline constexpr Key Apostrophe = 52;
inline constexpr Key Grave = 53;
inline constexpr Key Comma = 54;
inline constexpr Key Period = 55;
inline constexpr Key Slash = 56;
inline constexpr Key CapsLock = 57;

// Function Keys
inline constexpr Key F1 = 58;
inline constexpr Key F2 = 59;
inline constexpr Key F3 = 60;
inline constexpr Key F4 = 61;
inline constexpr Key F5 = 62;
inline constexpr Key F6 = 63;
inline constexpr Key F7 = 64;
inline constexpr Key F8 = 65;
inline constexpr Key F9 = 66;
inline constexpr Key F10 = 67;
inline constexpr Key F11 = 68;
inline constexpr Key F12 = 69;

// Navigation
inline constexpr Key PrintScreen = 70;
inline constexpr Key ScrollLock = 71;
inline constexpr Key Pause = 72;
inline constexpr Key Insert = 73;
inline constexpr Key Home = 74;
inline constexpr Key PageUp = 75;
inline constexpr Key Delete = 76;
inline constexpr Key End = 77;
inline constexpr Key PageDown = 78;
inline constexpr Key Right = 79;
inline constexpr Key Left = 80;
inline constexpr Key Down = 81;
inline constexpr Key Up = 82;

// Numpad
inline constexpr Key NumLock = 83;
inline constexpr Key KP_Divide = 84;
inline constexpr Key KP_Multiply = 85;
inline constexpr Key KP_Minus = 86;
inline constexpr Key KP_Plus = 87;
inline constexpr Key KP_Enter = 88;
inline constexpr Key KP_1 = 89;
inline constexpr Key KP_2 = 90;
inline constexpr Key KP_3 = 91;
inline constexpr Key KP_4 = 92;
inline constexpr Key KP_5 = 93;
inline constexpr Key KP_6 = 94;
inline constexpr Key KP_7 = 95;
inline constexpr Key KP_8 = 96;
inline constexpr Key KP_9 = 97;
inline constexpr Key KP_0 = 98;
inline constexpr Key KP_Period = 99;

// Modifiers
inline constexpr Key LeftCtrl = 224;
inline constexpr Key LeftShift = 225;
inline constexpr Key LeftAlt = 226;
inline constexpr Key LeftGUI = 227;
inline constexpr Key RightCtrl = 228;
inline constexpr Key RightShift = 229;
inline constexpr Key RightAlt = 230;
inline constexpr Key RightGUI = 231;

inline constexpr Key Count = 512;

}  // namespace KeyCodes

}  // namespace engine
