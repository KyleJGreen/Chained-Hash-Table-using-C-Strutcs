#include <stdio.h>  // for I/O
#include <stdlib.h> // for random # generator, malloc and free
#include <time.h>   // for srand(time)
#define TABLE_SIZE 25   // the size of our hash table

/* Kyle J Green
 * CSC 362
 * Chained Hash Table using Structs
 * April 21st, 2017
 * Description: Program used a hash table of singly linked lists to compare efficiency (defined by the number of
 * operations preformed) of search and insert operations for both ordered and unordered lists on tables of size
 * 23, 24, and 25.
 */

/* a node struct storing the int data and a next pointer */
struct node {                       // defines a node in a linked list
    int data;                           // the datum
    struct node *next;                  // pointer to the next node
};

/* A list struct which will store a front pointer to the first element of a linked list and the list’s current number
 * of items stored there (an int) */
struct list {
    struct node *front; // front pointer
    int items;  // number of items in the list
};

// prototypes
struct node *orderedInsert (int *, int , struct node *);    // insert an element into an ordered list
struct node *unorderedInsert (int *, int, struct node *);    // insert an element into an unordered list
int orderedSearch (int *, int, struct node *);     // search for an element in an ordered list
int unorderedSearch (int *, int, struct node *);     // search for an element in an unordered list
void printList(struct node *);  // prints a list
void exterminate (struct node *);   // destroys all elements to a list (deallocates the heap)

/* Insert a new int value item into an ordered linked list given its front pointer */
struct node *orderedInsert (int *count, int value, struct node *front)
{
    struct node *temp, *current, *previous;  // temp will point to new node, current and previous used to traverse list

    (*count)++; // add one to our number of operations performed
    temp=(struct node *)malloc(sizeof(struct node));    // allocate a node from heap
    temp->data=value;   // assign the new node its value
    temp->next=NULL;    // and it will be the current last node, so make next NULL
    if(front==NULL) return temp;    // special case of empty list, no list to traverse
    else if(value<front->data)  // special case 2:  if new node should be inserted at front
    {
        temp->next=front;   // let new node point at rest of list
        return temp;    // and return new node's pointer as new front of the list
    }
    else    // have to traverse list until we have a pointer at the last node
    {
        current=front;  // initialize our two pointers to work down the list, current always
        previous=NULL;  // pointing at current node while previous points at its predecessor
        while(current!=NULL&&current->data<value)  // traverse list until we either reach the end of find the right location
        {
            previous=current;   // previous always points at prior node
            current=current->next;  // current points at node we are inspecting
            (*count)++; // add one to our number of operations performed
        }
        previous->next=temp;    // attach new node to list by having previous point at it
        temp->next=current; // and reattach rest of list to temp's next field
        return front;   // return the front pointer so that we can reattach list in main
    }

}

/* Insert a new int value item into an unordered linked list given its front pointer */
struct node *unorderedInsert (int *count, int value, struct node *front)
{
    struct node *temp=(struct node *)malloc(sizeof(struct node));   // allocate a node from heap
    temp->data=value;   // add value as data to new node
    temp->next=front;   // set new node's next pointer to point at current front of list

    (*count)++; // add one to our number of operations performed
    return temp;    // return pointer to new node as new front of list
}

/* Search for an int value in an ordered linked list to locate it given its front pointer */
int orderedSearch (int *count, int value, struct node *front)
{
    /* iterate through the list until we reach a value either greater than or equal to the one we are searching for */
    while(front!=NULL && front->data < value)
    {
        (*count)++; // add one to our number of operations performed
        front=front->next;  // increment our node pointer to point to it's next field
    }

    /* Did we find the item??? */
    if(front == value)
        return value;   // if yes, return item
    else
        return -1;  // else return -1
}

/* Search for an int value in an unordered linked list to locate it given its front pointer */
int unorderedSearch (int *count, int value, struct node *front)
{
    /* iterate through the list until we find the value we are looking for*/
    while(front!=NULL && front->data != value)
    {
        (*count)++; // add one to our number of operations performed
        front=front->next;  // increment our node pointer to point to it's next field
    }

    /* Did we find the item??? */
    if(front == value)
        return value;   // if yes, return item
    else
        return -1;  // else return -1
}

/* print the int elements stored in a linked list given its front pointer */
void printList(struct node *f)
{
    /* traverse the list pointed to by f, printing each element */
    while(f!=NULL)
    {
        printf("%d\n", f->data);    // print the data
        f=f->next;  // move f to next node in list
    }
}

