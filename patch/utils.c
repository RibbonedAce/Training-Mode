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

void Create_HUDCam(void *hudcam_think_callback) {
    // create hud cobj
    GOBJ *hudcam_gobj = GObj_Create(19, 20, 0);
    ArchiveInfo **ifall_archive = (ArchiveInfo **)0x804d6d5c;
    COBJDesc ***dmgScnMdls = File_GetSymbol(*ifall_archive, (char *)0x803f94d0);
    COBJDesc *cam_desc = dmgScnMdls[1][0];
    COBJ *hud_cobj = COBJ_LoadDesc(cam_desc);
    // init camera
    GObj_AddObject(hudcam_gobj, R13_U8(-0x3E55), hud_cobj);
    GOBJ_InitCamera(hudcam_gobj, hudcam_think_callback, 7);
    hudcam_gobj->cobj_links = 1 << 18;
}

GOBJ *Setup_HUD(JOBJ *hud_jobj) {
    GOBJ *hud_gobj = GObj_Create(0, 0, 0);
    GObj_AddObject(hud_gobj, 3, hud_jobj);
    GObj_AddGXLink(hud_gobj, GXLink_Common, 18, 80);
    return hud_gobj;
}

void Init_Text(int canvas, Text **text_arr, JOBJ *hud_jobj, int num_objs, int offset, float text_scale) {
    for (int i = 0; i < num_objs; i++) {
        // Create text object
        Text *hud_text = Text_CreateText(2, canvas);
        text_arr[i] = hud_text;
        hud_text->kerning = 1;
        hud_text->align = 1;
        hud_text->use_aspect = 1;

        // Get position
        Vec3 text_pos;
        JOBJ *text_jobj;
        JOBJ_GetChild(hud_jobj, (int)&text_jobj, offset + i, -1);
        JOBJ_GetWorldPosition(text_jobj, 0, &text_pos);

        // adjust scale
        Vec3 *scale = &hud_jobj->scale;
        // text scale
        hud_text->scale.X = scale->X * 0.01 * text_scale;
        hud_text->scale.Y = scale->Y * 0.01 * text_scale;
        hud_text->aspect.X = 165;

        // text position
        hud_text->trans.X = text_pos.X + scale->X / 4.0;
        hud_text->trans.Y = text_pos.Y * -1 + scale->Y / 4.0;

        // dummy text
        Text_AddSubtext(hud_text, 0, 0, "-");
    }
}

int Default_Text_CreateCanvas(GOBJ *hud_gobj) {
    return Text_CreateCanvas(2, hud_gobj, 14, 15, 0, 18, 81, 19);
}