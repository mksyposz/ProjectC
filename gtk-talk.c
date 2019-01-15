#include<string.h>
#include<math.h>
#include<gtk/gtk.h>
#include"fifo.h"
#include"datastruct.h"
#include"board.h"

static GtkWidget *window; 
static PipesPtr potoki; 
static char *m_id, *y_id;:"w

struct move_button
{ 
    struct pair_ii p; 
    GtkWidget *grid;
    Board board;
}; 

void pokazBlad(char *komunikat)
{
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

static void zakoncz(GtkWidget *, gpointer); 

int main(int argc, char *argv[]) 
{ 
    if ((potoki=initPipes(argc,argv)) == NULL)
        return 1;
    if (argc == 2 && strcmp(argv[1],"A") == 0) { y_id="B > "; m_id="A > "; }
    else { m_id="B > "; y_id="A > "; }

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

    gtk_widget_show_all(window); 
    gtk_main();
    return 0;
} 

static void zakoncz(GtkWidget *widget, gpointer data)
{
    closePipes(potoki);
    gtk_main_quit();
}
