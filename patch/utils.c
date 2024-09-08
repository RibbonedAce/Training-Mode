#include "utils.h"

double Math_Sq(double x) {
    return x * x;
}

float Bezier(float time, float start, float end) {
    float bez = time * time * (3.0f - 2.0f * time);
    return bez * (end - start) + start;
}

float Bezier_Blend(float t) {
    return t * t * (3.0f - 2.0f * t);
}

void Default_Event_Exit() {
    Match *match = (Match *)MATCH;

    // end game
    match->state = 3;

    // cleanup
    Match_EndVS();

    // unfreeze
    HSD_Update *update = (HSD_Update *)HSD_UPDATE;
    update->pause_develop = 0;
}

int X_To_The_N(int x, int n) {
    int i; /* Variable used in loop counter */
    int number = 1;

    for (i = 0; i < n; ++i) {
        number *= x;
    }

    return number;
}

void Create_HUDCam(void *hudCamThinkCallback) {
    // create hud cobj
    GOBJ *hudcam_gobj = GObj_Create(19, 20, 0);
    ArchiveInfo **ifall_archive = (ArchiveInfo **)0x804d6d5c;
    COBJDesc ***dmgScnMdls = File_GetSymbol(*ifall_archive, (char *)0x803f94d0);
    COBJDesc *cam_desc = dmgScnMdls[1][0];
    COBJ *hud_cobj = COBJ_LoadDesc(cam_desc);
    // init camera
    GObj_AddObject(hudcam_gobj, R13_U8(-0x3E55), hud_cobj);
    GOBJ_InitCamera(hudcam_gobj, hudCamThinkCallback, 7);
    hudcam_gobj->cobj_links = 1 << 18;
}