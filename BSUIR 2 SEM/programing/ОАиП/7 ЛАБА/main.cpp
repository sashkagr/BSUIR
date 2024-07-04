#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* setString() {
    char* str=(char*)malloc(1);
    str[0]='\0';
    int i = 1, c;
    while ((c = getchar()) != EOF && c != '\n') {
        i++;
        str = (char*)realloc(str, i*sizeof(char));
        if (str != NULL) {
            str[i - 2] = c;
            str[i - 1] = '\0';
        }
    }
    return str;
}
TreeNode *createTreeNode(int value, unsigned int frequency)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->value = value;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}
PriorityQueue *createPriorityQueue(int capacity)
{
    PriorityQueue *queue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    queue->nodes = (TreeNode **)malloc(capacity * sizeof(TreeNode *));
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}
void destroyPriorityQueue(PriorityQueue *queue)
{
    free(queue->nodes);
    free(queue);
}
void enqueue(PriorityQueue *queue, TreeNode *node)
{
    if (queue->size == queue->capacity)
    {
        queue->capacity *= 2;
        queue->nodes = (TreeNode **)realloc(queue->nodes, queue->capacity * sizeof(TreeNode *));
    }
    int i = queue->size;
    while (i > 0 && queue->nodes[i - 1]->frequency > node->frequency)
    {
        queue->nodes[i] = queue->nodes[i - 1];
        i--;
    }
    queue->nodes[i] = node;
    queue->size++;
}
TreeNode *dequeue(PriorityQueue *queue)
{
    TreeNode *node = queue->nodes[0];
    queue->size--;
    for (int i = 0; i < queue->size; i++)
    {
        queue->nodes[i] = queue->nodes[i + 1];
    }
    return node;
}
TreeNode *buildHuffmanTree(unsigned int *frequencies)
{
    PriorityQueue *queue = createPriorityQueue(256);
    for (int i = 0; i < 256; i++)
    {
        if (frequencies[i] > 0)
        {
            TreeNode *node = createTreeNode(i, frequencies[i]);
            enqueue(queue, node);
        }
    }

    while (queue->size > 1)
    {
        TreeNode *node1 = dequeue(queue);
        TreeNode *node2 = dequeue(queue);
        TreeNode *parent = createTreeNode(-1, node1->frequency + node2->frequency);
        parent->left = node1;
        parent->right = node2;
        enqueue(queue, parent);
    }
    TreeNode *root = dequeue(queue);
    destroyPriorityQueue(queue);
    return root;
}
void destroyHuffmanTree(TreeNode *root)
{
    if (root != NULL)
    {
        destroyHuffmanTree(root->left);
        destroyHuffmanTree(root->right);
        free(root);
    }
}
void buildSymbolTableRecursive(TreeNode *node, SymbolCode *symbolTable, int symbolTableSize, unsigned char *code, int codeLength)
{
    if (node->left == NULL && node->right == NULL)
    {
        symbolTable[node->value].symbol = node->value;
        symbolTable[node->value].codeLength = codeLength;
        symbolTable[node->value].code = (unsigned char *)malloc(codeLength);
        memcpy(symbolTable[node->value].code, code, codeLength);
    }
    else
    {
        code[codeLength] = 0;
        buildSymbolTableRecursive(node->left, symbolTable, symbolTableSize, code, codeLength + 1);
        code[codeLength] = 1;
        buildSymbolTableRecursive(node->right, symbolTable, symbolTableSize, code, codeLength + 1);
    }
}
SymbolCode *buildSymbolTable(TreeNode *root)
{
    SymbolCode *symbolTable = (SymbolCode *)calloc(256, sizeof(SymbolCode));
    unsigned char code[256];
    buildSymbolTableRecursive(root, symbolTable, 256, code, 0);
    return symbolTable;
}
void destroySymbolTable(SymbolCode *symbolTable, int symbolTableSize)
{
    for (int i = 0; i < symbolTableSize; i++)
    {
        if (symbolTable[i].code != NULL)
        {
            free(symbolTable[i].code);
        }
    }
    free(symbolTable);
}
unsigned char *encodeMessage(const char *message, int messageLength, SymbolCode *symbolTable, int *encodedLength)
{

    unsigned char *buffer = (unsigned char *)calloc(messageLength * 8+1,1);
    int bufferPosition = 0;
    for (int i = 0; i < messageLength; i++)
    {
        SymbolCode symbolCode = symbolTable[(unsigned char)message[i]];
        for (int j = 0; j < symbolCode.codeLength; j++)
        {
            if (symbolCode.code[j] == 0)
            {
                buffer[bufferPosition / 8] &= ~(1 << (bufferPosition % 8));
            }
            else
            {
                buffer[bufferPosition / 8] |= 1 << (bufferPosition % 8);
            }
            bufferPosition++;
        }
    }
    *encodedLength = bufferPosition;
    return buffer;
}
char *decodeMessage(unsigned char *buffer, int bufferLength, TreeNode *root, int messageLength)
{
    char *message = (char *)malloc(messageLength + 1);
    int messagePosition = 0;
    TreeNode *currentNode = root;
    for (int i = 0; i < bufferLength; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (buffer[i] & (1 << j))
            {
                currentNode = currentNode->right;
            }
            else
            {
                currentNode = currentNode->left;
            }
            if (currentNode->left == NULL && currentNode->right == NULL)
            {
                message[messagePosition] = currentNode->value;
                messagePosition++;
                currentNode = root;
            }
            if (messagePosition == messageLength)
            {
                message[messageLength] = '\0';
                return message;
            }
        }
    }
    free(message);
    return NULL;
}

