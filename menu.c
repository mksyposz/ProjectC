#include<gtk/gtk.h>
#include<stdbool.h>

struct board_size_choice 
{
    GtkWidget *button[3];
    int pressed;
};

struct diff_choice
{ 
    GtkWidget *button[2]; 
    int pressed;
};

struct menu_choices
{ 
    struct board_size_choice *B; 
    struct diff_choice *D;
    GtkWidget *problem, *win;
}; 

void board_size_change1(GtkWidget *widget, struct board_size_choice*);
void board_size_change2(GtkWidget *widget, struct board_size_choice*); 
void board_size_change3(GtkWidget *widget, struct board_size_choice*);
void diff_choice_change1(GtkWidget *widget, struct diff_choice*); 
void diff_choice_change2(GtkWidget *widget, struct diff_choice*);
void check_conditions(GtkWidget *widget, struct menu_choices*); 

int main(int argc, char *argv[]) 
{ 
    gtk_init(&argc,&argv); 
    
    //GTK window creation:
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); 

    gtk_window_set_title(GTK_WINDOW(window),"Menu, kółko i krzyżyk");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER); 
    gtk_window_set_default_size(GTK_WINDOW(window),(gint)300,(gint)210);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),NULL);

    //GTK grid creation: 
    GtkWidget *grid = gtk_grid_new(); 
    gtk_container_add(GTK_CONTAINER(window),grid);

    GtkWidget *title = gtk_label_new(" <span size='20000'> MENU </span>"); 
    gtk_label_set_use_markup(GTK_LABEL(title),TRUE);
    gtk_grid_attach(GTK_GRID(grid),title,1,0,1,1);
    //Board size row config:
    //board size:
    GtkWidget *board_size = gtk_label_new("Board size:");
    gtk_label_set_xalign(GTK_LABEL(board_size),0.0);
    gtk_grid_attach(GTK_GRID(grid),board_size,0,1,1,1);
    //buttons:
    
    struct board_size_choice buttons_for_board_size;
    buttons_for_board_size.pressed = -1;
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid),button_box,1,1,1,1);
    
    buttons_for_board_size.button[0] = gtk_toggle_button_new_with_label("5x5");
    g_signal_connect(G_OBJECT(buttons_for_board_size.button[0]), "toggled",G_CALLBACK(board_size_change1),(gpointer)&buttons_for_board_size);
    gtk_container_add(GTK_CONTAINER(button_box),buttons_for_board_size.button[0]);

    buttons_for_board_size.button[1] = gtk_toggle_button_new_with_label("7x7");
    g_signal_connect(G_OBJECT(buttons_for_board_size.button[1]), "toggled",G_CALLBACK(board_size_change2),(gpointer)&buttons_for_board_size);
    gtk_container_add(GTK_CONTAINER(button_box),buttons_for_board_size.button[1]);
    
    buttons_for_board_size.button[2] = gtk_toggle_button_new_with_label("10x10");
    g_signal_connect(G_OBJECT(buttons_for_board_size.button[2]), "toggled",G_CALLBACK(board_size_change3),(gpointer)&buttons_for_board_size);
    gtk_container_add(GTK_CONTAINER(button_box),buttons_for_board_size.button[2]);

    //Difficulty row config:
    GtkWidget *difficulty = gtk_label_new("Difficulty:");
    gtk_label_set_xalign(GTK_LABEL(difficulty) ,0.0);
    gtk_grid_attach(GTK_GRID(grid),difficulty,0,2,1,1);
    
    //buttons
    struct diff_choice buttons_for_diff_choice;
    buttons_for_diff_choice.pressed = -1;
    GtkWidget *button_box2 = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box2),GTK_BUTTONBOX_EXPAND);
    gtk_grid_attach(GTK_GRID(grid),button_box2,1,2,1,1);
    
    buttons_for_diff_choice.button[0] = gtk_toggle_button_new_with_label("EASY"); 
    g_signal_connect(G_OBJECT(buttons_for_diff_choice.button[0]), "toggled",G_CALLBACK(diff_choice_change1),(gpointer)&buttons_for_diff_choice);
    gtk_container_add(GTK_CONTAINER(button_box2),buttons_for_diff_choice.button[0]);

    buttons_for_diff_choice.button[1] = gtk_toggle_button_new_with_label("HARD"); 
    g_signal_connect(G_OBJECT(buttons_for_diff_choice.button[1]), "toggled",G_CALLBACK(diff_choice_change2),(gpointer)&buttons_for_diff_choice);
    gtk_container_add(GTK_CONTAINER(button_box2),buttons_for_diff_choice.button[1]);

    //free line 
    GtkWidget *empty_line = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid),empty_line,0,3,1,1);

    struct menu_choices menu;
    menu.win = window;
    menu.problem = gtk_label_new(NULL);
    menu.B = &buttons_for_board_size;
    menu.D = &buttons_for_diff_choice; 

    //exit and play
    GtkWidget *button_box3 = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_button_box_set_layout(GTK_BUTTON_BOX(button_box3),GTK_BUTTONBOX_EDGE);
    gtk_grid_attach(GTK_GRID(grid), button_box3,0,4,2,1);
    
    GtkWidget *exit_button = gtk_button_new_with_label("EXIT");
    g_signal_connect_swapped(exit_button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    gtk_container_add(GTK_CONTAINER(button_box3),exit_button);

    GtkWidget *play_button = gtk_button_new_with_label("PLAY");
    g_signal_connect(G_OBJECT(play_button), "clicked", G_CALLBACK(check_conditions),(gpointer)&menu); 
    gtk_container_add(GTK_CONTAINER(button_box3),play_button);
    
    //Problem window
    gtk_grid_attach(GTK_GRID(grid), menu.problem,0,5,2,1);

    gtk_widget_show_all(window); 
    gtk_main();
}

