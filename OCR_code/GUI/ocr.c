#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <math.h>
#include <err.h>

#include <gtk/gtk.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../sudoku_solver/solver.h"
#include "../sudoku_solver/util.h"
#include "../image_pre-treatment/header/static_black_and_white.h"
#include "../image_pre-treatment/header/grayscale.h"
#include "../image_pre-treatment/header/median_blur.h"
#include "../image_pre-treatment/header/thresholding.h"
#include "../image_pre-treatment/header/utils.h"
#include "../image_pre-treatment/header/rotate.h"
#include "../image_pre-treatment/header/point.h"
#include "../neural_network/header/initialize_network.h"
#include "../neural_network/header/result_network.h"
#include "../neural_network/header/learning_network.h"
#include "../neural_network/header/data_network.h"
#include "../neural_network/header/result_network.h"

//initialize result grid and a value variable
int Final[9][9];
int value;

typedef struct GUI
{
	GtkWindow* interface;
	GtkButton* Solve;
	GtkButton* Solution;
    GtkButton* BlackAndWhite;
    GtkButton* Apply;
    GtkButton* Grayscaleimage;
    GtkButton* Blurredimage;
    GtkButton* Originalimage;
    GtkFileChooserButton* LoadImage;
    GtkImage* logo;
    GtkImage* ImageL;
    GtkEntry* angle;
    GtkFileChooserButton* SaveFolder;
    GtkLabel* NeuralNw;
    GtkEntry* Epoch;
    GtkButton* Train;
    GtkLabel* Res;
    GtkFileChooserButton* Load;
    GtkLabel* LoadN;

}GUI;

typedef struct Gtk_nn
{
    GtkWindow* interface;
    Network *nn;
    GtkFileChooserButton* SaveFolder;
    GtkLabel* NeuralNw;
    GtkEntry* Epoch;
    GtkButton* Train;
    GtkLabel* Res;
    GtkFileChooserButton* Load;
    GtkLabel* LoadN;
}Gtk_nn;

/** extract extension of name file
 * @param filename name of the file
*/
char *get_filename_ext(const char *filename)
{
    // get the filename extension
    char *t = strrchr(filename, '.');
    if (!t || t == filename)
        return "";
    return t + 1;
}

/** Loads an image in a surface, The format of the surface is SDL_PIXELFORMAT_RGB888.
 * @param path Path of the image.
*/
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surfacetemp = IMG_Load(path);
    if (surfacetemp == NULL){
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(surfacetemp,SDL_PIXELFORMAT_RGB888,0);
    if (surface == NULL){
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    SDL_FreeSurface(surfacetemp);
    return surface;
}

/** disable or enable buttons 
 * @param activation value of disable or enable
 * @param user_data information of struct
*/
void desac(int activation, gpointer user_data)
{
    GUI *gui = user_data;
    gtk_widget_set_sensitive(GTK_WIDGET(gui->Grayscaleimage), activation == 1?TRUE:FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->BlackAndWhite), activation == 1?TRUE:FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->Solve), activation == 1?TRUE:FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->Solution), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->Apply), activation == 1?TRUE:FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->Originalimage), activation == 1?TRUE:FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->angle), activation == 1?TRUE:FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(gui->Blurredimage), activation == 1?TRUE:FALSE);
}

/** Function that allows to choose the input file, the file must be an image otherwise the function returns an error
 * @param chooser selected button
 * @param user_data information of struct
*/
void on_file_choosed(GtkFileChooser *chooser, gpointer user_data)
{
    GUI *gui = user_data;
    gchar *filename = gtk_file_chooser_get_filename(chooser);
    char *ext = get_filename_ext(filename); //extract extension of name file
    if (strcmp(ext, "png") == 0 || strcmp(ext, "jpg") == 0)
        {
            SDL_Surface *image = load_image(filename);
            if (SDL_SaveBMP(image,"filename.bmp") != 0 ) //save the image as a texture
            {
                errx(EXIT_FAILURE,"FAIL");
            }
            GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("filename.bmp", 620, 620, TRUE, NULL);
            gtk_image_set_from_pixbuf(gui->ImageL, pixbuf);
            g_free(filename); 
            desac(1,user_data);
        }
    else
    {   
        //displays an error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gui->interface), 
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
            "ERROR: File is not an image");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog

        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, 620, 620, TRUE, NULL);
        gtk_image_set_from_pixbuf(gui->ImageL, pixbuf);

        desac(0,user_data);

        //gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(chooser), NULL); // reset filename
    }
    
}

