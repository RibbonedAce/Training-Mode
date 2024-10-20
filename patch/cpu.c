#include "cpu.h"

int CPUAction_PerformAction(GOBJ *cpu, int action_id, GOBJ *hmn) {
    FighterData *cpu_data = cpu->userdata;
    FighterData *hmn_data = hmn->userdata;

    // get CPU action
    int action_done = false;
    CPUAction *action_list = CPUAction_Actions[action_id];
    int cpu_state = cpu_data->state;
    s16 cpu_frame = cpu_data->stateFrame;
    if (cpu_frame == -1) {
        cpu_frame = 0;
    }

    // clear inputs
    Fighter_ZeroCPUInputs(cpu_data);

    if (action_id == 0) {
        // if no action, report command as done
        return true;
    }
    
    // perform command
    // loop through all inputs
    int action_parse = 0;
    CPUAction *action_input = &action_list[action_parse];
    while (action_input != 0 && action_input->state != 0xFFFF) {
        int is_state = false;
        if (action_input->state >= ASID_ACTIONABLE && action_input->state <= ASID_FALLS) {
            is_state = CPUAction_CheckMultipleState(cpu, action_input->state);
        } else if (action_input->state == cpu_state) {
            is_state = true;
        }

        // check if this is the current state
        if (is_state) {
            // check if i'm on the right frame
            if (cpu_frame >= action_input->frame_low) {
                OSReport("exec input %d of %s\n", action_parse, CPU_ACTIONS_NAMES[action_id]);

                // perform this action
                s8 dir;
                int held = action_input->input;
                s8 lstick_x = action_input->lstick_x;
                s8 lstick_y = action_input->lstick_y;
                s8 cstick_x = action_input->cstick_x;
                s8 cstick_y = action_input->cstick_y;

                // stick direction
                switch (action_input->stick_dir) {
                    case STCKDIR_TOWARD: {
                        dir = Fighter_GetOpponentDir(cpu_data, hmn_data);
                        lstick_x *= dir;
                        cstick_x *= dir;
                        break;
                    }
                    case STCKDIR_AWAY: {
                        dir = Fighter_GetOpponentDir(cpu_data, hmn_data) * -1;
                        lstick_x *= dir;
                        cstick_x *= dir;
                        break;
                    }
                    case STCKDIR_FRONT: {
                        dir = cpu_data->facing_direction;
                        lstick_x *= dir;
                        cstick_x *= dir;
                        break;
                    }
                    case STCKDIR_BACK: {
                        dir = cpu_data->facing_direction;
                        lstick_x *= dir * -1;
                        cstick_x *= dir * -1;
                        break;
                    }
                    case STICKDIR_RDM: {
                        // random direction
                        if (HSD_Randi(2) == 0) {
                            dir = 1;
                        } else {
                            dir = -1;
                        }

                        lstick_x *= dir;
                        cstick_x *= dir;
                        break;
                    }
                    default: {
                        break;
                    }
                }

                // perform this action
                cpu_data->cpu.held = held;
                cpu_data->cpu.lstickX = lstick_x;
                cpu_data->cpu.lstickY = lstick_y;
                cpu_data->cpu.cstickX = cstick_x;
                cpu_data->cpu.cstickY = cstick_y;

                // check if this was the last action
                action_done = action_input->is_last;
                
                break;
            }
        }

        // get next input
        action_parse++;
        action_input = &action_list[action_parse];
    }

    return action_done;
}

int CPUAction_CheckMultipleState(GOBJ *cpu, int group_kind) {
    static u16 gr_actionable[] = {
        ASID_WAIT, ASID_WALKSLOW, ASID_WALKMIDDLE, ASID_WALKFAST, ASID_RUN, ASID_SQUATWAIT, ASID_OTTOTTOWAIT, ASID_GUARD
    };
    static u16 air_actionable[] = {
        ASID_JUMPF, ASID_JUMPB, ASID_JUMPAERIALF, ASID_JUMPAERIALB, ASID_FALL, ASID_FALLAERIALF, ASID_FALLAERIALB,
        ASID_DAMAGEFALL, ASID_DAMAGEFLYROLL, ASID_DAMAGEFLYTOP
    };
    static u16 air_damage[] = {
        ASID_DAMAGEFLYHI, ASID_DAMAGEFLYN, ASID_DAMAGEFLYLW, ASID_DAMAGEFLYTOP, ASID_DAMAGEFLYROLL, ASID_DAMAGEFALL
    };
    static u16 jump_states[] = {ASID_JUMPF, ASID_JUMPB, ASID_JUMPAERIALF, ASID_JUMPAERIALB,
        // Puff/Kirby aerial jumps
        0X155, 0x156, 0x157, 0x158, 0x159
    };
    static u16 fall_states[] = {ASID_FALL, ASID_FALLAERIAL, ASID_FALLAERIALF, ASID_FALLAERIALB};

    FighterData *cpu_data = cpu->userdata;
    int cpu_state = cpu_data->state;

    // if 0, check the one that corresponds with ground state
    if (group_kind == ASID_ACTIONABLE) {
        group_kind += cpu_data->phys.air_state + 1;
    }

    if (group_kind == ASID_ACTIONABLEGROUND) {
        // ground
        // check ground actionable
        for (int i = 0; i < sizeof(gr_actionable) / sizeof(u8); i++) {
            if (cpu_state == gr_actionable[i]) {
                return true;
            }
        }
        // landing
        if (cpu_data->state == ASID_LANDING && cpu_data->stateFrame >= cpu_data->attr.normal_landing_lag) {
            return true;
        }
    } else if (group_kind == ASID_ACTIONABLEAIR) {
        // air
        // check air actionable
        for (int i = 0; i < sizeof(air_actionable) / sizeof(u16); i++) {
            if (cpu_state == air_actionable[i]) {
                return true;
            }
        }
        for (int i = 0; i < sizeof(air_damage) / sizeof(u16); i++) {
            if (cpu_state == air_damage[i] && !cpu_data->flags.hitstun) {
                return true;
            }
        }
    } else if (group_kind == ASID_DAMAGEAIR) {
        // damage state that requires wiggling
        // check air actionable
        for (int i = 0; i < sizeof(air_damage) / sizeof(u16); i++) {
            if (cpu_state == air_damage[i]) {
                return true;
            }
        }
    } else if (group_kind == ASID_JUMPS) {
        // jump states
        for (int i = 0; i < sizeof(jump_states) / sizeof(u16); i++) {
            if (cpu_state == jump_states[i]) {
                return true;
            }
        }
    } else if (group_kind == ASID_FALLS) {
        // fall states
        for (int i = 0; i < sizeof(fall_states) / sizeof(u16); i++) {
            if (cpu_state == fall_states[i]) {
                return true;
            }
        }
    }

    return false;
}