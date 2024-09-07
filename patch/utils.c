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