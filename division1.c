// gcc `pkg-config --cflags gtk+-3.0` -o 1 gtk_division1.c `pkg-config --libs gtk+-3.0`  -lvlc -w

#include <gtk/gtk.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>

int *shm_data;
GtkWidget *button1, *image_light;
gboolean set_green = FALSE;
gboolean set_red = FALSE;
GtkWidget *button2, *button3, *button4, *button5;
GtkWidget *button_image;	
int memory[5];


gboolean nofication(gpointer data){

	GtkWidget *label_nofication;
	label_nofication = (GtkWidget *)data;

	//chuyen so sang chuoi
	char s[0];
	s[0] = '0' + shm_data[0];
	gtk_label_set_text(GTK_LABEL(label_nofication), s);
	int value;
	
	//Neu vi tri thang may dang o tang 1 va den mau do -> set mau xanh
	if(shm_data[0] == 1 && !set_green){
		set_green = TRUE;
		set_red = FALSE;
		//Moi lan set image cho button thi object image se tu dong duoc tra ve NULL, nen moi lan dung lai lai phai tao moi lai image_object
		image_light = gtk_image_new_from_file("icon/blue.jpg");
		gtk_button_set_image((GtkButton *)button1, image_light);
		gtk_widget_show(image_light);
		for (int i = 0; i < 4; ++i){
			if(memory[i] == 1){
				value = i+2;
				memory[i] = 0;
			}
		}
		switch(value){
			case 2:
				// button_image = gtk_image_new_from_file("icon/T2.png");
				// gtk_button_set_image(GTK_BUTTON(button2), button_image);
				gtk_widget_set_name(button2, "info_btn");
				break;
			case 3:
				// button_image = gtk_image_new_from_file("icon/T3.png");
				// gtk_button_set_image(GTK_BUTTON(button3), button_image);
				gtk_widget_set_name(button3, "info_btn");
				break;
			case 4:
				// button_image = gtk_image_new_from_file("icon/T4.png");
				// gtk_button_set_image(GTK_BUTTON(button4), button_image);
				gtk_widget_set_name(button4, "info_btn");
				break;
			case 5:
				// button_image = gtk_image_new_from_file("icon/T5.png");
				// gtk_button_set_image(GTK_BUTTON(button5), button_image);
				gtk_widget_set_name(button5, "info_btn");
				break;
		}
	}

	//Neu vi tri thang may k phai tang 1, den xanh -> set mau do
	if(shm_data[0] != 1 && !set_red){
		image_light = gtk_image_new_from_file("icon/red.jpg");
		gtk_button_set_image((GtkButton *)button1, image_light);
		gtk_widget_show(image_light);	
		set_red = TRUE;
		set_green = FALSE;
	}
	
}

//Su kien khi click button2
static void on_clicked_division2(GtkWidget *GtkWidget, gpointer data){
	//shm[1] chua du lieu yeu cau dang duoc goi boi tang 1, va se get vao queue trong PSdevice
	//shm[3] chua du lieu yeu cau dang duoc goi boi tang 1, va se ko bi get vao queue
	int *shm;
	shm = (int *)data;
	shm[1] = 2;
	shm[3] = 2;
	g_print("sent request sucess!\n Start go division 2\n");
	// button_image = gtk_image_new_from_file("icon/2.png");
	// gtk_button_set_image(GTK_BUTTON(button2), button_image);
	gtk_widget_set_name(button2, "active_btn");
	memory[0] = 1;
}

//Su kien khi click button3
static void on_clicked_division3(GtkWidget *GtkWidget, gpointer data){
	int *shm;
	shm = (int *)data;
	shm[1] = 3;
	shm[3] = 3;
	g_print("sent request sucess!\n Start go division 3\n");
	// button_image = gtk_image_new_from_file("icon/3.png");
	// gtk_button_set_image(GTK_BUTTON(button3), button_image);
	gtk_widget_set_name(button3, "active_btn");
	memory[1] = 1;
}

//Su kien khi click button4
static void on_clicked_division4(GtkWidget *GtkWidget, gpointer data){
	int *shm;
	shm = (int *)data;
	shm[1] = 4;
	shm[3] = 4;
	g_print("sent request sucess!\n Start go division 4\n");
	// button_image = gtk_image_new_from_file("icon/4.png");
	// gtk_button_set_image(GTK_BUTTON(button4), button_image);
	gtk_widget_set_name(button4, "active_btn");
	memory[2] = 1;
}

//Su kien khi click button5
static void on_clicked_division5(GtkWidget *GtkWidget, gpointer data){
	int *shm;
	shm = (int *)data;
	shm[1] = 5;
	shm[3] = 5;
	g_print("sent request sucess!\n Start go division 5\n");
	// button_image = gtk_image_new_from_file("icon/5.png");
	// gtk_button_set_image(GTK_BUTTON(button5), button_image);
	gtk_widget_set_name(button5, "active_btn");
	memory[3] = 1;
}

