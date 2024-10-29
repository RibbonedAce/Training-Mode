#include "pivotfsmash.h"

static GXColor *tmgbar_colors[] = {
    &color_grey,
    &color_blue,
    &color_green,
    &color_red,
    &color_cyan,
    &color_yellow,
};

static char *tmgbar_helptext[] = {
    "Wait",
    "Grab",
    "Throw",
    "Dash",
    "Turn",
    "Smash",
};

// Main Menu
static char **PfshOptions_TDI[] = {"Random", "In", "Out", "Down", "None"};
static char **PfshOptions_Reset[] = {"Random", "Left", "Right", "Off"};
static EventOption PfshOptions_Main[] = {
    /*HUD*/ {
        .option_kind = OPTKIND_STRING, // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(DefaultOptions_OnOff) / 4, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "HUD", // pointer to a string
        .desc = "Toggle visibility of the HUD.", // string describing what this option does
        .option_values = DefaultOptions_OnOff, // pointer to an array of strings
        .onOptionChange = 0,
    },
    /*CPU TDI*/ {
        .option_kind = OPTKIND_STRING, // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(PfshOptions_TDI) / 4, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "CPU TDI", // pointer to a string
        .desc = "The way that the CPU DI's from your throws.",
        .option_values = PfshOptions_TDI, // pointer to an array of strings
        .onOptionChange = 0,
    },
    /*Reset*/ {
        .option_kind = OPTKIND_STRING, // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(PfshOptions_Reset) / 4, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "Auto Reset", // pointer to a string
        .desc = "The direction to auto-reset after each iteration. \nDoes not override manual reset direction.",
        .option_values = PfshOptions_Reset, // pointer to an array of strings
        .onOptionChange = MenuToggle_AutoReset,
    },
    /*Tips*/ {
        .option_kind = OPTKIND_STRING, // the type of option this is; menu, string list, integers list, etc
        .value_num = sizeof(DefaultOptions_OnOff) / 4, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "Tips", // pointer to a string
        .desc = "Toggle visibility of tips that pop up \nupon failed pivot.", // string describing what this option does
        .option_values = DefaultOptions_OnOff, // pointer to an array of strings
        .onOptionChange = Tips_ToggleCallback,
    },
    /*Help*/ {
        .option_kind = OPTKIND_FUNC, // the type of option this is; menu, string list, integers list, etc
        .value_num = 0, // number of values for this option
        .option_val = 0, // value of this option
        .menu = 0, // pointer to the menu that pressing A opens
        .option_name = "About", // pointer to a string
        .desc = "First, you must forward throw puff. \nThen, dash forward, dash back for one frame, \nand hit the c-stick forward. If done correctly, you should \nbe able to hit puff before she can jump out.",
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

// HUD functions
void HUD_Init(PivotFsmashData *event_data) {
    Create_HUDCam(HUD_CamThink);

    // Load jobj
    JOBJ *hud_jobj = JOBJ_LoadJoint(event_data->assets->hud);
    event_data->hud.gobj = Setup_HUD(hud_jobj);
    hud_jobj->trans.X = 0.0;
    hud_jobj->trans.Y = PFSHJOBJ_BARTRANSY;

    // create text canvas
    event_data->hud.canvas = Default_Text_CreateCanvas(event_data->hud.gobj);

    // reset bar colors
    JOBJ *timingbar_parent = hud_jobj->child->sibling;
    timingbar_parent->trans.X = 0.0;
    timingbar_parent->trans.Y = 0.0;
    timingbar_parent->scale.X = 24.0 * PFSHJOBJ_BARSCALEX / PFSHJOBJ_BARNUM;
    timingbar_parent->scale.Y = PFSHJOBJ_BARSCALEY;
    if (timingbar_parent->scale.X > PFSHJOBJ_BARSCALEX) {
        timingbar_parent->scale.X = PFSHJOBJ_BARSCALEX;
    }

    JOBJ *timingbar_back = timingbar_parent->child;
    timingbar_back->scale.X = PFSHJOBJ_BARNUM;
    timingbar_back->scale.Y = 1.0;

    JOBJ *timingbar_jobj = timingbar_back;

    for (int i = 0; i < PFSHJOBJ_BARNUM; ++i) {
        timingbar_jobj = JOBJ_LoadJoint(timingbar_jobj->desc);
        JOBJ_AddChild(timingbar_parent, timingbar_jobj);

        timingbar_jobj->trans.X = PFSHJOBJ_BARNUM * -1.0 / 2 + 0.5 + i;
        timingbar_jobj->trans.Y = 0.0;
        timingbar_jobj->scale.X = 0.9;
        timingbar_jobj->scale.Y = 0.9;

        HSD_Material *mat = timingbar_jobj->child->dobj->mobj->mat;
        mat->alpha = 0.7;
        mat->diffuse = color_grey;
    }

    // create example bar colors
    JOBJ *timingbar_ex_parent = hud_jobj->child;
    timingbar_ex_parent->trans.X = 0.0;
    timingbar_ex_parent->trans.Y = 0.0;
    timingbar_ex_parent->scale.X = 1.0;
    timingbar_ex_parent->scale.Y = 1.0;

    for (int i = 0; i < PFSHJOBJ_ACTIONNUM; ++i) {
        // Back square
        JOBJ *timingbar_ex_jobj = JOBJ_LoadJoint(timingbar_jobj->desc);
        JOBJ_AddChild(timingbar_ex_parent, timingbar_ex_jobj);

        timingbar_ex_jobj->trans.X = 4 * (i - PFSHJOBJ_ACTIONNUM / 2 + 0.5) * PFSHJOBJ_BARSCALEX;
        timingbar_ex_jobj->trans.Y = 3 * PFSHJOBJ_BARSCALEY;
        timingbar_ex_jobj->scale.X = 1.1 * PFSHJOBJ_BARSCALEY;
        timingbar_ex_jobj->scale.Y = 1.1 * PFSHJOBJ_BARSCALEY;

        HSD_Material *mat = timingbar_ex_jobj->child->dobj->mobj->mat;
        mat->alpha = 1.0;
        mat->diffuse = color_black;
        
        // Front square
        timingbar_ex_jobj = JOBJ_LoadJoint(timingbar_jobj->desc);
        JOBJ_AddChild(timingbar_ex_parent, timingbar_ex_jobj);

        timingbar_ex_jobj->trans.X = 4 * (i - PFSHJOBJ_ACTIONNUM / 2 + 0.5) * PFSHJOBJ_BARSCALEX;
        timingbar_ex_jobj->trans.Y = 3 * PFSHJOBJ_BARSCALEY;
        timingbar_ex_jobj->scale.X = 0.9 * PFSHJOBJ_BARSCALEY;
        timingbar_ex_jobj->scale.Y = 0.9 * PFSHJOBJ_BARSCALEY;

        mat = timingbar_ex_jobj->child->dobj->mobj->mat;
        mat->alpha = 0.7;
        mat->diffuse = *tmgbar_colors[i];
    }

    // init text
    for (int i = 0; i < PFSHJOBJ_ACTIONNUM; ++i) {
        Create_Simple_Text(
                event_data->hud.canvas, 
                4 * (i - PFSHJOBJ_ACTIONNUM / 2 + 0.5) * PFSHJOBJ_BARSCALEX,
                -(PFSHJOBJ_BARTRANSY + (2 * PFSHJOBJ_BARSCALEY)),
                0.01 * PFSHJOBJ_TEXTSCALE * PFSHJOBJ_BARSCALEY,
                tmgbar_helptext[i]
        );
    }

    // debug text
    #if TM_DEBUG > 0
        event_data->debug.is_grab_text = Debug_InitText(event_data->hud.canvas, 0);
        event_data->debug.is_throw_text = Debug_InitText(event_data->hud.canvas, 1);
        event_data->debug.is_dash_text = Debug_InitText(event_data->hud.canvas, 2);
        event_data->debug.is_turn_text = Debug_InitText(event_data->hud.canvas, 3);
        event_data->debug.is_smash_text = Debug_InitText(event_data->hud.canvas, 4);
        event_data->debug.reset_timer_text = Debug_InitText(event_data->hud.canvas, 5);
    #endif
}

void HUD_Think(PivotFsmashData *event_data, GOBJ *hmn) {
    FighterData *hmn_data = hmn->userdata;

    event_data->hud.timer++;

    // check to initialize timer
    if ((hmn_data->state == ASID_CATCH || hmn_data->state == ASID_CATCHDASH) && hmn_data->TM.state_frame == 1) {
        HUD_ClearTimer(event_data);
    }

    int curr_frame = event_data->hud.timer;
    JOBJ *hud_jobj = event_data->hud.gobj->hsd_object;

    // update action log
    if (curr_frame < sizeof(event_data->hud.action_log) / sizeof(u8)) {
        if (hmn_data->state == ASID_CATCH || hmn_data->state == ASID_CATCHDASH ||
                hmn_data->state == ASID_CATCHPULL || hmn_data->state == ASID_CATCHDASHPULL) {
            // look for grab
            event_data->hud.is_grab = true;
            event_data->hud.action_log[curr_frame] = PFACT_GRAB;
        } else if (hmn_data->state == ASID_THROWF) {
            // look for throw
            if (event_data->hud.is_grab) {
                event_data->hud.is_throw = true;
            }
            event_data->hud.action_log[curr_frame] = PFACT_THROW;
        } else if (hmn_data->state == ASID_DASH) {
            // look for dash
            if (event_data->hud.is_throw) {
                event_data->hud.is_dash = true;
            }
            event_data->hud.action_log[curr_frame] = PFACT_DASH;
        } else if (hmn_data->state == ASID_TURN) {
            // look for turnaround
            if (event_data->hud.is_dash) {
                event_data->hud.is_turn = true;
            }
            event_data->hud.action_log[curr_frame] = PFACT_TURN;
        } else if (hmn_data->state == ASID_ATTACKS4S) {
            // look for smash
            if (event_data->hud.is_turn) {
                event_data->hud.is_smash = true;
            }
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

    Tips_Think(event_data, hmn_data);

    if (event_data->hud.is_smash && hmn_data->state == ASID_ATTACKS4S && hmn_data->TM.state_frame > 15) {
        event_vars->Tip_Destroy();

        // update bar colors
        JOBJ *timingbar_jobj = hud_jobj->child->sibling->child->sibling;

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

    // debug text
    #if TM_DEBUG > 0
        Debug_UpdateText(event_data->debug.is_grab_text, event_data->hud.is_grab);
        Debug_UpdateText(event_data->debug.is_throw_text, event_data->hud.is_throw);
        Debug_UpdateText(event_data->debug.is_dash_text, event_data->hud.is_dash);
        Debug_UpdateText(event_data->debug.is_turn_text, event_data->hud.is_turn);
        Debug_UpdateText(event_data->debug.is_smash_text, event_data->hud.is_smash);
        Debug_UpdateText(event_data->debug.is_smash_text, event_data->hud.is_smash);
        Debug_UpdateText(event_data->debug.reset_timer_text, event_data->reset_timer);
    #endif

    JOBJ_AnimAll(hud_jobj);
}

void HUD_CamThink(GOBJ *gobj) {
    HUDCamThink(PfshOptions_Main[PFSHOPT_HUD]);
}

void HUD_ClearTimer(PivotFsmashData *event_data) {
    event_data->hud.timer = 0;

    // init action log
    for (int i = 0; i < sizeof(event_data->hud.action_log) / sizeof(u8); i++) {
        event_data->hud.action_log[i] = 0;
    }
}

// Fighter functions
void Fighter_Init(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu) {
    Reset_Fighter(event_data, hmn, cpu, DIR_RANDOM);
}

void Fighter_PlaceOnStage(GOBJ *fighter, float xpos, Direction facing_direction) {
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

// CPU functions
void CPU_Think(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu) {
    FighterData *hmn_data = hmn->userdata;
    FighterData *cpu_data = cpu->userdata;

    if (event_data->hud.is_throw) {
        if (cpu_data->state == ASID_THROWNF) {
            CPU_TDI(hmn_data, cpu_data);
        } else {
            CPUAction_PerformAction(cpu, CPUACT_JUMPAWAY, hmn);
        }
    }
    
}

void CPU_TDI(FighterData *hmn_data, FighterData *cpu_data) {
    // get knockback value
    float kb_angle = (float) hmn_data->throw_hitbox[0].angle * hmn_data->facing_direction;

    // perform TDI behavior
    int tdi_kind = PfshOptions_Main[PFSHOPT_TDI].option_val;
    if (tdi_kind == PFSHTDI_RANDOM) {
        tdi_kind = HSD_Randi(PFSHTDI_NUM - 1) + 1;
    }

    switch (tdi_kind) {
        case PFSHTDI_IN: {
            // get optimal tdi
            float tdi_angle = kb_angle + 90;

            // convert to analog input
            cpu_data->cpu.lstickX = cos(tdi_angle * M_1DEGREE) * 127;
            cpu_data->cpu.lstickY = sin(tdi_angle * M_1DEGREE) * 127;
            break;
        }
        case PFSHTDI_OUT: {
            // get optimal tdi
            float tdi_angle = kb_angle - 90;

            // convert to analog input
            cpu_data->cpu.lstickX = cos(tdi_angle * M_1DEGREE) * 127;
            cpu_data->cpu.lstickY = sin(tdi_angle * M_1DEGREE) * 127;
            break;
        }
        case PFSHTDI_DOWN: {
            cpu_data->cpu.lstickX = 0;
            cpu_data->cpu.lstickY = -127;
            break;
        }
        default: {
            Fighter_ZeroCPUInputs(cpu_data);
        }
    }
}

// Reset functions
void Reset_AutoThink(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu) {
    // No auto-reset
    if (PfshOptions_Main[PFSHOPT_RESET].option_val == PFSHRESET_OFF) {
        return;
    }

    FighterData *hmn_data = hmn->userdata;
    FighterData *cpu_data = cpu->userdata;

    // check if reset timer is set
    if (event_data->reset_timer > 0) {
        // decrement reset timer
        event_data->reset_timer--;

        // if reset timer is up, go back to ledge
        if (event_data->reset_timer == 0) {
            Reset_Fighter(event_data, hmn, cpu, DIR_RANDOM);
        }
    } else if (Reset_ShouldOnTimer(event_data, hmn_data, cpu_data)) {
        // check to set reset timer
        event_data->reset_timer = 30;
    } else if (Rest_ShouldInstantly(hmn_data, cpu_data)) {
        // reset instantly
        Reset_Fighter(event_data, hmn, cpu, DIR_RANDOM);
    }
}

void Reset_ManualThink(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu) {
    FighterData *hmn_data = hmn->userdata;

    // reset to designated direction
    if (hmn_data->input.down & HSD_BUTTON_DPAD_LEFT) {
        Reset_Fighter(event_data, hmn, cpu, DIR_LEFT);
    } else if (hmn_data->input.down & HSD_BUTTON_DPAD_RIGHT) {
        Reset_Fighter(event_data, hmn, cpu, DIR_RIGHT);
    }
}

void Reset_Fighter(PivotFsmashData *event_data, GOBJ *hmn, GOBJ *cpu, Direction hmn_direction) {
    FighterData *hmn_data = hmn->userdata;
    FighterData *cpu_data = cpu->userdata;

    // get random position if not already set
    float hmn_pos = -20 + HSD_Randi(40) + HSD_Randf();
    if (hmn_direction == DIR_RANDOM) {
        switch (PfshOptions_Main[PFSHOPT_RESET].option_val) {
            case PFSHRESET_LEFT: {
                hmn_direction = DIR_LEFT;
                break;
            }
            case PFSHRESET_RIGHT: {
                hmn_direction = DIR_RIGHT;
                break;
            }
            default: {
                hmn_direction = -1 + 2 * HSD_Randi(2);
            }
        }
    }
    Fighter_PlaceOnStage(hmn, hmn_pos, hmn_direction);

    float cpu_pos = hmn_pos + 10 * hmn_direction;
    Direction cpu_direction = -hmn_direction;
    Fighter_PlaceOnStage(cpu, cpu_pos, cpu_direction);

    // Set damage
    hmn_data->dmg.percent = 0;
    Fighter_SetHUDDamage(hmn_data->ply, 0);

    float cpu_dmg = 60 + HSD_Randi(20) + HSD_Randf();
    cpu_data->dmg.percent = cpu_dmg;
    Fighter_SetHUDDamage(cpu_data->ply, cpu_dmg);

    // Reset staling
    int *staleMoveTable = Fighter_GetStaleMoveTable(hmn_data->ply);
    memset(staleMoveTable, 0, 0x2C);

    // Set CPU behavior again
    cpu_data->cpu.ai = 15;

    Event_InitVariables(event_data);

    Remove_Particles_And_CamShake();
}

int Reset_ShouldOnTimer(PivotFsmashData *event_data, FighterData *hmn_data, FighterData *cpu_data) {
    if (!event_data->hud.is_grab && cpu_data->flags.hitlag) {
        // CPU was damaged with something other than a grab
        OSReport("Reset due to non-grab damage\n");
        return true;
    }
    if (event_data->hud.is_grab && !event_data->hud.is_throw && 
            (cpu_data->state == ASID_CAPTURECUT || cpu_data->state == ASID_THROWNHI || cpu_data->state == ASID_THROWNLW || cpu_data->state == ASID_THROWNB)) {
        // CPU escaped the grab or was thrown the wrong way
        OSReport("Reset due to grab escape\n");
        return true;
    }
    if (event_data->hud.is_throw && !event_data->hud.is_dash && 
            (cpu_data->state != ASID_THROWNF && !cpu_data->flags.hitstun || cpu_data->flags.hitlag)) {
        // CPU out of hitstun without player dashing
        OSReport("Reset due to not dashing\n");
        return true;
    }
    if (event_data->hud.is_dash && !event_data->hud.is_turn && 
            (cpu_data->flags.hitlag || !cpu_data->flags.hitstun)) {
        // CPU is not hit with Fsmash
        OSReport("Reset due to not forward smashing\n");
        return true;
    }
    if (event_data->hud.is_turn && !event_data->hud.is_smash && 
            hmn_data->TM.state_frame > 1) {
        // Player missed pivot
        OSReport("Reset due to missing pivot\n");
        return true;
    }
    if (event_data->hud.is_smash && hmn_data->TM.state_frame > 10) {
        // Player smash attack is complete
        OSReport("Reset due to smash\n");
        return true;
    }

    return false;
}

int Rest_ShouldInstantly(FighterData *hmn_data, FighterData *cpu_data) {
    return hmn_data->flags.dead == 1 || cpu_data->flags.dead == 1;
}

// Tips functions
void Tips_ToggleCallback(GOBJ *menu_gobj, int value) {
    Tips_Toggle(value);
}

void Tips_Think(PivotFsmashData *event_data, FighterData *hmn_data) {
    // Update c-stick and smash vars
    if (event_data->hud.is_dash) {
        float curr_cstick = fabs(hmn_data->input.cstick_x);
        if (curr_cstick >= PFSHTIP_SMASHTHRESHOLD && event_data->tip.last_cstick < PFSHTIP_SMASHTHRESHOLD) {
            event_data->tip.last_smash_frame = event_vars->game_timer;
        }
        event_data->tip.last_cstick = curr_cstick;
    }
    
    // Update l-stick and smash vars
    if (event_data->hud.is_throw) {
        float curr_lstick = fabs(hmn_data->input.lstick_x);
        if (curr_lstick >= PFSHTIP_SMASHTHRESHOLD && event_data->tip.lstick_minus_two < PFSHTIP_WALKTHRESHOLD && event_data->tip.lstick_minus_one < PFSHTIP_SMASHTHRESHOLD) {
            event_data->tip.last_dash_frame = event_vars->game_timer;
        }
        event_data->tip.lstick_minus_two = event_data->tip.lstick_minus_one;
        event_data->tip.lstick_minus_one = curr_lstick;
    }

    if (PfshOptions_Main[PFSHOPT_TIPS].option_val == OPTION_OFF || event_data->tip.is_displayed) {
        return;
    }

    // check for missed pivot input
    if (event_data->hud.is_turn && !event_data->hud.is_smash && event_data->tip.last_smash_frame > 0) {
        // early
        if (hmn_data->state == ASID_TURN) {
            event_data->tip.is_displayed = true;
            event_vars->Tip_Destroy();

            int frames_early = event_vars->game_timer - event_data->tip.last_smash_frame;
            event_vars->Tip_Display(PFSHTIP_DURATION, "Misinput:\nF-smashed %d frames early.", frames_early + 1);
        }

        // late
        else if (hmn_data->state == ASID_DASH) {
            event_data->tip.is_displayed = true;
            event_vars->Tip_Destroy();
            
            int frames_late = hmn_data->stateFrame;
            event_vars->Tip_Display(PFSHTIP_DURATION, "Misinput:\nF-smashed %d frames late.", frames_late - 1);
        }
    }

    // check for early/missed dash
    else if (event_data->hud.is_throw && !event_data->hud.is_dash && hmn_data->state == ASID_WALKSLOW) {
        // early
        if (event_data->tip.last_dash_frame > 0) {
            event_data->tip.is_displayed = true;
            event_vars->Tip_Destroy();

            int frames_early = event_vars->game_timer - event_data->tip.last_dash_frame;
            event_vars->Tip_Display(PFSHTIP_DURATION, "Misinput:\nDashed %d frames early.", frames_early - 1); // Dash has a 2-frame buffer
        } 
        
        // did not let go after throw
        else if (event_data->tip.lstick_minus_two >= PFSHTIP_SMASHTHRESHOLD && event_data->tip.lstick_minus_one >= PFSHTIP_SMASHTHRESHOLD) {
            event_data->tip.is_displayed = true;
            event_vars->Tip_Destroy();

            event_vars->Tip_Display(PFSHTIP_DURATION, "Misinput:\nDid not release stick after throw.");
        } 
        
        // dashed too slowly
        else if (event_data->tip.lstick_minus_two >= PFSHTIP_WALKTHRESHOLD && event_data->tip.lstick_minus_two < PFSHTIP_SMASHTHRESHOLD
            && event_data->tip.lstick_minus_one >= PFSHTIP_WALKTHRESHOLD && event_data->tip.lstick_minus_one < PFSHTIP_SMASHTHRESHOLD) {
            event_data->tip.is_displayed = true;
            event_vars->Tip_Destroy();

            event_vars->Tip_Display(PFSHTIP_DURATION, "Misinput:\nDash was input too slowly.");
        }
    }
}

// Menu Toggle functions
void MenuToggle_AutoReset(GOBJ *menu_gobj, int value) {
    // No auto-reset
    if (value == 3) {
        return;
    }

    // get fighter data
    GOBJ *hmn = Fighter_GetGObj(0);
    GOBJ *cpu = Fighter_GetGObj(1);
    PivotFsmashData *event_data = event_vars->event_gobj->userdata;

    Reset_Fighter(event_data, hmn, cpu, DIR_RANDOM);
}

// Event functions
void Event_Init(GOBJ *gobj) {
    PivotFsmashData *event_data = gobj->userdata;
    Init_Event_Vars("pfshData");

    GOBJ *hmn = Fighter_GetGObj(0);
    FighterData *hmn_data = hmn->userdata;
    GOBJ *cpu = Fighter_GetGObj(1);
    FighterData *cpu_data = cpu->userdata;

    // Init HUD
    HUD_Init(event_data);

    // Init Fighters
    Fighter_Init(event_data, hmn, cpu);
}

void Event_Think(GOBJ *event) {
    PivotFsmashData *event_data = event->userdata;

    // get fighter data
    GOBJ *hmn = Fighter_GetGObj(0);
    GOBJ *cpu = Fighter_GetGObj(1);

    HUD_Think(event_data, hmn);
    Reset_AutoThink(event_data, hmn, cpu);
    Reset_ManualThink(event_data, hmn, cpu);
    CPU_Think(event_data, hmn, cpu);
}

void Event_InitVariables(PivotFsmashData *event_data) {
    event_data->reset_timer = 0;
    event_data->tip.is_displayed = false;
    event_data->tip.last_cstick = 0;
    event_data->tip.last_smash_frame = 0;
    event_data->tip.lstick_minus_one = 1; // Start at max value so l-stick input from throw doesn't count as a dash attempt
    event_data->tip.lstick_minus_two = 1; // Start at max value so l-stick input from throw doesn't count as a dash attempt
    event_data->tip.last_dash_frame = 0;
    event_data->hud.is_grab = false;
    event_data->hud.is_throw = false;
    event_data->hud.is_turn = false;    
    event_data->hud.is_dash = false;
    event_data->hud.is_smash = false;

    HUD_ClearTimer(event_data);
}

// Debug functions
Text *Debug_InitText(int canvas_id, int index) {
    return Create_Simple_Text(canvas_id, 15, -10 + 2 * index, 0.01 * PFSHJOBJ_TEXTSCALE, "-");
}

void Debug_UpdateText(Text *text, s16 data) {
    Text_SetText(text, 0, "%2d", data);
}