#include "lcancel.h"

// Main Menu
static char **LcOptions_Barrel[] = {"Off", "Stationary", "Move"};
static char **LcOptions_HUD[] = {"On", "Off"};

// Tips Functions
void Tips_Toggle_Callback(GOBJ *menu_gobj, int value) {
    Tips_Toggle(value);
}


static EventOption LcOptions_Main[] = {
    /*Target*/ {
        .option_kind = OPTKIND_STRING, // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(LcOptions_Barrel) / 4, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "Target", // pointer to a string
        .desc = "Enable a target to attack. Use DPad down to\nmanually move it.",
        // string describing what this option does
        .option_values = LcOptions_Barrel, // pointer to an array of strings
        .onOptionChange = 0,
    },
    /*HUD*/ {
        .option_kind = OPTKIND_STRING, // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(LcOptions_HUD) / 4, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "HUD", // pointer to a string
        .desc = "Toggle visibility of the HUD.", // string describing what this option does
        .option_values = LcOptions_HUD, // pointer to an array of strings
        .onOptionChange = 0,
    },
    /*Tips*/ {
        .option_kind = OPTKIND_STRING, // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(LcOptions_HUD) / 4, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "Tips", // pointer to a string
        .desc = "Toggle the onscreen display of tips.", // string describing what this option does
        .option_values = LcOptions_HUD, // pointer to an array of strings
        .onOptionChange = Tips_Toggle_Callback,
    },
    /*Help*/ {
        .option_kind = OPTKIND_FUNC, // the type of option this is; menu, string list, integers list, etc
        .value_num = 0, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "Help", // pointer to a string
        .desc =
        "L-canceling is performed by pressing L, R, or Z up to \n7 frames before landing from a non-special aerial\nattack. This will cut the landing lag in half, allowing \nyou to act sooner after attacking.",
        // string describing what this option does
        .option_values = 0, // pointer to an array of strings
        .onOptionChange = 0,
    },
    /*Exit*/ {
        .option_kind = OPTKIND_FUNC, // the type of option this is; menu, string list, integers list, etc
        .value_num = 0, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "Exit", // pointer to a string
        .desc = "Return to the Event Selection Screen.", // string describing what this option does
        .option_values = 0, // pointer to an array of strings
        .onOptionChange = 0,
        .onOptionSelect = Default_Event_Exit,
    },
};

void Barrel_Break(GOBJ *barrel_gobj) {
    ItemData *barrel_data = barrel_gobj->userdata;
    Effect_SpawnSync(1063, barrel_gobj, &barrel_data->pos);
    SFX_Play(251);
    ScreenRumble_Execute(2, &barrel_data->pos);
    JOBJ *barrel_jobj = barrel_gobj->hsd_object;
    JOBJ_SetFlagsAll(barrel_jobj, JOBJ_HIDDEN);
    barrel_data->xd0c = 2;
    barrel_data->self_vel.X = 0;
    barrel_data->self_vel.Y = 0;
    barrel_data->itemVar1 = 1;
    barrel_data->itemVar2 = 40;
    barrel_data->xdcf3 = 1;
    ItemStateChange(barrel_gobj, 7, 2);
}

int Barrel_OnHurt(GOBJ *barrel_gobj) {
    // get event data
    LCancelData *event_data = event_vars->event_gobj->userdata;

    switch (LcOptions_Main[0].option_val) {
        // stationary
        case 1: {
            break;
        }
        // move
        case 2: {
            // Break this barrel
            Barrel_Break(event_data->barrel_gobj);

            // spawn new barrel at a random position
            barrel_gobj = Barrel_Spawn(1);
            event_data->barrel_gobj = barrel_gobj;
            break;
        }
        // off
        default: {
            break;
        }
    }

    return 0;
}

int Barrel_OnDestroy(GOBJ *barrel_gobj) {
    // get event data
    LCancelData *event_data = event_vars->event_gobj->userdata;

    // if this barrel is still the current barrel
    if (barrel_gobj == event_data->barrel_gobj) {
        event_data->barrel_gobj = 0;
    }

    return 0;
}

static void *item_callbacks[] = {
    (void *)0x803f58e0,
    (void *)0x80287458,
    Barrel_OnDestroy, // onDestroy
    (void *)0x80287e68,
    (void *)0x80287ea8,
    (void *)0x80287ec8,
    (void *)0x80288818,
    Barrel_OnHurt, // onhurt
    (void *)0x802889f8,
    (void *)0x802888b8,
    (void *)0x00000000,
    (void *)0x00000000,
    (void *)0x80288958,
    (void *)0x80288c68,
    (void *)0x803f5988,
};

