#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple rules, non optimal, more so to get used to making making a plan and then to create the code. To translate from an idea to code 
// Build a terminal game in C where the player plays repeated 
// Prisoners Dilemma game choice against an AI with 3 difficulty levels (Easy, Medium, Hard)

/* ==================== Requirements =====================

// The Situation:
                - 2 suspected criminals are arrested.
                - The police think that they were trying to rob a store--but the cops only have enough evidence to prove that they were trespassing.
                - Thus, the police need one of criminals to rat ut the other to charge them for the bigger crime.

                -- How can the police do this? -- Offer them a deal...

    The Offer:
            -- If no one confesses to robbery, the police can only charge the prisoners for trespassing. --- punishment: 1 year in jail each.

            -- If one confesses and the other doesn't, the police will be lenient on the rat and severely punish the quiet one. --- punishment: 10 yrs in jail for the quiet one; 0 years for the rat

            -- If both confess, the police punish both of them equally. --- punishment: They both get 5 years in jail.

 

Game Rules ->  

Players: 2 Players ---> one real player vs one ai user
Strategies: Cooperate (C) ,  Defect (D)
Payoffs:  cc(-1, -1) , dd(-5, -5), cd(-10, 0), dc(0, -10)           cc = both get three years of prison
                                                            dd = both get one year of prison
                                                            cd = user who cooperates gets 0 years of prison, user who didn't gets 5 years of prison
                                                            dc = user who doesn't cooperate gets 5 years of prison, User who does get's 0 years of prison

We will have it run 3 times, so a total of 3 rounds to be played by the user and the ai bot.
When the game ends, print total scores and who "won"

Difficulty ->
                Easy: AI picks simple strategy: They are simply rational and bc of that rational players by rule never play a strictly dominanted strat
                Medium: AI plays Tit-for-Tat (copy your last move that you did)
                Hard: Ai uses a more punishing strategt (e.g. Grim Trigger: if you ever defect, it defects forever)

UI -> 
        Runs in a terminal.
        Player sees:
                        Current round
                        Their last move and AI last move
                        Current scores
                        Prompt [c] Cooperate or [D] Defect?


SCENES -->          ## I want to note that for memory sake? will this be an issue or an issue to do to 
            TITLE_SCENE:
                        - Prisoner's Dilemma
                        - User gets prompt to type user_name
                        - returns- Welcome Game Theorist (scanf" %c, user_name)

            MAIN_MENU:
                        - Show:
                        - the three different level options --> Have the user select easy, medium, or hard
                        - User taps easy -> sends them to new scene
            
            LEVEL_INTRO_SCENE: (easy)
                        - Show Charector name and text box:
                        - "Welcome, Theorists."
                        - "In this level, you face a simple opponent "
                        - "Each round won results to 1 star."
                        - "The goal is to obtain all 3 stars to advance into the next round"
                        - User preses the spacebar key to advance the dialogue lines
                        - At the end: go to GAME_PLAY_SCENE

            GAME_PLAY_SCENE:
                        - Show:
                        - Round number (1,2,3)
                        - Current score
                        - textbox: "Choose your move"
                        - Player chooses action (Cooperate / Defect)
                        - AI chooses action based on diffuculty
                        - Result Text: " You cooperated"

            LEVEL_RESULT_SCENE:
                        - Compute stars: 
                        - 3 wins -> You won all 3 rounds. Here are (3 c program created special charector design of a simple star) for winning! You are ready for the next level- good luck!
                        - 2 wins -> You won 2 rounds. That is impressive. You obtained (2 c program created special charector design of a simple star) for winning!
                        - 1 wins -> You only won 1 rounds. Do better. Here is a pity (1 c program created special charector design of a simple star)
                        - Text box shows the message
                        - "Press spaceBar to continue back to the main menu"

            REPEAT;
*/

/* The offer (years in jail):
    - If no one confesses:
        Each gets 1 year -> payoff: (-1, -1)
    
    - If one confesses and the other stays quiet:
        Rat:   0 years  -> payoff: (0)
        Quiet: 10 years -> payoff: (-10)

    - If both confess:
        Each gets 5 years -> payoff: (-5, -5)

We model payoffs as negative numbers (bad = more negative).
Game is played for 3 rounds.
*/

// Difficulty levels
typedef enum {
    DIFF_EASY = 1,
    DIFF_MEDIUM = 2,
    DIFF_HARD = 3
} Difficulty;

// Scenes (states) of the game
typedef enum {
    SCENE_TITLE,
    SCENE_MAIN_MENU,
    SCENE_LEVEL_INTRO,
    SCENE_GAMEPLAY,
    SCENE_LEVEL_RESULT,
    SCENE_EXIT
} Scene;

