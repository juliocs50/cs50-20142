/****************************************************************************
* dictionary.c
*
* Computer Science 50
* Problem Set 6
*
* Implements a dictionary's functionality.
***************************************************************************/

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// declare hastable (buckets consist of three first letters of word)
node hashtable[28][28][28];

/**
* Returns true if word is in dictionary else false.
*/
bool check(const char* word)
{
    char buffer[LENGTH + 1];
    
    // ensure only lowercase letters, and copy to char buffer
    int i;
    for (i = 0; word[i] != '\0'; i++)
    {
     buffer[i] = tolower(word[i]);
    }
    buffer[i] = '\0';

// declare hash container
    hashcont hashes = hash(buffer);
    
    // check if the list contains any word with the first three chars
    // returns false if not
    if (hashtable[hashes.one][hashes.two][hashes.three].data == 0)
    {
     return false;
    }
    
    // check if the hashtable doesn't contain linked list
    else if (hashtable[hashes.one][hashes.two][hashes.three].next == 0)
    {
     // Checks for the word, returns true/false
     if (strcmp(hashtable[hashes.one][hashes.two][hashes.three].data, buffer) == 0)
     {
     return true;
     }
     else
     {
     return false;
     }
    }
    // the hashtable contains a linked list, check that for the word
    else
    {
     node* ptr = &hashtable[hashes.one][hashes.two][hashes.three];
    
     while (ptr != NULL)
     {
     if(strcmp(ptr->data, buffer) == 0)
     {
     return true;
     }
     ptr = ptr->next;
     }
    }
    return false;
}

/**
* Loads dictionary into memory. Returns true if successful else false.
*/
bool load(const char* dictionary)
{
    // declare buffer of size LENGTH + 1
    char buffer[LENGTH + 1];
        
    // declare hashcont to use with hash-function
    hashcont hashes;
    
    //new nodes
    node* newnode;
    node* prenode;
    
    // open dictionary file in read-only
    FILE* dict = fopen(dictionary, "r");
    
    int index = 0;
    int c = 10;
    while (c != EOF)
    {
     c = fgetc(dict);
    
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            buffer[index] = c;
            index++;
        }
        else if (c == 10)
        {
         // NULL terminate word
         buffer[index] = '\0';
        
         // prepare for new word
         index = 0;

         // store the hash values of the word loaded in buffer in hashes
hashes = hash(buffer);
        
// store in hashtable if spot not occupied
if (hashtable[hashes.one][hashes.two][hashes.three].data[0] == 0)
{
// store the word in the appropiate location in hashtable, and delete buffer meanwhile
for (int i = 0; buffer[i] != 0; i++)
{
hashtable[hashes.one][hashes.two][hashes.three].data[i] = buffer[i];
buffer[i] = 0;
}
}
// store in hashtable if spot is occupied, but doesn't contain linked list
else if (hashtable[hashes.one][hashes.two][hashes.three].next == 0)
{
// allocate new node
newnode = malloc(sizeof(node));
for (int i = 0; i < 46; i++)
{
newnode->data[i] = '\0';
}
newnode->next = NULL;
// store in node->data
for (int i = 0; buffer[i] != 0; i++)
{
newnode->data[i] = buffer[i];
buffer[i] = 0;
}
// link the node in the list
hashtable[hashes.one][hashes.two][hashes.three].next = newnode;
}
// else store at the end of the linked list
else
{
// allocate new node
newnode = malloc(sizeof(node));
for (int i = 0; i < 46; i++)
{
newnode->data[i] = '\0';
}
newnode->next = NULL;
// allocate previous node, with address of hashtable
prenode = &hashtable[hashes.one][hashes.two][hashes.three];
for (int i = 0; buffer[i] != 0; i++)
{
newnode->data[i] = buffer[i];
buffer[i] = 0;
}
// increment over the linked list until the end is reached
while (prenode->next != 0)
{
prenode = prenode->next;
}
// set the last node = to the new node, adding the new word in to the linked list
prenode->next = newnode;
}
}
}
fclose(dict);
return true;
}

/**
* Returns number of words in dictionary if loaded else 0 if not yet loaded.
*/
unsigned int size(void)
{
// for loops increment through hashtable
int counter = 0;
for (int i = 0; i < 28; i++)
    {
     for (int j = 0; j < 28; j++)
     {
     for (int k = 0; k < 28; k++)
     {
     // if hashtable spot contains data
     if (hashtable[i][j][k].data[0] != 0)
     {
     // if hashtable spot contains no linked list count 1
     if (hashtable[i][j][k].next == 0)
     {
     counter++;
     }
     // if hashtable spot contains linked list, count it
     else if (hashtable[i][j][k].next != 0)
     {
     node* ptr = &hashtable[i][j][k];
     while (ptr != NULL)
     {
     counter++;
     ptr = ptr->next;
     }
     }
     }
     }
     }
    }
    return counter;
}

/**
* Unloads dictionary from memory. Returns true if successful else false.
*/
bool unload(void)
{
// for the entity of the hashtable
    for (int i = 0; i < 28; i++)
    {
     for (int j = 0; j < 28; j++)
     {
     for (int k = 0; k < 28; k++)
     {
     // if spot contains linked list, free that linked list
     if (hashtable[i][j][k].next != 0)
     {
     // point cursor at head of linked list
     node* cursor = hashtable[i][j][k].next;
    
     // while the end is not reached
     while (cursor != NULL)
     {
     // create temp node, and = cursor
     node* temp = cursor;
     // move cursor to next node
     cursor = cursor->next;
     // free temp node
     free(temp);
     }
     }
     }
     }
    }
    return true;
}

/**
* Hash function to be used when loading dictionary into hashtable
*/
hashcont hash(const char* word)
{
// temporary hash container
hashcont temp;

// if letter contains apostrophe or is less than 3 chars
if (word[1] == 0 || word[2] == 0 || word[1] == 39 || word[2] == 39)
{
if (word[1] == 0)
{
temp.one = word[0] - 'a' + 1;
temp.two = 0;
temp.three = 0;
return temp;
}
else if (word[1] == 39)
{
temp.one = word[0] - 'a' + 1;
temp.two = 0;
if (word[2] != 0) temp.three = word[2] - 'a' + 1;
return temp;
}

else
{
temp.one = word[0] - 'a' + 1;
temp.two = word[1] - 'a' + 1;
temp.three = 0;
return temp;
}
}

// else
else
{
temp.one = word[0] - 'a' + 1;
temp.two = word[1] - 'a' + 1;
temp.three = word[2] - 'a' + 1;
return temp;	
}
}
