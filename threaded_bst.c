#include <stdio.h>

#include "utility.h"
#include "r_threaded_bst.h"

void threaded_tree_menu()
{
    int option;
    do
    {
        clearScreen();
        printf("\n ***** Threaded Tree Visualization Menu *****\n");
        printf("\n 1. Right threaded binary tree");
        printf("\n 2. Left threaded binary tree");
        printf("\n 3. Two-way threaded binary tree");
        printf("\n 4. Return To Main Menu\n");
        printf("\n\n Select option (1-4): ");
        scanf(" %d", &option);

        switch(option)
        {
        case 1:
            start_rthreaded_tree_program();
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            option = 4;
            printf("\n Returning to main menu...");
            break;
        }
    }
    while(option != 4);
}
