#include <iostream>
#include <stdlib.h>
#include <math.h>

typedef int EType;

typedef struct tnode
{
    EType data;
    struct tnode* left;
    struct tnode* right;
    int heightTree;
} TNode;

typedef struct tnode* TREE;

typedef struct node
{
    EType data;          // Store the keystroke by the user
    struct node *next;  // Pointer to the next node
} Node;

typedef struct
{
    Node *front;        // Front of the Queue
    Node *rear;         // Back of the Queue
} Queue;

void Init(Queue *queue);
int Enqueue(Queue *queue, EType data);
int Dequeue(Queue *queue);
int IsEmpty(Queue *queue);

bool create_matrix(int*** matrix,int size);
void insert_element(int*** matrix,int size);

bool check(int **matrix,int size);
int m(int** matrix,int size);
int create_tree(TREE* tree,int root);
void Binary_Tree(TREE tree,int **matrix,int size,int root);
void zero(int*** matrix,int size,int col);
TNode* insert_left(TREE tree,EType data, int root);
TNode* insert_right(TREE tree,EType data, int root);

int Balanced(TREE tree);
void save_inorder(TREE tree,Queue* queue);

TNode* AVL(TREE tree,int data);
int Height(TREE tree);
TNode* Rotate_Right(TREE tree);
TNode* Rotate_Left(TREE tree);
TNode* Right_Right(TREE tree);
TNode* Left_Left(TREE tree);
TNode* Left_Right(TREE tree);
TNode* Right_Left(TREE tree);
int Balanced_AVL(TREE tree);

void preorder(TREE tree);
void postorder(TREE tree);
void inorder(TREE tree);

using namespace std;

int main()
{
    TREE tree ;
    int **matrix;
    int size,root;
    Queue queue;

    Init(&queue);

    cout<<"Enter size of square matrix for adjacency matrix: ";
    cin>>size;

    if(!create_matrix(&matrix,size))
        return 1;

    cout<<endl;

    insert_element(&matrix,size);

    if(check(matrix,size)&&m(matrix,size)>=2)
        cout<<"\nThe adjacency matrix represent unrooted tree m = "<<m(matrix,size)<<endl;
    else
    {
        cout<<"\nThe adjacency matrix not represent unrooted tree"<<endl;
        return 1;
    }

    cout<<"\nChoose a node as the root: ";
    cin>>root;

    create_tree(&tree,root);
    Binary_Tree(tree,matrix,size,root-1);

    cout<<"\nBinary Tree Inorder Traverse: ";
    inorder(tree);
    cout<<endl;
    cout<<"Binary Tree Postorder Traverse: ";
    postorder(tree);
    cout<<endl;
    cout<<"Binary Tree Preorder Traverse: ";
    preorder(tree);
    cout<<endl;

    if(Balanced(tree))
    {
        cout<<"\nTree is not balanced\n"<<endl;

        save_inorder(tree,&queue);

        tree=NULL;

        for(int i=0; i<size; i++)
            tree=AVL(tree,Dequeue(&queue));

        cout<<"AVL Inorder Traverse: ";
        inorder(tree);
        cout<<endl;
        cout<<"AVL Postorder Traverse: ";
        postorder(tree);
        cout<<endl;
        cout<<"AVL Preorder Traverse: ";
        preorder(tree);
    }
    else
        cout<<"Tree is balanced"<<endl;

    for(int i=0; i<size; i++)
        free(matrix[i]);
    free(matrix);

    cout<<endl;

    return 0;
}

/**
 * Initialization of queue
 */
void Init(Queue *queue)
{
    queue->rear = NULL;
    queue->front = NULL;
}

/**
 * Add items in the queue
 *
 * @param queue is queue to which it will be added
 * @param data  is Value to be added
 * @return Successful or failed addition
 */
int Enqueue(Queue *queue, EType data)
{
    Node *P;
    if ((P=(Node *)malloc(sizeof(Node))) == NULL)
        return 0;
    P->data = data;
    P->next = NULL;
// Add first node to the Queue
    if (queue->rear==NULL)
    {
        queue->front=P;
        queue->rear =P;
    }
    else
    {
        queue->rear->next=P;
        queue->rear = P;
    }

    return 1;
}

/**
 *  Delete an item from the queue
 *
 * @param queue is the queue to delete an item from it
 * @return The value of the deleted item
 */
int Dequeue(Queue *queue)
{
    int data;
    Node *P;
    if(IsEmpty(queue))
    {
        return 0;
    }
    P = queue->front;
    queue->front=P->next;
    data = P->data;
    // Check if removing the last node from the Queue
    if (queue->front== NULL)
        queue->rear = NULL;
    free(P);
    return data;
}

/**
 * Check if the queue is empty or not
 */
int IsEmpty(Queue *queue)
{
    return (queue->front == NULL && queue->rear==NULL);
}

/**
 * Create a adjacency matrix
 *
 * @param matrix is The adjacency matrix that will be created
 * @param size   is matrix size
 * @return Success or failure to create the matrix
 */
