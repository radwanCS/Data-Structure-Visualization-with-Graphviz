#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "utility.h"

struct node
{
    int data;
    struct node *right;
    struct node *left;
    bool rightThread;
    bool leftThread;
};

struct node *dtt_search(struct node *tree, int val)
{
    if(tree == NULL || tree->data == val)
    {
        return tree;
    }
    else if(val < tree->data)
    {
        if(tree->leftThread)
        {
            return NULL;
        }
        else
        {
            return dtt_search(tree->left, val);
        }
    }
    else
    {
        if(tree->rightThread)
        {
            return NULL;
        }
        else
        {
            return dtt_search(tree->right, val);
        }
    }
};

int dtt_total_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else
    {
        if(tree->rightThread == true && tree->leftThread == true)
        {
            return 1;
        }
        else if(tree->leftThread == true && tree->rightThread == false)
        {
            return dtt_total_nodes(tree->right) + 1;
        }
        else if(tree->rightThread == true && tree->leftThread == false)
        {
            return dtt_total_nodes(tree->left) + 1;
        }
        else
        {
            return dtt_total_nodes(tree->left) + dtt_total_nodes(tree->right) + 1;
        }
    }
}

int dtt_total_external_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else if(tree->rightThread == true && tree->leftThread == true)
    {
        return 1;
    }
    else
    {
        if(tree->rightThread)
        {
            return dtt_total_external_nodes(tree->left);
        }
        else if(tree->leftThread == true)
        {
            return dtt_total_external_nodes(tree->right);
        }
        else
        {
            return dtt_total_external_nodes(tree->left) + dtt_total_external_nodes(tree->right);
        }
    }
}

int dtt_total_internal_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else if(tree->leftThread == true && tree->rightThread == true)
    {
        return 0;
    }
    else
    {
        if(tree->leftThread == true)
        {
            return dtt_total_internal_nodes(tree->right) + 1;
        }
        else if(tree->rightThread == true)
        {
            return dtt_total_internal_nodes(tree->left) + 1;
        }
        else
        {
            return dtt_total_internal_nodes(tree->left) + dtt_total_internal_nodes(tree->right) + 1;
        }
    }
}

struct node *dtt_insucc(struct node *ptr)
{
    if(ptr->rightThread == true)
    {
        return ptr->right;
    }
    ptr = ptr->right;
    while(ptr->leftThread == false)
    {
        ptr = ptr->left;
    }
    return ptr;
};

struct node *dtt_inpred(struct node *ptr)
{
    if(ptr->leftThread == true)
    {
        return ptr->left;
    }
    ptr = ptr->left;
    while(ptr->rightThread == false)
    {
        ptr = ptr->right;
    }
    return ptr;
};


struct node *dtt_delete_caseA(struct node *tree, struct node *parent, struct node *ptr)
{
    if(parent == NULL)
    {
        tree = NULL;
    }
    else if(ptr == parent->left)
    {
        parent->leftThread = true;
        parent->left = ptr->left;
    }
    else
    {
        parent->rightThread = true;
        parent->right = ptr->right;
    }
    free(ptr);
    return tree;
};

struct node *dtt_delete_caseB(struct node *tree, struct node *parent, struct node *ptr)
{
    struct node *child;
    if(ptr->leftThread == false)
    {
        child = ptr->left;
    }
    else
    {
        child = ptr->right;
    }

    if(parent == NULL)
    {
        tree = child;
    }
    else if(ptr == parent->left)
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }

    struct node *s = dtt_insucc(ptr);
    struct node *p = dtt_inpred(ptr);

    if(ptr->leftThread == false)
    {
        p->right = s;
    }
    else
    {
        if(ptr->rightThread == false)
        {
            s->left = p;
        }
    }
    free(ptr);
    return tree;
};

struct node *dtt_delete_caseC(struct node *tree, struct node *parent, struct node *ptr)
{
    struct node *parsucc = ptr;
    struct node *succ = ptr->right;

    while(succ->leftThread == false)
    {
        parsucc = succ;
        succ = succ->left;
    }
    ptr->data = succ->data;
    if(succ->leftThread == true && succ->rightThread == true)
    {
        tree = dtt_delete_caseA(tree, parsucc, succ);
    }
    else
    {
        tree = dtt_delete_caseB(tree, parsucc, succ);
    }
    return tree;
};

struct node *delete_dtt_node(struct node *tree, int val)
{
    struct node *parent;
    struct node *ptr = tree;

    int found = 0;

    while(ptr != NULL)
    {
        if(val == ptr->data)
        {
            found = 1;
            break;
        }
        parent = ptr;

        if(val < ptr->data)
        {
            if(ptr->leftThread == false)
            {
                ptr = ptr->left;
            }
            else
            {
                break;
            }
        }
        else
        {
            if(ptr->rightThread == false)
            {
                ptr = ptr->right;
            }
            else
            {
                break;
            }
        }
    }

