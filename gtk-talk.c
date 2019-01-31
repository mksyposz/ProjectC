#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<gtk/gtk.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"fifo.h"
#include"datastruct.h"
#include"board.h"

static GtkWidget *window; 
static PipesPtr potoki; 
static char *m_id, *y_id;
int player, loaded;

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
void save_button(GtkWidget *widget);
void block_buttons();

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
    if(loaded == -1 && player == 1)
        block_buttons();
    else if(loaded != player && loaded != -1)  
        block_buttons();
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
        GAME->board = init(size,size,diff);
        if(m_id[0] == 'A') player = t[2];
        else player = t[3];
        loaded = -1;
    }
    if(type == 1)
    { 
        char s[1000]; 
        fscanf(fptr,"%s",s);
        FILE *load = fopen(s,"r"); 
        int p;
        fscanf(load,"%d%d",&p,&loaded); 

        if (m_id[0] == 'A')
            player = p; 
        else 
            player = 1-p;
        fclose(load);
        GAME->board = init(5,5,4);
        GAME->board = load_board(s);
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
	char s[30]; 
	if(player == 0) 
	sprintf(s,"Your mark is x"); 
	else 
	sprintf(s,"Your mark is o"); 
	GtkWidget *label = gtk_label_new(s);
	gtk_grid_attach(GTK_GRID(G->grid),label,0,0,1,1);
    //reset grid
    for(int i = 0; i < G->v->count; i++) 
        gtk_container_remove(GTK_CONTAINER(G->grid),G->v->data[i]); 
    G->v->count = 0;
    
    int W = width(G->board); 
    int H = height(G->board);
    
    //board menu button
    GtkWidget *men = gtk_menu_new();
    GtkWidget *saveandexit = gtk_menu_item_new_with_label("Save & Exit"); 
    GtkWidget *exit = gtk_menu_item_new_with_label("Exit");
    
    gtk_menu_shell_append(GTK_MENU_SHELL(men),saveandexit); 
    gtk_menu_shell_append(GTK_MENU_SHELL(men),exit); 

    g_signal_connect(G_OBJECT(saveandexit),"activate",G_CALLBACK(save_button),NULL);
    g_signal_connect(G_OBJECT(exit),"activate",G_CALLBACK(zakoncz),NULL);

    GtkWidget *m_button = gtk_menu_button_new(); 
    gtk_menu_button_set_popup(GTK_MENU_BUTTON(m_button),men);
    gtk_menu_button_set_direction(GTK_MENU_BUTTON(m_button),GTK_ARROW_RIGHT);
    gtk_grid_attach(GTK_GRID(G->grid),m_button,W+1,0,1,1);

    gtk_widget_show(saveandexit); 
    gtk_widget_show(exit);
    vector_add(G->v,m_button);
    
    //board to grid
    for(int i = 1; i <= W; i++) 
    { 
        GtkWidget *button = gtk_button_new();
        struct move_button *K = malloc(sizeof(struct move_button)); 
        K->G = G; 
        K->x = i-1;
        gtk_grid_attach(GTK_GRID(G->grid),button,i,0,1,1);
        GtkWidget *image = gtk_image_new_from_file("marks/arrowdown.png");
        gtk_button_set_image(GTK_BUTTON(button),image);
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(game_move1),(gpointer)K);
        vector_add(G->v,button);
    } 

    GtkWidget *image_plus1 = gtk_image_new_from_file("marks/plus.png");
    GtkWidget *image_plus2 = gtk_image_new_from_file("marks/plus.png");
    
    GtkWidget *button_left = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button_left),image_plus1); 
    gtk_grid_attach(GTK_GRID(G->grid),button_left,0,H,1,1); 
    g_signal_connect(G_OBJECT(button_left),"clicked",G_CALLBACK(game_add_left),(gpointer)G);
    vector_add(G->v,button_left);

    GtkWidget *button_right = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button_right),image_plus2);
    gtk_grid_attach(GTK_GRID(G->grid),button_right,W+1,H,1,1);
    g_signal_connect(G_OBJECT(button_right),"clicked",G_CALLBACK(game_add_right),(gpointer)G);

    vector_add(G->v,button_right);
    
    while(G->board->up != NULL) G->board = G->board->up;
    for(int i = 1; i <= H; i++) 
    { 
        for(int j = 1; j <= W; j++) 
        { 
            GtkWidget *label; 
            if(G->board->val == 0) label = gtk_image_new_from_file("marks/x.png");
            else if(G->board->val == 1) label = gtk_image_new_from_file("marks/o.png"); 
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
            GtkWidget *image = gtk_image_new_from_file("marks/arrowup.png"); 
            gtk_button_set_image(GTK_BUTTON(wid),image);
            g_signal_connect(G_OBJECT(wid),"clicked",G_CALLBACK(game_move2),(gpointer)K);
        } 
        else 
            wid = gtk_label_new(NULL);
        gtk_grid_attach(GTK_GRID(G->grid),wid,i,H+1,1,1); 
        vector_add(G->v,wid);
        if(G->board->next != NULL) G->board = G->board->next;
    } 
    while(G->board->prev != NULL) G->board = G->board->prev;
	vector_add(G->v,label);
    gtk_widget_show_all(G->grid);
} 

