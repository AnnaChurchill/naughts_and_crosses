#include <stdio.h>
#include <stdlib.h>

/*
strategy steps:
1. check if I can win and do that
2. check if you can win next go and block
3. check if there is a place I can go which secures my win next time and do that
4. check if there is a place you can go which secures your win next time and block
5. go in middle first
6. go in a corner opposite another corner I am in
7. go in any corner
8. go in any edge
*/


/*ascii arithmetic for lines:
' ' = 32
'x' = 120
'o' = 111
' ' + ' ' + ' ' = 96
' ' + ' ' + 'x' = 184
' ' + ' ' + 'o' = 175
' ' + 'x' + 'x' = 272
' ' + 'o' + 'o' = 254
'o' + 'o' + 'o' = 333
'x' + 'x' + 'x' = 360
*/



#define DIMENSIONS (3)
#define TOTAL_SQUARES (9)
#define NUM_LINES (8)



struct game {
    char ** board;
    char me;
    char you;
    int goes_left;
    struct line * lines; // 8 lines
    int two_of_mine;
    int two_of_yours;
    int one_of_mine;
    int one_of_yours;
    int none_of_anyones;
    int three_of_mine;
    int three_of_yours;
};

struct line {
    int combo;
    char ** members;
};


void make_board(struct game * g) {
    
    g->board = (char **) malloc(sizeof(char *)*DIMENSIONS);
    
    for (int i = 0; i < DIMENSIONS; i++) {
        g->board[i] = (char *) malloc(sizeof(char)*DIMENSIONS);
        for (int ii = 0; ii < DIMENSIONS; ii++) {
            g->board[i][ii] = ' ';
        }
    }
    
    return; 
}

void make_lines(struct game * g) {
    g->lines = (struct line *) malloc(sizeof(struct line)*NUM_LINES);

    for (int i = 0; i < NUM_LINES; i++) {
        g->lines[i].combo = (int) ' '+' '+' ';
        g->lines[i].members = malloc(sizeof(char *)*DIMENSIONS);
    }

    g->lines[0].members[0] = &(g->board[0][0]);
    g->lines[0].members[1] = &(g->board[0][1]);
    g->lines[0].members[2] = &(g->board[0][2]);

    g->lines[1].members[0] = &(g->board[1][0]);
    g->lines[1].members[1] = &(g->board[1][1]);
    g->lines[1].members[2] = &(g->board[1][2]);

    g->lines[2].members[0] = &(g->board[2][0]);
    g->lines[2].members[1] = &(g->board[2][1]);
    g->lines[2].members[2] = &(g->board[2][2]);

    g->lines[3].members[0] = &(g->board[0][0]);
    g->lines[3].members[1] = &(g->board[1][0]);
    g->lines[3].members[2] = &(g->board[2][0]);

    g->lines[4].members[0] = &(g->board[0][1]);
    g->lines[4].members[1] = &(g->board[1][1]);
    g->lines[4].members[2] = &(g->board[2][1]);

    g->lines[5].members[0] = &(g->board[0][2]);
    g->lines[5].members[1] = &(g->board[1][2]);
    g->lines[5].members[2] = &(g->board[2][2]);

    g->lines[6].members[0] = &(g->board[0][0]);
    g->lines[6].members[1] = &(g->board[1][1]);
    g->lines[6].members[2] = &(g->board[2][2]);

    g->lines[7].members[0] = &(g->board[0][2]);
    g->lines[7].members[1] = &(g->board[1][1]);
    g->lines[7].members[2] = &(g->board[2][0]);


    return;
}

void find_combos(struct game * g) {

    for (int i = 0; i < NUM_LINES; i++) {
        g->lines[i].combo = (int) *(g->lines[i].members[0]) + *(g->lines[i].members[1]) + *(g->lines[i].members[2]);
    }
    return;
}

int lines_crossing(struct line l1, struct line l2) {
    
    for (int i = 0; i < DIMENSIONS; i++) {
        for (int ii = 0; ii < DIMENSIONS; ii++) {
            if (l1.members[i] == l2.members[ii]) {
                return i;
            }
        }
    }


    return -1;
    

    
}

