#include <stdio.h>
#include <stdlib.h>

#include "utility.h"

struct node
{
    int data;
    struct node *left;
    struct node *right;
    struct node *parent;
    char color;
};

struct node *leftRotate_redBlackTree(struct node *root, struct node *temp)
{
    struct node* right = temp->right;
    temp->right = right->left;
    if (temp->right)
        temp->right->parent = temp;
    right->parent = temp->parent;
    if (!temp->parent)
        root = right;
    else if (temp == temp->parent->left)
        temp->parent->left = right;
    else
        temp->parent->right = right;
    right->left = temp;
    temp->parent = right;
    return root;
}

struct node *rightRotate_redBlackTree(struct node *root, struct node *temp)
{
    struct node* left = temp->left;
    temp->left = left->right;
    if (temp->left)
        temp->left->parent = temp;
    left->parent = temp->parent;
    if (!temp->parent)
        root = left;
    else if (temp == temp->parent->left)
        temp->parent->left = left;
    else
        temp->parent->right = left;
    left->right = temp;
    temp->parent = left;
    return root;
};

struct node *insertFixUp_redBlackTree(struct node *root, struct node *pt)
{
    struct node* parent_pt = NULL;
    struct node* grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != 'B')
            && (pt->parent->color == 'R'))
    {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
             Parent of pt is left child
             of Grand-parent of
           pt */
        if (parent_pt == grand_parent_pt->left)
        {

            struct node* uncle_pt = grand_parent_pt->right;

            /* Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == 'R')
            {
                grand_parent_pt->color = 'R';
                parent_pt->color = 'B';
                uncle_pt->color = 'B';
                pt = grand_parent_pt;
            }

            else
            {

                /* Case : 2
                     pt is right child of its parent
                     Left-rotation required */
                if (pt == parent_pt->right)
                {
                    root = leftRotate_redBlackTree(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                     pt is left child of its parent
                     Right-rotation required */
                root = rightRotate_redBlackTree(root, grand_parent_pt);
                char t = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = t;
                pt = parent_pt;
            }
        }

        /* Case : B
             Parent of pt is right
             child of Grand-parent of
           pt */
        else
        {
            struct node* uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == 'R'))
            {
                grand_parent_pt->color = 'R';
                parent_pt->color = 'B';
                uncle_pt->color = 'B';
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left)
                {
                    root = rightRotate_redBlackTree(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                     pt is right child of its parent
                     Left-rotation required */
                root = leftRotate_redBlackTree(root, grand_parent_pt);
                char t = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = t;
                pt = parent_pt;
            }
        }
    }
    return root;
};

struct node *insert_redBlackTree(struct node *tree, struct node *new_node)
{
// If the tree is empty,
    // return a new node
    if (tree == NULL)
    {
        return new_node;
    }
    // Otherwise recur down the tree
    if (new_node->data < tree->data)
    {
        tree->left = insert_redBlackTree(tree->left, new_node);
        tree->left->parent = tree;
    }
    else if (new_node->data > tree->data)
    {
        tree->right = insert_redBlackTree(tree->right, new_node);
        tree->right->parent = tree;
    }
    else
    {
        printf("\n Duplicate Key!");
        return tree;
    }

    // Return the (unchanged) node pointer
    return tree;
}

struct node *delete_node_redBlackTree(struct node *tree, int val)
{
    printf("Feature not implemented yet");
    return tree;
};

struct node *search_redBlackTree(struct node *tree, int val)
{
    struct node *ptr = tree;
    while(ptr != NULL)
    {
        if(val == ptr->data)
        {
            return ptr;
        }
        else if(val < ptr->data)
        {
            ptr = ptr->left;
        }
        else
        {
            ptr = ptr->right;
        }
    }
    return NULL;
};

void preorder_redBlackTree(struct node *tree)
{
    if(tree != NULL)
    {
        printf(" %d", tree->data);
        preorder_redBlackTree(tree->left);
        preorder_redBlackTree(tree->right);
    }
}

void inorder_redBlackTree(struct node *tree)
{
    if(tree != NULL)
    {
        inorder_redBlackTree(tree->left);
        printf(" %d", tree->data);
        inorder_redBlackTree(tree->right);
    }
}

void postorder_redBlackTree(struct node *tree)
{
    if(tree != NULL)
    {
        postorder_redBlackTree(tree->left);
        postorder_redBlackTree(tree->right);
        printf(" %d", tree->data);
    }
}

struct node *delete_redBlackTree(struct node *tree)
{
    if(tree != NULL)
    {
        delete_redBlackTree(tree->left);
        delete_redBlackTree(tree->right);
        free(tree);
    }
    return tree;
};