static EventMenu LClMenu_Main = {
    .name = "L-Cancel Training", // the name of this menu
    .option_num = sizeof(LcOptions_Main) / sizeof(EventOption), // number of options this menu contains
    .scroll = 0, // runtime variable used for how far down in the menu to start
    .state = 0, // bool used to know if this menu is focused, used at runtime
    .cursor = 0, // index of the option currently selected, used at runtime
    .options = LcOptions_Main, // pointer to all of this menu's options
    .prev = 0, // pointer to previous menu, used at runtime
};

// Initial Menu
static EventMenu *Event_Menu = &LClMenu_Main;
EventMenu **menu_start = &Event_Menu;

// L-Cancel functions
void LCancel_Init(LCancelData *event_data) {
    // create hud cobj
    GOBJ *hudcam_gobj = GObj_Create(19, 20, 0);
    ArchiveInfo **ifall_archive = (ArchiveInfo **)0x804d6d5c;
    COBJDesc ***dmgScnMdls = File_GetSymbol(*ifall_archive, (char *)0x803f94d0);
    COBJDesc *cam_desc = dmgScnMdls[1][0];
    COBJ *hud_cobj = COBJ_LoadDesc(cam_desc);
    // init camera
    GObj_AddObject(hudcam_gobj, R13_U8(-0x3E55), hud_cobj);
    GOBJ_InitCamera(hudcam_gobj, LCancel_HUDCamThink, 7);
    hudcam_gobj->cobj_links = 1 << 18;

    GOBJ *hud_gobj = GObj_Create(0, 0, 0);
    event_data->hud.gobj = hud_gobj;
    // Load jobj
    JOBJ *hud_jobj = JOBJ_LoadJoint(event_data->assets->hud);
    GObj_AddObject(hud_gobj, 3, hud_jobj);
    GObj_AddGXLink(hud_gobj, GXLink_Common, 18, 80);

    // create text canvas
    int canvas = Text_CreateCanvas(2, hud_gobj, 14, 15, 0, 18, 81, 19);
    event_data->hud.canvas = canvas;

    // init text
    Text **text_arr = &event_data->hud.text_time;
    for (int i = 0; i < 3; i++) {
        // Create text object
        Text *hud_text = Text_CreateText(2, canvas);
        text_arr[i] = hud_text;
        hud_text->kerning = 1;
        hud_text->align = 1;
        hud_text->use_aspect = 1;

        // Get position
        Vec3 text_pos;
        JOBJ *text_jobj;
        JOBJ_GetChild(hud_jobj, (int)&text_jobj, 2 + i, -1);
        JOBJ_GetWorldPosition(text_jobj, 0, &text_pos);

        // adjust scale
        Vec3 *scale = &hud_jobj->scale;
        // text scale
        hud_text->scale.X = scale->X * 0.01 * LCLTEXT_SCALE;
        hud_text->scale.Y = scale->Y * 0.01 * LCLTEXT_SCALE;
        hud_text->aspect.X = 165;

        // text position
        hud_text->trans.X = text_pos.X + scale->X / 4.0;
        hud_text->trans.Y = text_pos.Y * -1 + scale->Y / 4.0;

        // dummy text
        Text_AddSubtext(hud_text, 0, 0, "-");
    }

    // save initial arrow position
    JOBJ *arrow_jobj;
    JOBJ_GetChild(hud_jobj, (int)&arrow_jobj, LCLARROW_JOBJ, -1);
    event_data->hud.arrow_base_x = arrow_jobj->trans.X;
    event_data->hud.arrow_timer = 0;
    arrow_jobj->trans.X = 0;
    JOBJ_SetFlags(arrow_jobj, JOBJ_HIDDEN);
}

// Init Function
void Event_Init(GOBJ *gobj) {
    LCancelData *event_data = gobj->userdata;

    Init_Event_Vars("lclData");

    // create HUD
    LCancel_Init(event_data);
}

