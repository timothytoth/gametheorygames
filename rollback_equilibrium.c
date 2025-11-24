#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// More practice of C programming for me incooperating game theory
// Now i am creating puzzles that solve for r equilibrium using 
// backward induction to solve. 

// username length and answer length
#define MAX_NAME_LENGTH 32
#define MAX_INPUT_LENGTH 100

// Set scenes that user will be going through 
typedef enum{
   SCENE_TITLE,
   SCENE_MAIN_MENU,
   SCENE_PUZZLE_1,
   SCENE_PUZZLE_2,
   SCENE_PUZZLE_3,
   SCENE_EXIT
} Scene;

// Track player identity and what scene they are in
typedef struct {
   char playerName[MAX_NAME_LENGTH];
   Scene scene;
} GameState;

// UTILITY FUNCTIONS
void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void flushInputLine(void) {                 
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void pressEnterToContinue(void) {
    printf("\nPress ENTER to continue...");
    flushInputLine();
}

void printSeparator(void) {
    printf("========================================\n");
}

// DRAW GAME TREES

void draw_puzzle_1_tree(void) {
    printf("\n");
    printf("PUZZLE 1: The Corporate Merger\n");
    printf("(Player 1 = Alice, Player 2 = Bob)\n");
    printf("Alice: S = Sweet terms(friendly), T = Tought terms(aggressive)\n");
    printf("Bob: N = Negotiate, F = Finalize\n\n");
    
    // This was nothing compared to Puzzle 2 LOL
    printf("                     (Alice)\n");
    printf("                    /         \\\n");
    printf("                   S           T\n");
    printf("                  /             \\\n");
    printf("              (Bob)            (Bob)\n");
    printf("              /   \\            /   \\\n");
    printf("             N     F          N     F\n");
    printf("            /       \\        /       \\\n");
    printf("        (Alice)   (5,4)   (0,1)    (3,2)\n");
    printf("         /   \\\n");
    printf("        S     T\n");
    printf("       /       \\\n");
    printf("    (2,3)     (Bob)\n");
    printf("              /   \\\n");
    printf("             S     T\n");
    printf("            /       \\\n");
    printf("         (4,5)    (Alice)\n");
    printf("                  /   \\\n");
    printf("                 S     T\n");
    printf("                /       \\\n");
    printf("             (1,0)    (2,2)\n");
    printf("\n");
}

void draw_puzzle_2_tree(void) {
    printf("\n");
    printf("PUZZLE 2: Urban/Rural Mall Problem\n");
    printf("(F = Frieda, BG = Big Giant, T = Titan)\n");
    printf("RM = Rural Mall, UM = Urban Mall\n\n");
    
    // Took TO long to format this to my liking and making it look good
    // out of spite i did it anyway.
    // In the future i am using gpt to format clean game trees
    // reiterating im doing this coding as a hobby. I want to learn the old fashioned way and enjoy my time coding
    // I am not using gpt to write or help teach me code.
    // Im using the fundamentals and basics
    // C programming for absolute beginners by Dean Miller and Greg Perry
    // I highly reccomend reading and doing the examples and problems along with the author 
    // Made it so i could understand the basics and begin doing projects and getting my hands dirty
    // Thus, these games i created. combining my skill and passion for economics and game theory.
    printf("                                     F\n");
    printf("                          __________/ \\__________\n");
    printf("                         /                       \\\n");
    printf("                       RM                         UM\n");
    printf("                      /                             \\\n");
    printf("                    BG                               BG\n");
    printf("                 __/  \\__                         __/  \\__\n");
    printf("                /        \\                       /        \\\n");
    printf("              RM          UM                   RM          UM\n");
    printf("             /              \\                 /              \\\n");
    printf("            T                T               T                T\n");
    printf("          /   \\            /   \\           /   \\            /   \\\n");
    printf("        RM     UM        RM     UM       RM     UM        RM     UM\n");
    printf("       /         \\      /         \\     /         \\      /         \\\n");
    printf("   (4,4,4)    (4,4,3)(4,3,4)   (2,5,5)(3,4,4)   (5,2,5)(5,5,2)   (1,5,5)\n");
    printf("\n");
}

void draw_puzzle_3_tree(void) {
    printf("\n");
    printf("PUZZLE 3: Coordination Game\n");
    printf("(B = Bart, C = Cassie, D = Dad)\n");
    printf("A = Amusment park, S = Science muesum\n\n");
    
// game tree 3
printf("                                    (B)\n");
printf("                      ______________/ \\\\________________\n");
printf("                     /                                 \\\n");
printf("                    A                                    S\n");
printf("                   /                                     \\\n");
printf("                 (C)                                     (C)\n");
printf("              __/  \\\\__                               __/  \\\\__\n");
printf("             /        \\\\                             /        \\\\ \n");
printf("            A            S                          A          S\n");
printf("           /              \\\\                       /            \\\n");
printf("     (3,2,4)             (D)                     (D)         (2,3,4)\n");
printf("                       /  |  \\\\                /  |  \\\n");
printf("                      /   |   \\\\              /   |   \\\n");
printf("                     /    |    \\\\            /    |    \\\n");
printf("              (3,2,2) (1,1,3) (2,3,2)   (3,2,2) (1,1,3) (2,3,2)\n");
printf("\n");

}



bool checkAnswer(const char* userAnswer, const char* correctAnswer) {
    // Remove whitespace from both strings for comparison
    char cleanUser[MAX_INPUT_LENGTH] = {0};
    char cleanCorrect[MAX_INPUT_LENGTH] = {0};
    
    int j = 0;
    for (int i = 0; userAnswer[i] != '\0'; i++) {
        if (userAnswer[i] != ' ' && userAnswer[i] != '\n') {
            cleanUser[j++] = userAnswer[i];
        }
    }
    cleanUser[j] = '\0';
    
    j = 0;
    for (int i = 0; correctAnswer[i] != '\0'; i++) {
        if (correctAnswer[i] != ' ' && correctAnswer[i] != '\n') {
            cleanCorrect[j++] = correctAnswer[i];
        }
    }
    cleanCorrect[j] = '\0';
    
    return strcmp(cleanUser, cleanCorrect) == 0;
}

bool checkFormat2Players(const char* input) {
    // Check if format matches (x,y) where x and y are digits
    char cleanInput[MAX_INPUT_LENGTH] = {0};
    int j = 0;
    
    // Remove whitespace
    for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
        if (input[i] != ' ') {
            cleanInput[j++] = input[i];
        }
    }
    cleanInput[j] = '\0';
    
    // Must start with ( and end with )
    if (cleanInput[0] != '(' || cleanInput[strlen(cleanInput)-1] != ')') {
        return false;
    }
    
    // Must have exactly one comma
    int commaCount = 0;
    for (int i = 0; cleanInput[i] != '\0'; i++) {
        if (cleanInput[i] == ',') commaCount++;
    }
    if (commaCount != 1) return false;
    
    // Check pattern: (digit,digit)
    int len = strlen(cleanInput);
    if (len < 5) return false; // Minimum: (x,y)
    
    // Find comma position
    int commaPos = -1;
    for (int i = 0; cleanInput[i] != '\0'; i++) {
        if (cleanInput[i] == ',') {
            commaPos = i;
            break;
        }
    }
    
    // Check if there's at least one digit before and after comma
    bool digitBefore = false, digitAfter = false;
    for (int i = 1; i < commaPos; i++) {
        if (cleanInput[i] >= '0' && cleanInput[i] <= '9') {
            digitBefore = true;
            break;
        }
    }
    for (int i = commaPos + 1; i < len - 1; i++) {
        if (cleanInput[i] >= '0' && cleanInput[i] <= '9') {
            digitAfter = true;
            break;
        }
    }
    
    return digitBefore && digitAfter;
}