void print_redBlackTree_helper(struct node *tree, FILE *dotFile, int *null_id)
{
    // Recursively using preorder traversal write the DOT representation of each node in the tree
    if(tree != NULL)
    {
        if(tree->left)
        {
            if(tree->color == 'R')
            {
                fprintf(dotFile, "\t%d [color=red, style=filled, fillcolor=tomato];\n", tree->data);
            }
            else
            {
                fprintf(dotFile, "\t%d [color = black, style=filled, fillcolor = gray33];\n", tree->data);
            }
            if(tree->left->color == 'R')
            {
                fprintf(dotFile, "\t%d [color=red, style=filled, fillcolor=tomato];\n", tree->left->data);
            }
            else
            {
                fprintf(dotFile, "\t%d [color = black, style=filled, fillcolor = gray33];\n", tree->left->data);
            }
            fprintf(dotFile, "\t%d -> %d;\n", tree->data, tree->left->data);
        }
        else
        {
            (*null_id)++;
            fprintf(dotFile, "\tNULL%d [label=\"NULL\" shape=rectangle, color = black, style = filled, fillcolor = gray33];\n", *null_id);
            fprintf(dotFile, "\t%d -> NULL%d;\n", tree->data, *null_id);
        }
        if(tree->right)
        {
            if(tree->color == 'R')
            {
                fprintf(dotFile, "\t%d [color = red, style=filled, fillcolor = tomato];\n", tree->data);
            }
            else
            {
                fprintf(dotFile, "\t%d [color = black, style = filled, fillcolor = gray33];\n", tree->data);
            }
            if(tree->right->color == 'R')
            {
                fprintf(dotFile, "\t%d [color = red, style=filled, fillcolor = tomato];\n", tree->right->data);
            }
            else
            {
                fprintf(dotFile, "\t%d [color = black, style = filled, fillcolor = gray33];\n", tree->right->data);
            }
            fprintf(dotFile, "\t%d -> %d;\n", tree->data, tree->right->data);
        }
        else
        {
            (*null_id)++;
            fprintf(dotFile, "\tNULL%d [label=\"NULL\" shape=rectangle, color = black, style = filled, fillcolor = gray33];\n", *null_id);
            fprintf(dotFile, "\t%d -> NULL%d;\n", tree->data, *null_id);
        }
        print_redBlackTree_helper(tree->left, dotFile, null_id);
        print_redBlackTree_helper(tree->right, dotFile, null_id);
    }
}

void print_redBlackTree(struct node *tree)
{

    static int null_id;

    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph BST {\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=circle, fontcolor=white];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d [color = black, style = filled, fillcolor = gray33];\n", tree->data);
    }
    else
    {
        null_id = 0;
        print_redBlackTree_helper(tree, dotFile, &null_id);
    }

    // Write the DOT file footer
    fprintf(dotFile, "}\n");

    // Close the file
    fclose(dotFile);
    view_diagram();
}

void save_redBlackTree(struct node *tree)
{
    static int null_id;
    FILE *dotFile = fopen("diagram.dot", "w"); // Open a file to write the DOT representation of the tree
    if (dotFile == NULL)
    {
        printf("Error opening file.");
        return;
    }

    // Write the DOT file header
    fprintf(dotFile, "digraph BST {\n");
    fprintf(dotFile, "\tnode [fontname=\"Arial\", shape=circle, fontcolor=white];\n");

    //Check if the tree is empty
    if(!tree)
    {
        fprintf(dotFile, "\n");
    }
    else if(!tree->right && !tree->left)
    {
        fprintf(dotFile, "\t%d [color = black, style = filled, fillcolor = gray33];\n", tree->data);
    }
    else
    {
        null_id = 0;
        print_redBlackTree_helper(tree, dotFile, &null_id);
    }
    // Write the DOT file footer
    fprintf(dotFile, "}\n");
    // Close the file
    fclose(dotFile);
    save_diagram();
}

void start_redBlackTree_program()
{
    int option, val;
    struct node *tree = NULL;
    struct node *ptr;
    do
    {
        clearScreen();
        printf("\n ***** Red Black Tree Visualization Menu *****\n");
        printf("\n 1. Insert node");
        printf("\n 2. Delete node");
        printf("\n 3. Preorder traversal");
        printf("\n 4. Inorder traversal");
        printf("\n 5. Postorder traversal");
        printf("\n 6. View tree Image");
        printf("\n 7. Save tree Image");
        printf("\n 8. Delete tree");
        printf("\n 9. EXIT");
        printf("\n\n Enter option: ");
        scanf(" %d", &option);
        switch(option)
        {
        case 1:
            printf("\n Enter the data of the node: ");
            scanf(" %d", &val);
            ptr = search_redBlackTree(tree, val);
            if(!ptr)
            {
                struct node *new_node = (struct node *)malloc(sizeof(struct node));
                new_node->data = val;
                new_node->left = new_node->right = new_node->parent = NULL;
                new_node->color = 'R';
                tree = insert_redBlackTree(tree, new_node);
                tree = insertFixUp_redBlackTree(tree, new_node);
                tree->color = 'B';
            }
            else
            {
                printf("\n Duplicate Value!");
                printf("\n\n Press enter to continue...");
                getchar();
                while (getchar() != '\n');
            }
            break;
        case 2:
            printf("\n Enter element to delete: ");
            scanf(" %d", &val);
            if(tree == NULL)
            {
                printf("\n Tree is empty");
            }
            else
            {
                printf("\n Feature not implemented yet!");
                //tree = delete_node_redBlackTree(tree, val);
            }
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 3:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in preorder traversal: ");
                preorder_redBlackTree(tree);
                printf("\n");
            }
            break;
        case 4:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in inorder traversal: ");
                inorder_redBlackTree(tree);
                printf("\n");
            }
            break;
        case 5:
            if(tree == NULL)
            {
                printf("\n The tree is empty");
            }
            else
            {
                printf("\n Elements in postorder traversal: ");
                postorder_redBlackTree(tree);
                printf("\n");
            }
            break;
        case 6:
            printf("\n Opening image with the default image viewer");
            print_redBlackTree(tree);
            printf("\n\n Press enter to continue...");
            getchar();
            while (getchar() != '\n');
            break;
        case 7:
            save_redBlackTree(tree);
            printf("\n\n Press enter to continue...");
            while (getchar() != '\n');
            break;
        case 8:
            tree = delete_redBlackTree(tree);
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
            option = 9;
            break;
        }
    }
    while(option != 9);
}