#define MAX_NAME_LEN 32
#define ROUNDS_PER_LEVEL 3

typedef struct {
    char playerName[MAX_NAME_LEN];

    Difficulty difficulty;
    Scene scene;

    int currentRound;         // 1..3 during gameplay

    int playerTotalPayoff;    // still tracked, not shown per round
    int aiTotalPayoff;

    int roundsPlayerWon;
    int roundsAIWon;
    char playerMoves[ROUNDS_PER_LEVEL];
    char aiMoves[ROUNDS_PER_LEVEL];

    int grimTriggered;        // for Hard difficulty (Grim Trigger)
} GameState;

/* ================= Utility Functions ================= */

void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Helper to flush leftover characters (like '\n') after scanf
void flushInputLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // discard rest of the line
    }
}

void pressEnterToContinue(void) {
    printf("\nPress ENTER to continue...");
    flushInputLine();
}

/* ============ Payoff Table HUD (graphics v1) ========= */

void printPayoffTable(void) {
    printf("Payoff Matrix (years in jail: You, AI)\n\n");
    printf("           AI\n");
    printf("         C          D\n");
    printf("You C  (-1, -1)  (-10,  0)\n");
    printf("    D   ( 0,-10)  ( -5, -5)\n\n");
    printf("Note: more negative = more years in jail = worse.\n\n");
}

/* ================= Payoff Logic ====================== */
/*
Strategies: C (cooperate), D (defect)

Payoffs (negative = years in jail):
    cc: (-1,  -1)   // both stay quiet, 1 year each
    dd: (-5,  -5)   // both confess, 5 years each
    cd: (-10,  0)   // player cooperates (quiet), AI defects (rats)
    dc: ( 0, -10)   // player defects (rats), AI cooperates (quiet)
*/

void computePayoffs(char playerMove, char aiMove,
                    int *playerPayoff, int *aiPayoff) {
    if (playerMove == 'C' && aiMove == 'C') {
        *playerPayoff = -1;
        *aiPayoff = -1;
    } else if (playerMove == 'D' && aiMove == 'D') {
        *playerPayoff = -5;
        *aiPayoff = -5;
    } else if (playerMove == 'C' && aiMove == 'D') {
        *playerPayoff = -10;
        *aiPayoff = 0;
    } else if (playerMove == 'D' && aiMove == 'C') {
        *playerPayoff = 0;
        *aiPayoff = -10;
    } else {
        // Fallback - should not happen if inputs are valid
        *playerPayoff = 0;
        *aiPayoff = 0;
    }
}

/* ================= Input Helpers ===================== */

char getPlayerMove(void) {
    char move;
    while (1) {
        printf("Choose your move: [C] Cooperate or [D] Defect: ");
        if (scanf(" %c", &move) != 1) {
            flushInputLine();
            continue;
        }

        if (move == 'C' || move == 'c') {
            return 'C';
        } else if (move == 'D' || move == 'd') {
            return 'D';
        } else {
            printf("Invalid choice. Please enter C or D.\n");
        }
    }
}

/* ============== AI Move Logic (Strategies) =========== */
/*
Difficulty ->

Easy:
    - AI is "rational" and never plays a strictly dominated strategy.
    - In Prisoner's Dilemma, Cooperate (C) is strictly dominated by Defect (D),
      so AI ALWAYS defects (D).

Medium (Tit-for-Tat):
    - Round 1: AI cooperates (C).
    - Round >= 2: AI copies the player's previous move.

Hard (Grim Trigger):
    - Start cooperating (C).
    - If the player ever defects in any previous round,
      AI defects (D) forever afterwards.
*/

char getAIMove(const GameState *state) {
    if (state->difficulty == DIFF_EASY) {
        return 'D';
    } else if (state->difficulty == DIFF_MEDIUM) {
        // Tit for Tat
        if (state->currentRound == 1) {
            return 'C';
        } else {
            char lastPlayerMove = state->playerMoves[state->currentRound - 2];
            return lastPlayerMove;
        }
    } else if (state->difficulty == DIFF_HARD) {
        // Grim Trigger
        if (state->grimTriggered) {
            return 'D';
        } else {
            for (int i = 0; i < state->currentRound - 1; i++) {
                if (state->playerMoves[i] == 'D') {
                    // Player defected previously, trigger permanent defection
                    return 'D';
                }
            }
            return 'C';
        }
    }

    // Default
    return 'C';
}

/* ============== Scene Implementations ================= */

