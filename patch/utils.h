#pragma once
#include "../MexTK/mex.h"

#define DIR_RANDOM 0
#define DIR_LEFT -1
#define DIR_RIGHT 1

// there's got to be a better way to do this...
// can't do an actual function since event_vars doesn't work properly across files
#define Init_Event_Vars(param) { \
    event_vars = *event_vars_ptr; \
    event_data->assets = File_GetSymbol(event_vars->event_archive, param); \
}

// can't do an actual function since event_vars doesn't work properly across files
#define Tips_Toggle(param) { \
    if (param == 1) { \
        event_vars->Tip_Destroy(); \
    } \
}

// if HUD enabled and not paused
// can't do an actual function since event_vars doesn't work properly across files
#define HUDCamThink(eventOption) { \
    if (eventOption.option_val == 0 && Pause_CheckStatus(1) != 2) { \
        CObjThink_Common(gobj); \
    } \
}

static GXColor color_white = {255, 255, 255, 255};
static GXColor color_red = {255, 128, 128, 255};
static GXColor color_yellow = {255, 255, 128, 255};
static GXColor color_green = {128, 255, 128, 255};
static GXColor color_cyan = {128, 255, 255, 255};
static GXColor color_blue = {128, 128, 255, 255};
static GXColor color_magenta = {255, 128, 255, 255};
static GXColor color_grey = {80, 80, 80, 255};
static GXColor color_dark_red = {255, 0, 0, 255};
static GXColor color_dark_yellow = {255, 255, 0, 255};
static GXColor color_dark_green = {0, 255, 0, 255};
static GXColor color_dark_cyan = {0, 255, 255, 255};
static GXColor color_dark_blue = {0, 0, 255, 255};
static GXColor color_dark_magenta = {255, 0, 255, 255};
static GXColor color_black = {40, 40, 40, 255};

double Math_Sq(double x);

float Bezier(float time, float start, float end);

float Bezier_Blend(float t);

void Default_Event_Exit();

int X_To_The_N(int x, int n);

void Create_HUDCam(void *hudcam_think_callback);

GOBJ *Setup_HUD(JOBJ *hud_jobj);

void Init_Text(int canvas, Text **text_arr, JOBJ *hud_jobj, int num_objs, int offset, float text_scale);

Text *Create_Simple_Text(int canvas_id, float trans_x, float trans_y, float scale, char *init_text);

int Default_Text_CreateCanvas(GOBJ *hud_gobj);

void Remove_Particles_And_CamShake();

float Fighter_GetOpponentDir(FighterData *from, FighterData *to);

void Remove_Particles_And_CamShake();