void Tips_Think(LCancelData *event_data, FighterData *hmn_data) {
    if (LcOptions_Main[2].option_val == 0) {
        // shield tip
        // if not shown
        if (event_data->tip.shield_isdisp == 0) {
            // update tip conditions
            // look for a freshly buffered guard off
            if (hmn_data->state == ASID_GUARDOFF && hmn_data->TM.state_frame == 0 &&
                // currently in guardoff first frame
                hmn_data->TM.state_prev[0] == ASID_GUARD && // was just in wait
                // was in aerial landing a few frames ago
                (hmn_data->TM.state_prev[3] >= ASID_LANDINGAIRN && hmn_data->TM.state_prev[3] <= ASID_LANDINGAIRLW)) {
                // increment condition count
                event_data->tip.shield_num++;

                // if condition met X times, show tip
                if (event_data->tip.shield_num >= 3) {
                    // display tip
                    char *shield_string =
                            "Tip:\nDon't hold the trigger! Quickly \npress and release to prevent \nshielding after landing.";
                    if (event_vars->Tip_Display(5 * 60, shield_string)) {
                        // set as shown
                        //event_data->tip.shield_isdisp = 1;
                        event_data->tip.shield_num = 0;
                    }
                }
            }
        }

        // hitbox tip
        // if not shown
        if (event_data->tip.hitbox_isdisp == 0) {
            // update hitbox active bool
            // check if currently in aerial attack)
            // check if in first frame of aerial attack
            if (hmn_data->state >= ASID_ATTACKAIRN && hmn_data->state <= ASID_ATTACKAIRLW) {
                // reset hitbox bool on first frame of aerial attack
                if (hmn_data->TM.state_frame == 0) {
                    event_data->tip.hitbox_active = 0;
                }

                // check if hitbox active
                for (int i = 0; i < sizeof(hmn_data->hitbox) / sizeof(ftHit); i++) {
                    if (hmn_data->hitbox[i].active != 0) {
                        event_data->tip.hitbox_active = 1;
                        break;
                    }
                }
            }

            // update tip conditions
            if (hmn_data->state >= ASID_LANDINGAIRN && hmn_data->state <= ASID_LANDINGAIRLW && hmn_data->TM.state_frame == 0 && // is in aerial landing
                event_data->is_fail == 0 &&
                // succeeded the last aerial landing
                event_data->tip.hitbox_active == 0) {
                // increment condition count
                event_data->tip.hitbox_num++;

                // if condition met X times, show tip
                if (event_data->tip.hitbox_num >= 3) {
                    // display tip
                    char *hitbox_string =
                            "Tip:\nDon't land too quickly! Make \nsure you land after the \nattack becomes active.";
                    if (event_vars->Tip_Display(5 * 60, hitbox_string)) {
                        // set as shown
                        //event_data->tip.hitbox_isdisp = 1;
                        event_data->tip.hitbox_num = 0;
                    }
                }
            }
        }

        // fastfall tip
        // if not shown
        if (event_data->tip.fastfall_isdisp == 0) {
            // update fastfell bool
            // check if currently in aerial attack)
            // check if in first frame of aerial attack
            if (hmn_data->state >= ASID_ATTACKAIRN && hmn_data->state <= ASID_ATTACKAIRLW) {
                // reset hitbox bool on first frame of aerial attack
                if (hmn_data->TM.state_frame == 0) {
                    event_data->tip.fastfall_active = 0;
                }

                // check if fastfalling
                if (hmn_data->flags.is_fastfall == 1) {
                    event_data->tip.fastfall_active = 1;
                }
            }

            // update tip conditions
            if (hmn_data->state >= ASID_LANDINGAIRN && hmn_data->state <= ASID_LANDINGAIRLW && hmn_data->TM.state_frame == 0 && // is in aerial landing
                (hmn_data->input.timer_trigger_any_ignore_hitlag >= 7 && hmn_data->input.timer_trigger_any_ignore_hitlag <= 15) && // was early for an l-cancel
                // succeeded the last aerial landing
                event_data->tip.fastfall_active == 0) {
                // increment condition count
                event_data->tip.fastfall_num++;

                // if condition met X times, show tip
                if (event_data->tip.fastfall_num >= 3) {
                    // display tip
                    char *fastfall_string =
                            "Tip:\nDon't forget to fastfall!\nIt will let you act sooner \nand help with your \ntiming.";
                    if (event_vars->Tip_Display(5 * 60, fastfall_string)) {
                        // set as shown
                        //event_data->tip.hitbox_isdisp = 1;
                        event_data->tip.fastfall_num = 0;
                    }
                }
            }
        }

        // late tip
        // if not shown
        if (event_data->tip.late_isdisp == 0) {
            // update tip conditions
            if (hmn_data->state >= ASID_LANDINGAIRN && hmn_data->state <= ASID_LANDINGAIRLW &&
                // is in aerial landing
                event_data->is_fail == 1 && // failed the l-cancel
                // was late for an l-cancel by pressing it just now
                hmn_data->input.down & (HSD_TRIGGER_L | HSD_TRIGGER_R | HSD_TRIGGER_Z)) {
                // increment condition count
                event_data->tip.late_num++;

                // if condition met X times, show tip
                if (event_data->tip.late_num >= 3) {
                    // display tip
                    char *late_string = "Tip:\nTry pressing the trigger a\nbit earlier, before the\nfighter lands.";
                    if (event_vars->Tip_Display(5 * 60, late_string)) {
                        // set as shown
                        //event_data->tip.hitbox_isdisp = 1;
                        event_data->tip.late_num = 0;
                    }
                }
            }
        }
    }
}

