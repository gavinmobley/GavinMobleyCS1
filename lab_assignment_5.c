#include <stdio.h>
#include <stdlib.h>

typedef struct node { // makes a struct called node
    char letter; // struct has a value called letter that can
    struct node* next;
} node;

// Returns number of nodes in the linkedList.
int length(node* head)
{
    int count = 0; // count at 0
    node* current = head;//curent now points to head
    while (current != NULL) {
        count++;// count incremments by 1
        current = current->next;// current is now moved to the next node
    }
    return count; // count tells us the number of nodes
}

// Parses the string in the linkedList.
// If the linked list is head -> |a|->|b|->|c|,
// then toCString function will return "abc".
char* toCString(node* head)
{
    int len = length(head);
    char* str = (char*)malloc((len + 1) * sizeof(char)); // +1 for null terminator
    if (str == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    node* current = head;
    int i = 0;
    while (current != NULL) {
        str[i++] = current->letter;
        current = current->next;
    }
    str[i] = '\0'; // Null terminator
    return str;
}

// Inserts character to the linkedlist.
// If the linked list is head -> |a|->|b|->|c|,
// then insertChar(&head, 'x') will update the linked list as follows:
// head -> |a|->|b|->|c|->|x|.
void insertChar(node** pHead, char c)
{
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->letter = c;
    newNode->next = NULL;

    if (*pHead == NULL) {
        *pHead = newNode;
    } else {
        node* current = *pHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Deletes all nodes in the linkedList.
void deleteList(node** pHead)
{
    node* current = *pHead;
    node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *pHead = NULL; // Update the head pointer
}

int main(void)
{
    int i, strLen, numInputs;
    node* head = NULL;
    char* str;
    char c;
    FILE* inFile = fopen("input.txt","r");

    fscanf(inFile, " %d\n", &numInputs);
    
    while (numInputs-- > 0)
    {
        fscanf(inFile, " %d\n", &strLen);
        for (i = 0; i < strLen; i++)
        {
            fscanf(inFile," %c", &c);
            insertChar(&head, c);
        }

        str = toCString(head);
        printf("String is : %s\n", str);

        free(str);
        deleteList(&head);

        if (head != NULL)
        {
            printf("deleteList is not correct!");
            break;
        }
    }

    fclose(inFile);

    return 0;
}