void show_board(struct game * g) {
    printf("%c|%c|%c\n%c|%c|%c\n%c|%c|%c\n\n",g->board[0][0],g->board[0][1],g->board[0][2],g->board[1][0],g->board[1][1],g->board[1][2],g->board[2][0],g->board[2][1],g->board[2][2]);
    return;
}

int your_go(struct game * g) {

    // show_board(g);
    printf("1|2|3\n4|5|6\n7|8|9\n\n");

    int pick_num;
    printf("Please type the number corresponding to where you want to go: ");
    scanf("\n %d", &pick_num);

    while (pick_num < 1 || pick_num > TOTAL_SQUARES) {
        printf("That number is invalid, please try again: ");
        scanf("\n %d", &pick_num);
    }

    int is_available = 1;
    while (is_available) {
        for (int i = 0; i < DIMENSIONS; i++) {
            for (int ii = 0; ii < DIMENSIONS; ii++) {
                if (i*DIMENSIONS + ii + 1 == pick_num) {
                    if (g->board[i][ii] == ' ') {
                        g->board[i][ii] = g->you; 
                        is_available = 0;
                    }
                    else {
                        printf("That number is invalid, please try again: ");
                        scanf("\n %d", &pick_num);
                    }
                    
                }
            }
        }
    }
    
    g->goes_left--;
    printf("\n");
    show_board(g);

    find_combos(g);
    for (int i = 0; i < NUM_LINES; i++) {
        if (g->lines[i].combo == g->three_of_yours) {
            return 1;
        }
    }
    return 0;
}

int my_go(struct game * g) {

    g->goes_left--;

    // step 1 - check if any of the line combos are two_of_mine
    for (int i = 0; i < NUM_LINES; i++) {
        // if I can win in this move
        if (g->lines[i].combo == g->two_of_mine) {
            if (*(g->lines[i].members[0]) == ' ') {
               *(g->lines[i].members[0]) = g->me;
            }
            else if (*(g->lines[i].members[1]) == ' ') {
               *(g->lines[i].members[1]) = g->me;
            }
            else {
               *(g->lines[i].members[2]) = g->me;
            }

            show_board(g);
            return 1;
        }
    }

    
    // step 2
    for (int i = 0; i < NUM_LINES; i++) {
        // if you can win in this move
        if (g->lines[i].combo == g->two_of_yours) {

            if (*(g->lines[i].members[0]) == ' ') {
               *(g->lines[i].members[0]) = g->me;
            }
            else if (*(g->lines[i].members[1]) == ' ') {
               *(g->lines[i].members[1]) = g->me;
            }
            else {
               *(g->lines[i].members[2]) = g->me;
            }

            show_board(g);
            return 0;
        }
    }

    // step 3 if there are two one_of_mine lines that share an empty square
    int lines_meet;
    
    for (int i = 0; i < NUM_LINES; i++) {
        if (g->lines[i].combo == g->one_of_mine) {
            for (int ii = i+1; ii < NUM_LINES; ii++) {
                if (g->lines[ii].combo == g->one_of_mine) {
                    lines_meet = lines_crossing(g->lines[i], g->lines[ii]);
                    if (lines_meet >= 0) {
                        if (*(g->lines[i].members[lines_meet]) == ' ') {
                            *(g->lines[i].members[lines_meet]) = g->me;
                            show_board(g);
                            return 0;
                        }
                        
                    }
                }
            }
        }
    }

    // step 4
    for (int i = 0; i < NUM_LINES; i++) {
        if (g->lines[i].combo == g->one_of_yours) {
            for (int ii = i+1; ii < NUM_LINES; ii++) {
                if (g->lines[ii].combo == g->one_of_yours) {
                    lines_meet = lines_crossing(g->lines[i], g->lines[ii]);
                    if (lines_meet >= 0) {
                        if (*(g->lines[i].members[lines_meet]) == ' ') {
                            *(g->lines[i].members[lines_meet]) = g->me;
                            show_board(g);
                            return 0;
                        }
                        
                    }
                }
            }
        }
    }

    // step 5
    if (g->board[1][1] == ' ') {
        g->board[1][1] = g->me;
        show_board(g);
        return 0;
    }
    
    // step 6
    if (g->board[0][0] == g->me && g->board[2][2] == ' ') {
        g->board[2][2] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[0][0] == ' ' && g->board[2][2] == g->me) {
        g->board[0][0] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[0][2] == ' ' && g->board[2][0] == g->me) {
        g->board[0][2] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[2][0] == ' ' && g->board[0][2] == g->me) {
        g->board[2][0] = g->me;
        show_board(g);
        return 0;
    }


    // step 7
    if (g->board[2][2] == ' ') {
        g->board[2][2] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[0][0] == ' ') {
        g->board[0][0] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[0][2] == ' ') {
        g->board[0][2] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[2][0] == ' ') {
        g->board[2][0] = g->me;
        show_board(g);
        return 0;
    }

    // step 8
    if (g->board[0][1] == ' ') {
        g->board[0][1] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[1][0] == ' ') {
        g->board[1][0] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[1][2] == ' ') {
        g->board[1][2] = g->me;
        show_board(g);
        return 0;
    }
    else if (g->board[2][1] == ' ') {
        g->board[2][1] = g->me;
        show_board(g);
        return 0;
    }
}



