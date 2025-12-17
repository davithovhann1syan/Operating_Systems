/*Program 1: Parent Program (parent.c)
 - Allocate an integer array of size 10 using malloc.
 - Initialize the array with values from 1 to 10.
 - Print these values to confirm successful memory allocation and initialization.
 - Use fork to create a child process.
 - In the child process, use exec to replace the child’s code with a separate program (child.c).
 - The parent should wait for the child to finish.
 - Free the allocated memory in the parent after the child has completed.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{	

	int size = 10;
    int *array = (int *)malloc(size * sizeof(int));
    
    if (array == NULL)
    {
        perror("memory allocation failed failed");
        return 1;
    }
    
    for (int i = 0; i < size; i++) 
    {
        array[i] = i + 1;
    }
    
    printf("Array values: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    pid_t pid = fork();
    
    if (pid < 0)
    {
        perror("fork failed");
        free(array);
        return 1;
    }
    
    if (pid == 0)
    {
        execl("./child", "child", NULL);
        perror("exec failed");
        exit(1);
        
    }
    else
    {
        wait(NULL);
        printf("Child process finished\n");
    }
    
    free(array);
    array = NULL;    
    return 0;
}
