#pragma once

#include "../MexTK/mex.h"
#include "utils.h"

typedef struct CPUAction CPUAction;

struct CPUAction {
    u16 state; // state to perform this action. -1 for last
    u8 frame_low; // first possible frame to perform this action
    u8 frame_hi; // last possible frame to perfrom this action
    s8 lstick_x; // left stick X value
    s8 lstick_y; // left stick Y value
    s8 cstick_x; // c stick X value
    s8 cstick_y; // c stick Y value
    int input; // button to input
    unsigned char is_last: 1; // flag to indicate this was the final input
    unsigned char stick_dir: 3; // 0 = none, 1 = towards opponent, 2 = away from opponent, 3 = forward, 4 = backward
};

// Stick Direction Definitions
enum STICKDIR {
    STCKDIR_NONE,
    STCKDIR_TOWARD,
    STCKDIR_AWAY,
    STCKDIR_FRONT,
    STCKDIR_BACK,
    STICKDIR_RDM,
};

enum CPU_ACTIONS {
    CPUACT_NONE,
    CPUACT_SHIELD,
    CPUACT_GRAB,
    CPUACT_UPB,
    CPUACT_DOWNB,
    CPUACT_SPOTDODGE,
    CPUACT_ROLLAWAY,
    CPUACT_ROLLTOWARDS,
    CPUACT_ROLLRDM,
    CPUACT_NAIR,
    CPUACT_FAIR,
    CPUACT_DAIR,
    CPUACT_BAIR,
    CPUACT_UAIR,
    CPUACT_SHORTHOP,
    CPUACT_FULLHOP,
    CPUACT_JUMPAWAY,
    CPUACT_JUMPTOWARDS,
    CPUACT_AIRDODGE,
    CPUACT_FFTUMBLE,
    CPUACT_FFWIGGLE,
    CPUACT_JAB,
    CPUACT_FTILT,
    CPUACT_UTILT,
    CPUACT_DTILT,
    CPUACT_USMASH,
    CPUACT_DSMASH,
    CPUACT_FSMASH,
};

static char *CPU_ACTIONS_NAMES[] = {
    "CPUACT_NONE",
    "CPUACT_SHIELD",
    "CPUACT_GRAB",
    "CPUACT_UPB",
    "CPUACT_DOWNB",
    "CPUACT_SPOTDODGE",
    "CPUACT_ROLLAWAY",
    "CPUACT_ROLLTOWARDS",
    "CPUACT_ROLLRDM",
    "CPUACT_NAIR",
    "CPUACT_FAIR",
    "CPUACT_DAIR",
    "CPUACT_BAIR",
    "CPUACT_UAIR",
    "CPUACT_SHORTHOP",
    "CPUACT_FULLHOP",
    "CPUACT_JUMPAWAY",
    "CPUACT_JUMPTOWARDS",
    "CPUACT_AIRDODGE",
    "CPUACT_FFTUMBLE",
    "CPUACT_FFWIGGLE",
    "CPUACT_JAB",
    "CPUACT_FTILT",
    "CPUACT_UTILT",
    "CPUACT_DTILT",
    "CPUACT_USMASH",
    "CPUACT_DSMASH",
    "CPUACT_FSMASH",
};

// CPU Action Definitions
static CPUAction CPUAction_ActionShield[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_GUARDREFLECT, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionGrab[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_A | PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_Z, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionUpB[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_KNEEBEND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        127, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_B, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLE, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        127, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_B, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionDownB[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLE, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        -127, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_B, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionSpotdodge[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        -127, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionRollAway[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_AWAY, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_GUARDREFLECT, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_AWAY, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionRollTowards[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_TOWARD, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_GUARDREFLECT, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_TOWARD, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionRollRandom[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STICKDIR_RDM, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_GUARDREFLECT, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STICKDIR_RDM, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionNair[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_A, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionFair[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        127, // c stick X value
        0, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_FRONT, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionDair[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        -127, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionBair[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        127, // c stick X value
        0, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_BACK, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionUair[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        127, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionJump[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionJumpFull[] = {
    {
        ASID_GUARD, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R | PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_KNEEBEND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionJumpAway[] = {
    {
        ASID_JUMPS, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        0, // button to input
        0, // is the last input
        STCKDIR_AWAY, // specify stick direction
    },
    {
        ASID_ACTIONABLE, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_AWAY, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionJumpTowards[] = {
    {
        ASID_JUMPS, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        0, // button to input
        0, // is the last input
        STCKDIR_TOWARD, // specify stick direction
    },
    {
        ASID_ACTIONABLE, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_X, // button to input
        0, // is the last input
        STCKDIR_TOWARD, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionAirdodge[] = {
    {
        ASID_DAMAGEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        0, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_TRIGGER_R, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionFFTumble[] = {
    {
        ASID_DAMAGEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        -127, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionFFWiggle[] = {
    {
        ASID_DAMAGEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        127, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        0, // button to input
        0, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    {
        ASID_ACTIONABLEAIR, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        -127, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionJab[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_A, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionFTilt[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        80, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_A, // button to input
        1, // is the last input
        STCKDIR_TOWARD, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionUTilt[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        80, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_A, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionDTilt[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        -80, // left stick Y value
        0, // c stick X value
        0, // c stick Y value
        PAD_BUTTON_A, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionUSmash[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        127, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionDSmash[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        0, // c stick X value
        -127, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_NONE, // specify stick direction
    },
    -1,
};

static CPUAction CPUAction_ActionFSmash[] = {
    {
        ASID_ACTIONABLEGROUND, // state to perform this action. -1 for last
        0, // first possible frame to perform this action
        0, // last possible frame to perfrom this action
        0, // left stick X value
        0, // left stick Y value
        127, // c stick X value
        0, // c stick Y value
        0, // button to input
        1, // is the last input
        STCKDIR_TOWARD, // specify stick direction
    },
    -1,
};

static CPUAction *CPUAction_Actions[] = {
    0, // none 0
    CPUAction_ActionShield, // shield 1
    CPUAction_ActionGrab, // grab 2
    CPUAction_ActionUpB, // up b 3
    CPUAction_ActionDownB, // down b 4
    CPUAction_ActionSpotdodge, // spotdodge 5
    CPUAction_ActionRollAway, // roll away 6
    CPUAction_ActionRollTowards, // roll towards 7
    CPUAction_ActionRollRandom, // roll random
    CPUAction_ActionNair, // nair 8
    CPUAction_ActionFair, // fair 9
    CPUAction_ActionDair, // dair 10
    CPUAction_ActionBair, // bair 11
    CPUAction_ActionUair, // uair 12
    CPUAction_ActionJump, // short hop 13
    CPUAction_ActionJumpFull, // full hop 14
    CPUAction_ActionJumpAway, // jump away 15
    CPUAction_ActionJumpTowards, // jump towards 16
    CPUAction_ActionAirdodge, // airdodge 17
    CPUAction_ActionFFTumble, // fastfall 18
    CPUAction_ActionFFWiggle, // wiggle fastfall 19
    CPUAction_ActionJab, // wiggle fastfall 19
    CPUAction_ActionFTilt,
    CPUAction_ActionUTilt,
    CPUAction_ActionDTilt,
    CPUAction_ActionUSmash,
    CPUAction_ActionDSmash,
    CPUAction_ActionFSmash,
};

int CPUAction_PerformAction(GOBJ *cpu, int action_id, GOBJ *hmn);

int CPUAction_CheckMultipleState(GOBJ *cpu, int group_kind);