bool checkFormat3Players(const char* input) {
    // Check if format matches (x,y,z) where x, y, z are digits
    char cleanInput[MAX_INPUT_LENGTH] = {0};
    int j = 0;
    
    // Remove whitespace
    for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
        if (input[i] != ' ') {
            cleanInput[j++] = input[i];
        }
    }
    cleanInput[j] = '\0';
    
    // Must start with ( and end with )
    if (cleanInput[0] != '(' || cleanInput[strlen(cleanInput)-1] != ')') {
        return false;
    }
    
    // Must have exactly two commas
    int commaCount = 0;
    for (int i = 0; cleanInput[i] != '\0'; i++) {
        if (cleanInput[i] == ',') commaCount++;
    }
    if (commaCount != 2) return false;
    
    // Check for digits in each section
    int len = strlen(cleanInput);
    if (len < 7) return false; // Minimum: (x,y,z)
    
    return true;
}

// SCENE FUNCTIONS

void runTitleScene(GameState *state) {
    clearScreen();
    printSeparator();
    printf("Game Theory Problems with Backward Induction!\n");
    printSeparator();
    printf("\n");

    printf("A Game Theory Challenge!\n");
    printf("Created by Tim Toth\n");
    printf("Learn a key lesson in game theory\n");
    printf("Enjoy!\n\n");

    printf("Enter your username: ");

    if(fgets(state->playerName, MAX_NAME_LENGTH, stdin) != NULL) {
        size_t len = strlen(state->playerName);
        if (len > 0 && state->playerName[len-1] == '\n') {
            state->playerName[len-1] = '\0';
        }
    }

    if (strlen(state->playerName) == 0) {
        strcpy(state->playerName, "Theorist");
    }

    printf("\nWelcome, Game Theorist %s!\n", state->playerName);
    pressEnterToContinue(); 

    state->scene = SCENE_MAIN_MENU;
} 

