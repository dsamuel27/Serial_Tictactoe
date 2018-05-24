
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
# include <pthread.h>

#define DIMENSION 799
#define NUM_THREADS 2
// a two dimensional array will represent the 3 by 3 grid.
//static int grid[DIMENSION][DIMENSION]; //= {{-2,-2,3},{-1,5,6},{-1,8,9}};
int **grid;
// these chars determine the turn for the player and a_i
typedef struct pack{
    int x;
    int y;
    int orientation;
    int index;
} pack;
char a_i;

void set_grid();
void print_grid();

int isValid(int number);
int can_win_p(int player,pack * x);

int check_horizontal_can_win_p( int player,int i, int j);
int check_vertical_can_win_p( int player, int i, int j);
int check_diagonal_can_win_one( int player, int i,int j);
int check_diagonal_can_win_two( int player, int i, int j);


int global_index = 0;
int check_win_arr[DIMENSION+DIMENSION+2];
pthread_cond_t array_empty =PTHREAD_COND_INITIALIZER;
pthread_cond_t array_full = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_task(void* args) {
    
    
    
    int ind = 0;
    pack* x = (pack *) args;
    
    
    int local_arr[(DIMENSION+DIMENSION+2)/NUM_THREADS];
    
    
    
        
    
    
        
        for (ind = 0; ind < ((DIMENSION+DIMENSION+2)/NUM_THREADS); ind++) {
            
           
            local_arr[ind] =  can_win_p(1,&x[ind]);
            //printf("%d\n",local_arr[ind]);
            
        }
        
    
    pthread_mutex_lock(&mutex);
        for (ind = 0; ind < ((DIMENSION+DIMENSION+2)/NUM_THREADS); ind++) {
            //printf("local arr value : %d\n",local_arr[ind]);
            check_win_arr[global_index] = local_arr[ind];
            global_index++;
            
        }
        pthread_mutex_unlock(&mutex);
    
    
    //pthread_barrier_wait(&barrier);
}

void reset_check_win_arr() {
    int i = 0;
    for ( i = 0; i < (DIMENSION+DIMENSION+2);i++) {
        check_win_arr[i] = -1;
    }
    global_index = 0;
}