/** Function that allows to choose the input file.
 * The file must be an file with an extension .nn otherwise the function returns an error
 * @param chooser selected button
 * @param user_data information of struct
*/
void on_file(GtkFileChooser *chooser, gpointer user_data)
{
    Gtk_nn *gtk_nn = user_data;
    gchar *filename = gtk_file_chooser_get_filename(chooser);
    char *ext = get_filename_ext(filename); //extract extension of name file
    if (strcmp(ext, "nn") == 0)
        {
            load_weight_bias(filename,gtk_nn->nn);
            printinfo(gtk_nn->nn);
        }
    else
    {   
        //displays an error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_nn->interface), 
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
            "ERROR: File is not an extension .nn");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog
    }
    
}

/** displays the binarized image
 * @param gray selected button
 * @param user_data information of struct
*/
void BlackW(GtkButton *gray, gpointer user_data)
{
    GUI *gui = user_data;
    (void)gray;

    //image recovery
    const char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gui->LoadImage));
    
    SDL_Surface *image = load_image(filename);
    surface_to_grayscale(image);
    surface_to_blur(image);
    binarisation(image,0.1);

    if (SDL_SaveBMP(image,"filename.bmp") != 0 ) //save the image as a texture
    {
        errx(EXIT_FAILURE,"FAIL Black and White");
    }

    //returns the image in the widget associated with the display
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("filename.bmp", 620, 620, TRUE, NULL);
    gtk_image_set_from_pixbuf(gui->ImageL, pixbuf);
}

/** displays the grayscale image
 * @param gray selected button
 * @param user_data information of struct
*/
void gray(GtkButton *gray, gpointer user_data)
{
    GUI *gui = user_data;
    (void)gray;

    //image recovery
    const char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gui->LoadImage));
    
    SDL_Surface *image = load_image(filename);
    surface_to_grayscale(image);

    if (SDL_SaveBMP(image,"filename.bmp") != 0 ) //save the image as a texture
    {
        errx(EXIT_FAILURE,"FAIL GRAYSCALE");
    }

    //returns the image in the widget associated with the display
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("filename.bmp", 620, 620, TRUE, NULL);
    gtk_image_set_from_pixbuf(gui->ImageL, pixbuf);
}

/** displays the blurred image
 * @param button selected button
 * @param user_data information of struct
*/
void blurred(GtkButton *button, gpointer user_data)
{
    GUI *gui = user_data;
    (void)button;
    //image recovery
    const char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gui->LoadImage));
    
    SDL_Surface *image = load_image(filename);
    surface_to_grayscale(image);
    surface_to_blur(image);

    if (SDL_SaveBMP(image,"filename.bmp") != 0 ) //save the image as a texture
    {
        errx(EXIT_FAILURE,"FAIL BLURRED");
    }

    //returns the image in the widget associated with the display
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("filename.bmp", 620, 620, TRUE, NULL);
    gtk_image_set_from_pixbuf(gui->ImageL, pixbuf);
}

/** displays the original image
 * @param button selected button
 * @param user_data information of struct
*/
void origin(GtkButton *button, gpointer user_data)
{   
    GUI *gui = user_data;
    (void)button;

    //image recovery
    const char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gui->LoadImage));
    
    SDL_Surface *image = load_image(filename);

    if (SDL_SaveBMP(image,"filename.bmp") != 0 ) //save the image as a texture
    {
        errx(EXIT_FAILURE,"FAIL ORIGINAL");
    }

    //returns the image in the widget associated with the display
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("filename.bmp", 620, 620, TRUE, NULL);
    gtk_image_set_from_pixbuf(gui->ImageL, pixbuf);
}

/** remove extension of name file
 * @param s the name of the char
*/
void remove_extension(char* s) {
  char* dot = 0;
  while (*s) {
    if (*s == '.') dot = s;  // last dot
    else if (*s == '/' || *s == '\\') dot = 0;  // ignore dot before path separators
    s++;
  }
  if (dot) *dot = '\0';
}

/** Solve Sudoku and write the result to the Result file
 * @param button selected button
 * @param user_data information of struct
*/
void Solve_Clicked(GtkButton *button, gpointer user_data)
{
    GUI *gui = user_data;

    char *file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gui->LoadImage));

    //Initializing the grid
    int sudoku[9][9];
    
    char* res = "grid_00"; //"grid_00" replace with received grid

    //Creating files
    FILE *Grille; 
    FILE *Result;

    Grille = fopen(res, "r");

    //parse the file
    Convert(Grille, sudoku);

    //Solves Sudoku
    SolveSudoku(sudoku, 0, 0);
    
    remove_extension(file); //remove extension of name file

    //create the result file with the name of the argument passed in parameter
    char * filename = strcat(file,".result"); 

    Result = fopen(filename, "w"); 

    //write the result to the Result file
    Write(sudoku, Result); 

    fclose(Grille); //close text file
    fclose(Result); //close text file
    gtk_widget_set_sensitive(GTK_WIDGET(gui->Solution), TRUE);
} 

