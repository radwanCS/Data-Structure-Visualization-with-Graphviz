#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "headers/utility.h"
#include "headers/bst.h"
#include "headers/threaded_bst.h"
#include "headers/avl_tree.h"
#include "headers/red_black_tree.h"
#include "headers/splay_tree.h"

void mainMenu()
{
    clearScreen();
    printf("\nData Structure Visualization\n");
    printf("----------------------------\n");
    printf("\n ****** MAIN MENU *******");
    printf("\n\n 1. Tree Data Structure");
    printf("\n 2. Linked List Data Structure");
    printf("\n 3. EXIT");
    printf("\n\n Select option (1-3): ");
}

void treeMenu()
{
    int option;
    do
    {
        clearScreen();
        printf("\n ****** Tree Data Structure Visualization *******");
        printf("\n\n 1. Binary Search Tree BST");
        printf("\n 2. Threaded Binary Tree");
        printf("\n 3. AVL Tree");
        printf("\n 4. Red Black Tree");
        printf("\n 5. Splay Tree");
        printf("\n 6. Return to main menu");
        printf("\n\n Select option (1-6): ");
        scanf(" %d", &option);
        switch(option)
        {
        case 1:
            startBSTProgram();
            break;
        case 2:
            threaded_tree_menu();
            break;
        case 3:
            start_avl_tree_program();
            break;
        case 4:
            start_redBlackTree_program();
            break;
        case 5:
            start_splay_tree_program();
            break;
        case 6:
            option = 6;
            break;
        default:
            option = 6;
            printf("\n Invalid input!");
            break;
        }
    }
    while(option != 5);
}

int main()
{
    int option;
    do
    {
        mainMenu();
        scanf(" %d", &option);
        switch(option)
        {
        case 1:
            treeMenu();
            break;
        case 2:
            break;
        case 3:
            option = 3;
            break;
        default:
            option = 3;
            printf("\n Invalid input!");
            break;
        }
    }
    while(option != 3);
}