int main() {
    clock_t start,end;
    reset_check_win_arr();
    pack **win_check = (pack **) malloc(sizeof(pack)*NUM_THREADS);
    int division = (DIMENSION+DIMENSION+2)/NUM_THREADS;
    int p; int x_value = 0; int y_value = 0; // orientation = 0 horizntal, 1 vertical, 2 diag one, 3 diag two
    for (p = 0; p < NUM_THREADS; p++) {
        win_check[p] = (pack*) malloc (sizeof(pack)*division);
    }
    int q;
    int tots = 0;
    for (p = 0; p < NUM_THREADS; p++) {
        for ( q = 0; q < division; q++) {
            if (tots >= 0 && tots < DIMENSION) {
                win_check[p][q].x = x_value;
                win_check[p][q].y = y_value;
                win_check[p][q].orientation = 0;
                win_check[p][q].index = tots;
                
                x_value++;
                if (x_value == DIMENSION) {
                    x_value = 0;
                }
            } else if ( tots >= DIMENSION && tots < (DIMENSION*2)) {
                win_check[p][q].x = x_value;
                win_check[p][q].y = y_value;
                win_check[p][q].orientation = 1;
                win_check[p][q].index = tots;
                // printf("X: %d, Y: %d\n",x_value,y_value);
                y_value++;
                if(y_value == DIMENSION) {
                    y_value = 0;
                }
            } else if ( tots == (DIMENSION*2)) {
                win_check[p][q].x = 0;
                win_check[p][q].y = 0;
                win_check[p][q].orientation = 2;
                win_check[p][q].index = tots;
                
            } else if ( tots == ((DIMENSION*2)+1)) {
                win_check[p][q].x = 0;
                win_check[p][q].y = DIMENSION-1;
                win_check[p][q].orientation = 3;
                win_check[p][q].index = tots;
            }
            tots++;
        }
    }
   
    set_grid();
    //print_grid();
    pthread_t arr[NUM_THREADS];
    start = clock();
    for (p =0; p < NUM_THREADS;p++) {
        pthread_create(&arr[p],NULL,&thread_task,win_check[p]);
    }
    
    for (p = 0; p < NUM_THREADS; p++) {
        pthread_join(arr[p],NULL);
    }
    end = clock();
    int x = -1;
    for (p = 0; p < (DIMENSION+DIMENSION+2);p++) {
        if (check_win_arr[p] != -1) {
            x = check_win_arr[p];
            break;
        }
    }
    //printf("X %d\n",x);
    if ( x != -1) {
    int i = x / DIMENSION;
    int j = x % DIMENSION;
    grid[i][j] = -2;
    
    
    printf("Grid after solver\n");
    //print_grid();
        
        double time = (double ) (end-start)/CLOCKS_PER_SEC;
        printf("Time for solution: %f\n",time);
    }
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



/* check if win can be found in two diaganols, returns value that can be broken into i and j index */
int check_diagonal_can_win_one(int player,int i, int j) {
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int tempone;
    int counter = 0;
    
    while (i < DIMENSION && j < DIMENSION) {
        if (grid[i][j] == int_to_check) {
            counter++;
        } else {
            tempone = (i * DIMENSION) + j;
        }
        i++; j++;
    }
    if (counter == DIMENSION - 1) {
        rt_value = tempone;
    }
    return rt_value;
    
}
int check_vertical_can_win_p(int player,int i, int j) {
    int int_to_check;
    //printf("%d %d\n",i,j);
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int counter = 0;
    int temp;
    for (i = 0; i < DIMENSION; i++) {
        if (grid[i][j] == int_to_check) {
            counter++;
        } else {
            temp = (i * DIMENSION) + j;
        }
    }
    
    if (counter == DIMENSION-1) {
        //printf("Vert cointer = %d\ntemp = %d\n",counter,temp);
        rt_value = temp;
    }
    return rt_value;
    
}
int check_diagonal_can_win_two(int player,int i, int j) {
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int tempone;
    int counter = 0;
    while (i < DIMENSION && j > -1) {
        if (grid[i][j] == int_to_check) {
            counter++;
        } else {
            tempone = (i * DIMENSION) + j;
        }
        i++; j--;
    }
    if (counter == DIMENSION - 1) {
        rt_value = tempone;
    }
    return rt_value;
    
}

int check_horizontal_can_win_p(int player, int i, int j) {
    int int_to_check;
    int rt_value = 0;
    if (player == 0) {
        int_to_check = -1;
    } else {
        int_to_check = -2;
    }
    int counter = 0;
    int temp;
    for (j = 0; j < DIMENSION; j++) {
        if (grid[i][j] == int_to_check) {
            counter++;
        } else {
            temp = (i * DIMENSION) + j;
        }
    }
    if (counter == DIMENSION-1) {
        rt_value = temp;
    }
    return rt_value;
    
}
/* Check if player in argument can win at a[i][j] */
int can_win_p(int player,pack* x) {
    //printf("CHECK\n");
    int rt_value = -1;
     //printf("%d\n",x->orientation);
    if (x->orientation == 0) {
        if (check_horizontal_can_win_p(player,x->x,x->y)) {
        rt_value= check_horizontal_can_win_p(player,x->x,x->y);
        }
    } else if ( x->orientation == 1) {
        if (check_vertical_can_win_p(player,x->x,x->y)) {
        rt_value=  check_vertical_can_win_p(player,x->x,x->y);
        }
    } else if (x->orientation == 2) {
        if (check_diagonal_can_win_one(player,x->x,x->y)) {
        rt_value= check_diagonal_can_win_one(player,x->x,x->y);
        }
    } else {
        if(check_diagonal_can_win_two(player,x->x,x->y)) {
        rt_value =  check_diagonal_can_win_two(player,x->x,x->y);
        }
    }
    
    
    //printf("RT VALUE: %d\n",rt_value);
    
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
    i = DIMENSION - 1;
    j = DIMENSION - 1;
    
    int index = 0;
    while(index < (DIMENSION - 1)) {
        grid[i][j] = -2;
        i--;
        index++;
        
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