void runMainMenuScene(GameState *state) {
    clearScreen();
    printSeparator();
    printf("            Main Menu          \n");
    printSeparator();
    printf("\n");
    printf("Sup, %s. Select a puzzle and solve by finding the Rollback Equilibrium!\n\n", state->playerName);

    printf("  1) Puzzle 1: The Corporate Merger\n");
    printf("  2) Puzzle 2: Urban/Rural Mall Problem\n");
    printf("  3) Puzzle 3: Coordination Game\n");
    printf("\n  0) Exit\n\n");

    int choice;
    printf("Enter your choice: ");
    if(scanf("%d", &choice) != 1) {
        flushInputLine();
        return;
    }
    flushInputLine();

    if (choice == 0) {
        state->scene = SCENE_EXIT;
    } else if (choice == 1) {
        state->scene = SCENE_PUZZLE_1;
    } else if (choice == 2) {
        state->scene = SCENE_PUZZLE_2;
    } else if (choice == 3) {
        state->scene = SCENE_PUZZLE_3;
    } else {
        printf("Invalid choice!\n");
        pressEnterToContinue();
    }
}

void runPuzzleScene(GameState *state) {
    clearScreen();
    printSeparator();
    printf("            Puzzle 1            \n");
    printSeparator();
    
    printf("\nSTORY:\n");
    printf("Two rival CEOs are negotiating a merger.\n");
    printf("CEO Alice moves first: SWEET terms (S) or TOUGH terms (T)?\n");
    printf("CEO Bob responds: NEGOTIATE (N) or FINALIZE (F)?\n");
    printf("Payoffs are in billions of dollars.\n\n");
    
    printf("YOUR TASK:\n");
    printf("Use backward induction to find the rollback equilibrium.\n");
    printf("Start from the bottom (terminal nodes) and work backwards!\n");

    draw_puzzle_1_tree();
    
    char userAnswer[MAX_INPUT_LENGTH];
    int formatAttempts = 0;
    bool correctAnswer = false;
    
    while (!correctAnswer) {
        printf("\nWhat is the rollback equilibrium payoff?\n");
        printf("Enter your answer in the format (x,y): ");
        
        if(fgets(userAnswer, MAX_INPUT_LENGTH, stdin) == NULL) {
            continue;
        }
        
        // Check format first
        if (!checkFormat2Players(userAnswer)) {
            formatAttempts++;
            if (formatAttempts == 1) {
                printf("\nIncorrect format. Format needs to be in (x,y)\n");
            } else if (formatAttempts == 2) {
                printf("\nReally? Do I need to say it again? Format has to be in (x,y) format\n");
            } else {
                printf("\nYou're really making me add these extra lines of code to tell you the format again?\n");
                printf("Format: (x,y)\n");
            }
            continue;
        }
        
        // Format is correct, now check answer
        if (checkAnswer(userAnswer, "(4,5)")) {
            printf("\nCorrect! The roll back equilibrium here is (4,5).\n");
            printf("Alice opens friendly, Bob negotiates, and the deal creates maximum value\n");
            printf("with Bob capturing the bigger slice.\n");
            correctAnswer = true;
        } else {
            printf("\nThat is incorrect. Please try again.\n");
        }
    }
    
    pressEnterToContinue();
    state->scene = SCENE_MAIN_MENU;
}

