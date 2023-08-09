#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "../../headers/utility.h"

struct node
{
    int data;
    struct node *left;
    struct node *right;
    bool rightThread;
};

struct node *rtt_search(struct node *tree, int val)
{
    if(tree == NULL)
    {
        return NULL;
    }
    else if(tree->data == val)
    {
        return tree;
    }
    else
    {
        if(tree->rightThread)
        {
            if(val > tree->data)
            {
                return NULL;
            }
            else
            {
                return rtt_search(tree->left, val);
            }
        }
        else
        {
            if(val > tree->data)
            {
                return rtt_search(tree->right, val);
            }
            else
            {
                return rtt_search(tree->left, val);
            }
        }
    }
};

struct node *rtt_inorderSuccessor(struct node *ptr)
{
    if(ptr->rightThread == true)
    {
        return ptr->right;
    }
    ptr = ptr->right;
    while(ptr->left != NULL)
    {
        ptr = ptr->left;
    }
    return ptr;
};

struct node *rtt_max(struct node *tree)
{
    while(tree->rightThread != true)
    {
        tree = tree->right;
    }
    return tree;
};

struct node *rtt_inorderPred(struct node *tree, struct node *ptr)
{
    if(tree == NULL)
    {
        return NULL;
    }

    struct node *pred = NULL;
    while(tree != NULL)
    {
        if(ptr->data < tree->data)
        {
            tree = tree->left;
        }
        else if(ptr->data > tree->data)
        {
            pred = tree;
            tree = tree->right;
        }
        else
        {
            if(tree->left != NULL)
            {
                pred = rtt_max(tree->left);
            }
            break;
        }
    }

    return pred;
};