void game_move1(GtkWidget *widget,struct move_button *X)
{ 
    X->G->board = move1(X->G->board,X->x,player);
    char buf[200]; 
    sprintf(buf,"1%d",X->x);
    send_move(buf);
    board_to_grid(GAME);
    block_buttons();

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
    else if(check_win(GAME->board,X->x) == 2) 
    { 
        pokazBlad("Remis"); 
        zakoncz(NULL,NULL);
    } 
} 

void game_move2(GtkWidget *widget, struct move_button *X) 
{ 
    X->G->board = move2(X->G->board,X->x,player); 
    char buf[200]; 
    for(int i = 0; i < 200; i++) buf[i] = '\0';
    sprintf(buf,"2%d",X->x);
    send_move(buf);
    board_to_grid(GAME);
    block_buttons();
    
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
    else if(check_win(GAME->board,X->x) == 2) 
    { 
        pokazBlad("Remis"); 
        zakoncz(NULL,NULL);
    }
} 

void game_add_left(GtkWidget *widget, struct game *G)
{ 
    G->board = add_left_column(G->board);
    send_move("3");
    board_to_grid(G);
} 

void game_add_right(GtkWidget *widget,struct game *G)
{ 
    G->board = add_right_column(G->board); 
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
            GAME->board = move1(GAME->board,pos,1-player);
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
            else if(check_win(GAME->board,pos) == 2) 
            { 
                pokazBlad("Remis"); 
                zakoncz(NULL,NULL);
            } 
        } 
        else if(buf[0] == '2')
        { 
            GAME->board = move2(GAME->board,pos,1-player);
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
            else if(check_win(GAME->board,pos) == 2) 
            { 
                pokazBlad("Remis"); 
                zakoncz(NULL,NULL);
            } 
            //if(check_win(GAME->board,pos) != -1) zakoncz(NULL,NULL);
        } 
        //block_buttons();
        for(int i = 1; i < GAME->v->count; i++) 
            gtk_widget_set_sensitive(GAME->v->data[i],TRUE);
    } 
    else if(buf[0] == '3') 
    { 
        GAME->board = add_left_column(GAME->board); 
        board_to_grid(GAME);
    } 
    else if(buf[0] == '4') 
    { 
        GAME->board = add_right_column(GAME->board); 
        board_to_grid(GAME);
    } 
    return TRUE;
} 

void save_button(GtkWidget *widget)
{ 
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",GTK_WINDOW(window),
            GTK_FILE_CHOOSER_ACTION_SAVE,"_Cancel",GTK_RESPONSE_CANCEL,"_Save",GTK_RESPONSE_OK,NULL);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog),"Untilted"); 
    mkdir(".history",0777);
    
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),"./.history");
    gtk_widget_show_all(dialog); 
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog)); 
    //if(gtk_widget_is_sensitive(GAME->v->data[1]) == TRUE) 
    if(resp == GTK_RESPONSE_OK)
    {
        FILE *cfg; 
        cfg = fopen(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)),"w");
        int move = player; 
        if(gtk_widget_is_sensitive(GAME->v->data[1]) == FALSE) move = 1 - move;
        if (m_id[0] == 'A')
            fprintf(cfg,"%d\n",player); 
        else 
            fprintf(cfg,"%d\n",1-player);
        fprintf(cfg,"%d\n%d\n%s",move,GAME->board->win_cond,board_to_string(GAME->board));
        fclose(cfg);
        zakoncz(NULL,NULL);
    } 
    gtk_widget_destroy(dialog); 
} 

void block_buttons()
{ 
    for(int i = 1; i < GAME->v->count; i++) 
        gtk_widget_set_sensitive(GAME->v->data[i],FALSE);
} 
