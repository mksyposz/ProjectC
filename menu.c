#include<gtk/gtk.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>

int EXIT_CODE;

struct menu_choices
{ 
    GtkWidget *board[3]; 
    GtkWidget *diff[2];
    GtkWidget *mark[2][2];
    int pressed[4];
    GtkWidget *problem, *win;
}; 

void menu_set_title(GtkWidget*,int);

void menu_set_board_size_buttons(GtkWidget*,struct menu_choices*,int); 
void board_size_change1(GtkWidget*,struct menu_choices*);
void board_size_change2(GtkWidget*,struct menu_choices*); 
void board_size_change3(GtkWidget*,struct menu_choices*);

void menu_set_difficulty_buttons(GtkWidget*,struct menu_choices*,int);
void diff_choice_change1(GtkWidget*,struct menu_choices*); 
void diff_choice_change2(GtkWidget*,struct menu_choices*);

void menu_add_free_line(GtkWidget*,int);

void problem_text(GtkWidget*,struct menu_choices*,int); 

void exit_load_play_buttons(GtkWidget*,struct menu_choices*,int);
void check_conditions(GtkWidget *widget, struct menu_choices*); 
void menu_set_marks(GtkWidget*,struct menu_choices*,int); 

void mark_choice_change_player11(GtkWidget*, struct menu_choices*);
void mark_choice_change_player12(GtkWidget*, struct menu_choices*);
void mark_choice_change_player21(GtkWidget*, struct menu_choices*);
void mark_choice_change_player22(GtkWidget*, struct menu_choices*);

void load_button_action(GtkWidget*,struct menu_choices*);

int main(int argc, char *argv[]) 
{ 
    EXIT_CODE = 1;
    srand(time(0));
    gtk_init(&argc,&argv); 
    
    //GTK window creation:
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 

    gtk_window_set_title(GTK_WINDOW(window),"Menu, kółko i krzyżyk");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER); 
    gtk_window_set_default_size(GTK_WINDOW(window),(gint)300,(gint)300);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),NULL);
    
    //GTK grid creation: 
    GtkWidget *grid = gtk_grid_new(); 
    gtk_container_add(GTK_CONTAINER(window),grid);

    struct menu_choices menu; 
    menu.win = window;
    menu_set_title(grid,0);
    menu_set_board_size_buttons(grid,&menu,1);
    menu_set_difficulty_buttons(grid,&menu,2);
    menu_set_marks(grid,&menu,3);
    menu_add_free_line(grid,5);
    exit_load_play_buttons(grid,&menu,6); 
    problem_text(grid,&menu,7);

    gtk_widget_show_all(window); 
    gtk_main();
    return EXIT_CODE;
}

void menu_set_title(GtkWidget *grid,int row)
{ 
    GtkWidget *title = gtk_label_new("<span size='20000'> MENU </span>"); 
    gtk_label_set_use_markup(GTK_LABEL(title),TRUE);
    gtk_grid_attach(GTK_GRID(grid),title,1,row,1,1);

} 

void menu_set_board_size_buttons(GtkWidget *grid, struct menu_choices *X, int row)
{ 
    GtkWidget *label = gtk_label_new("Board size:");
    gtk_label_set_xalign(GTK_LABEL(label),0.0);
    gtk_grid_attach(GTK_GRID(grid),label,0,row,1,1);

    X->pressed[0] = -1;
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid),button_box,1,row,1,1);
    
    X->board[0] = gtk_toggle_button_new_with_label("5x5");
    g_signal_connect(G_OBJECT(X->board[0]), "toggled",G_CALLBACK(board_size_change1),(gpointer)X);
    gtk_container_add(GTK_CONTAINER(button_box),X->board[0]);

    X->board[1] = gtk_toggle_button_new_with_label("7x7");
    g_signal_connect(G_OBJECT(X->board[1]), "toggled",G_CALLBACK(board_size_change2),(gpointer)X);
    gtk_container_add(GTK_CONTAINER(button_box),X->board[1]);
    
    X->board[2] = gtk_toggle_button_new_with_label("10x10");
    g_signal_connect(G_OBJECT(X->board[2]),"toggled",G_CALLBACK(board_size_change3),(gpointer)X);
    gtk_container_add(GTK_CONTAINER(button_box),X->board[2]);

} 

void board_size_change1(GtkWidget *widget, struct menu_choices *X)
{ 
    if(X->pressed[0] != -1) 
    { 
        if(X->pressed[0] == 0) 
            X->pressed[0] = -1; 
        else 
        { 
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->board[X->pressed[0]]),FALSE);
            X->pressed[0] = 0;
        } 
    } 
    else 
        X->pressed[0] = 0;
} 

