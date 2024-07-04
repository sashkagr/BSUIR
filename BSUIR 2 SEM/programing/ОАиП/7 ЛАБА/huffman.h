#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct TreeNode
{
    int value;
    unsigned int frequency;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;
typedef struct PriorityQueue
{
    TreeNode **nodes;
    int size;
    int capacity;
} PriorityQueue;
typedef struct SymbolCode
{
    int symbol;
    int codeLength;
    unsigned char *code;
} SymbolCode;

TreeNode *createTreeNode(int value, unsigned int frequency);


PriorityQueue *createPriorityQueue(int capacity);
void destroyPriorityQueue(PriorityQueue *queue);
void enqueue(PriorityQueue *queue, TreeNode *node);
TreeNode *dequeue(PriorityQueue *queue);

TreeNode *buildHuffmanTree(unsigned int *frequencies);
void destroyHuffmanTree(TreeNode *root);

SymbolCode *buildSymbolTable(TreeNode *root);
void destroySymbolTable(SymbolCode *symbolTable, int symbolTableSize);

#endif