// Oleg Kotov

#include "keys.h"
#include <SDL2/SDL.h>

using namespace Input;

Key SDLKeycodeToKey( int32_t keycode )
{
    switch ( keycode )
    {
        case SDLK_a: return Key::A;
        case SDLK_b: return Key::B;
        case SDLK_c: return Key::C;
        case SDLK_d: return Key::D;
        case SDLK_e: return Key::E;
        case SDLK_f: return Key::F;
        case SDLK_g: return Key::G;
        case SDLK_h: return Key::H;
        case SDLK_i: return Key::I;
        case SDLK_j: return Key::J;
        case SDLK_k: return Key::K;
        case SDLK_l: return Key::L;
        case SDLK_m: return Key::M;
        case SDLK_n: return Key::N;
        case SDLK_o: return Key::O;
        case SDLK_p: return Key::P;
        case SDLK_q: return Key::Q;
        case SDLK_r: return Key::R;
        case SDLK_s: return Key::S;
        case SDLK_t: return Key::T;
        case SDLK_u: return Key::U;
        case SDLK_v: return Key::V;
        case SDLK_w: return Key::W;
        case SDLK_x: return Key::X;
        case SDLK_y: return Key::Y;
        case SDLK_z: return Key::Z;
            
        case SDLK_LEFT:  return Key::Left;
        case SDLK_UP:    return Key::Up;
        case SDLK_RIGHT: return Key::Right;
        case SDLK_DOWN:  return Key::Down;
        
        case SDLK_0: return Key::Zero;
        case SDLK_1: return Key::One;
        case SDLK_2: return Key::Two;
        case SDLK_3: return Key::Three;
        case SDLK_4: return Key::Four;
        case SDLK_5: return Key::Five;
        case SDLK_6: return Key::Six;
        case SDLK_7: return Key::Seven;
        case SDLK_8: return Key::Eight;
        case SDLK_9: return Key::Nine;
            
        case SDLK_KP_MULTIPLY:    return Key::Multiply;
        case SDLK_KP_MEMADD:      return Key::Add;
        case SDLK_KP_MEMSUBTRACT: return Key::Subtract;
        case SDLK_KP_DECIMAL:     return Key::Decimal;
        case SDLK_KP_DIVIDE:      return Key::Divide;
            
        case SDLK_BACKSPACE: return Key::BackSpace;
        case SDLK_TAB:       return Key::Tab;
        case SDLK_KP_ENTER:  return Key::Enter;
        case SDLK_PAUSE:     return Key::Pause;

        case SDLK_NUMLOCKCLEAR: return Key::NumLock;
        case SDLK_SCROLLLOCK:   return Key::ScrollLock;
        case SDLK_CAPSLOCK:     return Key::CapsLock;

        case SDLK_ESCAPE:   return Key::Escape;
        case SDLK_SPACE:    return Key::SpaceBar;
        case SDLK_PAGEUP:   return Key::PageUp;
        case SDLK_PAGEDOWN: return Key::PageDown;
        case SDLK_END:      return Key::End;
        case SDLK_HOME:     return Key::Home;
        case SDLK_INSERT:   return Key::Insert;
        case SDLK_DELETE:   return Key::Delete;

        case SDLK_F1:  return Key::F1;
        case SDLK_F2:  return Key::F2;
        case SDLK_F3:  return Key::F3;
        case SDLK_F4:  return Key::F4;
        case SDLK_F5:  return Key::F5;
        case SDLK_F6:  return Key::F6;
        case SDLK_F7:  return Key::F7;
        case SDLK_F8:  return Key::F8;
        case SDLK_F9:  return Key::F9;
        case SDLK_F10: return Key::F10;
        case SDLK_F11: return Key::F11;
        case SDLK_F12: return Key::F12;

        case SDLK_LSHIFT: return Key::LeftShift;
        case SDLK_RSHIFT: return Key::RightShift;
        case SDLK_LCTRL:  return Key::LeftControl;
        case SDLK_RCTRL:  return Key::RightControl;
        case SDLK_LALT:   return Key::LeftAlt;
        case SDLK_RALT:   return Key::RightAlt;
        case SDLK_LGUI:   return Key::LeftCommand;
        case SDLK_RGUI:   return Key::RightCommand;

        case SDLK_SEMICOLON:    return Key::Semicolon;
        case SDLK_EQUALS:       return Key::Equals;
        case SDLK_COMMA:        return Key::Comma;
        case SDLK_UNDERSCORE:   return Key::Underscore;
        case SDLK_PERIOD:       return Key::Period;
        case SDLK_SLASH:        return Key::Slash;
        case SDLK_BACKQUOTE:    return Key::Tilde;
        case SDLK_LEFTBRACKET:  return Key::LeftBracket;
        case SDLK_BACKSLASH:    return Key::Backslash;
        case SDLK_RIGHTBRACKET: return Key::RightBracket;
        case SDLK_QUOTE:        return Key::Quote;
            
        default: return Key::AnyKey;
    }
}