struct node *caseA(struct node *tree, struct node *parent, struct node *ptr)
{
    if(parent == NULL)
    {
        tree = NULL;
    }
    else if(ptr == parent->left)
    {
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

struct node *caseB(struct node *tree, struct node *parent, struct node *ptr)
{
    struct node *child;
    struct node *s = rtt_inorderSuccessor(ptr);
    struct node *p = rtt_inorderPred(tree, ptr);

    if(ptr->rightThread == false)
    {
        child = ptr->right;
    }
    else
    {
        child = ptr->left;
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

    if(ptr->left != NULL)
    {
        p->right = s;
    }
    else
    {
        if(ptr->rightThread == false)
        {
            s->left = NULL;
        }
    }
    free(ptr);
    return tree;
};

struct node *caseC(struct node *tree, struct node *parent, struct node *ptr)
{
    struct node *parsucc = ptr;
    struct node *succ = ptr->right;

    // Find left most child of successor
    while(succ->left != NULL)
    {
        parsucc = succ;
        succ = succ->left;
    }

    ptr->data = succ->data;

    if(succ->left == NULL && succ->rightThread == true)
    {
        tree = caseA(tree, parsucc, succ);
    }
    else
    {
        tree = caseB(tree, parsucc, succ);
    }
    return tree;

};


struct node *delete_rtt_node(struct node *tree, int val)
{
    struct node *parent = NULL;
    struct node *ptr = tree;
    bool found = false;

    while(ptr != NULL)
    {
        if(val == ptr->data)
        {
            found = true;
            break;
        }
        parent = ptr;
        if(val < ptr->data)
        {
            if(ptr->left != NULL)
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

    if(found == false)
    {
        printf(" value not present in tree\n");
    }
    else if(ptr->left != NULL && ptr->rightThread == false)
    {
        tree = caseC(tree, parent, ptr);
    }
    else if(ptr->left != NULL && ptr->rightThread == true)
    {
        tree = caseB(tree, parent, ptr);
    }
    else if(ptr->left == NULL && ptr->rightThread == false)
    {
        tree = caseB(tree, parent, ptr);
    }
    else
    {
        tree = caseA(tree, parent, ptr);
    }
    return tree;
}

struct node *rtt_insert(struct node *tree, int val)
{
    struct node *ptr = tree;
    struct node *parent = NULL;

    while(ptr != NULL)
    {
        if(ptr->data == val)
        {
            printf("Duplicate Key !\n");
            return tree;
        }

        parent = ptr;

        if(val > ptr->data)
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
        else
        {
            ptr = ptr->left;
        }
    }

    struct node *new_node;
    new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = val;
    new_node->left = new_node->right = NULL;
    new_node->rightThread = true;

    if(parent == NULL)
    {
        tree = new_node;
        tree->left = NULL;
        tree->right = NULL;
    }
    else if(val < parent->data)
    {
        new_node->right = parent;
        parent->left = new_node;
    }
    else
    {
        new_node->right = parent->right;
        parent->rightThread = false;
        parent->right = new_node;
    }

    return tree;
};

struct node *rtt_leftMost(struct node *n)
{
    if(n == NULL)
    {
        return NULL;
    }
    while(n->left != NULL)
    {
        n = n->left;
    }
    return n;
};

int rtt_height(struct node *node)
{
    if(node == NULL)
    {
        return 0;
    }
    else
    {
        int lheight = rtt_height(node->left);
        int rheight;

        if(node->rightThread)
        {
            rheight = 0;
        }
        else
        {
            rheight = rtt_height(node->right);
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

int rtt_total_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else
    {
        if(tree->rightThread == true)
        {
            return rtt_total_nodes(tree->left) + 1;
        }
        else
        {
            return rtt_total_nodes(tree->left) + rtt_total_nodes(tree->right) + 1;
        }
    }
}

int rtt_total_external_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else if(tree->left == NULL && tree->rightThread == true)
    {
        return 1;
    }
    else
    {
        if(tree->rightThread == true)
        {
            return rtt_total_external_nodes(tree->left);
        }
        else
        {
            return rtt_total_external_nodes(tree->left) + rtt_total_external_nodes(tree->right);
        }
    }
}

int rtt_total_internal_nodes(struct node *tree)
{
    if(tree == NULL)
    {
        return 0;
    }
    else if(tree->left == NULL && tree->rightThread == true)
    {
        return 0;
    }
    else
    {
        if(tree->rightThread == true)
        {
            return rtt_total_internal_nodes(tree->left) + 1;
        }
        else
        {
            return rtt_total_internal_nodes(tree->left) + rtt_total_internal_nodes(tree->right) + 1;
        }
    }
}

void rtt_inorder_traversal(struct node *tree)
{
    struct node *cur = rtt_leftMost(tree);
    while (cur != NULL)
    {
        printf("%d ", cur->data);
        if (cur->rightThread)
        {
            cur = cur->right;
        }
        else
        {
            cur = rtt_leftMost(cur->right);
        }
    }
}

struct node *rtt_delete_tree(struct node *tree)
{
    if(tree != NULL)
    {
        if(tree->rightThread)
        {
            rtt_delete_tree(tree->left);
            free(tree);
        }
        else
        {
            rtt_delete_tree(tree->left);
            rtt_delete_tree(tree->right);
            free(tree);
        }
    }
    return tree;
}

void write_current_level_rtt_nodes(struct node *tree, int level, FILE *dotFile)
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
        write_current_level_rtt_nodes(tree->left, level - 1, dotFile);
        write_current_level_rtt_nodes(tree->right, level - 1, dotFile);
    }
}

void generate_rtt_nodes(struct node *tree, FILE *dotFile)
{
    int h = rtt_height(tree);
    int i;
    for(i = 1; i <= h; i++)
    {
        write_current_level_rtt_nodes(tree, i, dotFile);
    }
}

void print_rtt_helper(struct node *tree, FILE *dotFile, int *null_id)
{
    // Recursively using Preorder traversal write the DOT representation of each node in the tree

    struct node *ptr = rtt_leftMost(tree);
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
            ptr = rtt_leftMost(ptr->right);
        }
    }
}

void print_rtt(struct node *tree)
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
        generate_rtt_nodes(tree, dotFile);
        print_rtt_helper(tree, dotFile, &null_id);
    }

    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);

    view_diagram();
}

void save_rtt(struct node *tree)
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
        generate_rtt_nodes(tree, dotFile);
        print_rtt_helper(tree, dotFile, &null_id);
    }
    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);

    save_diagram();
}

void start_rthreaded_tree_program()
{
    int option, val;
    struct node *tree = NULL;
    struct node *ptr;

    do
    {
        clearScreen();
        printf("\n ***** Right-Threaded BST Visualization Menu *****\n");
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
            scanf( " %d", &val);
            tree = rtt_insert(tree, val);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 2:
            printf("\n Enter the value to search for: ");
            scanf(" %d", &val);
            ptr = rtt_search(tree, val);
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
                tree = delete_rtt_node(tree, val);
            }
            printf("\n Element have been deleted");
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 4:
            val = rtt_height(tree);
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
            val = rtt_total_nodes(tree);
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
            val = rtt_total_external_nodes(tree);
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
            val = rtt_total_internal_nodes(tree);
            if(val)
            {
                printf("\n Tree has total of %d internal nodes", val);
            }
            else
            {
                printf("\n Tree is empty hence has 0 internal nodes");
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
                printf("\n Elements in inorder traversal: ");
                rtt_inorder_traversal(tree);
                printf("\n ");
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 9:
            printf("\n Opening image with the default image viewer");
            print_rtt(tree);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 10:
            save_rtt(tree);
            printf("\n\n Press enter to continue...");
            while (getchar() != '\n');
            break;
        case 11:
            tree = rtt_delete_tree(tree);
            tree = NULL;
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
};