void LCancel_Think(LCancelData *event_data, FighterData *hmn_data) {
    // run tip logic
    Tips_Think(event_data, hmn_data);

    JOBJ *hud_jobj = event_data->hud.gobj->hsd_object;

    // log fastfall frame
    // if im in a fastfall-able state
    if (hmn_data->phys.self_vel.Y < 0) {
        // can i fastfall?
        // did i fastfall yet?
        if (hmn_data->flags.is_fastfall) {
            event_data->is_fastfall = 1; // set as fastfall this session
        } else {
            event_data->fastfall_frame++; // increment frames
        }
    } else {
        // cant fastfall, reset frames
        event_data->fastfall_frame = 0;
    }

    // if aerial landing
    if (hmn_data->state >= ASID_LANDINGAIRN && hmn_data->state <= ASID_LANDINGAIRLW && hmn_data->TM.state_frame == 0) {
        // increment total lcls
        event_data->hud.lcl_total++;

        // determine succession
        int is_fail = 1;
        if (hmn_data->input.timer_trigger_any_ignore_hitlag < 7) {
            is_fail = 0;
            event_data->hud.lcl_success++;
        }
        event_data->is_fail = is_fail; // save l-cancel bool

        // Play appropriate sfx
        if (is_fail == 0) {
            SFX_PlayRaw(303, 255, 128, 20, 3);
        } else {
            SFX_PlayCommon(3);
        }

        // update timing text
        int frame_box_id;
        if (hmn_data->input.timer_trigger_any_ignore_hitlag >= 30) {
            // update text
            Text_SetText(event_data->hud.text_time, 0, "No Press");
            frame_box_id = 29;
        } else {
            Text_SetText(event_data->hud.text_time, 0, "%df/7f", hmn_data->input.timer_trigger_any_ignore_hitlag + 1);
            frame_box_id = hmn_data->input.timer_trigger_any_ignore_hitlag;
        }

        // update arrow
        JOBJ *arrow_jobj;
        JOBJ_GetChild(hud_jobj, (int)&arrow_jobj, LCLARROW_JOBJ, -1);
        event_data->hud.arrow_prevpos = arrow_jobj->trans.X;
        event_data->hud.arrow_nextpos = event_data->hud.arrow_base_x - frame_box_id * LCLARROW_OFFSET;
        JOBJ_ClearFlags(arrow_jobj, JOBJ_HIDDEN);
        event_data->hud.arrow_timer = LCLARROW_ANIMFRAMES;

        // Print airborne frames
        if (event_data->is_fastfall) {
            Text_SetText(event_data->hud.text_air, 0, "%df", event_data->fastfall_frame - 1);
        } else {
            Text_SetText(event_data->hud.text_air, 0, "-");
        }
        event_data->is_fastfall = 0; // reset fastfall bool

        // Print succession
        float succession = (float) event_data->hud.lcl_success / (float) event_data->hud.lcl_total * 100.0;
        Text_SetText(event_data->hud.text_scs, 0, "%.1f%%", succession);

        // Play HUD anim
        JOBJ_RemoveAnimAll(hud_jobj);
        JOBJ_AddAnimAll(hud_jobj, 0, event_data->assets->hudmatanim[is_fail], 0);
        JOBJ_ReqAnimAll(hud_jobj, 0);
    }

    // if autocancel landing
    // came from aerial attack
    if (hmn_data->state == ASID_LANDING && hmn_data->TM.state_frame == 0 && // if first frame of landing
        (hmn_data->TM.state_prev[0] >= ASID_ATTACKAIRN && hmn_data->state <= ASID_ATTACKAIRLW)) {
        // state as autocancelled
        Text_SetText(event_data->hud.text_time, 0, "Auto-canceled");

        // Play HUD anim
        JOBJ_RemoveAnimAll(hud_jobj);
        JOBJ_AddAnimAll(hud_jobj, 0, event_data->assets->hudmatanim[2], 0);
        JOBJ_ReqAnimAll(hud_jobj, 0);
    }

    // update arrow animation
    if (event_data->hud.arrow_timer > 0) {
        // decrement timer
        event_data->hud.arrow_timer--;

        // get this frames position
        float time = 1 - (float) event_data->hud.arrow_timer / (float) LCLARROW_ANIMFRAMES;
        float xpos = Bezier(time, event_data->hud.arrow_prevpos, event_data->hud.arrow_nextpos);

        // update position
        JOBJ *arrow_jobj;
        JOBJ_GetChild(hud_jobj, (int)&arrow_jobj, LCLARROW_JOBJ, -1); // get timing bar jobj
        arrow_jobj->trans.X = xpos;
        JOBJ_SetMtxDirtySub(arrow_jobj);
    }

    // update HUD anim
    JOBJ_AnimAll(hud_jobj);
}

