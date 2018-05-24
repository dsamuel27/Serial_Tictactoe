//
//  serial_tictactoe.c
//  
//
//  Created by David Samuel on 2018-04-24.
//
/*
    The objective of this program is to implement a serial tic tac toe game in which you face an
    artificial intelligence.
 The AI will check the following steps to win the game.
 1. Win: Check to see if the AI can line up three X’s or O’s in a row.
 2. Block: Check to see if the AI can block the opposing player from getting three X’s or O’s in a row.
 3. Fork: Can the AI create an opportunity to create two ways to win? SCRAP
 4. Block opposing Fork: Can the AI block a possible fork for the opponent? SCRAP
 5. Place center: Can the AI place its “game piece” in the center of the board?
 6. Place opposite corner: If the opponent placed their game piece in a corner, then can the AI place it’s piece in the opposing corner.
 7. Empty Corner: Can the AI place the “game piece” in a free corner?
 8. Empty Side: Can the AI place the game piece in an empty side?
 
 
 NOTES
 New objective - change how win condition is checked.
 
 old
 
 - want to make program for N by N array for effective measuring. - Work on printing N by N grid and winning with N in a row.
 - each cell will check for horizontal, vertical, and both diagnol win conditions. True brute force that will
   work for n by n grids.
 -
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIMENSION 3
// a two dimensional array will represent the 3 by 3 grid.
//static int grid[DIMENSION][DIMENSION]; //= {{-2,-2,3},{-1,5,6},{-1,8,9}};
int **grid;
// these chars determine the turn for the player and a_i

char a_i;

void set_grid();
void print_grid();
int check_win(int player);
int isValid(int number);
int can_win(int player);
int can_block(int player);
int check_horizontal_win(int i,int j, int player);
int check_vertical_win(int i, int j, int player);
int check_diagonal_win(int i, int j, int player);
int check_horizontal_can_win( int player);
int check_vertical_can_win( int player);
int check_diagonal_can_win( int player);
int check_opposite_corner(int player);
int place_corner(int player);
int any_space(int player);
int place_center(int player) ;

int main() {
    clock_t start,end;
    printf("Player uses letter X, and AI uses letter O\n");
    printf("Player goes first, AI goes second\n");
    
   
    set_grid();
    //printf("SUCCESS\n");
    //fill_grid();
    print_grid();
    
    int num_moves = 0;
    int turn = 0;
    while (check_win(0) == -1 && check_win(1) == -1 && num_moves != (DIMENSION*DIMENSION)) {
        if (turn == 0) {
            int cell_num;
            printf("Which cell would you like to place an X: ");
            scanf("%d",&cell_num);
            while(!(isValid(cell_num))) {
                printf("Please enter valid cell number: ");
                scanf("%d",&cell_num);
            }
                cell_num--;
                  int x = cell_num/ DIMENSION;
                  int y = cell_num % DIMENSION;
                  
                  grid[x][y] = -1;
                  num_moves++;
                  turn = 1;
                  } else {
                      printf("Computer player's turn\n");
                      // AI's moveset. First checks if AI can make winninf move
                      
                      int skip = 1;
                      int i,j;
                      clock_t start = clock();
                      if (can_win(1)) {
                          printf("can win\n");
                          int temp = can_win(1);
                          int x = temp/DIMENSION;
                          int y = temp % DIMENSION;
                          if (grid[x][y] != -1 && grid[x][y] != -2) {
                          grid[x][y] = -2;
                          skip = 0;
                          }
                      }
                      clock_t end = clock();
                      
                      if (skip) {
                          if (can_block(1)) {
                              printf("can block\n");
                              int temp = can_block(1);
                              int x = temp/DIMENSION;
                              int y = temp % DIMENSION;
                              if (grid[x][y] != -1 && grid[x][y] != -2) {
                                  grid[x][y] = -2;
                                  skip = 0;
                              }
                          }
                      }
                      
                      
                      
                      if (skip) {
                          if (place_center(1)) {
                              printf("center\n");
                              skip = 0;
                          }
                      }
                      
                      if (skip) {
                          if (check_opposite_corner(1)) {
                              printf("opposite corner\n");
                              skip = 0;
                          }
                      }
                      
                      if (skip) {
                          if (place_corner(1)) {
                              printf("corner\n");
                              skip = 0;
                          }
                      }
                      
                      if (skip) {
                          if (any_space(1)){
                              printf("any space\n");
                              skip = 0;
                          }
                      }
                      turn = 0;
                      num_moves++;
                       double time = (double) (end - start)/CLOCKS_PER_SEC;
                      printf("TIme can_win %f\n",time);
                  }
       
                  print_grid();
        
    }
                  
    if (check_win(0) != -1) {
        printf("Human player won!\n");
    }
    
    else if (check_win(1) != -1) {
        printf("Computer player won!\n");
    } else {
        printf("No player won the game\n");
    }
   
    
    
}
/* places token in first available space */
int any_space(int player) {
    int rt_value = 0;
    int token;
    if (player == 0) {
        
        token = -1;
    } else {
        token = -2;
    }
    
    int i,j;
    
    for (i = 0; i < DIMENSION; i++) {
        for (j = 0; j <DIMENSION; j++) {
            if (grid[i][j] != -1 && grid[i][j] != -2) {
                grid[i][j] = token;
                rt_value = 1;
                break;
            }
        }
        if (rt_value == 1) { break;}
    }
    
    return rt_value;
}