    if(found == 0)
    {
        printf("Value not present in the tree \n");
    }
    else if(ptr->leftThread == false && ptr->rightThread == false)
    {
        tree = dtt_delete_caseC(tree, parent, ptr);
    }
    else if(ptr->leftThread == false)
    {
        tree = dtt_delete_caseB(tree, parent, ptr);
    }
    else if(ptr->rightThread == false)
    {
        tree = dtt_delete_caseB(tree, parent, ptr);
    }
    else
    {
        tree = dtt_delete_caseA(tree, parent, ptr);
    }
    return tree;
}

struct node *dtt_insert(struct node *tree, int val)
{
    struct node *ptr = tree;
    struct node *parent = NULL;

    while(ptr != NULL)
    {
        if(val == ptr->data)
        {
            printf("Duplicate Key !\n");
            return tree;
        }
        parent = ptr;
        if(val < ptr->data)
        {
            if(ptr->leftThread == false)
            {
                ptr = ptr->left;
            }
            else
            {
                break;
            }
        }
        else
        {
            if(ptr->rightThread == false)
            {
                ptr = ptr->right;
            }
            else
            {
                break;
            }
        }
    }

    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = val;
    new_node->leftThread = true;
    new_node->rightThread = true;

    if(parent == NULL)
    {
        tree = new_node;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    else if(val < parent->data)
    {
        new_node->left = parent->left;
        new_node->right = parent;
        parent->leftThread = false;
        parent->left = new_node;
    }
    else
    {
        new_node->left = parent;
        new_node->right = parent->right;
        parent->rightThread = false;
        parent->right = new_node;
    }
    return tree;
};

struct node *dtt_leftMost(struct node *n)
{
    if(n == NULL)
    {
        return NULL;
    }
    while(n->leftThread == false)
    {
        n = n->left;
    }
    return n;
};

void dtt_inorder_traversal(struct node *tree)
{
    struct node *ptr = dtt_leftMost(tree);
    while(ptr != NULL)
    {
        printf("%d ", ptr->data);
        if(ptr->rightThread)
        {
            ptr = ptr->right;
        }
        else
        {
            ptr = dtt_leftMost(ptr->right);
        }
    }
}

struct node *dtt_delete_tree(struct node *tree)
{
    struct node *ptr = dtt_leftMost(tree);
    struct node *prev;
    while(ptr != NULL)
    {
        prev = ptr;
        if(ptr->rightThread)
        {
            ptr = ptr->right;
        }
        else
        {
            ptr = dtt_leftMost(ptr->right);
        }
        free(prev);
    }
    tree = NULL;
    return tree;
};

int dtt_height(struct node *node)
{
    if(node == NULL)
    {
        return 0;
    }
    else
    {
        int lheight;
        int rheight;

        if(node->leftThread)
        {
            lheight = 0;
        }
        else
        {
            lheight = dtt_height(node->left);
        }

        if(node->rightThread)
        {
            rheight = 0;
        }
        else
        {
            rheight = dtt_height(node->right);
        }

        if(lheight > rheight)
        {
            return (lheight + 1);
        }
        else
        {
            return (rheight + 1);
        }
    }
}

void write_current_level_dtt_nodes(struct node *tree, int level, FILE *dotFile)
{
    if(tree == NULL)
    {
        return;
    }
    if(level == 1)
    {
        // Write the current node
        if (tree->left && tree->right)
        {
            fprintf(dotFile, "\t%d [label=\"<left> |<data> %d |<right> \"];\n", tree->data, tree->data);
        }
        else if (tree->left && !tree->right)
        {
            fprintf(dotFile, "\t%d [label=\"<left> |<data> %d |<right> X\"];\n", tree->data, tree->data);
        }
        else if (!tree->left && tree->right)
        {
            fprintf(dotFile, "\t%d [label=\"<left> X|<data> %d |<right> \"];\n", tree->data, tree->data);
        }
        else
        {
            fprintf(dotFile, "\t%d [label=\"<left> X|<data> %d |<right> X\"];\n", tree->data, tree->data);
        }
    }
    else if(level > 1)
    {
        write_current_level_dtt_nodes(tree->left, level - 1, dotFile);
        write_current_level_dtt_nodes(tree->right, level - 1, dotFile);
    }
}

void generate_dtt_nodes(struct node *tree, FILE *dotFile)
{
    int h = dtt_height(tree);
    int i;
    for(i = 1; i <= h; i++)
    {
        write_current_level_dtt_nodes(tree, i, dotFile);
    }
}

void print_dtt_helper(struct node *tree, FILE *dotFile, int *null_id)
{
    // Recursively using Preorder traversal write the DOT representation of each node in the tree
    struct node *ptr = dtt_leftMost(tree);
    while(ptr != NULL)
    {
        if(ptr->left)
        {
            fprintf(dotFile, "\t%d:left -> %d:data;\n", ptr->data, ptr->left->data);
        }
        else
        {
            (*null_id)++;
            fprintf(dotFile, "\tNULL%d [shape=point style=invis];\n", *null_id);
            fprintf(dotFile, "\t%d:left -> NULL%d [style=invis];\n", ptr->data, *null_id);
        }

        if(ptr->right)
        {
            fprintf(dotFile, "\t%d:right -> %d:data;\n", ptr->data, ptr->right->data);
        }
        else
        {
            (*null_id)++;
            fprintf(dotFile, "\tNULL%d [shape=point style=invis];\n", *null_id);
            fprintf(dotFile, "\t%d:right -> NULL%d [style=invis];\n", ptr->data, *null_id);
        }

        if(ptr->rightThread)
        {
            ptr = ptr->right;
        }
        else
        {
            ptr = dtt_leftMost(ptr->right);
        }
    }
}

void print_dtt(struct node *tree)
{
    static int null_id;

    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph LTBT{\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=record, height=0.5, width=1.5];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d [label=\"<left> X|<data> %d |<right> X\"];\n", tree->data, tree->data);
    }
    else
    {
        null_id = 0;
        generate_dtt_nodes(tree, dotFile);
        print_dtt_helper(tree, dotFile, &null_id);
    }

    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);

    view_diagram();
}