bool create_matrix(int*** matrix,int size)
{
    *matrix=(int **)malloc ((size)*sizeof(int*));
    if(*matrix==NULL)
    {
        cout<<"Out of memory";
        return false;
    }
    for (int i=0; i<size; i++)
    {
        (*matrix)[i]=(int *)malloc (size*sizeof(int));
        if((*matrix)[i]==NULL)
        {
            cout<<"Out of memory";
            return false;
        }
    }
    return true;
}

/**
 * Add items to the adjacency matrix
 */
void insert_element(int*** matrix,int size)
{
    cout<<"Enter elements for adjacency matrix"<<endl;

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            cin>>(*matrix)[i][j];
}

/**
 * Check if the adjacency matrix represents unrooted tree
 *
 * @return Whether the matrix represents an unrooted tree or not
 */
bool check(int **matrix,int size)
{
    int count =0;
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            if(matrix[i][i]==0)
            {
                if(matrix[i][j]>1)
                    return false;
                else if(matrix[i][j]==1)
                {
                    if(matrix[i][j]==matrix[j][i])
                        count++;
                    else
                        return false;
                }
            }
            else
                return false;
        }
    }
    if(size-1==count/2)
        return true;
    else
        return false;
}

/**
 * Find a m value for the tree
 *
 * @return The value of the m
 */
int m(int** matrix,int size)
{
    int sum,m=0;
    for(int i=0; i<size; i++)
    {
        sum=0;
        for(int j=0; j<size; j++)
        {
            sum+=matrix[i][j];
        }
        if(sum>m)
            m=sum;
    }
    return m;
}

/**
 * Create a tree
 *
 * @param tree is The tree to be created
 * @param root is Tree start value
 * @return Success or failure of tree creation
 */
int create_tree(TREE* tree,int root)
{
    TNode* tmp;
    tmp = (TNode*) malloc(sizeof(TNode));
    if (tmp == NULL)
    {
        cout<<"Out of space!\n";
        return 1;
    }
    tmp->left=tmp->right=NULL;
    tmp->data=root;

    *tree=tmp;

    return 0;
}

/**
 * Turn the matrix into a binary tree.
 *
 * @param tree    is The tree to be formed from the matrix
 * @param matrix  is The matrix from which the tree will be formed.
 * @param size    is Matrix size
 * @param root    is The previous node in the tree.
 */
void Binary_Tree (TREE tree,int **matrix,int size, int root)
{
    zero(&matrix,size,root);
    int x=0;
    for(int i=0; i<size; i++)
    {
        if(matrix[root][i]==1 && x==0)
        {
            x=i+1;
            insert_left(tree,x,root+1);
            Binary_Tree(tree,matrix,size,i);
        }
        else if(matrix[root][i]==1 && x!=0)
        {
            insert_right(tree,i+1,x);
            x=i+1;
            Binary_Tree(tree,matrix,size,i);
        }
    }
}

/**
 * Make column values equal to zero
 *
 * @param matrix is the matrix that's going to make the values of one of its ranks equal to zero
 * @param size   is Matrix size
 * @param col    is The column number that will make the values of its elements zero
 */
void zero(int*** matrix,int size,int col)
{
    for(int i=0; i<size; i++)
        (*matrix)[i][col]=0;
}

/**
 * Add a new node to the left of the previous node
 *
 * @param tree  is The tree where the node will be added
 * @param data  is New node value
 * @param root  is The previous node to be searched
 * @return Tree after adding node
 */
TNode* insert_left(TREE tree,EType data, int root)
{
    TNode* tmp;
    tmp = (TNode*) malloc(sizeof(TNode));
    if (tmp == NULL)
    {
        cout<<"Out of space!\n";
        return (tree);
    }
    tmp->left=tmp->right=NULL;
    tmp->data=data;

    if(tree==NULL)
        tree=tmp;

    //This part of the function searches for a node in a way similar to the first depth algorithm Then add the node to the left
    else if(tree->data==root)
        tree->left=tmp;
    else if(tree->right!=NULL && tree->left!=NULL)
    {
        tree->left=insert_left(tree->left,data,root);
        tree->right=insert_left(tree->right,data,root);
    }
    else if(tree->left!=NULL)
        tree->left=insert_left(tree->left,data,root);
    else if(tree->right!=NULL)
        tree->right=insert_left(tree->right,data,root);

    return tree;
}

/**
 * Add a new node to the right of the previous node
 *
 * @param tree  is The tree where the node will be added
 * @param data  is New node value
 * @param root  is The previous node to be searched
 * @return Tree after adding node
 */
TNode* insert_right(TREE tree,EType data,int root)
{
    TNode* tmp;
    tmp = (TNode*) malloc(sizeof(TNode));
    if (tmp == NULL)
    {
        cout<<"Out of space!\n";
        return (tree);
    }
    tmp->left=tmp->right=NULL;
    tmp->data=data;

    if(tree==NULL)
        tree=tmp;

    //This part of the function searches for a node in a way similar to the first depth algorithm Then add the node to the right
    else if(tree->data==root)
        tree->right=tmp;
    else if(tree->right!=NULL && tree->left!=NULL)
    {
        tree->right=insert_right(tree->right,data,root);
        tree->left=insert_right(tree->left,data,root);
    }
    else if(tree->right!=NULL)
        tree->right=insert_right(tree->right,data,root);
    else if(tree->left!=NULL)
        tree->left=insert_right(tree->left,data,root);

    return tree;
}

