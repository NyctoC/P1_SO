#include <gtk/gtk.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

std::string opened_file_path;

void compress_file(GtkWidget *widget, gpointer data) {
    if (!opened_file_path.empty()) {
        std::string command = "./SircComp -c \"" + opened_file_path + "\"";
        system(command.c_str());
    }
}

void open_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(data),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        GtkWidget *text_view = GTK_WIDGET(g_object_get_data(G_OBJECT(data), "text_view"));
        GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_set_text(text_buffer, content.c_str(), -1);

        opened_file_path = filename;
        GtkWidget *compress_button = GTK_WIDGET(g_object_get_data(G_OBJECT(data), "compress_button"));
        gtk_widget_set_sensitive(compress_button, TRUE);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *open_button;
    GtkWidget *compress_button;
    GtkWidget *text_view;
    GtkWidget *scrolled_window;

    gtk_init(&argc, &argv);
    g_type_init();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Text File Reader");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    open_button = gtk_button_new_with_label("Open File");
    g_signal_connect(open_button, "clicked", G_CALLBACK(open_file), window);
    gtk_box_pack_start(GTK_BOX(vbox), open_button, FALSE, FALSE, 0);

    compress_button = gtk_button_new_with_label("Compress");
    gtk_widget_set_sensitive(compress_button, FALSE);
    g_signal_connect(compress_button, "clicked", G_CALLBACK(compress_file), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), compress_button, FALSE, FALSE, 0);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    g_object_set_data(G_OBJECT(window), "text_view", text_view);
    g_object_set_data(G_OBJECT(window), "compress_button", compress_button);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}