//ham activate duoc goi khi su kien activate xay ra, su kien activate duoc goi khi chay ham g_application_run
static void activate (GtkApplication *app, gpointer user_data){
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *btn_nofication, *btn_exit, *label_nofication;

	GtkWidget *image;
	// <-------------  For add stylesheet.css
    GtkCssProvider* Provider = gtk_css_provider_new();
    GdkDisplay* Display = gdk_display_get_default();
    GdkScreen* Screen = gdk_display_get_default_screen(Display);

    gtk_style_context_add_provider_for_screen(Screen, GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), "style.css", NULL);
    // End add stylesheet.css ---------------->



  	/* create a new window, and set its title */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Elavator");
	gtk_container_set_border_width (GTK_CONTAINER (window), 80);
	gtk_window_set_icon_from_file (GTK_WINDOW(window),"icon/thangmay.png",NULL);

  	/* Here we construct the container that is going pack our buttons */
	grid = gtk_grid_new ();

  	/* Pack the container in the window */
	gtk_container_add (GTK_CONTAINER (window), grid);

  	//Division 1
	button1 = gtk_button_new();
	gtk_grid_attach (GTK_GRID (grid), button1, 0, 1, 2, 1);
	// gtk_button_set_always_show_image ((GtkButton *) button1, TRUE);

  	//Division 2
	button2 = gtk_button_new();
 	button_image = gtk_image_new_from_file("icon/T2.png");
    g_object_ref(button_image);
    gtk_button_set_image(GTK_BUTTON(button2),button_image);
	gtk_grid_attach (GTK_GRID (grid), button2, 0, 5, 2, 1);
	gtk_widget_set_name(button2, "info_btn");
	// gtk_widget_set_size_request(button2, 60, 60);

  	//Division 3
	button3= gtk_button_new();
 	button_image = gtk_image_new_from_file("icon/T3.png");
    g_object_ref(button_image);
    gtk_button_set_image(GTK_BUTTON(button3),button_image);	
	gtk_grid_attach (GTK_GRID (grid), button3, 0, 4, 2, 1);
	gtk_widget_set_name(button3, "info_btn");

  	//Division 4
	button4= gtk_button_new();
 	button_image = gtk_image_new_from_file("icon/T4.png");
    g_object_ref(button_image);
    gtk_button_set_image(GTK_BUTTON(button4),button_image);	
	gtk_grid_attach (GTK_GRID (grid), button4, 0, 3, 2, 1);
	gtk_widget_set_name(button4, "info_btn");

  	//Division 5
	button5= gtk_button_new();
 	button_image = gtk_image_new_from_file("icon/T5.png");
    g_object_ref(button_image);
    gtk_button_set_image(GTK_BUTTON(button5),button_image);	
	gtk_grid_attach (GTK_GRID (grid), button5, 0, 2, 2, 1);
	gtk_widget_set_name(button5, "info_btn");

	g_signal_connect (button2, "clicked", G_CALLBACK (on_clicked_division2), user_data);	
	g_signal_connect (button3, "clicked", G_CALLBACK (on_clicked_division3), user_data);
	g_signal_connect (button4, "clicked", G_CALLBACK (on_clicked_division4), user_data);
	g_signal_connect (button5, "clicked", G_CALLBACK (on_clicked_division5), user_data);

	//Nofication
	btn_nofication = gtk_button_new();
	gtk_grid_attach (GTK_GRID (grid), btn_nofication,0, 0, 2, 1);
	//label nofication
	label_nofication = gtk_label_new_with_mnemonic("");
	gtk_widget_set_name(label_nofication,"info_label");
	gtk_widget_set_size_request(label_nofication, 60, 40);
	gtk_container_add(GTK_CONTAINER (btn_nofication), label_nofication);

	/* Now that we are done packing our widgets, we show them all
	* in one go, by calling gtk_widget_show_all() on the window.
	* This call recursively calls gtk_widget_show() on all widgets
	* that are contained in the window, directly or indirectly.
	*/
	gtk_widget_show_all (window);
	//thuc thi lai ham nofication voi moi 500ms
	g_timeout_add(500, (GSourceFunc)nofication, label_nofication);
}

int main (int argc, char **argv){
	//Lay dia chi shared memory
	int *shm, shmid;
	key_t key;
	key = 1234;
	shmid = shmget(key, 128, 0666);
	shm = shmat(shmid, NULL, 0);

	shm_data = (int *)shm;
	//yeu cau tu tang 1 ban dau duoc khoi tao la 0
	shm_data[3] = 0;

	GtkApplication *app;
	int status;
	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), shm);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
