// gcc `pkg-config --cflags gtk+-3.0` -o 25 division25.c `pkg-config --libs gtk+-3.0`  -w

#include <gtk/gtk.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//Global variable: request division
int request_division = 0;
int *shm_data;
GtkWidget *btn_light, *image_light;
gboolean set_red = FALSE, set_green = FALSE, called = FALSE;
int number_window;
GtkWidget *button_image;
GtkWidget *button_request;

gboolean nofication(gpointer data){

	GtkWidget *label;
	label = (GtkWidget *)data;
	char s[0];
	gchar * chuyenChuoi;
	s[0] = '0' + shm_data[0];
	
	gtk_label_set_text((GtkLabel *)label,s);
	//get request to division1 
	if(shm_data[0] )
	if(shm_data[0] == request_division && !set_green && shm_data[3] == request_division){
		image_light = gtk_image_new_from_file("icon/blue.jpg");
		gtk_button_set_image(GTK_BUTTON(btn_light), image_light);
		gtk_widget_show(image_light);

		gtk_image_clear(button_image);
		button_image = gtk_image_new_from_file("icon/play.png");
		g_object_ref(button_image);
		gtk_button_set_image(GTK_BUTTON(button_request),button_image);

		set_green = TRUE;
		set_red = FALSE;
		called = FALSE;
	}

	if(shm_data[0] == request_division && !set_green && called){
		image_light = gtk_image_new_from_file("icon/blue.jpg");
		gtk_button_set_image((GtkButton *)btn_light, image_light);
		gtk_widget_show(image_light);

		gtk_image_clear(button_image);
		button_image = gtk_image_new_from_file("icon/play.png");
		g_object_ref(button_image);
		gtk_button_set_image(GTK_BUTTON(button_request),button_image);
		
		set_green = TRUE;
		set_red = FALSE;
		called = FALSE;
	}
	if(shm_data[0] != request_division && !set_red){
		image_light = gtk_image_new_from_file("icon/red.jpg");
		gtk_button_set_image((GtkButton *)btn_light, image_light);
		gtk_widget_show(image_light);
		set_red = TRUE;
		set_green = FALSE;
	}
}

static void on_cliked_button(GtkWidget *widget, gpointer data){
	int *shm;
	shm = (int *)data;
	shm[2] = request_division;
	printf("sent request succes\n");
	gtk_image_clear(button_image);
	button_image = gtk_image_new_from_file("icon/pause.png");
	g_object_ref(button_image);
	gtk_button_set_image(GTK_BUTTON(button_request),button_image);
	called = TRUE;
}

static void activate(GtkApplication *app, gpointer user_data){
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *btn_nofication, *label_nofication;
	// GtkWidget *btn_exit;	
	//Xu ly chuoi
	char c, s[10];
	char s2[20] = "Division";
	c = '0' + request_division;
	s[0] = c;
	strcat(s2, s);

    // <-------------  For add stylesheet.css
    GtkCssProvider* Provider = gtk_css_provider_new();
    GdkDisplay* Display = gdk_display_get_default();
    GdkScreen* Screen = gdk_display_get_default_screen(Display);

    gtk_style_context_add_provider_for_screen(Screen, GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), "style.css", NULL);
    // End add stylesheet.css ---------------->


	window = gtk_application_window_new(app);
	gtk_window_set_title (GTK_WINDOW (window), s2);
	gtk_container_set_border_width (GTK_CONTAINER (window), 50);
	gtk_window_set_position (window,GTK_WIN_POS_CENTER);
	if(number_window == 5){
		gtk_window_move(GTK_WINDOW(window),300,100);
	}
	if(number_window == 4){
		gtk_window_move(GTK_WINDOW(window),520,200);
	}
	if(number_window == 3){
		gtk_window_move(GTK_WINDOW(window),300,500);
	}
	if(number_window == 2){
		gtk_window_move(GTK_WINDOW(window),520,600);
	}


			//Creat grid
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER (window), grid);
	//button request
	button_request = gtk_button_new();
	button_image = gtk_image_new_from_file("icon/play.png");
	g_object_ref(button_image);
	gtk_button_set_image(GTK_BUTTON(button_request),button_image);

	gtk_grid_attach(GTK_GRID(grid), button_request, 0,2 , 2, 1);
	g_signal_connect (button_request, "clicked", G_CALLBACK(on_cliked_button), user_data);
	//btn light
	btn_light = gtk_button_new();
	gtk_grid_attach(GTK_GRID(grid), btn_light, 0, 1, 2, 1);

	btn_nofication = gtk_button_new();
	gtk_grid_attach(GTK_GRID(grid), btn_nofication, 0, 0, 2, 1);
	//label nofication
	label_nofication = gtk_label_new_with_mnemonic("");
	gtk_widget_set_name(label_nofication,"info_label");
	gtk_widget_set_size_request(label_nofication, 60, 40);
	gtk_container_add(GTK_CONTAINER(btn_nofication), label_nofication);

	gtk_widget_show_all(window);

	g_timeout_add(500, nofication, label_nofication);

}

