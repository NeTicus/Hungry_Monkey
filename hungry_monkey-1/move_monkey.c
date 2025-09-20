#include <stdlib.h>
#include <string.h>
#include "hungry_monkey.h"

struct monkey_action move_monkey(int field[][FIELD_WIDTH], void *monkey_state){
    int monkey_row = FIELD_HEIGHT - 1;  // The monkey is always on the bottom row
    int monkey_col = -1;     // Stores the monkey's current column from left to right

    // Search the bottom row to find the monkey's current column
    for (int col = 0; col < FIELD_WIDTH; col++) {
        if (field[monkey_row][col] == MONKEY_VAL) {
            monkey_col = col;
            break;
        }
    }

    // If this is the first time move_monkey is being called, allocate memory for the state
    if (monkey_state == NULL) {
        monkey_state = malloc(sizeof(int));  // Not storing anything specific for now
        *((int*)monkey_state) = 0;  // Initialize state with a dummy value
    }

    // Looks for the closest reachable treat
    int best_row = -1, best_col = -1;   // Store the coordinates of the best treat
    int min_distance = FIELD_WIDTH + FIELD_HEIGHT;  // Initialize to a large value

    // Scan the field from top to bottom, left to right
    for (int row = 0; row < FIELD_HEIGHT; row++) {
        for (int col = 0; col < FIELD_WIDTH; col++) {
            if (field[row][col] == TREAT_VAL) {
                int vertical = monkey_row - row;    // How many rows the treat is above the monkey
                int horizontal = abs(monkey_col - col); // Horizontal distance from monkey to treat

                // Only considers treats the monkey can reach in time
                if (horizontal <= vertical) { 
                    int total_distance = vertical + horizontal;

                    // Keeps track of the closest reachable treat
                    if (total_distance < min_distance) {
                        min_distance = total_distance;
                        best_row = row;
                        best_col = col;
                    }
                }
            }
        }
    }

    struct monkey_action action;
    action.state = monkey_state;

    if (best_col == -1 || monkey_col == -1) {
        action.move = MOVE_FWD; // Stays in place if there are no reachable treats
    } else if (best_col < monkey_col) {
        action.move = MOVE_LEFT;
    } else if (best_col > monkey_col) {
        action.move = MOVE_RIGHT;
    } else {
        action.move = MOVE_FWD;
    }

    return action;
}