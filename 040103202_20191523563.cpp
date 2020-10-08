//  مهاب محمد نافع زكريا عبد المجيد يوسف
//  20191523563
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

    cout<<"Enter size of square matrix for adjacency matrix:";
    cin>>size;

    if(!create_matrix(&matrix,size))
        return 1;

    insert_element(&matrix,size);

    if(check(matrix,size)&&m(matrix,size)>=2)
        cout<<"The adjacency matrix represent unrooted tree m = "<<m(matrix,size)<<endl;
    else
    {
        cout<<"The adjacency matrix not represent unrooted tree"<<endl;
        return 1;
    }

    cout<<"Choose a node as the root"<<endl;
    cin>>root;

    create_tree(&tree,root);
    Binary_Tree(tree,matrix,size,root-1);

    cout<<"Binary Tree Inorder Traverse: ";
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
        cout<<"Tree is not balanced"<<endl;

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

    return 0;
}

void Init(Queue *queue)
{
    queue->rear = NULL;
    queue->front = NULL;
}

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
int IsEmpty(Queue *queue)
{
    return (queue->front == NULL && queue->rear==NULL);
}

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

void insert_element(int*** matrix,int size)
{
    cout<<"Enter elements for adjacency matrix"<<endl;

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            cin>>(*matrix)[i][j];
}

bool check(int **matrix,int size)
{
    int sum=0,count =0,m;
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

int m(int** matrix,int size){
    int sum,m=0;
    for(int i=0;i<size;i++){
        sum=0;
        for(int j=0;j<size;j++){
            sum+=matrix[i][j];
        }
        if(sum>m)
            m=sum;
    }
    return m;
}

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

void zero(int*** matrix,int size,int col){
    for(int i=0; i<size; i++)
        (*matrix)[i][col]=0;
}

void preorder(TREE tree)
{
    if (tree != NULL)
    {
        cout<<tree->data<<"  ";
        preorder(tree->left);
        preorder(tree->right);
    }
}

void postorder(TREE tree)
{
    if (tree != NULL)
    {
        postorder(tree->left);
        postorder(tree->right);
        cout<<tree->data<<"  ";
    }
}

void inorder(TREE tree)
{
    if (tree != NULL)
    {
        inorder(tree->left);
        cout<<tree->data<<"  ";
        inorder(tree->right);
    }
}

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

TNode* Right_Right(TREE tree)
{
    tree=Rotate_Left(tree);
    return(tree);
}

TNode* Left_Left(TREE tree)
{
    tree=Rotate_Right(tree);
    return(tree);
}

TNode* Left_Right(TREE tree)
{
    tree->left=Rotate_Left(tree->left);
    tree=Rotate_Right(tree);

    return(tree);
}

TNode* Right_Left(TREE tree)
{
    tree->right=Rotate_Right(tree->right);
    tree=Rotate_Left(tree);
    return(tree);
}

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