void runTitleScene(GameState *state) {
    clearScreen();
    printf("========================================\n");
    printf("          PRISONER'S DILEMMA            \n");
    printf("========================================\n\n");
    printf("The Situation.\n");
    printf("-- 2 suspected criminals are arrested.\n");
    printf("-- The police think that they were trying to rob a store--but the cops only have enough evidence to prove that they were trespassing.\n");
    printf("-- Thus, the police need one of the criminals to rat on the other to charge them for the bigger crime.\n\n");
    printf("-- How can the police do this? --Offer them a deal...\n\n");
    
    printf("The Offer.\n");
    printf("-- If no one confesses to robbery, the police can only charge the prisoners for trespassing. --- punishment: 1 year in jail each.\n");
    printf("-- If one confesses and the other doesn't, the police will be lenient on the rat and severely punish the quiet one. --- punishment: 10 yrs in jail for the quiet one; 0 years for the rat.\n");
    printf("-- If both confess, the police punish both of them equally. --- punishment: They both get 5 years in jail.\n\n");
    
    printf("Game Rules.\n");
    printf("-- Strategies: Cooperate (C) ,  Defect (D)\n");
    printf("Payoffs:  cc(-1, -1) , dd(-5, -5), cd(-10, 0), dc(0, -10)\n\n");

    printf("There will be 3 rounds to this game.\n");
    printf("Enter your name: ");
    scanf("%31s", state->playerName);
    flushInputLine();   // clear leftover '\n'

    printf("\nWelcome, Game Theorist %s.\n", state->playerName);
    printf("\nThis game was created by Timothy Toth.\n");
    pressEnterToContinue();

    state->scene = SCENE_MAIN_MENU;
}

void runMainMenuScene(GameState *state) {
    clearScreen();
    printf("========================================\n");
    printf("               MAIN MENU                \n");
    printf("========================================\n\n");
    printf("%s. Select your challenge:\n\n", state->playerName);
    printf("  1) Easy\n");
    printf("  2) Medium\n");
    printf("  3) Hard\n");
    printf("  0) Exit\n\n");

    int choice;
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
        flushInputLine();
        return;
    }
    flushInputLine();

    if (choice == 0) {
        state->scene = SCENE_EXIT;
        return;
    } else if (choice >= 1 && choice <= 3) {
        state->difficulty = (Difficulty)choice;
        state->scene = SCENE_LEVEL_INTRO;
    } else {
        printf("Invalid option. Try again.\n");
        pressEnterToContinue();
    }
}

void runLevelIntroScene(GameState *state) {
    clearScreen();
    printf("========================================\n");
    printf("             LEVEL INTRO                \n");
    printf("========================================\n\n");

    printf("Judge: \"Welcome, Game Theorist %s.\"\n", state->playerName);

    // NEW: Difficulty-dependent opponent introduction
    if (state->difficulty == DIFF_EASY) {
        printf("Judge: \"In this level, you face a simple opponent.\"\n");
    } else if (state->difficulty == DIFF_MEDIUM) {
        printf("Judge: \"In this level, you will face a worthy opponent.\"\n");
    } else if (state->difficulty == DIFF_HARD) {
        printf("Judge: \"In this level, you will face an expert opponent.\"\n");
    }

    printf("Judge: \"Each round you win earns you 1 star.\"\n");
    printf("Judge: \"Win all 3 rounds to collect all 3 stars.\"\n");
    pressEnterToContinue();

    // Initialize round data
    state->currentRound = 1;
    state->playerTotalPayoff = 0;
    state->aiTotalPayoff = 0;
    state->roundsPlayerWon = 0;
    state->roundsAIWon = 0;
    state->grimTriggered = 0;
    for (int i = 0; i < ROUNDS_PER_LEVEL; i++) {
        state->playerMoves[i] = '-';
        state->aiMoves[i] = '-';
    }

    state->scene = SCENE_GAMEPLAY;
}