void board_size_change2(GtkWidget *widget, struct menu_choices *X)
{ 
    if(X->pressed[0] != -1) 
    { 
        if(X->pressed[0] == 1) 
            X->pressed[0] = -1; 
        else 
        { 
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->board[X->pressed[0]]),FALSE);
            X->pressed[0] = 1;
        } 
    } 
    else 
        X->pressed[0] = 1;
} 

void board_size_change3(GtkWidget *widget, struct menu_choices *X)
{ 
    if(X->pressed[0] != -1) 
    { 
        if(X->pressed[0] == 2) 
            X->pressed[0] = -1; 
        else 
        { 
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->board[X->pressed[0]]),FALSE);
            X->pressed[0] = 2;
        } 
    } 
    else 
        X->pressed[0] = 2;
}

void menu_set_difficulty_buttons(GtkWidget *grid, struct menu_choices *X,int row) 
{ 
    GtkWidget *label = gtk_label_new("Difficulty:"); 
    gtk_label_set_xalign(GTK_LABEL(label),0.0); 
    gtk_grid_attach(GTK_GRID(grid),label,0,row,1,1); 

    X->pressed[1] = -1; 
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL); 
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box),GTK_BUTTONBOX_EXPAND); 
    gtk_grid_attach(GTK_GRID(grid),button_box,1,row,1,1); 

    X->diff[0] = gtk_toggle_button_new_with_label("EASY"); 
    g_signal_connect(G_OBJECT(X->diff[0]), "toggled",G_CALLBACK(diff_choice_change1),(gpointer)X);
    gtk_container_add(GTK_CONTAINER(button_box),X->diff[0]);

    X->diff[1] = gtk_toggle_button_new_with_label("HARD"); 
    g_signal_connect(G_OBJECT(X->diff[1]), "toggled",G_CALLBACK(diff_choice_change2),(gpointer)X);
    gtk_container_add(GTK_CONTAINER(button_box),X->diff[1]);

} 

void diff_choice_change1(GtkWidget *widget, struct menu_choices *X)
{ 
    if(X->pressed[1] == 1) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->diff[1]),FALSE);
    if(X->pressed[1] == 0) 
        X->pressed[1] = -1; 
    else 
        X->pressed[1] = 0;
} 

void diff_choice_change2(GtkWidget *widget, struct menu_choices* X)
{ 
    if(X->pressed[1] == 0) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->diff[0]),FALSE);
    if(X->pressed[1] == 1) 
        X->pressed[1] = -1; 
    else 
        X->pressed[1] = 1;
} 

void menu_add_free_line(GtkWidget *grid, int row) 
{ 
    GtkWidget *empty_line = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid),empty_line,0,row,1,1);
} 

void problem_text(GtkWidget *grid, struct menu_choices *X, int row)
{ 
    X->problem = gtk_label_new(NULL); 
    gtk_grid_attach(GTK_GRID(grid),X->problem,0,row,2,1);
} 

void exit_load_play_buttons(GtkWidget *grid, struct menu_choices *X, int row) 
{     
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box),GTK_BUTTONBOX_EDGE);
    gtk_grid_attach(GTK_GRID(grid), button_box,0,row,2,1);
    
    GtkWidget *exit_button = gtk_button_new_with_label("EXIT");
    g_signal_connect_swapped(exit_button, "clicked", G_CALLBACK(gtk_widget_destroy),X->win);
    gtk_container_add(GTK_CONTAINER(button_box),exit_button);

    GtkWidget *load_button = gtk_button_new_with_label("LOAD"); 
    g_signal_connect(G_OBJECT(load_button),"clicked",G_CALLBACK(load_button_action),(gpointer)X); 
    gtk_container_add(GTK_CONTAINER(button_box),load_button);

    GtkWidget *play_button = gtk_button_new_with_label("PLAY");
    g_signal_connect(G_OBJECT(play_button), "clicked", G_CALLBACK(check_conditions),(gpointer)X); 
    gtk_container_add(GTK_CONTAINER(button_box),play_button);
} 

void check_conditions(GtkWidget *widget, struct menu_choices *X) 
{ 
    char str[4][200];
    for(int i = 0; i < 4; i++) 
        for(int j = 0; j < 200; j++) 
            str[i][j] = 0;
    int prob = 0;
    if(X->pressed[0] == -1)
    { 
        prob = 1;
        sprintf(str[0],"Please pick board size.\n");
    } 
    if(X->pressed[1] == -1) 
    { 
        prob = 1;
        sprintf(str[1],"Please pick difficulty.\n"); 
    }  
    if(X->pressed[2] == -1) 
    { 
        prob = 1; 
        sprintf(str[2],"Player1 pick prefered mark.\n");
    } 
    if(X->pressed[3] == -1) 
    { 
        prob = 1; 
        sprintf(str[3],"Player2 pick prefered mark.\n");
    } 
    if(prob == 1) 
    { 
        strcat(str[0],str[1]); 
        strcat(str[0],str[2]); 
        strcat(str[0],str[3]);
        for(int i = 0; i < 200; i++) if(str[0][i] == '\0') {str[0][i-1] = '\0'; break;}
        gtk_label_set_text(GTK_LABEL(X->problem),str[0]);
    } 
    else 
    { 
        FILE *cfg; 
        cfg = fopen(".config","w");
        if(X->pressed[2] == X->pressed[3]) 
        { 
            X->pressed[2] = rand()%2; 
            X->pressed[3] = 1 - X->pressed[2];
        } 
        fprintf(cfg,"0 %d %d %d %d\n",X->pressed[0],X->pressed[1],X->pressed[2],X->pressed[3]);
        fclose(cfg); 
        EXIT_CODE = 0;
        gtk_widget_destroy(GTK_WIDGET(X->win));
    } 
} 