// Think Function
void Event_Think(GOBJ *event) {
    LCancelData *event_data = event->userdata;

    // get fighter data
    GOBJ *hmn = Fighter_GetGObj(0);
    FighterData *hmn_data = hmn->userdata;

    LCancel_Think(event_data, hmn_data);
    Barrel_Think(event_data);
}

void LCancel_HUDCamThink(GOBJ *gobj) {
    // if HUD enabled and not paused
    if (LcOptions_Main[1].option_val == 0 && Pause_CheckStatus(1) != 2) {
        CObjThink_Common(gobj);
    }
}
// Barrel Functions
void Barrel_Think(LCancelData *event_data) {
    GOBJ *barrel_gobj = event_data->barrel_gobj;

    switch (LcOptions_Main[0].option_val) {
        // stationary
        case 1: {
            // if not spawned, spawn
            if (barrel_gobj == 0) {
                // spawn barrel at center stage
                barrel_gobj = Barrel_Spawn(0);
                event_data->barrel_gobj = barrel_gobj;
            }

            ItemData *barrel_data = barrel_gobj->userdata;
            barrel_data->can_hold = 0;

            // check to move barrel
            // get fighter data
            GOBJ *hmn = Fighter_GetGObj(0);
            FighterData *hmn_data = hmn->userdata;
            if (hmn_data->input.down & PAD_BUTTON_DPAD_DOWN) {
                if (hmn_data->phys.air_state == 0) {
                    // check for ground in front of player
                    Vec3 coll_pos;
                    int line_index;
                    int line_kind;
                    Vec3 line_unk;
                    float fromX = hmn_data->phys.pos.X + hmn_data->facing_direction * 16;
                    float toX = fromX;
                    float fromY = hmn_data->phys.pos.Y + 5;
                    float toY = fromY - 10;
                    int isGround = GrColl_RaycastGround(&coll_pos, &line_index, &line_kind, &line_unk, (Vec3 *)-1, (Vec3 *)-1, (Vec3 *)-1, 0, fromX, fromY, toX, toY, 0); // ensure player is grounded
                    if (isGround == 1) {
                        // update last pos
                        event_data->barrel_lastpos = coll_pos;

                        // place barrel here
                        barrel_data->pos = coll_pos;
                        barrel_data->coll_data.ground_index = line_index;

                        // update ECB
                        barrel_data->coll_data.topN_Curr = barrel_data->pos;
                        // move current ECB location to new position
                        Coll_ECBCurrToPrev(&barrel_data->coll_data);
                        barrel_data->cb.coll(barrel_gobj);

                        SFX_Play(221);
                    } else {
                        // play SFX
                        SFX_PlayCommon(3);
                    }
                }
                break;
            }
        }
        // move
        case 2: {
            // if not spawned, spawn
            if (barrel_gobj == 0) {
                // spawn barrel at center stage
                barrel_gobj = Barrel_Spawn(1);
                event_data->barrel_gobj = barrel_gobj;
            }

            ItemData *barrel_data = barrel_gobj->userdata;
            barrel_data->can_hold = 0;
            barrel_data->can_nudge = 0;

            break;
        }
        // off
        default: {
            // if spawned, remove
            if (barrel_gobj != 0) {
                Item_Destroy(barrel_gobj);
                event_data->barrel_gobj = 0;
            }

            break;
        }
    }
}

