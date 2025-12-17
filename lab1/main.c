#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	char str[] = "Hello";
	
	char *ptr = str+1;
	

	
	printf("Pointer's address: %p\n", &ptr);
	
	printf("Pointer's value: %p\n", ptr);	
	
	printf("Value stored at the address pointed by the pointer: %c\n\n", *ptr);	
	
	int child_pid = fork();

	if (child_pid == 0)
	{
		printf("Executing child program\n");
		execl("~/lab1/child", "child", NULL);
		exit(1); //status of a child
	}	

	else
       	{	
		printf("Parent PID: %d waiting for child\n", getpid());
		wait(NULL); // waits for child
	}
	
	return 0;
}
