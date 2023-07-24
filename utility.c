#include "utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#def CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

void clearScreen()
{
    system(CLEAR_SCREEN);
}

void view_diagram()
{
#ifdef _WIN32
    system("dot -Tpng diagram.dot -o diagram.png");
    system("start diagram.png");
#else
    system("dot -Tpng diagram.dot -o diagram.png");
    system("xdg-open diagram.png");
#endif
}

void save_diagram()
{
#ifdef _WIN32
    _mkdir("output");
#else
    mkdir("output", 0700);
#endif
    char filename[100];
    printf("\n Enter the filename to save: ");
    getchar();
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';
    char filepath[200];
    sprintf(filepath, "output/%s.png", filename);
    char command[300];
    sprintf(command, "dot -Tpng diagram.dot -o %s", filepath);
    int result = system(command);
    if (result == 0)
    {
        printf("\n Tree image saved successfully as %s\n", filepath);
    }
    else
    {
        printf("\n Error saving tree image.\n");
    }
}

