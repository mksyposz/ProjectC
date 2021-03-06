#include"board.h"

struct node *init(int size_w, int size_h, int win_condition) 
{ 
    struct node *prev = NULL; 
    int h = size_h;
    while(size_w--) 
    { 
        struct node *new_node = single_column(h,win_condition);
        while(new_node->up != NULL) 
        { 
            new_node->next = NULL; 
            new_node->prev = prev;
            if(prev != NULL) prev->next = new_node;
            new_node = new_node->up; 
            if(prev != NULL) prev = prev->up;
        } 
        new_node->next = NULL; 
        new_node->prev = prev;
        if(prev != NULL) prev->next = new_node;
        while(new_node->down != NULL) new_node = new_node->down; 
        prev = new_node;
    } 
    while(prev->prev != NULL) prev = prev->prev;
    return prev;
} 

struct node *single_column(int height, int win_condition) 
{ 
    struct node *prev = NULL;
    while(height--) 
    { 
        struct node *new_node = malloc(sizeof(struct node));
        new_node->val = -1;
        new_node->win_cond = win_condition;
        new_node->next = NULL; 
        new_node->prev = NULL;
        new_node->down = prev; 
        new_node->up = NULL;
        if(prev != NULL) prev->up = new_node;
        prev = new_node;
    } 
    while(prev->down != NULL) prev = prev->down;
    return prev;
} 

struct node *add_row(struct node* board)
{ 
    struct node *ret = board;
    int k = board->win_cond;
    while(board != NULL) 
    { 
        struct node *curr = board;
        while(curr->up != NULL) curr = curr->up;
        struct node *new_node = malloc(sizeof(struct node));
        new_node->val = -1;
        new_node->win_cond = k;
        new_node->next = NULL; 
        new_node->up = NULL;
        new_node->down = curr;
        new_node->prev = NULL;
        curr->up = new_node;
        if(curr->prev != NULL) 
        { 
            curr->prev->up->next = new_node;
            new_node->prev = curr->prev->up;
        }
        board = board->next; 
    }
    return ret; 
} 

struct node *move1(struct node* board,int x, int player) 
{ 
    int id = 0; 
    while(id < x) 
    { 
        board = board->next; 
        id++;
    }
    while(board->val != -1 && board->up != NULL) 
    { 
        board = board->up;
    } 
    if(board->up == NULL && board->val != -1) 
    { 
        while(board->down != NULL) board = board->down;
        while(board->prev != NULL) board = board->prev;
        board = add_row(board);
        id = 0; 
        while(id < x) 
        { 
            board = board->next; 
            id++;
        }
        while(board->up != NULL) board = board->up;
    }
    board->val = player;
    while(board->down != NULL) board = board->down; 
    while(board->prev != NULL) board = board->prev; 
    return board;
} 

struct node *move2(struct node* board, int x, int player)
{ 
    if(!is_legal(board,x,player)) return board; 
    int id = 0; 
    while(id < x) 
    { 
        board = board->next; 
        id++;
    } 
    while(board->up != NULL)
    { 
        struct node *nxt = board->up; 
        int k = board->val;
        if(nxt->val == -1) break;
        board->val = nxt->val; 
        nxt->val = k; 
        board = nxt;
    }
    while(board->down != NULL) board = board->down; 
    while(board->prev != NULL) board = board->prev; 
    return move1(board,x,player); 
} 

struct node *add_right_column(struct node* board) 
{ 
    int h = 0; 
    while(board->up != NULL) 
    { 
        h++; 
        board = board->up;
    } 
    h++;
    while(board->down != NULL) board = board->down;
    struct node *col = single_column(h,board->win_cond);
    while(board->next != NULL) board = board->next;
    while(board->up != NULL) 
    { 
        board->next = col; 
        col->prev = board;
        board = board->up;
        col = col->up;
    } 
    board->next = col; 
    col->prev = board;
    while(board->down != NULL) board = board->down; 
    while(board->prev != NULL) board = board->prev; 
    return board;
}

struct node *add_left_column(struct node* board) 
{ 
    int h = 0; 
    while(board->up != NULL) 
    { 
        h++; 
        board = board->up;
    } 
    h++;
    while(board->down != NULL) board = board->down;
    struct node *col = single_column(h,board->win_cond);
    while(board->up != NULL)
    { 
        board->prev = col;
        col->next = board; 
        board = board->up; 
        col = col->up;
    } 
    board->prev = col;
    col->next = board; 

    while(col->down != NULL) col = col->down; 
    return col;
} 

bool is_legal(struct node* board, int x, int player) 
{
    int id = 0;
    while(id < x) 
    { 
        board = board->next; 
        id++;
    }
    if(board->val == player) return true; 
    return false;
}

void write_board(struct node *board) 
{ 
    while(board->up != NULL) board = board->up;
    while(board != NULL) 
    { 
        while(board->next != NULL) 
        { 
            if(board->val == -1) printf(" "); 
            else if(board->val == 0) printf("x"); 
            else printf("o"); 
            board = board->next;
        }
        if(board->val == -1) printf(" "); 
        else if(board->val == 0) printf("x"); 
        else printf("o"); 
        printf("|\n"); 
        while(board->prev != NULL) board = board->prev;
        board = board->down; 
    } 
    printf("----------------------------------\n");
} 