void printHuffmanTreeRecursive(TreeNode *node, int depth)
{
    if (node != NULL)
    {
        printHuffmanTreeRecursive(node->right, depth + 1);
        for (int i = 0; i < depth; i++)
        {
            printf(" ");
        }
        if (node->left == NULL && node->right == NULL)
        {
            printf("%c (%d)\n", node->value, node->frequency);
        }
        else
        {
            printf("* (%d)\n", node->frequency);
        }
        printHuffmanTreeRecursive(node->left, depth + 1);
    }
}
void printSymbolTable(SymbolCode *symbolTable, int symbolTableSize)
{
    for (int i = 0; i < symbolTableSize; i++)
    {
        if (symbolTable[i].code != NULL)
        {
            printf("%c: ", symbolTable[i].symbol);
            for (int j = 0; j < symbolTable[i].codeLength; j++)
            {
                printf("%d", symbolTable[i].code[j]);
            }
            printf("\n");
        }
    }
}

int main()
{
   // char *message = (char*)calloc(1, 1);
    printf("Enter string:   ");
    char *message = setString();
    //const char *message = "afabcdeabcaadea";
    int messageLength = strlen(message);
    unsigned int frequencies[256] = {0};
    for (int i = 0; i < messageLength; i++)
    {
        frequencies[(unsigned char)message[i]]++;
    }

    TreeNode *root = buildHuffmanTree(frequencies);
    SymbolCode *symbolTable = buildSymbolTable(root);
    int encodedLength = 0;
    unsigned char *encodedMessage = encodeMessage(message, messageLength, symbolTable, &encodedLength);
    char *decodedMessage = decodeMessage(encodedMessage, encodedLength, root, messageLength);

    printf("Original message: %s\n", message);

    printf("Encoded message: ");
    for (int i = 0; i < encodedLength; i++)
    {
        printf("%u ", encodedMessage[i]);
    }
    printf("\n");
    printf("Encoded message in bits: ");
    for (int i = 0; i < encodedLength; i++) {
        if (encodedMessage[i / 8] & (1 << (i % 8))) {
            printf("1");
        } else {
            printf("0");
        }
        if((i+1)%4==0)
            printf(" ");
    }
    printf("\n");

    printf("\nDecoded message: %s\n", decodedMessage);
    printf("Symbol table:\n");
    printSymbolTable(symbolTable, 256);
    printf("Huffman tree:\n");
    printHuffmanTreeRecursive(root,0);

    free(root);
    destroySymbolTable(symbolTable,256);
    free(encodedMessage);
    free(decodedMessage);
    system("pause");
    return 0;

}