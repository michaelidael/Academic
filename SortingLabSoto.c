
#include <stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAXVAL 1000000

void randArray(int A[], int size, int maxval);
void bubbleSort(int A[], int n);
void insertionSort(int arr[], int n);
void swap(int *a, int *b);
void arrayCopy(int from[], int to[], int size);
long timediff(clock_t t1, clock_t t2);
void quickSort(int* numbers, int low, int high);
void mergeSort(int arr[], int l, int r);
void selectionSort(int arr[], int n);


int main() {

    int i, j, start, end;
    int sizes[] = {1000, 10000, 20000, 30000, 40000, 50000};
    int * originalArray;
    int * sortedArray;
    long elapsed;

  for(i = 0; i < 6; i++)
  {
    originalArray= (int *) malloc(sizeof(int) * sizes[i]);
    sortedArray = (int *) malloc(sizeof(int) * sizes[i]);

    //fill array with random numbers.
    randArray(originalArray, sizes[i],MAXVAL);

//do the same for bubbleSort

  arrayCopy(originalArray, sortedArray, sizes[i]);
  start =  clock();
  bubbleSort(sortedArray, sizes[i]);
  end = clock();
  elapsed = timediff(start, end);
  printf("sorting %d values takes %ld milli seconds for bubbleSort. \n\n", sizes[i], elapsed);

  //do the same for selectionSort
  arrayCopy(originalArray, sortedArray, sizes[i]);
  start =  clock();
 selectionSort(sortedArray, sizes[i]);
  end = clock();
  elapsed = timediff(start, end);
  printf("sorting %d values takes %ld milli seconds for selectionSort. \n\n", sizes[i], elapsed);

    //do the same for insertionSort
  arrayCopy(originalArray, sortedArray, sizes[i]);
  start =  clock();
  insertionSort(sortedArray, sizes[i]);
  end = clock();
  elapsed = timediff(start, end);
  printf("sorting %d values takes %ld milli seconds for insertionSort. \n\n", sizes[i], elapsed);


  // do the same for mergeSort
  arrayCopy(originalArray, sortedArray, sizes[i]);
  start =  clock();
  mergeSort(sortedArray, 0, sizes[i]);
  end = clock();
  elapsed = timediff(start, end);
  printf("sorting %d values takes %ld milli seconds for mergeSort. \n\n", sizes[i], elapsed);

  //QUICKSORT
    arrayCopy(originalArray, sortedArray, sizes[i]);

    start =  clock();
    quickSort(sortedArray, 0, sizes[i]);
    end = clock();
    elapsed = timediff(start, end);

  printf("sorting %d values takes %ld milli seconds for quickSort. \n\n", sizes[i], elapsed);


  //free the arrays.
  free(sortedArray);
  free(originalArray);


  }

  return 0;



}




long timediff(clock_t t1, clock_t t2) {
    long elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
 return elapsed;

}

void arrayCopy(int from[], int to[], int size)
{



  int i;
  for(i = 0; i < size; i++)
  {
    to[i] = from[i];
  }


}

// Pre-condition: A is an array of size greater than or equal to size and
//                maxval is positive.
// Post-condition: A is filled with random integers in the range [1,maxval]
//                 from index 0 to index size-1.
void randArray(int A[], int size, int maxval) {

     int i;
     for(i = 0; i < size; i++)
     {
       A[i] = rand()% MAXVAL + 1;
     }


}

// Sorts the first length elements of vals in ascending order.
void bubbleSort(int A[], int n) {

     int i,j;

     // Loop through each element to insert.
     for (i=n-2; i>=0; i--) {

         for (j=0; j<=i; j++)
             if (A[j] > A[j+1])
                swap(&A[j], &A[j+1]);
     }
}

// Swaps the integers pointed to by a and b.
void swap(int *a, int *b) {
     int temp = *a;
     *a = *b;
     *b = temp;
}

void insertionSort(int arr[], int n)
{
    int i, item, j;
    for (i = 1; i < n; i++)
    {
         item = arr[i];

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
          for(j=i-1; j>=0; j--)
          {
              if(arr[j]>item)
                arr[j+1] = arr[j];
              else
                break;

          }
          arr[j+1] = item;
    }
}




// Pre-condition: low and high are valid indexes into values
// Post-condition: Returns the partition index such that all the values
//                 stored in vals from index low to until that index are
//                 less or equal to the value stored there and all the values
//                 after that index until index high are greater than that
//
int partition(int *vals, int low, int high)
{
  // Pick a random partition element and swap it into index low.
  int i = low + rand()%(high-low+1);
  swap(&vals[low], &vals[i]);

  int lowpos = low; //here is our pivot located.

  low++; //our starting point is after the pivot.

// Run the partition so long as the low and high counters don't cross.
  while(low<=high)
  {
    // Move the low pointer until we find a value too large for this side.
    while(low<=high && vals[low]<=vals[lowpos]) low++;

    // Move the high pointer until we find a value too small for this side.
    while(high>=low && vals[high] > vals[lowpos]) high--;

    // Now that we've identified two values on the wrong side, swap them.
    if (low<high)
     swap(&vals[low], &vals[high]);
  }

  // Swap the pivot element element into its correct location.
  swap(&vals[lowpos], &vals[high]);

  return high; //return the partition point
}

// Pre-condition: s and f are value indexes into numbers.
// Post-condition: The values in numbers will be sorted in between indexes s
//                 and f.
void quickSort(int* numbers, int low, int high) {

    // Only have to sort if we are sorting more than one number
    if (low < high) {
        int split = partition(numbers,low,high);
        quickSort(numbers,low,split-1);
        quickSort(numbers,split+1,high);
    }
}

void selectionSort(int arr[], int n)
{
  int i, j, min_idx, temp;
  // One by one move boundary of unsorted subarray
  for (i = 0; i < n-1; i++)
  {
      //printf("\nIteration# %d\n",i+1);
    // Find the minimum element in unsorted array
    min_idx = i;
    for (j = i+1; j < n; j++)
    if (arr[j] < arr[min_idx])
      min_idx = j;
    // Swap the found minimum element with the first element
    temp = arr[i];
    arr[i] = arr[min_idx];
    arr[min_idx] = temp;


  }
}


void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int *L = (int*) malloc(n1*sizeof(int));
    int *R = (int*) malloc(n2*sizeof(int));

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // get the mid point
        int m = (l+r)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

       // printf("Testing l=%d r=%d m=%d\n", l, r, m);

        merge(arr, l, m, r);
    }
}
