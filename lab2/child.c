/*Program 2: Child Program (child.c)
 - Allocate an integer array of size 5 using calloc.
 - Print the initial values of this array.
 - Use realloc to expand this array to a size of 10.
 - Set the new elements (from index 5 to 9) to values 11 to 15.
 - Print the expanded array to confirm the correct allocation and initialization of new elements.
 - Change the array size to 3.
 - Print array
 - Free the memory
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
 {
    int size = 5;
    printf("Child process executing with PID: %d\n", getpid());
    
    int *array = (int *)calloc(size, sizeof(int));
    
    if (array == NULL)
    {
        perror("calloc failed");
        return 1;
    }
    
    
    printf("Initial array (size 5): ");
    for (int i = 0; i < size; i++) 
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    int new_size = 10;
    array = (int *)realloc(array, new_size * sizeof(int));
    
    if (array == NULL)
    {
        perror("realloc failed");
        return 1;
    }
    
    for (int i = size; i < new_size; i++)
    {
        array[i] = i + 6;
    }
    
    printf("Expanded array (size 10): ");
    for (int i = 0; i < new_size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    
    int last_size = 3; 
    array = (int *)realloc(array, last_size * sizeof(int));
    
    if (array == NULL)
    {
        perror("realloc failed");
        return 1;
    }
    
    
    printf("Resized array (size 3): ");
    for (int i = 0; i < last_size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    free(array);
    array = NULL;
    return 0;
}