void menu_set_marks(GtkWidget *grid, struct menu_choices *X, int row) 
{ 
    for(int i = 0; i < 2; i++) 
    { 
        GtkWidget *label; 
        if(i == 0)  label = gtk_label_new("Player1");
        else label = gtk_label_new("Player2:");
        gtk_label_set_xalign(GTK_LABEL(label),0.0); 
        gtk_grid_attach(GTK_GRID(grid),label,0,row+i,1,1); 
        
        X->pressed[2+i] = -1;
        GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL); 
        gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box),GTK_BUTTONBOX_CENTER);
        gtk_grid_attach(GTK_GRID(grid),button_box,1,row+i,1,1); 

        X->mark[i][0] = gtk_toggle_button_new(); 
        GtkWidget *image = gtk_image_new_from_file("marks/xmark.png");
        gtk_button_set_image(GTK_BUTTON(X->mark[i][0]),image);
        if(i == 0) g_signal_connect(G_OBJECT(X->mark[i][0]),"toggled",G_CALLBACK(mark_choice_change_player11),(gpointer)X); 
        else g_signal_connect(G_OBJECT(X->mark[i][0]),"toggled",G_CALLBACK(mark_choice_change_player21),(gpointer)X);
        gtk_container_add(GTK_CONTAINER(button_box),X->mark[i][0]); 
        
        X->mark[i][1] = gtk_toggle_button_new(); 
        image = gtk_image_new_from_file("marks/circlemark.png"); 
        gtk_button_set_image(GTK_BUTTON(X->mark[i][1]),image); 
        if(i == 0) g_signal_connect(G_OBJECT(X->mark[i][1]),"toggled",G_CALLBACK(mark_choice_change_player12),(gpointer)X); 
        else g_signal_connect(G_OBJECT(X->mark[i][1]),"toggled",G_CALLBACK(mark_choice_change_player22),(gpointer)X);
        gtk_container_add(GTK_CONTAINER(button_box),X->mark[i][1]); 
    } 
} 

void mark_choice_change_player11(GtkWidget *widget, struct menu_choices *X)
{ 
    if(X->pressed[2] == 1) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->mark[0][1]),FALSE); 
    if(X->pressed[2] == 0) 
        X->pressed[2] = -1; 
    else 
        X->pressed[2] = 0;
}

void mark_choice_change_player12(GtkWidget *widget, struct menu_choices *X)
{ 
    if(X->pressed[2] == 0) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->mark[0][0]),FALSE); 
    if(X->pressed[2] == 1) 
        X->pressed[2] = -1; 
    else 
        X->pressed[2] = 1;
} 

void mark_choice_change_player21(GtkWidget *widget, struct menu_choices *X)
{ 
    if(X->pressed[3] == 1) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->mark[1][1]),FALSE); 
    if(X->pressed[3] == 0) 
        X->pressed[3] = -1; 
    else 
        X->pressed[3] = 0;
} 

void mark_choice_change_player22(GtkWidget *widget, struct menu_choices *X)
{ 
    if(X->pressed[3] == 0) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->mark[1][0]),FALSE); 
    if(X->pressed[3] == 1) 
        X->pressed[3] = -1; 
    else 
        X->pressed[3] = 1;
} 

void load_button_action(GtkWidget *widget, struct menu_choices *X)
{ 
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",GTK_WINDOW(X->win),GTK_FILE_CHOOSER_ACTION_OPEN,
            "_Cancel",GTK_RESPONSE_CANCEL,"_Open",GTK_RESPONSE_OK,NULL); 
    
    mkdir(".history",0777);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),"./.history");
    gtk_widget_show_all(dialog); 
    gint resp = gtk_dialog_run(GTK_DIALOG(dialog)); 
    if(resp == GTK_RESPONSE_OK)
    {
        FILE *cfg; 
        cfg = fopen(".config","w"); 
        fprintf(cfg,"1 %s\n",gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog))); 
        fclose(cfg);
        gtk_widget_destroy(X->win);
        EXIT_CODE = 0;
    } 
    gtk_widget_destroy(dialog);
} 
