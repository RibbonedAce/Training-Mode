#pragma once

#include "../../../../MexTK/mex.h"
#include "../../../tmdata/source/events.h"
#include "../../../utils.h"

#define LSDH_TIPDURATION 1.7 * 60
#define PFSH_HITBOXNUM 30 * 4
#define LCLTEXT_SCALE 4.5
#define LCLJOBJ_BAR 4

typedef struct PivotFsmashAssets PivotFsmashAssets;
typedef struct PivotFsmashData PivotFsmashData;
typedef struct PfshHitboxData PfshHitboxData;
typedef struct PfshHitlogData PfshHitlogData;

struct PivotFsmashAssets {
    JOBJDesc *hud;
    void **hudmatanim; // pointer to array
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
        Text *text_angle;
        Text *text_galint;
        int canvas;
        int timer;
        float airdodge_angle;
        u8 is_release: 1;
        u8 is_jump: 1;
        u8 is_airdodge: 1;
        u8 is_aerial: 1;
        u8 is_land: 1;
        u8 is_actionable: 1;
        u16 release_frame;
        u16 jump_frame;
        u16 airdodge_frame;
        u16 aerial_frame;
        u16 land_frame;
        u16 actionable_frame;
        u8 action_log[30];
    } hud;

    struct {
        s16 refresh_num; // number of times refreshed
        u8 refresh_cond_num; // number of times tip condition has been met
        u8 refresh_displayed: 1;
        u8 is_input_release: 1;
        u8 is_input_jump: 1;
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
    LDACT_NONE,
    LDACT_CLIFFWAIT,
    LDACT_FALL,
    LDACT_JUMP,
    LDACT_AIRDODGE,
    LDACT_ATTACK,
    LDACT_LANDING,
};

void Tips_Toggle_Callback(GOBJ *menu_gobj, int value);

void PivotFsmash_HUDCamThink(GOBJ *gobj);

void PivotFsmash_HUDInit(PivotFsmashData *event_data);

void Fighter_UpdatePosition(GOBJ *fighter);

void PivotFsmash_InitVariables(PivotFsmashData *event_data);

void Fighter_UpdateCamera(GOBJ *fighter);

void Fighter_PlaceOnLedge(PivotFsmashData *event_data, GOBJ *hmn, int line_index, float ledge_dir);

void PivotFsmash_FtInit(PivotFsmashData *event_data);

void Event_Init(GOBJ *gobj);

int Fighter_CheckFall(FighterData *hmn_data);

void Tips_Think(PivotFsmashData *event_data, FighterData *hmn_data);

void PivotFsmash_HUDThink(PivotFsmashData *event_data, FighterData *hmn_data);

void PivotFsmash_HitLogThink(PivotFsmashData *event_data, GOBJ *hmn);

void PivotFsmash_ResetThink(PivotFsmashData *event_data, GOBJ *hmn);

void PivotFsmash_ChangeLedgeThink(PivotFsmashData *event_data, GOBJ *hmn);

void Event_Think(GOBJ *event);

void PivotFsmash_ToggleStartPosition(GOBJ *menu_gobj, int value);

void PivotFsmash_ToggleAutoReset(GOBJ *menu_gobj, int value);

GOBJ *PivotFsmash_HitLogInit();

void PivotFsmash_HitLogGX(GOBJ *gobj, int pass);

int Ledge_Find(int search_dir, float xpos_start, float *ledge_dir);

void RebirthWait_Phys(GOBJ *fighter);

int RebirthWait_IASA(GOBJ *fighter);