//              _n____n__
//             /         \---||--<
//            /___________\
//            _|____|____|_
//            _|____|____|_
//             |    |    |
//            --------------
//            | || || || ||\
//            | || || || || \++++++++------<
//            ===============
//            |   |  |  |   |
//            (| O | O| O| O |)
//            |   |   |   |   |
//            (| O | O | O | O |)
//            |   |   |   |    |
//            (| O |  O | O  | O |)
//            |   |    |    |    |
//            (| O |  O |  O |  O |)
//            ======================

/* destroy a linked list given its front pointer (deallocate all heap memory) */
void exterminate (struct node *f)
{
    struct node *temp=f;    // need a temp pointer as one pointer will point to next
    while(f!=NULL)  // while we still have list nodes to deallocate
    {
        f=f->next;  // f now points to the next node in the list
        free(temp); // so we can deallocate the current node
        temp=f; // set temp to point to f
    }
}

/* Prints the size of our arrays, the number of operations for both ordered and unordered lists, the longest and the
 * shortest list. */
void output (int orderedCount, int unorderedCount, int longestList, int shortestList)
{
printf("%s%d\n%-40s%d\n%-40s%d\n%-40s%d\n%-40s%d", "For array size ", TABLE_SIZE, "Number of Operations (ordered list):",
       orderedCount, "Number of Operations (unordered list): ", unorderedCount, "Longest list: ",
       longestList, "Shortest list: ", shortestList); // print the output
}

/* main method */
int main() {
    struct list orderedList[TABLE_SIZE];    // create an ordered list
    struct list unorderedList[TABLE_SIZE];   // create an unordered list
    int values[1000];   // Create an array of 1000 int values
    int orderedCount, unorderedCount, shortestList = 1000, longestList;  // for counting the number of operations it takes to search for an element

    srand(time(NULL));  // seed the random number generator

    /* Initialize all of the pointers in lists1 and lists2 to NULL and their sizes to 0 */
    for(int i = 0; i < TABLE_SIZE; i++) {
        orderedList[values[i] % TABLE_SIZE].front = NULL;   // ordered
        orderedList[values[i] % TABLE_SIZE].items = 0;  // ordered
        unorderedList[values[i] % TABLE_SIZE].front = NULL; // unordered
        unorderedList[values[i] % TABLE_SIZE].items = 0;    //unordered
    }

    /* randomly generate values between 0 and 999,999, insert them into the array and
     * add these values to both the ordered and unordered lists */
    for (int i = 0; i < 1000; i++) {
        values[i] = rand()%999999; // insert random value into array

        /* Iterate through the array and insert each value into both hash tables. */
        orderedList[values[i] % TABLE_SIZE].front = orderedInsert(&orderedCount, values[i], orderedList[values[i] % TABLE_SIZE].front); // ordered
        unorderedList[values[i] % TABLE_SIZE].front = unorderedInsert(&unorderedCount, values[i], unorderedList[values[i] % TABLE_SIZE].front); // unordered
        (orderedList[values[i] % TABLE_SIZE].items)++;  // increment size of ordered list
        (unorderedList[values[i] % TABLE_SIZE].items)++;    // increment size of unordered list
    }

    /* Iterate through the same array of 1000* random values and search for each value in
     * both hash tables. */
    for (int i = 0; i < 1000; i++) {

        /* for every 3rd element sought, rather than accessing it from the original array of 1000
         * int values, generate a new random number from 0 to 999,999 */
        if(i != 0 && i % 3 == 0) {
            int number = rand()%999999; // randomly generated number for every third iteration
            orderedSearch(&orderedCount, number, orderedList[values[i] % TABLE_SIZE].front); // ordered
            unorderedSearch(&unorderedCount, number, unorderedList[values[i] % TABLE_SIZE].front); // unordered
        }
            /* for elements in iterations not divisible by three, use the values[] array */
        else {
            orderedSearch(&orderedCount, values[i], orderedList[values[i] % TABLE_SIZE].front); // ordered
            unorderedSearch(&unorderedCount, values[i], unorderedList[values[i] % TABLE_SIZE].front); // unordered
        }
    }

    /* iterate through the list of list sizes to find the shortest and the longest list */
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        if(orderedList[i].items < shortestList)
            shortestList = orderedList[i].items;    // update shortest list
        if(orderedList[i].items > longestList)
            longestList = orderedList[i].items; // update longest list
    }

    output(orderedCount, unorderedCount, longestList, shortestList);    // output the results

    /* Loop through arrays of lists, destroying the lists (deallocate all nodes) */
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        /* EXTERMINATE!!! */
        exterminate(orderedList[i].front);  // destroy ordered list
        exterminate(unorderedList[i].front);    // destroy unordered list
    }

    return 0;   // end program
}