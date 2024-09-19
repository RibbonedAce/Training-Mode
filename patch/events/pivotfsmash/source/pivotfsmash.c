#include "pivotfsmash.h"

static GXColor *tmgbar_colors[] = {
    &color_grey,
    &color_blue,
    &color_green,
    &color_red,
    &color_cyan,
    &color_yellow,
};

// Tips Functions
void Tips_Toggle_Callback(GOBJ *menu_gobj, int value) {
    Tips_Toggle(value);
}

// Main Menu
static char **PfshOptions_HUD[] = {"On", "Off"};
static EventOption PfshOptions_Main[] = {
    /*HUD*/ {
        .option_kind = OPTKIND_STRING, // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(PfshOptions_HUD) / 4, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "HUD", // pointer to a string
        .desc = "Toggle visibility of the HUD.", // string describing what this option does
        .option_values = PfshOptions_HUD, // pointer to an array of strings
        .onOptionChange = 0,
    },
    /*Help*/ {
        .option_kind = OPTKIND_FUNC, // the type of option this is; menu, string list, integers list, etc
        .value_num = 0, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "About", // pointer to a string
        .desc = "You must forward throw puff, first. \nThen, dash forward, dash back for one frame, \nand hit the c-stick forward. If done correctly, you should \nbe able to hit puff before she can jump out.",
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

static EventMenu PfshMenu_Main = {
    .name = "Pivot F-Smash Training", // the name of this menu
    .option_num = sizeof(PfshOptions_Main) / sizeof(EventOption), // number of options this menu contains
    .scroll = 0, // runtime variable used for how far down in the menu to start
    .state = 0, // bool used to know if this menu is focused, used at runtime
    .cursor = 0, // index of the option currently selected, used at runtime
    .options = PfshOptions_Main, // pointer to all of this menu's options
    .prev = 0, // pointer to previous menu, used at runtime
};

// Initial Menu
static EventMenu *Event_Menu = &PfshMenu_Main;
EventMenu **menu_start = &Event_Menu;

void PivotFsmash_HUDCamThink(GOBJ *gobj) {
    HUDCamThink(PfshOptions_Main[0]);
}

// Pivot F-Smash functions
void PivotFsmash_HUDInit(PivotFsmashData *event_data) {
    Create_HUDCam(PivotFsmash_HUDCamThink);

    // Load jobj
    JOBJ *hud_jobj = JOBJ_LoadJoint(event_data->assets->hud);
    event_data->hud.gobj = Setup_HUD(hud_jobj);

    // create text canvas
    event_data->hud.canvas = Default_Text_CreateCanvas(event_data->hud.gobj);

    // init text
//    Init_Text(event_data->hud.canvas, &event_data->hud.text_angle, hud_jobj, 2, 2, DEFTEXT_SCALE);

    // reset bar colors
    JOBJ *timingbar_parent = hud_jobj->child->sibling->child->sibling;
    timingbar_parent->scale.X = 12.0 / PFSHJOBJ_BARNUM;
    timingbar_parent->scale.Y = 0.5;
    if (PFSHJOBJ_BARNUM < 24) {
        timingbar_parent->scale.X = 0.5;
    }

    JOBJ *timingbar_back = timingbar_parent->child;
    timingbar_back->scale.X = PFSHJOBJ_BARNUM;

    JOBJ *timingbar_jobj = timingbar_back;

    for (int i = 0; i < PFSHJOBJ_BARNUM; ++i) {
        timingbar_jobj = JOBJ_LoadJoint(timingbar_jobj->desc);
        JOBJ_AddChild(timingbar_parent, timingbar_jobj);

        timingbar_jobj->trans.X = PFSHJOBJ_BARNUM * -1.0 / 2 + 0.5 + i;
        timingbar_jobj->scale.X = 0.9;
        timingbar_jobj->scale.Y = 0.9;

        HSD_Material *mat = timingbar_jobj->child->dobj->mobj->mat;
        mat->alpha = 0.7;
        mat->diffuse = color_grey;
    }
}

void Fighter_UpdatePosition(GOBJ *fighter) {
    FighterData *fighter_data = fighter->userdata;

    // Update Position (Copy Physics XYZ into all ECB XYZ)
    fighter_data->coll_data.topN_Curr.X = fighter_data->phys.pos.X;
    fighter_data->coll_data.topN_Curr.Y = fighter_data->phys.pos.Y;
    fighter_data->coll_data.topN_Prev.X = fighter_data->phys.pos.X;
    fighter_data->coll_data.topN_Prev.Y = fighter_data->phys.pos.Y;
    fighter_data->coll_data.topN_CurrCorrect.X = fighter_data->phys.pos.X;
    fighter_data->coll_data.topN_CurrCorrect.Y = fighter_data->phys.pos.Y;
    fighter_data->coll_data.topN_Proj.X = fighter_data->phys.pos.X;
    fighter_data->coll_data.topN_Proj.Y = fighter_data->phys.pos.Y;

    // Update Collision Frame ID
    fighter_data->coll_data.coll_test = *stc_colltest;

    // Adjust JObj position (code copied from 8006c324)
    JOBJ *fighter_jobj = fighter->hsd_object;
    fighter_jobj->trans.X = fighter_data->phys.pos.X;
    fighter_jobj->trans.Y = fighter_data->phys.pos.Y;
    fighter_jobj->trans.Z = fighter_data->phys.pos.Z;
    JOBJ_SetMtxDirtySub(fighter_jobj);

    // Update Static Player Block Coords
    Fighter_SetPosition(fighter_data->ply, fighter_data->flags.ms, &fighter_data->phys.pos);
}

void PivotFsmash_InitVariables(PivotFsmashData *event_data) {
    event_data->hud.timer = 0;
    event_data->tip.is_input_release = 0;
    event_data->tip.is_input_jump = 0;
    event_data->tip.refresh_displayed = 0;
    event_data->hud.is_release = 0;
    event_data->hud.is_jump = 0;
    event_data->hud.is_airdodge = 0;
    event_data->hud.is_aerial = 0;
    event_data->hud.is_land = 0;
    event_data->hud.is_actionable = 0;

    // init action log
    for (int i = 0; i < sizeof(event_data->hud.action_log) / sizeof(u8); i++) {
        event_data->hud.action_log[i] = 0;
    }
}

void Fighter_Reset(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu) {
    FighterData *hmn_data = hmn->userdata;
    FighterData *cpu_data = cpu->userdata;

    // init refresh num
    event_data->tip.refresh_num = 0; // setting this to -1 because the per frame code will add 1 and make it 0

    // get random position
    float hmn_pos = -20 + HSD_Randi(40) + HSD_Randf();
    float hmn_direction = -1 + 2 * HSD_Randi(2);
    Fighter_PlaceOnStage(hmn, hmn_pos, hmn_direction);

    float cpu_pos = hmn_pos + 10 * hmn_direction;
    float cpu_direction = -hmn_direction;
    Fighter_PlaceOnStage(cpu, cpu_pos, cpu_direction);

    hmn_data->dmg.percent = 0;
    Fighter_SetHUDDamage(hmn_data->ply, 0);

    float cpu_dmg = 60 + HSD_Randi(20) + HSD_Randf();
    cpu_data->dmg.percent = cpu_dmg;
    Fighter_SetHUDDamage(cpu_data->ply, cpu_dmg);

    // Set CPU behavior again
    cpu_data->cpu.ai = 15;

    PivotFsmash_InitVariables(event_data);

    Remove_Particles_And_CamShake();
}

void Fighter_PlaceOnStage(GOBJ *fighter, float xpos, float facing_direction) {
    FighterData *fighter_data = fighter->userdata;

    // Sleep first
    Fighter_EnterSleep(fighter, 0);
    Fighter_EnterRebirth(fighter);
    Match_CreateHUD(fighter_data->ply);

    // place player in a random position in wait
    fighter_data->facing_direction = facing_direction;
    Fighter_EnterWait(fighter);

    fighter_data->phys.pos.X = xpos;
    fighter_data->phys.pos.Y = 0;
    Fighter_UpdatePosition(fighter);

    // remove velocity
    fighter_data->phys.self_vel.X = 0;
    fighter_data->phys.self_vel.Y = 0;
}

// Fighter functions
void PivotFsmash_FtInit(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu) {
    Fighter_Reset(event_data, hmn, cpu);
}

// Init Function
void Event_Init(GOBJ *gobj) {
    PivotFsmashData *event_data = gobj->userdata;
    Init_Event_Vars("pfshData");

    GOBJ *hmn = Fighter_GetGObj(0);
    FighterData *hmn_data = hmn->userdata;
    GOBJ *cpu = Fighter_GetGObj(1);
    FighterData *cpu_data = cpu->userdata;

    // Init HUD
    PivotFsmash_HUDInit(event_data);

    // Init Fighters
    PivotFsmash_FtInit(event_data, hmn, cpu);
}

int Fighter_CheckFall(FighterData *hmn_data) {
    int is_fall = 0;

    // look for Fall input
    float stick_x = fabs(hmn_data->input.lstick_x);
    float stick_y = hmn_data->input.lstick_y;
    if (stick_x >= 0.2875 && hmn_data->input.timer_lstick_tilt_x < 2 ||
        stick_y <= -0.2875 && hmn_data->input.timer_lstick_tilt_y < 2) {
        is_fall = 1;
    }

    return is_fall;
}

void Tips_Think(PivotFsmashData *event_data, FighterData *hmn_data) {
}

void PivotFsmash_HUDThink(PivotFsmashData *event_data, FighterData *hmn_data) {
    // run tip logic
//    Tips_Think(event_data, hmn_data);

    JOBJ *hud_jobj = event_data->hud.gobj->hsd_object;

    // increment timer
    event_data->hud.timer++;

    // check to initialize timer
    if ((hmn_data->state == ASID_CATCH || hmn_data->state == ASID_CATCHDASH) && hmn_data->TM.state_frame == 1) {
        PivotFsmash_InitVariables(event_data);

        event_data->tip.refresh_num++;
    }

    int curr_frame = event_data->hud.timer;

    // update action log
    if (curr_frame < sizeof(event_data->hud.action_log) / sizeof(u8)) {
        if (hmn_data->state == ASID_CATCH || hmn_data->state == ASID_CATCHDASH ||
                hmn_data->state == ASID_CATCHPULL || hmn_data->state == ASID_CATCHDASHPULL) {
            // look for grab
            event_data->hud.action_log[curr_frame] = PFACT_GRAB;
        } else if (hmn_data->state == ASID_THROWF) {
            // look for throw
            event_data->hud.action_log[curr_frame] = PFACT_THROW;
        } else if (hmn_data->state == ASID_DASH) {
            // look for dash
            event_data->hud.action_log[curr_frame] = PFACT_DASH;
        } else if (hmn_data->state == ASID_TURN) {
            // look for turnaround
            event_data->hud.action_log[curr_frame] = PFACT_TURN;
        } else if (hmn_data->state == ASID_ATTACKS4S) {
            // look for smash
            event_data->hud.action_log[curr_frame] = PFACT_SMASH;
        } else if (hmn_data->state == ASID_WAIT || hmn_data->state == ASID_CATCHWAIT) {
            // look for wait
            event_data->hud.action_log[curr_frame] = PFACT_NONE;
        } else {
            // unknown state, probably a bug
            OSReport("Unknown state: 0x%.8x\n", hmn_data->state);
            event_data->hud.action_log[curr_frame] = PFACT_NONE;
        }
    }

    // look for actionable
    if (hmn_data->state == ASID_ATTACKS4S && hmn_data->TM.state_frame > 15) {
        event_data->hud.is_actionable = 1;
        event_data->hud.actionable_frame = event_data->hud.timer;

        // destroy any tips
        event_vars->Tip_Destroy();

        // update bar colors
        JOBJ *timingbar_jobj = hud_jobj->child->sibling->child->sibling->child->sibling;

        for (int count = 0; count < PFSHJOBJ_BARNUM; ++count) {
            DOBJ *d = timingbar_jobj->child->dobj;

            HSD_Material *mat = d->mobj->mat;
            mat->alpha = 0.7;

            // set color
            GXColor *bar_color = tmgbar_colors[event_data->hud.action_log[count]];
            mat->diffuse = *bar_color;

            timingbar_jobj = timingbar_jobj->sibling;
            if (timingbar_jobj == 0 || count >= sizeof(event_data->hud.action_log) / sizeof(u8)) {
                break;
            }
        }

        // apply HUD animation
        JOBJ_RemoveAnimAll(hud_jobj);
        JOBJ_ReqAnimAll(hud_jobj, 0);
    }

    // update HUD anim
    JOBJ_AnimAll(hud_jobj);
}

void PivotFsmash_ResetThink(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu) {
    FighterData *hmn_data = hmn->userdata;
    FighterData *cpu_data = cpu->userdata;

    // check if reset timer is set
    if (event_data->reset_timer > 0) {
        // decrement reset timer
        event_data->reset_timer--;

        // if reset timer is up, go back to ledge
        if (event_data->reset_timer == 0) {
            Fighter_Reset(event_data, hmn, cpu);
        }
    } else if (Should_Reset_On_Timer(hmn_data, cpu_data)) {
        // check to set reset timer
        event_data->reset_timer = 30;
    } else if (Should_Reset_Instantly(hmn_data, cpu_data)) {
        // reset instantly
        Fighter_Reset(event_data, hmn, cpu);
    }
}

int Should_Reset_On_Timer(FighterData *hmn_data, FighterData *cpu_data) {
    return cpu_data->flags.dead == 1;
}

int Should_Reset_Instantly(FighterData *hmn_data, FighterData *cpu_data) {
    return hmn_data->flags.dead == 1;
}

void PivotFsmash_ChangeLedgeThink(PivotFsmashData *event_data, GOBJ *hmn) {
    FighterData *hmn_data = hmn->userdata;

    if (hmn_data->input.down & (HSD_BUTTON_DPAD_LEFT | HSD_BUTTON_DPAD_RIGHT)) {
        // get current ledge position
        CollVert *collvert = *stc_collvert;
        CollLine *collline = *stc_collline;
        CollLine *this_line = &collline[event_data->ledge_line];
        float ledge_pos = 0;
        if (event_data->ledge_dir == -1) {
            ledge_pos = collvert[this_line->info->vert_next].pos_curr.X;
        } else if (event_data->ledge_dir == 1) {
            ledge_pos = collvert[this_line->info->vert_prev].pos_curr.X;
        }
    }
}

// Think Function
void Event_Think(GOBJ *event) {
    PivotFsmashData *event_data = event->userdata;

    // get fighter data
    GOBJ *hmn = Fighter_GetGObj(0);
    GOBJ *cpu = Fighter_GetGObj(1);
    FighterData *hmn_data = hmn->userdata;

    // no ledgefall
    FtCliffCatch *ft_state = (FtCliffCatch *)&hmn_data->state_var;
    if (hmn_data->state == ASID_CLIFFWAIT) {
        ft_state->fall_timer = 2;
    }

    PivotFsmash_HUDThink(event_data, hmn_data);
    PivotFsmash_ResetThink(event_data, hmn, cpu);
//    PivotFsmash_ChangeLedgeThink(event_data, hmn);
}

// Menu Toggle functions
void PivotFsmash_ToggleStartPosition(GOBJ *menu_gobj, int value) {
    // get fighter data
    GOBJ *hmn = Fighter_GetGObj(0);
    GOBJ *cpu = Fighter_GetGObj(1);
    PivotFsmashData *event_data = event_vars->event_gobj->userdata;

    Fighter_Reset(event_data, hmn, cpu);
}

void PivotFsmash_ToggleAutoReset(GOBJ *menu_gobj, int value) {
}