/*checks to see if any empty corner is free. If it is place the token */
int place_corner(int player) {
    int rt_value = 0;
    int token;
    if (player == 0) {
        
        token = -1;
    } else {
        token = -2;
    }
    
    if (grid[0][0] != -1 && grid[0][0] != -2 ) {
        rt_value = 1;
        grid[0][0] = token;
    } else if (grid[0][DIMENSION-1] != -1 && grid[0][DIMENSION-1] != -2) {
        rt_value = 1;
        grid[0][DIMENSION-1] = token;
    } else if (grid[DIMENSION-1][0] != -1 && grid[DIMENSION-1][0] != -2) {
        rt_value = 1;
        grid[DIMENSION-1][0] = token;
        
    } else if (grid[DIMENSION-1][DIMENSION-1] != -1 && grid[DIMENSION-1][DIMENSION-1] != -2) {
        rt_value = 1;
        grid[DIMENSION-1][DIMENSION-1] = token;
    }
    
    return rt_value;
}

/* check if token can be placed in opposite corner of opponent's cell, and places it */

int check_opposite_corner(int player) {
    int opposing_player;
    int rt_value = 0;
    int my_token;
    int opposing_token;
    if (player == 0) {
        opposing_player = 1;
        my_token = -1;
        opposing_token = -2;
    } else {
        opposing_player  = 0;
        my_token = -2;
        opposing_token = -1;
    }
    
    if (grid[0][0] == opposing_token && (grid[DIMENSION-1][DIMENSION-1] != -1 && grid[DIMENSION-1][DIMENSION-1] != -2 )) {
        grid[DIMENSION-1][DIMENSION-1] = my_token;
        rt_value = 1;
    } else if(grid[DIMENSION-1][DIMENSION-1] == opposing_token && (grid[0][0] != -1 && grid[0][0] != -2 )) {
        grid[0][0] = my_token;
        rt_value = 1;
    } else if (grid[0][DIMENSION-1] == opposing_token && (grid[DIMENSION-1][0] != -1 && grid[DIMENSION-1][0] != -2)) {
        grid[DIMENSION-1][0] = my_token;
        rt_value = 1;
        
    } else if (grid[DIMENSION-1][0] == opposing_token && (grid[0][DIMENSION-1] != -1 && grid[0][DIMENSION-1] != -2 )) {
        grid[0][DIMENSION-1] = my_token;
        rt_value = 1;
    }
    
    return rt_value;
}
 /* Returns 1 if given coordinate is in range and the cell is empty */
int isValid(int num) {

    if (num >= 1 && num <= (DIMENSION*DIMENSION)) {
        num = num - 1;
        int x = num / DIMENSION;
        int y = num % DIMENSION;
        if (grid[x][y] != -1 && grid[x][y] != -2) {
            return 1;
        }
    }
    return 0;
}

/* check if value can be placed in the center and place in centre */
int place_center(int player) {
    int rt_value = 0;
    int token;
    if (player == 0) {
        
        token = -1;
    } else {
        token = -2;
    }
    int x = (DIMENSION*DIMENSION)/2;
    int i = x/DIMENSION;
    int j = x % DIMENSION;
    
    if (grid[i][j] != -1 && grid[i][j] != -2) {
        grid[i][j] = token;
        rt_value = 1;
    }
    return rt_value;
}

