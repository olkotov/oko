// Oleg Kotov

#pragma once

#include <stdint.h>

namespace Input
{

enum class Key : uint8_t
{
    AnyKey,
    
    // Character Keys
    
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    
    // Arrow keys
    
    Left,
    Up,
    Right,
    Down,
    
    // Numbers
    
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,

    NumPadZero,
    NumPadOne,
    NumPadTwo,
    NumPadThree,
    NumPadFour,
    NumPadFive,
    NumPadSix,
    NumPadSeven,
    NumPadEight,
    NumPadNine,
    
    // Num Pad
    
    Multiply,
    Add,
    Subtract,
    Decimal,
    Divide,

    BackSpace,
    Tab,
    Enter,
    Pause,

    NumLock,
    ScrollLock,
    CapsLock,

    Escape,
    SpaceBar,
    PageUp,
    PageDown,
    End,
    Home,
    Insert,
    Delete,
    
    // Function Keys
    
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    
    // Modifier Keys
    
    LeftShift,
    RightShift,
    LeftControl,
    RightControl,
    LeftAlt,
    RightAlt,
    LeftCommand,
    RightCommand,
    
    // Misc Keys
    
    Semicolon,
    Equals,
    Comma,
    Underscore,
    Period,
    Slash,
    Tilde,
    LeftBracket,
    Backslash,
    RightBracket,
    Quote,

    // Mouse
    
    LeftMouseButton,
    RightMouseButton,
    MiddleMouseButton,
    ThumbMouseButton,
    ThumbMouseButton2,
    
    // Gamepads
    
    Gamepad_LeftX,
    Gamepad_LeftY,
    Gamepad_RightX,
    Gamepad_RightY,
    Gamepad_LeftTriggerAxis,
    Gamepad_RightTriggerAxis,

    Gamepad_LeftThumbstick,
    Gamepad_RightThumbstick,
    Gamepad_Special_Left,
    Gamepad_Special_Right,
    Gamepad_FaceButton_Bottom,
    Gamepad_FaceButton_Right,
    Gamepad_FaceButton_Left,
    Gamepad_FaceButton_Top,
    Gamepad_LeftShoulder,
    Gamepad_RightShoulder,
    Gamepad_LeftTrigger,
    Gamepad_RightTrigger,
    Gamepad_DPad_Up,
    Gamepad_DPad_Down,
    Gamepad_DPad_Right,
    Gamepad_DPad_Left,
    
    MouseX,
    MouseY,
    Mouse2D,
    MouseScrollUp,
    MouseScrollDown,
    MouseWheelAxis,
    
    NumKeys
};

};

Input::Key SDLKeycodeToKey( int32_t keycode );
Input::Key SDLScancodeToKey( int32_t scancode );
Input::Key SDLMouseButtonToKey( int32_t button );

