#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapify(int arr[], int n, int i)
{
	// Find largest among root,
	// left child and right child

	// Initialize largest as root
	int largest = i;

	// left = 2*i + 1
	int left = 2 * i + 1;

	// right = 2*i + 2
	int right = 2 * i + 2;

	// If left child is larger than root
	if ( left < n && arr[ left ] > arr[ largest ] )
		largest = left;

	// If right child is larger than largest
	// so far
	if ( right < n && arr[ right ] > arr[ largest ] )
		largest = right;

	// Swap and continue heapifying
	// if root is not largest
	// If largest is not root
	if ( largest != i )
	{
		swap( &arr[ i ], &arr[ largest ] );

		// Recursively heapify the affected
		// sub-tree
		heapify( arr, n, largest );
	}
}

void heapSort(int arr[], int n)
{
	// Build max heap
	for ( int i = n / 2 - 1; i >= 0; i-- )
		heapify( arr, n, i );

	// Heap sort
	for ( int i = n - 1; i >= 0; i-- )
	{
		swap( &arr[ 0 ], &arr[ i ] );

		// Heapify root element
		// to get highest element at
		// root again
		heapify( arr, i, 0 );
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int *pData, int *tmp, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 =  r - m;

	/* create temp arrays */
	int *L = tmp;
	int *R = tmp + n1;

	/* Copy data to temp arrays L[  ] and R[  ] */
	for ( i = 0; i < n1; i++ )
		L[ i ] = pData[ l + i ];
	for ( j = 0; j < n2; j++ )
		R[ j ] = pData[ m + 1+ j ];

	/* Merge the temp arrays back into pData[ l..r ]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while ( i < n1 && j < n2 )
	{
		if ( L[ i ] <= R[ j ] )
		{
			pData[ k ] = L[ i ];
			i++;
		}
		else
		{
			pData[ k ] = R[ j ];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[  ], if there
		are any */
	while ( i < n1 )
	{
		pData[ k ] = L[ i ];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[  ], if there
		are any */
	while ( j < n2 )
	{
		pData[ k ] = R[ j ];
		j++;
		k++;
	}
}

void mergeSortUtil(int pData[], int *tmp, int l, int r)
{
	if ( l < r )
	{
		// get the mid point
		int m = ( l + r ) / 2;

		// Sort first and second halves
		mergeSortUtil( pData, tmp, l, m );
		mergeSortUtil( pData, tmp, m + 1, r );

		// merge the two halfs of the array
		merge( pData, tmp, l, m, r );
	}
}

void mergeSort(int pData[], int l, int r)
{
	int *tmp = ( int* ) Alloc( sizeof( int ) * ( r + 1 ) );
	mergeSortUtil( pData, tmp, l, r );
	DeAlloc( tmp );
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, j, item;
	for ( i = 0; i < n; i++ )
	{
		item = pData[ i ];
		for( j = i - 1; j >= 0; j-- )
		{
			if( pData[ j ] > item )
				pData[ j + 1 ] = pData[ j ];
			else
				break;
		}
		pData[ j + 1 ] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	for (int i = 0; i < n - 1; i++)
		for (int j = 0; j < n - i - 1; j++)
			if (pData[j] > pData[j + 1])
				swap(&pData[j], &pData[j + 1]);
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i, j, min_idx;

	// selection sort
	for ( i = 0; i < n - 1; i++ )
	{
		// find min value
		min_idx = i;
		for ( j = i + 1; j < n; j++ )
			if ( pData[ j ] < pData[ min_idx ] )
				min_idx = j;

		// swap old min with new min
		swap( &pData[ i ], &pData[ min_idx ] );
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);

		// Implement parse data block
		if ( ppData == NULL )
		{
			return 0;
		}

		for ( int i = 0; i < dataSz; i++ )
		{
			fscanf( inFile, "%d", &( *ppData )[ i ] );
		}
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
