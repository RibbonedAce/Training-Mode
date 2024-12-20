#pragma once

#include "../../../../MexTK/mex.h"
#include "../../../tmdata/source/events.h"
#include "../../../utils.h"
#include "../../../cpu.h"

// HUD Proportions
#define PFSHJOBJ_TEXTSCALE 3
#define PFSHJOBJ_BARNUM 48
#define PFSHJOBJ_BARSCALEX 1.5
#define PFSHJOBJ_BARSCALEY 1.5
#define PFSHJOBJ_BARTRANSY 15
#define PFSHJOBJ_ACTIONNUM 6

// Menu Options
#define PFSHOPT_HUD 0
#define PFSHOPT_TDI 1
#define PFSHOPT_RESET 2
#define PFSHOPT_TIPS 3
#define PFSHOPT_HELP 4
#define PFSHOPT_EXIT 5

// TDI Definitions
#define PFSHTDI_RANDOM 0
#define PFSHTDI_IN 1
#define PFSHTDI_OUT 2
#define PFSHTDI_DOWN 3
#define PFSHTDI_NONE 4
#define PFSHTDI_NUM 5

// Reset Definitions
#define PFSHRESET_RANDOM 0
#define PFSHRESET_LEFT 1
#define PFSHRESET_RIGHT 2
#define PFSHRESET_OFF 3

// Tip Definitions
#define PFSHTIP_DURATION 3 * 60
#define PFSHTIP_SMASHTHRESHOLD 0.8
#define PFSHTIP_WALKTHRESHOLD 0.2875

typedef struct PivotFsmashAssets PivotFsmashAssets;
typedef struct PivotFsmashData PivotFsmashData;

struct PivotFsmashAssets {
    JOBJDesc *hud;
};

struct PivotFsmashData {
    EventDesc *event_desc;
    PivotFsmashAssets *assets;
    s16 reset_timer;
    GOBJ *hitlog_gobj;
    CameraBox *cam;

    struct {
        GOBJ *gobj;
        int canvas;
        int timer;
        u8 is_grab: 1;
        u8 is_throw: 1;
        u8 is_dash: 1;
        u8 is_turn: 1;
        u8 is_smash: 1;
        u8 action_log[PFSHJOBJ_BARNUM];
    } hud;

    struct {
        Text *is_grab_text;
        Text *is_throw_text;
        Text *is_dash_text;
        Text *is_turn_text;
        Text *is_smash_text;
        Text *reset_timer_text;
    } debug;

    struct {
        u8 is_displayed: 1;
        int last_smash_frame;
        float last_cstick;
        int last_dash_frame;
        float lstick_minus_one;
        float lstick_minus_two;
    } tip;
};

enum PFSH_ACTION {
    PFACT_NONE,
    PFACT_GRAB,
    PFACT_THROW,
    PFACT_DASH,
    PFACT_TURN,
    PFACT_SMASH,
};

// HUD functions
void HUD_Init(PivotFsmashData *event_data);

void HUD_Think(PivotFsmashData *event_data, GOBJ *hmn);

void HUD_CamThink(GOBJ *gobj);

void HUD_ClearTimer(PivotFsmashData *event_data);

// Fighter functions
void Fighter_Init(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu);

void Fighter_PlaceOnStage(GOBJ *fighter, float xpos, Direction facing_direction);

void Fighter_UpdatePosition(GOBJ *fighter);

// CPU functions
void CPU_Think(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu);

void CPU_TDI(FighterData *hmn_data, FighterData *cpu_data);

// Reset functions
void Reset_AutoThink(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu);

void Reset_ManualThink(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu);

void Reset_Fighter(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu, Direction hmn_direction);

int Reset_ShouldOnTimer(PivotFsmashData *event_data, FighterData *hmn_data, FighterData *cpu_data);

int Rest_ShouldInstantly(FighterData *hmn_data, FighterData *cpu_data);

// Tips functions
void Tips_ToggleCallback(GOBJ *menu_gobj, int value);

void Tips_Think(PivotFsmashData *event_data, FighterData *hmn_data);

// Menu Toggle functions
void MenuToggle_AutoReset(GOBJ *menu_gobj, int value);

// Event functionss
void Event_Init(GOBJ *gobj);

void Event_Think(GOBJ *event);

void Event_InitVariables(PivotFsmashData *event_data);

// Debug functions
Text *Debug_InitText(int canvas_id, int index);

void Debug_UpdateText(Text *text, s16 data);