#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<gtk/gtk.h>
#include"fifo.h"
#include"datastruct.h"
#include"board.h"

static GtkWidget *window; 
static PipesPtr potoki; 
static char *m_id, *y_id;
int player;

struct game
{ 
    Board board;
    struct vector *v;
    GtkWidget *grid;
}; 

struct game *GAME;

struct move_button
{ 
    struct game *G; 
    int x;
};

void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_OTHER,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void zakoncz(GtkWidget *, gpointer); 
void board_to_grid(struct game*);
void game_move1(GtkWidget *,struct move_button *);
void game_move2(GtkWidget *, struct move_button *); 

void game_add_left(GtkWidget *,struct game *);
void game_add_right(GtkWidget *,struct game *);
void game_config();

void send_move(); 
static gboolean get_move(gpointer data);

int main(int argc, char *argv[]) 
{ 
    if ((potoki=initPipes(argc,argv)) == NULL) 
        return 1;
    if (argc == 2 && strcmp(argv[1],"A") == 0) { y_id="B > "; m_id="A > "; }
    else { m_id="B > "; y_id="A > "; }
    
    game_config();
    gtk_init(&argc, &argv);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    if (m_id[0] == 'A')
        gtk_window_set_title(GTK_WINDOW(window),"Player1");
    else 
        gtk_window_set_title(GTK_WINDOW(window),"Player2");

    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(zakoncz), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    vector_init(GAME->v); 
    GAME->grid = grid;
    board_to_grid(GAME);
    if(player == 1)
        gtk_widget_set_sensitive(GAME->grid,FALSE);
    g_timeout_add(100,get_move,NULL);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
} 

void game_config()
{ 
    GAME = malloc(sizeof(struct game));
    GAME->v = malloc(sizeof(struct vector));
    FILE *fptr = fopen(".config","r"); 
    int type;
    fscanf(fptr,"%d",&type);
    if(type == 0) 
    { 
        int t[4]; 
        fscanf(fptr,"%d%d%d%d",&t[0],&t[1],&t[2],&t[3]);
        int size = 0; 
        int diff = 0;
        if(t[0] == 0) size = 5; 
        if(t[0] == 1) size = 7; 
        if(t[0] == 2) size = 10;
        if(t[1] == 0) diff = 4; 
        if(t[1] == 1) diff = 5;
        GAME->board = init(size,diff);
        if(m_id[0] == 'A') player = t[2]; 
        else player = t[3];
    } 
    fclose(fptr);
} 

static void zakoncz(GtkWidget *widget, gpointer data)
{
    closePipes(potoki);
    gtk_main_quit();
}

void board_to_grid(struct game *G)
{ 
    for(int i = 0; i < G->v->count; i++) 
        gtk_container_remove(GTK_CONTAINER(G->grid),G->v->data[i]); 
    G->v->count = 0;
    int W = width(G->board); 
    int H = height(G->board); 
    for(int i = 1; i <= W; i++) 
    { 
        GtkWidget *button = gtk_button_new();
        struct move_button *K = malloc(sizeof(struct move_button)); 
        K->G = G; 
        K->x = i-1;
        gtk_grid_attach(GTK_GRID(G->grid),button,i,0,1,1);
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(game_move1),(gpointer)K);
        vector_add(G->v,button);
    } 

    GtkWidget *button_left = gtk_button_new(); 
    gtk_grid_attach(GTK_GRID(G->grid),button_left,0,H,1,1); 
    g_signal_connect(G_OBJECT(button_left),"clicked",G_CALLBACK(game_add_left),(gpointer)G);
    vector_add(G->v,button_left);

    GtkWidget *button_right = gtk_button_new();
    gtk_grid_attach(GTK_GRID(G->grid),button_right,W+1,H,1,1);
    g_signal_connect(G_OBJECT(button_right),"clicked",G_CALLBACK(game_add_right),(gpointer)G);
    vector_add(G->v,button_right);
    
    while(G->board->up != NULL) G->board = G->board->up;
    for(int i = 1; i <= H; i++) 
    { 
        for(int j = 1; j <= W; j++) 
        { 
            GtkWidget *label; 
            if(G->board->val == 0) label = gtk_label_new("x");
            else if(G->board->val == 1) label = gtk_label_new("o"); 
            else label = gtk_label_new(" ");
            vector_add(G->v,label);
            gtk_grid_attach(GTK_GRID(G->grid),label,j,i,1,1);
            if(G->board->next != NULL) G->board = G->board->next;
        }
        while(G->board->prev != NULL) G->board = G->board->prev; 
        if(G->board->down != NULL) G->board = G->board->down; 
    } 
    for(int i = 1; i <= W; i++) 
    { 
        GtkWidget *wid;
        if(G->board->val == player) 
        { 
            wid = gtk_button_new(); 
            struct move_button *K = malloc(sizeof(struct move_button)); 
            K->G = G; 
            K->x = i-1;
            g_signal_connect(G_OBJECT(wid),"clicked",G_CALLBACK(game_move2),(gpointer)K);
        } 
        else 
            wid = gtk_label_new(NULL);
        gtk_grid_attach(GTK_GRID(G->grid),wid,i,H+1,1,1); 
        vector_add(G->v,wid);
        if(G->board->next != NULL) G->board = G->board->next;
    } 
    while(G->board->prev != NULL) G->board = G->board->prev;
    gtk_widget_show_all(G->grid);
} 