void board_size_change1(GtkWidget *widget, struct board_size_choice *X)
{ 
    if(X->pressed == 1) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->button[1]),FALSE);
    if(X->pressed == 2) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->button[2]),FALSE);

    if(X->pressed != 0) 
        X->pressed = 0;
    else 
        X->pressed = -1;

} 

void board_size_change2(GtkWidget *widget, struct board_size_choice *X)
{ 
    if(X->pressed == 0) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->button[0]),FALSE);
    if(X->pressed == 2) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->button[2]),FALSE);

    if(X->pressed != 1) 
        X->pressed = 1;
    else 
        X->pressed = -1;
} 

void board_size_change3(GtkWidget *widget, struct board_size_choice *X)
{ 
    if(X->pressed == 0) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->button[0]),FALSE);
    if(X->pressed == 1) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->button[1]),FALSE);

    if(X->pressed != 2)
        X->pressed = 2;
    else 
        X->pressed = -1;
} 

void diff_choice_change1(GtkWidget *widget, struct diff_choice *X)
{ 
    if(X->pressed == 1) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->button[1]),FALSE);
    if(X->pressed == 0) 
        X->pressed = -1; 
    else 
        X->pressed = 0;
} 
void diff_choice_change2(GtkWidget *widget, struct diff_choice* X)
{ 
    if(X->pressed == 0) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(X->button[0]),FALSE);
    if(X->pressed == 1) 
        X->pressed = -1; 
    else 
        X->pressed = 1;
} 

void check_conditions(GtkWidget *widget, struct menu_choices *X) 
{ 
    char str[100];
    if(X->B->pressed == -1 && X->D->pressed == -1) 
    { 
        sprintf(str,"Please pick board size.\nPlease pick difficulty");
        gtk_label_set_text(GTK_LABEL(X->problem),str); 
    } 
    else if(X->B->pressed == -1) 
    { 
        sprintf(str,"Please pick board size.");
        gtk_label_set_text(GTK_LABEL(X->problem),str); 
    } 
    else if(X->D->pressed == -1) 
    { 
        sprintf(str,"Please pick difficulty."); 
        gtk_label_set_text(GTK_LABEL(X->problem),str); 
    }  
    else 
        gtk_widget_destroy(GTK_WIDGET(X->win));
} 