/**
 * Check if the binary tree is balanced or not
 *
 * @return Balanced or not.
 */
int Balanced(TREE tree)
{
    if(tree==NULL)
        return 0;
    int left_h=Balanced(tree->left);
    if (left_h==-1)
        return -1;
    int right_h=Balanced(tree->right);
    if (right_h==-1)
        return -1;
    if(abs(left_h-right_h)>1)
        return -1;
    return (1+max(left_h,right_h));
}

/**
 * Save inorder for binary tree
 *
 * @param tree   is The binary tree that will be saved inorder
 * @param queue  is The queue to be saved
 */
void save_inorder(TREE tree,Queue* queue)
{
    if (tree != NULL)
    {
        save_inorder(tree->left,queue);
        if(!Enqueue(queue,tree->data))
            return ;
        save_inorder(tree->right,queue);
    }
}

/**
 * AVL tree configuration
 *
 * @param tree  is The AVL tree that's going to be formed.
 * @param data  is The value to be added to the tree
 * @return tree
 */
TNode* AVL(TREE tree,int data)
{
    if(tree==NULL)
    {
        tree=(TNode*)malloc(sizeof(TNode));
        tree->data=data;
        tree->left=NULL;
        tree->right=NULL;
    }
    else if(data > tree->data)
    {
        tree->right=AVL(tree->right,data);
        if(Balanced_AVL(tree)==-2)
            if(data>tree->right->data)
                tree=Right_Right(tree);
            else
                tree=Right_Left(tree);
    }
    else if(data<tree->data)
    {
        tree->left=AVL(tree->left,data);
        if(Balanced_AVL(tree)==2)
            if(data < tree->left->data)
                tree=Left_Left(tree);
            else
                tree=Left_Right(tree);
    }

    tree->heightTree=Height(tree);

    return(tree);
}

/**
 * Find the height of the tree
 *
 * @return Height
 */
int Height(TREE tree)
{
    int lh,rh;
    if(tree==NULL)
        return(0);

    if(tree->left==NULL)
        lh=0;
    else
        lh=1+tree->left->heightTree;

    if(tree->right==NULL)
        rh=0;
    else
        rh=1+tree->right->heightTree;

    if(lh>rh)
        return(lh);

    return(rh);
}

/**
 * Rotate to the right
 *
 * @return tree
 */
TNode* Rotate_Right(TREE tree)
{
    TNode* y;
    y=tree->left;
    tree->left=y->right;
    y->right=tree;
    tree->heightTree=Height(tree);
    y->heightTree=Height(y);
    return(y);
}

/**
 * Rotate to the left
 *
 * @return tree
 */
TNode* Rotate_Left(TREE tree)
{
    TNode* y;
    y=tree->right;
    tree->right=y->left;
    y->left=tree;
    tree->heightTree=Height(tree);
    y->heightTree=Height(y);

    return(y);
}

/**
 * Rotate to the right twice
 *
 * @return tree
 */
TNode* Right_Right(TREE tree)
{
    tree=Rotate_Left(tree);
    return(tree);
}

/**
 * Rotate to the left twice
 *
 * @return tree
 */
TNode* Left_Left(TREE tree)
{
    tree=Rotate_Right(tree);
    return(tree);
}

/**
 * Rotate to the left and then rotate to the right
 *
 * @return tree
 */
TNode* Left_Right(TREE tree)
{
    tree->left=Rotate_Left(tree->left);
    tree=Rotate_Right(tree);

    return(tree);
}

/**
 * Rotate to the right and then rotate to the left
 *
 * @return tree
 */
TNode* Right_Left(TREE tree)
{
    tree->right=Rotate_Right(tree->right);
    tree=Rotate_Left(tree);
    return(tree);
}

/**
 * Check the balance of the AVL tree
 *
 * @return The tree is balanced or not.
 */
int Balanced_AVL(TREE tree)
{
    int lh,rh;
    if(tree==NULL)
        return(0);

    if(tree->left==NULL)
        lh=0;
    else
        lh=1+tree->left->heightTree;

    if(tree->right==NULL)
        rh=0;
    else
        rh=1+tree->right->heightTree;

    return(lh-rh);
}

/**
 * Find a inorder for the tree
 */
void inorder(TREE tree)
{
    if (tree != NULL)
    {
        inorder(tree->left);
        cout<<tree->data<<"  ";
        inorder(tree->right);
    }
}

/**
 * Find a preorder for the tree
 */
void preorder(TREE tree)
{
    if (tree != NULL)
    {
        cout<<tree->data<<"  ";
        preorder(tree->left);
        preorder(tree->right);
    }
}

/**
 * Find a postorder for the tree
 */
void postorder(TREE tree)
{
    if (tree != NULL)
    {
        postorder(tree->left);
        postorder(tree->right);
        cout<<tree->data<<"  ";
    }
}