/* check if opposing player can be blocked at this position */
int can_block(int player) {
    int opposing_player;
    int rt_value = 0;
    if (player == 0) {
        opposing_player = 1;
    } else {
        opposing_player = 0;
    }
    
    if (can_win(opposing_player) != 0) {
        rt_value = can_win(opposing_player);
    }
    
    return rt_value;
}

/* Check if player in argument can win at a[i][j] */
int can_win(int player) {
    int rt_value = 0;
  
            
            if (check_diagonal_can_win(player) || check_vertical_can_win(player) || check_horizontal_can_win(player)) {
                if (check_diagonal_can_win(player)) {
                    rt_value = check_diagonal_can_win(player);
                } else if (check_vertical_can_win(player)) {
                    rt_value = check_vertical_can_win(player);
                } else {
                    rt_value = check_horizontal_can_win(player);
                }
            }
            
            
    
    return rt_value;
}

/* check if win can be found in two diaganols, returns value that can be broken into i and j index */
int check_diagonal_can_win(int player) {
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int counter_one = 0;
    int counter_two = 0;
 
    
    int i = 0;
    int j = 0;
    int tempone;
    int temptwo;
   
    
    while (i < DIMENSION && j < DIMENSION) {
        if (grid[i][j] == int_to_check) {
            counter_one++;
        } else {
            tempone = (i * DIMENSION) + j;
        }
        i++; j++;
    }
    
    if (counter_one == DIMENSION - 1) {
        rt_value = tempone;
    }
    while (i < DIMENSION && j < DIMENSION) {
        if (grid[i][j] == int_to_check) {
            counter_two++;
        } else {
            temptwo = (i * DIMENSION) + j;
        }
        i++; j++;
    }
    
    if (counter_two == DIMENSION - 1) {
        rt_value = temptwo;
    }

    
 
    return rt_value;
}
int check_vertical_can_win(int player) {
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int counter = 0;
    int i; int j;
    int temp;
    int brk = 0;
    for (j = 0; j < DIMENSION; j++) {
        for (i = 0; i < DIMENSION; i++) {
            if (grid[i][j] == int_to_check) {
                counter++;
            } else {
                temp = (i * DIMENSION) + j;
            }
        }
        if (counter == DIMENSION-1) {
            rt_value = temp; break;
        } else {
            counter = 0;
        }
    }
    return rt_value;
}
int check_horizontal_can_win(int player) {
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int counter = 0;
    int i; int j;
    int temp;
    int brk = 0;
    for (i = 0; i < DIMENSION; i++) {
        for (j = 0; j < DIMENSION; j++) {
            if (grid[i][j] == int_to_check) {
                counter++;
            } else {
                temp = (i * DIMENSION) + j;
            }
        }
        if (counter == DIMENSION-1) {
            rt_value = temp; break;
        } else {
            counter = 0;
        }
    }
    return rt_value;
    
}

