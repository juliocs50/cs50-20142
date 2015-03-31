/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    for (int i = 0; i< n; i++) {
    if (values[i] == value) {
        return true;
        }
        }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    for (int i = 1; i< n; i++)
    {
        int j=i;
        int counter = values[i];
        while (j>0 && values[j-1]>counter)
        {
            values[n] = values[n-1];
            j=j-1;
            values[j]=counter;
            }
       }
 }
 bool isSorted(int values[], int n)
 {
    for (int i=0; i<n-1;i++) {
        if (values[i]> values[i+1])  
            {
            return false;
            }
            }
            return true;
 } 