void save_dtt(struct node *tree)
{
    static int null_id;
    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph RTBT{\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=record, height=0.5, width=1.5];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d [label=\"<left> X|<data> %d |<right> X\"];\n", tree->data, tree->data);
    }
    else
    {
        null_id = 0;
        generate_dtt_nodes(tree, dotFile);
        print_dtt_helper(tree, dotFile, &null_id);
    }
    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);

    save_diagram();
}


void start_double_threaded_bst_program()
{
    int option, val;
    struct node *tree = NULL;
    struct node *ptr;

    do
    {
        clearScreen();
        printf("\n ***** Left-Threaded BST Visualization Menu *****\n");
        printf("\n 1.  Insert node");
        printf("\n 2.  Search node");
        printf("\n 3.  Delete node");
        printf("\n 4.  Find Height of the tree");
        printf("\n 5.  Find total number of nodes");
        printf("\n 6.  Find total number of external nodes");
        printf("\n 7.  Find total number of internal nodes");
        printf("\n 8.  Inorder traversal");
        printf("\n 9.  View tree Image");
        printf("\n 10. Save tree Image");
        printf("\n 11. Delete tree");
        printf("\n 12. Return To Main Menu\n");
        printf("\n\n Select option (1-13): ");
        scanf(" %d", &option);
        switch(option)
        {
        case 1:
            printf("\n Enter element to insert: ");
            scanf(" %d", &val);
            tree = dtt_insert(tree, val);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 2:
            printf("\n Enter the value to search for: ");
            scanf(" %d", &val);
            ptr = dtt_search(tree, val);
            if(ptr == NULL)
            {
                printf("\n Element doesn't exist in the BST");
            }
            else
            {
                printf("\n Element is found");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n'); // wait for user to press enter
            break;
        case 3:
            printf("\n Enter element to delete: ");
            scanf(" %d", &val);
            if(tree == NULL)
            {
                printf("\n Tree is empty");
            }
            else
            {
                tree = delete_dtt_node(tree, val);
            }
            printf("\n Element have been deleted");
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 4:
            val = dtt_height(tree);
            if(val)
            {
                printf("\n Tree height is: %d", val);
            }
            else
            {
                printf("\n Tree is empty hence its height is 0");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 5:
            val = dtt_total_nodes(tree);
            if(val)
            {
                printf("\n Tree has total of %d nodes", val);
            }
            else
            {
                printf("\n Tree is empty hence has 0 nodes");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 6:
            val = dtt_total_external_nodes(tree);
            if(val)
            {
                printf("\n Tree has total of %d external nodes", val);
            }
            else
            {
                printf("\n Tree is empty hence has 0 external nodes");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 7:
            val = dtt_total_internal_nodes(tree);
            if(val)
            {
                printf("\n Tree has total of %d internal nodes", val);
            }
            else
            {
                printf("\n Tree is empty or has single node hence has 0 internal nodes");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 8:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in Inorder traversal: ");
                dtt_inorder_traversal(tree);
                printf("\n ");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 9:
            printf("\n Opening image with the default image viewer");
            print_dtt(tree);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 10:
            save_dtt(tree);
            printf("\n\n Press enter to continue...");
            while (getchar() != '\n');
            break;
        case 11:
            tree = dtt_delete_tree(tree);
            if(tree == NULL)
            {
                printf("\n Tree has been deleted successfully");
            }
            else
            {
                printf("\n Unable to delete tree for some reason");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        default:
            printf("\n Returning to threaded tree menu...\n");
            option = 12;
            break;
        }
    }
    while(option != 12);
}