void runPuzzle2Scene(GameState *state) {
    clearScreen();
    printSeparator();
    printf("            Puzzle 2            \n");
    printSeparator();
    
    printf("\nSTORY:\n");
    printf("Consider three major department stores - Big Giant, Titan, and Frieda's - that are\n");
    printf("contemplating opening a branch in one of two new Boston-area shopping malls.\n");
    printf("Urban Mall is located close to the large and rich population center of the area;\n");
    printf("it is relatively small and can accommodate at most two department stores as anchors\n");
    printf("for the mall. Rural Mall is farther out in a rural and relatively poor area; it can\n");
    printf("accommodate as many as three anchor stores. None of the three stores wants to have\n");
    printf("branches in both malls, because there is sufficient overlap of customers between the\n");
    printf("malls that locating in both would just mean competing with itself. Each store would\n");
    printf("rather be in a mall with one or more other department stores than be alone in the same\n");
    printf("mall, because a mall with multiple department stores will attract enough additional\n");
    printf("customers that each store's profit will be higher. Further, each store prefers Urban\n");
    printf("Mall to Rural Mall because of the richer customer base. Each store must choose between\n");
    printf("trying to get a space in Urban Mall (knowing that if the attempt fails, it will try for\n");
    printf("a space in Rural Mall) and trying to get a space in Rural Mall right away (without even\n");
    printf("attempting to get into Urban Mall).\n\n");
    
    printf("The stores rank the five possible outcomes as follows:\n");
    printf("5 (best) - in Urban Mall with one other department store\n");
    printf("4 - in Rural Mall with one or two other department stores\n");
    printf("3 - alone in Urban Mall\n");
    printf("2 - alone in Rural Mall\n");
    printf("1 (worst) - alone in Rural Mall after having attempted to get into Urban Mall and failed\n\n");
    
    printf("The three stores are sufficiently different in their managerial structures that they\n");
    printf("experience different lag times in doing the paperwork required to request a space in a\n");
    printf("new mall. Frieda's moves quickly, followed by Big Giant, and finally by Titan, which is\n");
    printf("the least efficient in readying a location plan. When all three have made their requests,\n");
    printf("the malls decide which stores to let in. Because of the name recognition that both Big\n");
    printf("Giant and Titan have with potential customers, a mall would take either (or both) of those\n");
    printf("stores before it took Frieda's. Thus, Frieda's does not get one of the two spaces in Urban\n");
    printf("Mall if all three stores request those spaces; this is true even though Frieda's moves first.\n\n");
    
    printf("YOUR TASK:\n");
    printf("Use backward induction to find the Rollback equilibrium.\n");
    printf("Start from Titan's decisions and work backwards!\n");

    draw_puzzle_2_tree();
    
    char userAnswer[MAX_INPUT_LENGTH];
    int formatAttempts = 0;
    bool correctAnswer = false;
    
    while (!correctAnswer) {
        printf("\nWhat is the ROllback equilibrium payoff?\n");
        printf("Enter your answer in the format (x,y,z): ");
        
        if(fgets(userAnswer, MAX_INPUT_LENGTH, stdin) == NULL) {
            continue;
        }
        
        // Check format first
        if (!checkFormat3Players(userAnswer)) {
            formatAttempts++;
            if (formatAttempts == 1) {
                printf("\nIncorrect format. Format needs to be in (x,y,z)\n");
            } else if (formatAttempts == 2) {
                printf("\nReally? Do I need to say it again? Format has to be in (x,y,z) format\n");
            } else {
                printf("\nYou're really making me add these extra lines of code to tell you the format again?\n");
                printf("Format: (x,y,z)\n");
            }
            continue;
        }
        
        // Format is correct, now check answer
        if (checkAnswer(userAnswer, "(2,5,5)")) {
            printf("\nCorrect. Outcome is (2,5,5).\n");
            printf("Frieda moves first but gets no power from it.\n");
            printf("Big Giant and Titan hold the bargaining advantage because malls value their brand recognition.\n");
            printf("This is a first-mover disadvantage game.\n");
            printf("Being early gives info but not control.\n");
            printf("Followers capture the top payoffs.\n");
            correctAnswer = true;
        } else {
            printf("\nThat is incorrect. Please try again.\n");
        }
    }
    
    pressEnterToContinue();
    state->scene = SCENE_MAIN_MENU;
}