/** copy a matrix into another matrix
 * @param M the matrix to copy
*/
void copy(int M[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            Final[i][j] = M[i][j];
        }
    }
}

/** lets you know if a pixel is black or not
 * @param image selected image
 * @param x value of x axe
 * @param y value of y axe
*/
char BlackY(SDL_Surface *image, int x, int y)
{
    Uint32 pixel;
    Uint8 r, g,b;

    pixel = get_pixel(image,x,y);
    SDL_GetRGB(pixel, image->format, &r, &g, &b);

    return (r != 255);
}

/** function that shows me an image within another image
 * the pixels are displayed in a certain color depending on whether or not they are present in the unsolved sudoku
 * @param image selected image
 * @param number selected image number
 * @param x value of x axe
 * @param y value of y axe
 * @param inblue value to know if the pixel is blue
*/
void printNumber(SDL_Surface* image,SDL_Surface* number, int x, int y, int inblue)
{
    Uint32 pixelB;
    pixelB = SDL_MapRGB(image->format, 0, 0, 0);
    Uint32 pixelG;
    pixelG = SDL_MapRGB(image->format, 17, 111, 184);

    for(int i = x*80; i < (x+1)*80; i++)
    {
        for(int j = y*80; j < (y+1)*80; j++)
        {
            if(!BlackY(image,i,j) && BlackY(number,i-x*80, j-y*80))
            {
                if(inblue)
                    put_pixel(image,i,j,pixelG);
                else
                    put_pixel(image,i,j,pixelB);
            }
        }
    }

}

/** function that shows me the sudoku solution as an image
 * save the image
 * @param button selected button
 * @param user_data information of struct
*/
void Solution_show(GtkButton *button, gpointer user_data)
{
    GUI *gui = user_data;

    int grid[9][9];

    //"grid_00" replace with received grid
    FILE *rFile = fopen("grid_00", "r"); //open file
    Convert(rFile,grid); //convert file
    copy(grid); //copy grid to Final
    SolveSudoku(Final,0,0); //solve grid

    SDL_Surface *imageGrid = load_image("sudokuvide.jpg"); //we load the empty sudoku image
    //SDL_Surface * imageGrid = format_surface(imageGrid1);

    //we load the images of the numbers
    SDL_Surface *number1  = load_image("num/1.png"); 

    SDL_Surface *number2  = load_image("num/2.png");

    SDL_Surface *number3 = load_image("num/3.png");;

    SDL_Surface *number4 = load_image("num/4.png");

    SDL_Surface *number5 = load_image("num/5.png");

    SDL_Surface *number6 = load_image("num/6.png");

    SDL_Surface *number7 = load_image("num/7.png");

    SDL_Surface *number8 = load_image("num/8.png");

    SDL_Surface *number9 = load_image("num/9.png");

    int val = 0;
    int inblue = 0;

    //write the numbers in the correct place in the picture 
    for(int i = 0; i < 9;i++)
    {
        for(int j = 0; j < 9; j++)
        {
            inblue = 0;
            val = grid[j][i];
            if(val == 0)
            {
                inblue = 1;
                val = Final[j][i];
            }
            if(val == 1)
                printNumber(imageGrid,number1,i,j, inblue);
            else if(val == 2)
                printNumber(imageGrid,number2,i,j, inblue);
            else if(val == 3)
                printNumber(imageGrid,number3,i,j, inblue);
            else if(val == 4)
                printNumber(imageGrid,number4,i,j, inblue);
            else if(val == 5)
                printNumber(imageGrid,number5,i,j, inblue);
            else if(val == 6)
                printNumber(imageGrid,number6,i,j, inblue);      
            else if(val == 7)
                printNumber(imageGrid,number7,i,j, inblue);
            else if(val == 8)
                printNumber(imageGrid,number8,i,j, inblue);
            else if(val == 9)
                printNumber(imageGrid,number9,i,j, inblue);      
        }
    }

    //free all surfaces
    SDL_FreeSurface(number9);
    SDL_FreeSurface(number8);
    SDL_FreeSurface(number7);
    SDL_FreeSurface(number6);
    SDL_FreeSurface(number5);
    SDL_FreeSurface(number4);
    SDL_FreeSurface(number3);
    SDL_FreeSurface(number2);
    SDL_FreeSurface(number1);

    if (SDL_SaveBMP(imageGrid,"Solution/SudokuSolution.bmp") != 0 ) //Save Solution
        {
            errx(EXIT_FAILURE,"FAIL");
        }

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("Solution/SudokuSolution.bmp", 620, 620, TRUE, NULL); //Show Solution
    gtk_image_set_from_pixbuf(gui->ImageL, pixbuf);
    SDL_FreeSurface(imageGrid);
}