void game_move1(GtkWidget *widget,struct move_button *X)
{ 
    move1(X->G->board,X->x,player);
    char buf[200]; 
    sprintf(buf,"1%d",X->x);
    send_move(buf);
    gtk_widget_set_sensitive(X->G->grid,FALSE);
    board_to_grid(GAME);
    if(check_win(GAME->board,X->x) == player) 
    {
        pokazBlad("Wygrales");
        zakoncz(NULL,NULL);  
    }
    else if(check_win(GAME->board,X->x) == 1-player)
    { 
        pokazBlad("Przegrales"); 
        zakoncz(NULL,NULL);
    } 
   
} 

void game_move2(GtkWidget *widget, struct move_button *X) 
{ 
    move2(X->G->board,X->x,player); 
    char buf[200]; 
    for(int i = 0; i < 200; i++) buf[i] = '\0';
    sprintf(buf,"2%d",X->x);
    send_move(buf);
    gtk_widget_set_sensitive(X->G->grid,FALSE);
    board_to_grid(GAME);

    if(check_win(GAME->board,X->x) == player) 
    {
        pokazBlad("Wygrales");
        zakoncz(NULL,NULL);  
    }
    else if(check_win(GAME->board,X->x) == 1-player)
    { 
        pokazBlad("Przegrales"); 
        zakoncz(NULL,NULL);
    } 
    } 

void game_add_left(GtkWidget *widget, struct game *G)
{ 
    add_left_column(G->board);
    send_move("3");
    board_to_grid(G);
} 

void game_add_right(GtkWidget *widget,struct game *G)
{ 
    add_right_column(G->board); 
    send_move("4");
    board_to_grid(G);
} 

void send_move(char *tekst)
{ 
    sendStringToPipe(potoki,tekst);
} 

static gboolean get_move(gpointer data)
{ 
    char buf[25];
    getStringFromPipe(potoki,buf,25); 
    if(buf[0] == '1' || buf[0] == '2') 
    { 
        int pos = 0; 
        int id = 1; 
        while(buf[id] != '\0')
        { 
            pos*=10; 
            pos+=(buf[id]-'0');
            id++;
        } 
        if(buf[0] == '1') 
        { 
            move1(GAME->board,pos,1-player);
            board_to_grid(GAME); 
            if(check_win(GAME->board,pos) == player) 
            {
                pokazBlad("Wygrales");
                zakoncz(NULL,NULL);  
            }
            else if(check_win(GAME->board,pos) == 1-player)
            { 
                pokazBlad("Przegrales"); 
                zakoncz(NULL,NULL);
            } 
        } 
        else if(buf[0] == '2')
        { 
            move2(GAME->board,pos,1-player);
            board_to_grid(GAME); 
            if(check_win(GAME->board,pos) == player) 
            {
                pokazBlad("Wygrales");
                zakoncz(NULL,NULL);  
            }
            else if(check_win(GAME->board,pos) == 1-player)
            { 
                pokazBlad("Przegrales"); 
                zakoncz(NULL,NULL);
            } 
            if(check_win(GAME->board,pos) != -1) zakoncz(NULL,NULL);
        } 
        gtk_widget_set_sensitive(GAME->grid,TRUE);
    } 
    else if(buf[0] == '3') 
    { 
        add_left_column(GAME->board); 
        board_to_grid(GAME);
    } 
    else if(buf[0] == '4') 
    { 
        add_right_column(GAME->board); 
        board_to_grid(GAME);
    } 
    return TRUE;
} 