void runPuzzle3Scene(GameState *state) {
    clearScreen();
    printSeparator();
    printf("            Puzzle 3            \n");
    printSeparator();
    
    printf("\nSTORY:\n");
    printf("To give Mom a day of rest, Dad plants to take his two children,\n");
    printf("Bart and Cassie, on an outing on Sunday. \n");
    printf("Bart prefers to go to the amusment park (A),\n");
    printf("whereas Cassie preferes to go to the science muesum (S).\n\n");

    printf("Each child gets 3 units of value from his/her more preferred activitiy\n");
    printf("and only 2 units of value from his/her less preferred activity.\n");
    printf("Dad get 2 units of value for either of the two activities.\n\n");

    printf("To choose their activity, Dad plans first to ask Bart for his preference,\n");
    printf("then to ask Cassie after she hears Bart’s choice.\n");
    printf("Each child can choose either the amusment park (A) or the science museum (S).\n");
    printf("If both children choose the same activity, then that is what they will all do.\n");
    printf("If the children choose different activities, Dad will make a tie-breaking decision.\n");
    printf("As the parent, Dad has an additional option: He can choose the amusement park, the science museum, or his personal favorite, the mountain hike (M).\n");
    printf("Bart and Cassie each get 1 unit of value from the mountain hike, and Dad gets 3 units of value from the mountain hike.\n");
    printf("Because Dad wants his children to cooperate with each other, he gets\n");
    printf("2 extra units of value if the children choose the same activity (no\n");
    printf("matter which one of the two it is).\n\n");

    printf("YOUR TASK:\n");
    printf("Use backward induction to find the rollback equilibrium.\n");
    printf("Sometimes you have to look backwards to go forwards.\n");

    draw_puzzle_3_tree();
    
    char userAnswer[MAX_INPUT_LENGTH];
    int formatAttempts = 0;
    bool correctAnswer = false;
    
    while (!correctAnswer) {
        printf("\nWhat is the Rollback equilibrium payoff?\n");
        printf("Enter your answer in the format (x,y,z): ");
        
        if(fgets(userAnswer, MAX_INPUT_LENGTH, stdin) == NULL) {
            continue;
        }
        
        // Check format first
        if (!checkFormat3Players(userAnswer)) {
            formatAttempts++;
            if (formatAttempts == 1) {
                printf("\nIncorrect format. Format needs to be in (x,y,z)\n");
            } else if (formatAttempts == 2) {
                printf("\nReally? Do I need to say it again? Format has to be in (x,y,z) format\n");
            } else {
                printf("\nYou're really making me add these extra lines of code to tell you the format again?\n");
                printf("Format: (x,y,z)\n");
            }
            continue;
        }
        
        // Format is correct, now check answer
        if (checkAnswer(userAnswer, "(3,2,4)")) {
            printf("\nCorrect! Outcome is (3,2,4).\n");
            printf("The amusement park wins!\n");
            printf("Bart gets the highest payoff, and the group stays unified.\n");
            printf("You identified a Coordination Game Nash Equilibrium.\n");
            printf("When players value being together, matching strategies wins.\n\n");
            correctAnswer = true;
        } else {
            printf("\nThat is incorrect. Please try again.\n");
        }
    }
    
    pressEnterToContinue();
    state->scene = SCENE_MAIN_MENU;
}

// MAIN FUNCTION 

int main(int argc, char *argv[]) {
    GameState state;
    memset(&state, 0, sizeof(GameState));
    state.scene = SCENE_TITLE;

    while (state.scene != SCENE_EXIT) {
        switch(state.scene) {
            case SCENE_TITLE:
                runTitleScene(&state);
                break;
            case SCENE_MAIN_MENU:
                runMainMenuScene(&state);
                break;
            case SCENE_PUZZLE_1:
                runPuzzleScene(&state);
                break;
            case SCENE_PUZZLE_2:
                runPuzzle2Scene(&state);
                break;
            case SCENE_PUZZLE_3:
                runPuzzle3Scene(&state);
                break;
            case SCENE_EXIT:
            default:
                break;
        }
    }

    clearScreen();
    printf("Thank you for playing, %s!\n", state.playerName);

    return 0;
}