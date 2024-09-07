#pragma once
#include "../MexTK/mex.h"

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

double Math_Sq(double x);

float Bezier(float time, float start, float end);

float Bezier_Blend(float t);

void Default_Event_Exit();

int X_To_The_N(int x, int n);