// type can be either 0 or 1. 0 = forward diagnol, 1 = backward diagnol
int check_diagonal_win(int i,int j,int player) {
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int counter_one = 0;
    int counter_two = 0;
    int top_i = i - 1;
    int bottom_i = i + 1;
    
    int left_j = j - 1;
    int right_j = j +1;
    
    int iterations = 0;
    
    int depth = (DIMENSION - 1)/2;
    
    for (iterations = 0; iterations < depth; iterations++) {
    
    if (top_i > -1 && left_j > -1) {
        
        if (grid[top_i][left_j] == int_to_check) {
            counter_one++;
        }
    }
   
    
    if (bottom_i < DIMENSION && right_j < DIMENSION) {
        
        if (grid[bottom_i][right_j] == int_to_check) {
        counter_one++;
        }
    }
       //printf("bottom i %d, right j %d\n",bottom_i,right_j);
    
    if (top_i > -1 && right_j < DIMENSION) {
        if (grid[top_i][right_j] == int_to_check) {
        counter_two++;
        }
    }
        
    
    if (bottom_i < DIMENSION && left_j > -1) {
        //printf("bottom i %d, left j %d\n",bottom_i,left_j);
        if (grid[bottom_i][left_j] == int_to_check) {
        counter_two++;
        }
    }
       
        top_i--;
        bottom_i++;
        left_j--;
        right_j++;
    }
    if (grid[i][j] == int_to_check) {
        counter_one++;
        counter_two++;
    }
    if (counter_one == DIMENSION || counter_two == DIMENSION ) {
        rt_value = 1;
    }
    //printf("Diagnol rt value %d\n",rt_value);
    
    return rt_value;
}
int check_vertical_win(int i,int j,int player) {
    //printf("VERTICAL\n");
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int counter = 0;
    
    int top_i = i - 1;
    int bottom_i = i + 1;
    
    int iterations = 0;
    
    int depth = (DIMENSION - 1)/2;
    //printf("depth : %d\n",depth);
    
    for (iterations = 0; iterations < depth; iterations++) {
    if (top_i > -1) {
        if (grid[top_i][j] == int_to_check) {
            counter++;
        }
    }
    
    if (bottom_i < DIMENSION) {
        if (grid[bottom_i][j] == int_to_check) {
            counter++;
        }
    }
    
  
        top_i--;
        bottom_i++;
    }
    if (grid[i][j] == int_to_check) {
        counter++;
    }
    if (counter == DIMENSION) {rt_value = 1;}
    //printf("vert rt value %d\n",rt_value);
    //printf("SUCCESS TWO\n");
    return rt_value;
}
int check_horizontal_win(int i,int j,int player) {
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int counter = 0;
    
    int left_j = j - 1;
    int right_j = j + 1;
    
    int iterations = 0;
    
    int depth = (DIMENSION - 1)/2;
    
    for (iterations = 0; iterations < depth; iterations++) {
    if (left_j > -1) {
        if (grid[i][left_j] == int_to_check) {
            counter++;
        }
    }
    
    if (right_j < DIMENSION) {
        if (grid[i][right_j] == int_to_check) {
            counter++;
        }
    }
    
 
        left_j--;
        right_j++;
    }
    if (grid[i][j] == int_to_check) {
        counter++;
    }
    if (counter == DIMENSION) {rt_value = 1;}
    
    //printf("Horizontal rt value %d\n",rt_value);
    //printf("SUCCESS ONE\n");
    return rt_value;
}

/* this function will check win condition for a player. The argument player represents the ai or the human player. If the argument is 0 then its the human otherwise its the AI */

int check_win(int player){
   
    int rt_value = -1;
    int count = 0;
    
    int i,j;
    for (i=0;i < DIMENSION; i++) {
        for (j =0; j < DIMENSION; j++) {
            
            
            if (check_diagonal_win(i,j,player) || check_vertical_win(i,j,player) || check_horizontal_win(i,j,player)) {
                rt_value = 1; break;
            }
       
            
        }
         if (rt_value == 1) break;
    }
    return rt_value;
    }




 /*This function will set all the values in the grid to a number corresponding to a cell's location */
void set_grid() {
    int x = DIMENSION;
    grid = (int **) malloc(sizeof(int*)*x);
    int f;
    for (f = 0; f < DIMENSION; f++) {
        grid[f] = (int *) malloc(sizeof(int)*x);
    }
    //grid[2][0] = 5;
    int i,j;
    int num = 1;
    for (i = 0; i < x; i++) {
        for (j = 0; j <x; j++) {
            
            grid[i][j] = num;
            num++;
            
           
        }
    }
 
    
}


/*This function prints out the contents of the grid. */
void print_grid(){
    int i,j;
    printf("\n");
    int max = DIMENSION * DIMENSION;
    for (i = 0; i < DIMENSION; i++) {
        for (j = 0; j < DIMENSION; j++) {
            printf("  ");
            if (grid[i][j] >= 1 && grid[i][j] <= max) {
                printf("%d",grid[i][j]);
            } else if ( grid[i][j] == -1) {
                printf("X");
            } else if (grid[i][j] == -2) {
                printf("O");
            }
            
            printf("  ");
            if (j < DIMENSION-1) {
                printf("|");
            } else {
                printf("\n");
                if (i < DIMENSION-1) {
                printf("-----------------\n");
                }
            }
            
        }
        printf("\n");
    }
}


