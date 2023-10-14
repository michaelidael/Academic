#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// structure to represent the Coordinate
typedef struct
{
    int x, y;
}Coordinate;

// global object to store my location coordinate
Coordinate myLoc;

// function declaration
void ReadData(FILE* file, Coordinate coords[], int n);
int compareTo(Coordinate *ptrP1, Coordinate *ptrP2);
void sort(Coordinate coords[], int n, int t);
void mergeSort(Coordinate coords[], int low, int high, int t);
void merge(Coordinate coords[], int low, int mid, int high);
void insertionSort(Coordinate coords[], int low, int high);
int binarySearch(Coordinate coords[], int n, Coordinate key);


int main()
{
    // open input file
    FILE* fp = fopen("assignment3input.txt", "r");

    // check file opened successfully, else display error message
    if(fp == NULL)
    {
        printf("Unable to open file: assignment3input.txt\n");
    }
    else
    {
        int n, s, t, i;
        // open output file
        FILE* outFp = fopen("out.txt", "w");

        Coordinate coords[1000000];
        Coordinate key;

        // read coordinates for myLocation, n, s, and t
        fscanf(fp, "%d %d %d %d %d",&myLoc.x, &myLoc.y, &n, &s, &t);

        ReadData(fp, coords, n); // read the n coordinates in the array
        sort(coords, n, t); // sort the coordinates in ascending order of distances using compareTo function

        // output the sorted coordinates to output file
        for(i=0;i<n;i++)
        {
            fprintf(outFp, "%d %d\n",coords[i].x, coords[i].y);
        }

        // loop to search for s coordinates and output the result to output file
        for(i=0;i<s;i++)
        {
            fscanf(fp, "%d %d",&key.x, &key.y);
            int index = binarySearch(coords, n, key);
            if(index == -1) // key not found
                fprintf(outFp,"%d %d not found\n",key.x,key.y);
            else // key found, output the rank which is 1 more than index
                fprintf(outFp,"%d %d found at rank %d\n",key.x,key.y, index+1);
        }

        // close the files
        fclose(fp);
        fclose(outFp);
    }

    return 0;
}

// function that reads n coordinates from file into the array coords
void ReadData(FILE* file, Coordinate coords[], int n)
{
    int i=0;
    // loop to read n points from file into coords array
    while(i < n)
    {
        fscanf(file, "%d %d",&coords[i].x, &coords[i].y);
        i++;
    }
}

// function to compare Coordinate ptrP1 and ptrP2 and
// returns < 0, if ptrP1 < ptrP2
// return 0, if ptrP1 = ptrP2
// return > 0, if ptrP1 > ptrP2
int compareTo(Coordinate *ptrP1, Coordinate *ptrP2)
{
    // compute the distance of ptrP1 and ptrP2 from myLoc
    float disP1 = sqrt((float)pow(myLoc.x-ptrP1->x,2) + (float)pow(myLoc.y-ptrP1->y, 2));
    float disP2 = sqrt((float)pow(myLoc.x-ptrP2->x,2) + (float)pow(myLoc.y-ptrP2->y, 2));

    // distance of ptrP1 < distance of ptrP2 , return -1
    if(disP1 < disP2)
        return -1;
    // distance of ptrP1 > distance of ptrP2 , return 1
    else if(disP1 > disP2)
        return 1;
    else // distances are equal, compare them first o x coordinate and then on y
    {
        // x of ptrP1 < x of ptrP2 , return -1
        if(ptrP1->x < ptrP2->x)
            return -1;
        // x of ptrP1 > x of ptrP2 , return 1
        else if(ptrP1->x > ptrP2->x)
            return 1;
        // x coordinates are equal, compare on y
        else
        {
            // y of ptrP1 < y of ptrP2 , return -1
            if(ptrP1->y < ptrP2->y)
                return -1;
            // y of ptrP1 > dyistance of ptrP2 , return 1
            else if(ptrP1->y > ptrP2->y)
                return 1;
            else // y also equal, both are same points return 0
                return 0;
        }
    }
}

// function to sort the array of Coordinates of size n
void sort(Coordinate coords[], int n, int t)
{
    // size of array <= threshold
    if(n <= t)
    {
        insertionSort(coords, 0, n-1); // sort the array using insertion sort
    }
    else // size of array > threshold
        mergeSort(coords, 0, n-1, t); // sort the array using merge sort
}

// recursive function to sort the array using merge sort
void mergeSort(Coordinate coords[], int low, int high, int t)
{
    int n = (high-low)+1; // get the size of the subarray
    // size of subarray <= threshold, sort it using insertion sort
    if(n <= t)
    {
        insertionSort(coords, low, high);
    }
    else // size of subarray > threshold
    {
        // get the mid index of the subarray
        int mid = (low+high)/2;
        mergeSort(coords, low, mid, t); // sort left sub array using merge sort
        mergeSort(coords, mid+1, high, t); // sort right sub array using merge sort
        merge(coords, low, mid, high); // merge the sorted subarrays
    }
}

// function to merge the sorted sub arrays [low, mid], [mid+1, high] to get the sorted array [low, high]
void merge(Coordinate coords[], int low, int mid, int high)
{
    // create a temporary array of size of the combined left and right subarrays
    Coordinate temp[high-low+1];
    // initialize i, j and k to start index of left subarray, right subarray and temp
    int i=low, j = mid+1, k=0;

    // loop to continue until we reach end of a subarray
    while(i <= mid && j <= high)
    {
        if(compareTo(&coords[i], &coords[j]) <= 0) // element of left subarray <= element of right subarray
        {
            // insert element from left subarray to temp
            temp[k] = coords[i];
            i++;
        }
        else
        {
            // insert element from right subarray to temp
            temp[k] = coords[j];
            j++;
        }
        k++;
    }

    // copy the remaining elements of left subrray to temp
    while(i <= mid)
    {
        temp[k] = coords[i];
        k++;
        i++;
    }

    // copy the remaining elements of right subrray to temp
    while(j <= high)
    {
        temp[k] = coords[j];
        k++;
        j++;
    }

    // copy the sorted elements from temp to coords
    for(i=low, k=0;i<=high;i++, k++)
    {
        coords[i] = temp[k];
    }
}

// function to sort the input array from low to high(inclusive) using insertion sort
void insertionSort(Coordinate coords[], int low, int high)
{
    int i, j;
    Coordinate key;
    for(i=low+1;i<=high;i++)
    {
        j = i-1;
        key = coords[i];

        while(j>=low && compareTo(&key, &coords[j]) < 0)
        {
            coords[j+1] = coords[j];
            j--;
        }

        coords[j+1] = key;
    }
}

// function to search for key in coords array of size n and returns the index of key if found, else return -1
int binarySearch(Coordinate coords[], int n, Coordinate key)
{
    // initialize low to first index and high to last index of coords
    int low = 0, high = n-1;
    // loop that continues until the range is invalid
    while(low <= high)
    {
        // get the index of mid element
        int mid = (low+high)/2;
        // get the distance of key from mid element
        float dist = compareTo(&key, &coords[mid]);
        if(dist == 0) // key found, return mid
            return mid;
        else if(dist < 0) // key < element at mid, search in left subarray
            high = mid-1;
        else // key > element at mid, search in right subarray
            low = mid+1;
    }

    return -1; // key not found
}