void Barrel_Rand_Pos(Vec3 *pos, Vec3 *barrel_lastpos) {
    // setup time
    int raycast_num = 0;
    int raytime_start, raytime_end, raytime_time;
    raytime_start = OSGetTick();

    while (true) {
        // get position
        int line_index;
        int line_kind;
        Vec3 line_angle;
        float from_x = Stage_GetCameraLeft() + HSD_Randi(Stage_GetCameraRight() - Stage_GetCameraLeft()) + HSD_Randf();
        float to_x = from_x;
        float from_y = Stage_GetCameraBottom() + HSD_Randi(Stage_GetCameraTop() - Stage_GetCameraBottom()) + HSD_Randf();
        float to_y = from_y - 1000;
        int is_ground = GrColl_RaycastGround(pos, &line_index, &line_kind, &line_angle, (Vec3 *)-1, (Vec3 *)-1, (Vec3 *)-1, 0, from_x, from_y, to_x, to_y, 0);
        raycast_num++;
        if (is_ground == 0) {
            continue;
        }

        // ensure it isn't too close to the previous
        float distance = sqrtf(Math_Sq(pos->X - barrel_lastpos->X) + Math_Sq(pos->Y - barrel_lastpos->Y));
        if (distance < 25) {
            continue;
        }

        // ensure left and right have ground
        Vec3 near_pos;
        float near_fromX = pos->X + 8;
        float near_fromY = pos->Y + 4;
        to_y = near_fromY - 4;
        is_ground = GrColl_RaycastGround(&near_pos, &line_index, &line_kind, &line_angle, (Vec3 *)-1, (Vec3 *)-1, (Vec3 *)-1, 0, near_fromX, near_fromY, near_fromX, to_y, 0);
        raycast_num++;
        if (is_ground == 0) {
            continue;
        }
        near_fromX = pos->X - 8;
        is_ground = GrColl_RaycastGround(&near_pos, &line_index, &line_kind, &line_angle, (Vec3 *)-1, (Vec3 *)-1, (Vec3 *)-1, 0, near_fromX, near_fromY, near_fromX, to_y, 0);
        raycast_num++;
        if (is_ground == 0) {
            continue;
        }

        // output num and time
        raytime_end = OSGetTick();
        raytime_time = OSTicksToMilliseconds(raytime_end - raytime_start);
        OSReport("lcl: %d ray in %dms\n", raycast_num, raytime_time);

        break;
    }
}

GOBJ *Barrel_Spawn(int pos_kind) {
    LCancelData *event_data = event_vars->event_gobj->userdata;
    Vec3 *barrel_lastpos = &event_data->barrel_lastpos;

    // determine position to spawn
    Vec3 pos;
    pos.Z = 0;

    switch (pos_kind) {
        // random pos
        case 1: {
            Barrel_Rand_Pos(&pos, barrel_lastpos);
            break;
        }
        // center stage
        default: {
            // get position
            int line_index;
            int line_kind;
            Vec3 line_angle;
            float from_x = 0;
            float to_x = from_x;
            float from_y = 6;
            float to_y = from_y - 1000;
            int is_ground = GrColl_RaycastGround(&pos, &line_index, &line_kind, &line_angle, (Vec3 *)-1, (Vec3 *)-1, (Vec3 *)-1, 0, from_x, from_y, to_x, to_y, 0);
            if (is_ground == 0) {
                Barrel_Rand_Pos(&pos, barrel_lastpos);
            }
            break;
        }
    }

    // spawn item
    SpawnItem spawnItem;
    spawnItem.parent_gobj = 0;
    spawnItem.parent_gobj2 = 0;
    spawnItem.it_kind = ITEM_BARREL;
    spawnItem.hold_kind = 0;
    spawnItem.unk2 = 0;
    spawnItem.pos = pos;
    spawnItem.pos2 = pos;
    spawnItem.vel.X = 0;
    spawnItem.vel.Y = 0;
    spawnItem.vel.Z = 0;
    spawnItem.facing_direction = 1;
    spawnItem.damage = 0;
    spawnItem.unk5 = 0;
    spawnItem.unk6 = 0;
    spawnItem.unk7 = 0x80;
    spawnItem.is_spin = 0;
    GOBJ *barrel_gobj = Item_CreateItem2(&spawnItem);
    Item_CollAir(barrel_gobj, null);

    // replace collision callback
    ItemData *barrel_data = barrel_gobj->userdata;
    barrel_data->it_cb = item_callbacks;
    barrel_data->camerabox->kind = 0;

    // update last barrel pos
    event_data->barrel_lastpos = pos;

    return barrel_gobj;
}