/** Rotate manual 
 * @param rotate selected button
 * @param user_data information of struct
*/
void rotate_clicked(GtkButton *rotate, gpointer user_data)
{
    GUI *gui = user_data;

    char *ptr;
    double angle;
    
    char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gui->LoadImage));

    const gchar *input = gtk_entry_get_text(gui->angle);

    //checks if the input is a number
    int a = 0;
    for (int i = 0; input[i] != 0; i++)
    {
        if (!isdigit(input[i]))
        {
            a += 1;
        }
    }

    if (a > 0)
    {
        //display an error message
        GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(gui->interface), 
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
        "ERROR: It's not a number!");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog       
    }
    
    else 
    {
        angle = strtod(input, &ptr);

        SDL_Surface *image_surface = load_image("filename.bmp");

        SDL_Surface *new_surface = rotate_angle(image_surface,angle);

        if (SDL_SaveBMP(new_surface, "rotate.bmp") != 0 )
        {
            errx(EXIT_FAILURE,"FAIL ROTATE");
        }

        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale("rotate.bmp", 620, 620, TRUE, NULL);
        gtk_image_set_from_pixbuf(gui->ImageL, pixbuf);
    }
}

/** implementation of neural network tests
 * @param rotate selected button
 * @param user_data information of struct
*/
void trainS(GtkButton *rotate, gpointer user_data)
{
    Gtk_nn *gtk_nn = user_data;

    char* folder = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtk_nn->SaveFolder));

    if (folder == NULL)
    {
        //displays an error message
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_nn->interface), 
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
            "ERROR: Folder is not selected");
        gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
        gtk_widget_destroy(dialog); // destroy dialog
    }

    else{
        char *ptr;
        double angle;

        int sizeD = 0;
        int sizeL = 0;
        int sizeDT = 0;
        int sizeLT = 0;
        double** array = data_to_array("../../data/MNIST/train.data",&sizeD);
        double** labels = label_to_array("../../data/MNIST/train.label",&sizeL);
        double** arrayTest = data_to_array("../../data/MNIST/test.data",&sizeDT);
        double** labelsTest = label_to_array("../../data/MNIST/test.label",&sizeLT);

        if(sizeL != sizeD)
            errx(1,"Label and data have different size");
        if(sizeLT != sizeDT)
            errx(1,"Label and data have different size");

        const gchar *input = gtk_entry_get_text(gtk_nn->Epoch);

        //checks if the input is a number
        int a = 0;
        for (int i = 0; input[i] != 0; i++)
        {
            if (!isdigit(input[i]))
            {
                a += 1;
            }
        }

        if (a > 0)
        {
            //display an error message
            GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_nn->interface), 
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, 
            "ERROR: It's not a number!");
            gtk_dialog_run(GTK_DIALOG(dialog)); // run dialog
            gtk_widget_destroy(dialog); // destroy dialog       
        }
        
        else 
        {
            angle = strtod(input, &ptr);
            training(gtk_nn->nn,0.1,angle,sizeD,0,array,labels);
            value = test_accuracy(gtk_nn->nn,arrayTest,labelsTest,sizeDT);
            gchar a;
            g_snprintf(&a, 150, "Result : %d/%u",value,sizeLT);
            gtk_label_set_text(gtk_nn->Res,&a);
            char file[800];
            char* name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtk_nn->SaveFolder));
            sprintf(file,"%s/result.nn",name);
            save_weight_bias(file,gtk_nn->nn);
        }
    }
}

