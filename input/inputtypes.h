// Oleg Kotov

#pragma once

enum InputContext // InputCategory
{
    InputContext_MainMenu, // frontend / ui
    InputContext_Game
};

enum InputAction
{
    // movement

    InputAction_MoveForward,
    InputAction_MoveBack,
    InputAction_MoveLeft,
    InputAction_MoveRight,
    InputAction_Jump,
    InputAction_Duck, // crouch
    InputAction_Run, // sprint

    // combat
    InputAction_PrimaryAttack,
    InputAction_SecondaryAttack,
    InputAction_SpecialAttack,
    InputAction_Reload,

    // 
    InputAction_Use,

    // misc
    InputAction_Screenshot,
    InputAction_QuickSave,
    InputAction_QuickLoad
};

enum Key
{
    Key_W,
    Key_A,
    Key_S,
    Key_D,
    Key_Space,
    Key_E,
    Key_Mouse1,
    Key_Mouse2,
    Key_F5
};

enum SoundChannel
{
    SoundChannel_General,
    SoundChannel_Voice,
    SoundChannel_Music,
    SoundChannel_SFX
};