int main(int argc, char **argv){
	int *shm, shmid;
	key_t key;
	key = 1234;
	shmid = shmget(key, 128, 0666);
	shm = shmat(shmid, NULL, 0);
	shm_data = shm;

	number_window = 0;
	// app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	// g_signal_connect(app, "activate", G_CALLBACK(activate), shm);
	// status = g_application_run(G_APPLICATION (app), argc, argv);
	// g_object_unref(app);

	// return status;

	//Multiprocess
	pid_t child_pid;
	child_pid = fork();
	if(child_pid == 0){
		
		pid_t child_pid2 = fork();
		if(child_pid2 == 0){
			pid_t child_pid3 = fork();
			if(child_pid3 == 0){
				//process division 5
				GtkApplication *app5;
				int status;
				request_division = 5;
				number_window = 5;
				// printf("%d\n", request_division);
				app5 = gtk_application_new("org.gtk.division5", G_APPLICATION_FLAGS_NONE);
				g_signal_connect(app5, "activate", G_CALLBACK(activate), shm);
				status = g_application_run(G_APPLICATION (app5), argc, argv);
				g_object_unref(app5);
				return status;

			}else if(child_pid3 > 0){
				//process division 4
				GtkApplication *app4;
				int status;
				number_window = 4;
				request_division = 4;
				// printf("%d\n", request_division);
				app4 = gtk_application_new("org.gtk.division4", G_APPLICATION_FLAGS_NONE);
				g_signal_connect(app4, "activate", G_CALLBACK(activate), shm);
				status = g_application_run(G_APPLICATION (app4), argc, argv);
				g_object_unref(app4);
				return status;

			}
			else{
				printf("Error\n");
			}
		}
		else if(child_pid2 > 0){
			//process division 3
			GtkApplication *app3;
			int status;
			number_window = 3;
			request_division = 3;
			// printf("%d\n", request_division);
			app3 = gtk_application_new("org.gtk.division3", G_APPLICATION_FLAGS_NONE);
			g_signal_connect(app3, "activate", G_CALLBACK(activate), shm);
			status = g_application_run(G_APPLICATION (app3), argc, argv);
			g_object_unref(app3);
			return status;
			
		}else{
			printf("Error2\n");
		}
		
	}
	else if(child_pid > 0){
		//process Divison 2
		GtkApplication *app2;
		int status;
		request_division = 2;
		number_window = 2;
		// printf("%d\n", request_division);
		app2 = gtk_application_new("org.gtk.division2", G_APPLICATION_FLAGS_NONE);
		g_signal_connect(app2, "activate", G_CALLBACK(activate), shm);
		status = g_application_run(G_APPLICATION (app2), argc, argv);
		g_object_unref(app2);
		return status;

	}
	else{
		printf("Error\n");
	}


}