/** main function
 * @param argc number of arguments
 * @param argv arguments
*/
int main (int argc, char **argv)
{   
    srand((long int)time);

    //initialize N
    int model[] = {784,150,10};

    // Initializes GTK.
    gtk_init(&argc ,&argv);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();

    GError* error = NULL;
    if(gtk_builder_add_from_file(builder, "ocr.glade", &error) == 0)
    {
        g_printerr("Error: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Widget : interface
    GtkWindow* interface = GTK_WINDOW(gtk_builder_get_object(builder, "interface"));

    // Sets window title
    gtk_window_set_title(GTK_WINDOW(interface), "Scan & Solve");

    // Gets the widgets.

    GtkButton* Solve = GTK_BUTTON(gtk_builder_get_object(builder, "Solve"));
    GtkButton* Solution = GTK_BUTTON(gtk_builder_get_object(builder, "Solution"));
    GtkButton* BlackAndWhite = GTK_BUTTON(gtk_builder_get_object(builder, "BlackAndWhite"));
    GtkButton* Apply = GTK_BUTTON(gtk_builder_get_object(builder, "Apply"));
    GtkButton* Grayscaleimage = GTK_BUTTON(gtk_builder_get_object(builder, "Grayscaleimage"));
    GtkButton* Blurredimage = GTK_BUTTON(gtk_builder_get_object(builder, "Blurredimage"));
    GtkButton* Originalimage = GTK_BUTTON(gtk_builder_get_object(builder, "Originalimage"));
    GtkButton* Train = GTK_BUTTON(gtk_builder_get_object(builder, "Train"));

    GtkFileChooserButton* LoadImage = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "LoadImage"));
    GtkFileChooserButton* SaveFolder = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "SaveFolder"));
    GtkFileChooserButton* Load = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "Load"));

    GtkImage* ImageL = GTK_IMAGE(gtk_builder_get_object(builder, "ImageL"));
    GtkImage* logo = GTK_IMAGE(gtk_builder_get_object(builder, "logo"));

    GtkEntry *angle = GTK_ENTRY(gtk_builder_get_object(builder, "angle"));
    GtkEntry* Epoch = GTK_ENTRY(gtk_builder_get_object(builder, "Epoch"));

    GtkLabel* Res = GTK_LABEL(gtk_builder_get_object(builder, "Res"));
    GtkLabel* NeuralNw = GTK_LABEL(gtk_builder_get_object(builder, "NeuralNw"));
    GtkLabel* LoadN = GTK_LABEL(gtk_builder_get_object(builder, "LoadN"));
    
    
    // Creates the "GUI" structure.
    GUI gui =
        {   
            .SaveFolder = SaveFolder,
            .NeuralNw = NeuralNw,
            .Epoch = Epoch,
            .Train = Train,
            .Res = Res,
            .Load = Load,
            .LoadN = LoadN,
            .angle = angle,
            .ImageL = ImageL,
            .logo = logo,
            .LoadImage = LoadImage,
            .Originalimage = Originalimage,
            .Blurredimage = Blurredimage,
            .Grayscaleimage = Grayscaleimage,
            .Apply = Apply,
            .BlackAndWhite = BlackAndWhite,
            .Solution = Solution,
            .Solve = Solve,
            .interface = interface
        };

    // Creates the "Gtk_nn" structure.
    Gtk_nn gtk_nn = 
    {
        .interface = interface,
        .SaveFolder = SaveFolder,
        .NeuralNw = NeuralNw,
        .Epoch = Epoch,
        .Train = Train,
        .Res = Res,
        .Load = Load,
        .LoadN = LoadN,
        .nn = NewNetwork(model,3)
    };

    // Connects signal handlers.
    g_signal_connect(interface, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(LoadImage, "selection-changed", G_CALLBACK(on_file_choosed), &gui);
    g_signal_connect(Load, "selection-changed", G_CALLBACK(on_file), &gtk_nn);
    g_signal_connect(BlackAndWhite, "clicked", G_CALLBACK(BlackW), &gui);
    g_signal_connect(Grayscaleimage, "clicked", G_CALLBACK(gray), &gui);
    g_signal_connect(Blurredimage, "clicked", G_CALLBACK(blurred), &gui);
    g_signal_connect(Originalimage, "clicked", G_CALLBACK(origin), &gui);
    g_signal_connect(Solve, "clicked", G_CALLBACK(Solve_Clicked), &gui);
    g_signal_connect(Apply, "clicked", G_CALLBACK(rotate_clicked), &gui);
    g_signal_connect(Solution, "clicked", G_CALLBACK(Solution_show), &gui);
    g_signal_connect(Train, "clicked", G_CALLBACK(trainS), &gtk_nn);

    gtk_widget_show(GTK_WIDGET(interface));

    // Runs the main loop.
    gtk_main();

    // Exits.
    
    FreeNetwork(gtk_nn.nn);

	SDL_Quit(); 

    return EXIT_SUCCESS;
}
