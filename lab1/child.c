#include <stdio.h>

void triple(int *arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		*(arr + i) = arr[i] * 3;
	}

}

int main()
{
	int array[100];

	for (int i = 0; i < 100; i++)
	{
		*(array + i) = i;
	}

	
	printf("Triple called:\n");	
	triple(array, 100);

	printf("Printing first 5 elements:\n");

	for (int i = 0; i < 5; i++)
	{
		printf("%d ", array[i]);
	}

	printf("\nPrinting last 5 elements: ");

	for (int i = 99; i > 94; i--)
	{
		printf("%d ", array[i]);
	}

	return 0;

}