void runGameplayScene(GameState *state) {
    // Play exactly 3 rounds here, then move to result scene.
    for (state->currentRound = 1;
         state->currentRound <= ROUNDS_PER_LEVEL;
         state->currentRound++) {

        clearScreen();
        printf("========================================\n");
        printf("              GAMEPLAY                  \n");
        printf("========================================\n\n");

        printf("Round %d of %d\n\n", state->currentRound, ROUNDS_PER_LEVEL);

        // Show payoff matrix every round so the player remembers the rules
        printPayoffTable();

        if (state->currentRound > 1) {
            printf("Last round summary:\n");
            printf("  You played: %c\n", state->playerMoves[state->currentRound - 2]);
            printf("  AI played : %c\n", state->aiMoves[state->currentRound - 2]);
            printf("  Score so far - You: %d wins, AI: %d wins\n\n",
                   state->roundsPlayerWon, state->roundsAIWon);
        }

        char pMove = getPlayerMove();
        char aMove = getAIMove(state);

        // Update grim trigger flag if needed
        if (state->difficulty == DIFF_HARD && pMove == 'D') {
            state->grimTriggered = 1;
        }

        int pPayoff, aPayoff;
        computePayoffs(pMove, aMove, &pPayoff, &aPayoff);

        // Track total payoff internally (not shown per round now)
        state->playerTotalPayoff += pPayoff;
        state->aiTotalPayoff += aPayoff;

        state->playerMoves[state->currentRound - 1] = pMove;
        state->aiMoves[state->currentRound - 1] = aMove;

        // Decide who won THIS round, based on payoff (less jail = better)
        if (pPayoff > aPayoff) {
            state->roundsPlayerWon++;
        } else if (aPayoff > pPayoff) {
            state->roundsAIWon++;
        }

        // Show ONLY this round's result (no cumulative payoff numbers)
        clearScreen();
        printf("========================================\n");
        printf("           ROUND %d RESULTS             \n", state->currentRound);
        printf("========================================\n\n");

        printf("  You played: %c\n", pMove);
        printf("  AI played : %c\n\n", aMove);
        printf("  Your payoff this round: %d\n", pPayoff);
        printf("  AI payoff this round   : %d\n\n", aPayoff);

        printf("Round wins so far:\n");
        printf("  You: %d wins\n", state->roundsPlayerWon);
        printf("  AI : %d wins\n", state->roundsAIWon);

        pressEnterToContinue();
    }

    // After 3 rounds, go to result scene
    state->scene = SCENE_LEVEL_RESULT;
}

void runLevelResultScene(GameState *state) {
    clearScreen();
    printf("========================================\n");
    printf("             LEVEL RESULTS              \n");
    printf("========================================\n\n");

    printf("Round wins:\n");
    printf("  You: %d\n", state->roundsPlayerWon);
    printf("  AI : %d\n\n", state->roundsAIWon);

    // Optional: still show total payoff (for game theory nerd flex)
    printf("Total payoff (more negative = worse, years in jail):\n");
    printf("  You: %d\n", state->playerTotalPayoff);
    printf("  AI : %d\n\n", state->aiTotalPayoff);

    if (state->roundsPlayerWon > state->roundsAIWon) {
        printf("Judge: \"You win the overall match. Well played.\"\n\n");
    } else if (state->roundsPlayerWon < state->roundsAIWon) {
        printf("Judge: \"The AI wins this time. Study the matrix and try again.\"\n\n");
    } else {
        printf("Judge: \"The match is a tie. Equal mistakes, equal brilliance.\"\n\n");
    }

    int stars = 0;
    if (state->roundsPlayerWon == 3) stars = 3;
    else if (state->roundsPlayerWon == 2) stars = 2;
    else if (state->roundsPlayerWon == 1) stars = 1;
    else stars = 0;

    // Build a simple ASCII star display
    printf("Stars earned: ");
    for (int i = 0; i < stars; i++) {
        printf("*");
    }
    for (int i = stars; i < 3; i++) {
        printf("-");
    }
    printf("\n\n");

    if (stars == 3) {
        printf("Judge: \"You won all 3 rounds.\"\n");
        printf("Judge: \"*** You are ready for the next level. Good luck! ***\"\n");
    } else if (stars == 2) {
        printf("Judge: \"You won 2 rounds. That is impressive.\"\n");
        printf("Judge: \"You obtained 2 stars!\"\n");
    } else if (stars == 1) {
        printf("Judge: \"You only won 1 round. Do better next time.\"\n");
        printf("Judge: \"Here is a pity star.\"\n");
    } else {
        printf("Judge: \"You lost all rounds. Tough break.\"\n");
        printf("Judge: \"Try again to improve your strategy.\"\n");
    }

    printf("\nPress ENTER to return to the main menu...");
    pressEnterToContinue();

    state->scene = SCENE_MAIN_MENU;
}

/* ========================= main ======================= */

int main(void) {
    GameState state;
    memset(&state, 0, sizeof(GameState));
    state.scene = SCENE_TITLE;

    while (state.scene != SCENE_EXIT) {
        switch (state.scene) {
            case SCENE_TITLE:
                runTitleScene(&state);
                break;
            case SCENE_MAIN_MENU:
                runMainMenuScene(&state);
                break;
            case SCENE_LEVEL_INTRO:
                runLevelIntroScene(&state);
                break;
            case SCENE_GAMEPLAY:
                runGameplayScene(&state);
                break;
            case SCENE_LEVEL_RESULT:
                runLevelResultScene(&state);
                break;
            case SCENE_EXIT:
            default:
                break;
        }
    }

    clearScreen();
    printf("Goodbye, %s. Thanks for playing.\n",
           state.playerName[0] ? state.playerName : "Theorist");

    return 0;
}
