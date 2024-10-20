#pragma once

#include "../../../../MexTK/mex.h"
#include "../../../tmdata/source/events.h"
#include "../../../utils.h"
#include "../../../cpu.h"

#define LSDH_TIPDURATION 1.7 * 60
#define PFSH_HITBOXNUM 30 * 4
#define DEFTEXT_SCALE 4
#define PFSHJOBJ_BARNUM 48
#define PFSHJOBJ_BARSCALE 1.5
#define PFSHJOBJ_BARTRANSY 15
#define PFSH_ACTIONNUM 6

typedef struct PivotFsmashAssets PivotFsmashAssets;
typedef struct PivotFsmashData PivotFsmashData;
typedef struct PfshHitboxData PfshHitboxData;
typedef struct PfshHitlogData PfshHitlogData;

struct PivotFsmashAssets {
    JOBJDesc *hud;
};

struct PivotFsmashData {
    EventDesc *event_desc;
    PivotFsmashAssets *assets;
    s16 ledge_line;
    s16 ledge_dir;
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
        s16 refresh_num; // number of times refreshed
        u8 refresh_cond_num; // number of times tip condition has been met
        u8 refresh_displayed: 1;
    } tip;
};

struct PfshHitboxData {
    int kind;
    float size;
    Vec3 pos_curr;
    Vec3 pos_prev;
};

struct PfshHitlogData {
    int num;
    PfshHitboxData hitlog[PFSH_HITBOXNUM];
};

enum PFSH_ACTION {
    PFACT_NONE,
    PFACT_GRAB,
    PFACT_THROW,
    PFACT_DASH,
    PFACT_TURN,
    PFACT_SMASH,
};

void Tips_Toggle_Callback(GOBJ *menu_gobj, int value);

void PivotFsmash_HUDCamThink(GOBJ *gobj);

void PivotFsmash_HUDInit(PivotFsmashData *event_data);

void Fighter_UpdatePosition(GOBJ *fighter);

void PivotFsmash_InitVariables(PivotFsmashData *event_data);

void Fighter_Reset(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu, float hmn_direction);

void Fighter_PlaceOnStage(GOBJ *fighter, float xpos, float facing_direction);

void PivotFsmash_FtInit(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu);

void Event_Init(GOBJ *gobj);

void Tips_Think(PivotFsmashData *event_data, FighterData *hmn_data);

void PivotFsmash_HUDThink(PivotFsmashData *event_data, GOBJ *hmn);

void PivotFsmash_ResetThink(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu);

int Should_Reset_On_Timer(PivotFsmashData *event_data, FighterData *hmn_data, FighterData *cpu_data);

int Should_Reset_Instantly(FighterData *hmn_data, FighterData *cpu_data);

void PivotFsmash_ManualResetThink(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu);

void PivotFsmash_CPUThink(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu);

void Event_Think(GOBJ *event);

void PivotFsmash_ToggleStartPosition(GOBJ *menu_gobj, int value);