void free_game(struct game * g) {
    
    for (int i = 0; i < DIMENSIONS; i++) {
        free(g->board[i]);
    }
    free(g->board);

    for (int i = 0; i < NUM_LINES; i++) {
        free(g->lines[i].members);
    }
    free(g->lines);

    free(g);
    return;
}


int main() {

    struct game * g = (struct game *) malloc(sizeof(struct game));
    g->goes_left = TOTAL_SQUARES;

    printf("Hello and welcome to naughts and crosses!\n");

    // pick symbols
    char symbol;
    printf("Would you prefer to be naughts or crosses? (Please type x or o): ");
    scanf("%c", &symbol);
    while (symbol != 'o' && symbol != 'x') {
        printf("That is not an option, please try again: ");
        scanf("\n%c", &symbol);
    }
    if (symbol == 'o') {
        g->me = 'x';
        g->you = 'o';
        g->one_of_mine = 184;
        g->one_of_yours = 175;
        g->two_of_mine = 272;
        g->two_of_yours = 254;
        g->three_of_mine = 360;
        g->three_of_yours = 330;
    }
    else {
        g->me = 'o';
        g->you = 'x';
        g->one_of_yours = 184;
        g->one_of_mine = 175;
        g->two_of_yours = 272;
        g->two_of_mine = 254;
        g->three_of_yours = 360;
        g->three_of_mine = 330;
    }
    g->none_of_anyones = 96;
    

    // choose who goes first
    char first;
    printf("Would you like to go first? (y/n): ");
    scanf("\n%c", &first);
    while (first != 'y' && first != 'n') {
        printf("Your answer did not match any of the options. Please try again: ");
        scanf("\n%c", &first);
    }


    // create board
    
    make_board(g);
    make_lines(g);
    int line_options[] = {96, 184, 175, 272, 254, 333, 360};

    int winner = 0;

    if (first == 'y') {
        show_board(g);
        your_go(g);       
    }

    while (g->goes_left > 1) {
        printf("Computer goes:\n");
        if (my_go(g) == 1) {
            printf("The computer wins! Better luck next time :/\n");
            g->goes_left = 0;
            winner = 1;
            break;
        }
        printf("Your go:\n");
        if (your_go(g) == 1) {
            printf("You win! You beat the computer! Yay! :)\n");
            g->goes_left = 0;
            winner = 1;
            break;
        }

    }

    if (g->goes_left == 1) {
        my_go(g);
    }
    
    if (winner == 0) {
        printf("It's a tie! :)\n");
    }
    


    free_game(g);
    return 0;
}