Key SDLScancodeToKey( int32_t scancode )
{
    switch ( scancode )
    {
        case SDL_SCANCODE_A: return Key::A;
        case SDL_SCANCODE_B: return Key::B;
        case SDL_SCANCODE_C: return Key::C;
        case SDL_SCANCODE_D: return Key::D;
        case SDL_SCANCODE_E: return Key::E;
        case SDL_SCANCODE_F: return Key::F;
        case SDL_SCANCODE_G: return Key::G;
        case SDL_SCANCODE_H: return Key::H;
        case SDL_SCANCODE_I: return Key::I;
        case SDL_SCANCODE_J: return Key::J;
        case SDL_SCANCODE_K: return Key::K;
        case SDL_SCANCODE_L: return Key::L;
        case SDL_SCANCODE_M: return Key::M;
        case SDL_SCANCODE_N: return Key::N;
        case SDL_SCANCODE_O: return Key::O;
        case SDL_SCANCODE_P: return Key::P;
        case SDL_SCANCODE_Q: return Key::Q;
        case SDL_SCANCODE_R: return Key::R;
        case SDL_SCANCODE_S: return Key::S;
        case SDL_SCANCODE_T: return Key::T;
        case SDL_SCANCODE_U: return Key::U;
        case SDL_SCANCODE_V: return Key::V;
        case SDL_SCANCODE_W: return Key::W;
        case SDL_SCANCODE_X: return Key::X;
        case SDL_SCANCODE_Y: return Key::Y;
        case SDL_SCANCODE_Z: return Key::Z;
            
        case SDL_SCANCODE_LEFT:  return Key::Left;
        case SDL_SCANCODE_UP:    return Key::Up;
        case SDL_SCANCODE_RIGHT: return Key::Right;
        case SDL_SCANCODE_DOWN:  return Key::Down;
        
        case SDL_SCANCODE_0: return Key::Zero;
        case SDL_SCANCODE_1: return Key::One;
        case SDL_SCANCODE_2: return Key::Two;
        case SDL_SCANCODE_3: return Key::Three;
        case SDL_SCANCODE_4: return Key::Four;
        case SDL_SCANCODE_5: return Key::Five;
        case SDL_SCANCODE_6: return Key::Six;
        case SDL_SCANCODE_7: return Key::Seven;
        case SDL_SCANCODE_8: return Key::Eight;
        case SDL_SCANCODE_9: return Key::Nine;

        case SDL_SCANCODE_KP_MULTIPLY:    return Key::Multiply;
        case SDL_SCANCODE_KP_MEMADD:      return Key::Add;
        case SDL_SCANCODE_KP_MEMSUBTRACT: return Key::Subtract;
        case SDL_SCANCODE_KP_DECIMAL:     return Key::Decimal;
        case SDL_SCANCODE_KP_DIVIDE:      return Key::Divide;
 
        case SDL_SCANCODE_BACKSPACE: return Key::BackSpace;
        case SDL_SCANCODE_TAB:       return Key::Tab;
        case SDL_SCANCODE_KP_ENTER:  return Key::Enter;
        case SDL_SCANCODE_PAUSE:     return Key::Pause;
            
        case SDL_SCANCODE_NUMLOCKCLEAR: return Key::NumLock;
        case SDL_SCANCODE_SCROLLLOCK:   return Key::ScrollLock;
        case SDL_SCANCODE_CAPSLOCK:     return Key::CapsLock;

        case SDL_SCANCODE_ESCAPE:   return Key::Escape;
        case SDL_SCANCODE_SPACE:    return Key::SpaceBar;
        case SDL_SCANCODE_PAGEUP:   return Key::PageUp;
        case SDL_SCANCODE_PAGEDOWN: return Key::PageDown;
        case SDL_SCANCODE_END:      return Key::End;
        case SDL_SCANCODE_HOME:     return Key::Home;
        case SDL_SCANCODE_INSERT:   return Key::Insert;
        case SDL_SCANCODE_DELETE:   return Key::Delete;

        case SDL_SCANCODE_F1:  return Key::F1;
        case SDL_SCANCODE_F2:  return Key::F2;
        case SDL_SCANCODE_F3:  return Key::F3;
        case SDL_SCANCODE_F4:  return Key::F4;
        case SDL_SCANCODE_F5:  return Key::F5;
        case SDL_SCANCODE_F6:  return Key::F6;
        case SDL_SCANCODE_F7:  return Key::F7;
        case SDL_SCANCODE_F8:  return Key::F8;
        case SDL_SCANCODE_F9:  return Key::F9;
        case SDL_SCANCODE_F10: return Key::F10;
        case SDL_SCANCODE_F11: return Key::F11;
        case SDL_SCANCODE_F12: return Key::F12;

        case SDL_SCANCODE_LSHIFT: return Key::LeftShift;
        case SDL_SCANCODE_RSHIFT: return Key::RightShift;
        case SDL_SCANCODE_LCTRL:  return Key::LeftControl;
        case SDL_SCANCODE_RCTRL:  return Key::RightControl;
        case SDL_SCANCODE_LALT:   return Key::LeftAlt;
        case SDL_SCANCODE_RALT:   return Key::RightAlt;
        case SDL_SCANCODE_LGUI:   return Key::LeftCommand;
        case SDL_SCANCODE_RGUI:   return Key::RightCommand;

        case SDL_SCANCODE_SEMICOLON:    return Key::Semicolon;
        case SDL_SCANCODE_EQUALS:       return Key::Equals;
        case SDL_SCANCODE_COMMA:        return Key::Comma;
        // case SDL_SCANCODE_UNDERSCORE:   return Key::Underscore;
        case SDL_SCANCODE_PERIOD:       return Key::Period;
        case SDL_SCANCODE_SLASH:        return Key::Slash;
        // case SDL_SCANCODE_BACKQUOTE:    return Key::Tilde;
        case SDL_SCANCODE_LEFTBRACKET:  return Key::LeftBracket;
        case SDL_SCANCODE_BACKSLASH:    return Key::Backslash;
        case SDL_SCANCODE_RIGHTBRACKET: return Key::RightBracket;
        // case SDL_SCANCODE_QUOTE:        return Key::Quote;
            
        default: return Key::AnyKey;
    }
}

Key SDLMouseButtonToKey( int32_t button )
{
    switch ( button )
    {
        case SDL_BUTTON_LEFT:   return Key::LeftMouseButton;
        case SDL_BUTTON_RIGHT:  return Key::RightMouseButton;
        case SDL_BUTTON_MIDDLE: return Key::MiddleMouseButton;
        case SDL_BUTTON_X1:     return Key::ThumbMouseButton;
        case SDL_BUTTON_X2:     return Key::ThumbMouseButton2;

        default: return Key::AnyKey;
    }
}