int dir1(struct node *board,int player) 
{
    if(board == NULL) return 0;
    if(board->val == player)
        return 1+dir1(board->up,player);
    return 0;
} 

int dir2(struct node *board,int player) 
{ 
    if(board == NULL) return 0;
    if(board->val == player)
        return 1+dir2(board->down,player); 
    return 0;
} 

int dir3(struct node *board,int player) 
{ 
    if(board == NULL) return 0; 
    if(board->val == player) 
        return 1+dir3(board->prev,player); 
    return 0;
} 

int dir4(struct node *board,int player) 
{ 
    if(board == NULL) return 0; 
    if(board->val == player) 
        return 1+dir4(board->next,player); 
    return 0;
} 

int dir5(struct node *board, int player) 
{ 
    if(board == NULL) return 0; 
    if(board->val == player) 
    { 
        if(board->prev != NULL) 
            return 1+dir5(board->prev->up,player); 
        else return 1;
    }
    return 0; 
} 

int dir6(struct node *board, int player) 
{ 
    if(board == NULL) return 0; 
    if(board->val == player) 
    { 
        if(board->next != NULL) 
            return 1+dir6(board->next->down,player); 
        else return 1;
    } 
    return 0;
}

int dir7(struct node *board, int player) 
{ 
    if(board == NULL) return 0; 
    if(board->val == player)
    { 
        if(board->prev != NULL) 
            return 1+dir7(board->prev->down,player); 
        else return 1;
    } 
    return 0;
} 

int dir8(struct node *board, int player)
{ 
    if(board == NULL) return 0; 
    if(board->val == player) 
    { 
        if(board->next != NULL) 
            return 1+dir8(board->next->up,player); 
        else return 1;
    } 
    return 0;
} 

int check_win(struct node *board,int x) 
{ 
    int id = 0; 
    while(id < x) 
    { 
        board = board->next; 
        id++;
    } 
    int winner = -1;
    while(board != NULL) 
    { 
        if(board->val == -1) return winner;
        if(dir1(board,board->val) + dir2(board,board->val) - 1 >= board->win_cond ||
            dir3(board,board->val) + dir4(board,board->val) - 1 >= board->win_cond ||
            dir5(board,board->val) + dir6(board,board->val) - 1 >= board->win_cond ||
            dir7(board,board->val) + dir8(board,board->val) - 1 >= board->win_cond)
        { 
            if(winner == -1) winner = board->val; 
            else if(winner != board->val) winner = 2;
        } 
        board = board->up;
    } 
    return winner;
} 

int width(struct node *board) 
{ 
    int res = 1;
    while(board->next != NULL) 
    { 
        res++; 
        board = board->next; 
    } 
    return res;
} 

int height(struct node *board) 
{ 
    int res = 1; 
    while(board->up != NULL) 
    { 
        res++; 
        board = board->up; 
    } 
    return res;
} 

char *board_to_string(struct node *board)
{ 
    char *res; 
    res = malloc(sizeof(char) * (width(board)+1) * height(board));
    while(board->up != NULL) board = board->up;
    int id = 0;
    while(board != NULL) 
    { 
        while(board->next != NULL) 
        { 
            if(board->val == -1) res[id] = ' '; 
            else if(board->val == 0) res[id] = 'x';
            else res[id] = 'o';
            board = board->next;
            id++;
        }
        if(board->val == -1) res[id] = ' ';
        else if(board->val == 0) res[id] = 'x'; 
        else res[id] = 'o';
        id++;
        res[id++] = '\n'; 

        while(board->prev != NULL) board = board->prev;
        board = board->down; 
    } 
    return res;
} 

struct node *load_board(char *path) 
{
    FILE *load = fopen(path,"r"); 
    int a, b, win_cond; 
    fscanf(load,"%d%d%d",&a,&b,&win_cond); 
    char str[1000]; 
    fgets(str,10,load); //end of line read
    int w = 0, h = 0; 
    while(feof(load) == 0)
    { 
        fgets(str,1000,load); 
        if(h == 0) 
            while(str[w] != '\0' && str[w] != '\n') w++;
        h++;          
    }
    h = h - 1; //there's one more while loop to know we read EOF
    rewind(load);
    fscanf(load,"%d%d%d",&a,&b,&win_cond); 
    fgets(str,10,load); //end of line read
    struct node* board = init(w,h,win_cond);
    struct node* ret = board;
    while(board->up != NULL) board = board->up;
    while(feof(load) == 0) 
    { 
        fgets(str,1000,load); 
        int id = 0;
        while(board->next != NULL) 
        { 
            if(str[id] == ' ') board->val = -1;
            else if(str[id] == 'x') board->val = 0;
            else board->val = 1;
            id++; 
            board = board->next;
        } 
        if(str[id] == ' ') board->val = -1;
        else if(str[id] == 'x') board->val = 0;
        else board->val = 1;
        while(board->prev != NULL) board = board->prev; 
        if(board->down != NULL) board = board->down; 
    } 
    return ret;
}
