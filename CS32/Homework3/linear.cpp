// Return true if any of the array elements is negative, false
// otherwise.
bool anyNegative(const double a[], int n)
{
    if (n == 0) 
        return false;
    
    if (a[n-1] < 0)
        return true;
    else
        return anyNegative(a, n-1);
}

// Return the number of negative elements in the array.
int countNegatives(const double a[], int n)
{
    if (n == 0) 
        return 0;
    if (a[n-1] < 0)
        return 1+countNegatives(a, n-1);
    else
        return countNegatives(a, n-1);
}

// Return the subscript of the first negative element in the array.
// If no element is negative, return -1.
int firstNegative(const double a[], int n)
{
    if (n == 0) 
        return -1;
    if (a[n-1] < 0)
        return n-1;
    else
        return anyNegative(a, n-1);
}

// Return the subscript of the largest element in the array.  If
// more than one element has the same largest value, return the
// smallest subscript of such an element.  If the array is empty,
// return -1.
int indexOfMax(const double a[], int n)
{
    if (n == 0) 
        return -1;
    
    if (n == 1)
        return n-1;
    
    int index = 1+indexOfMax(a+1, n-1);
    
    if (a[index] > a[0])
        return index;
    else
        return 0;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n1 == 0)
        return false;
    if (n2 == 0)
        return true;
    
    int values;
                    
    if (a1[n1-1] == a2[n2-1])
    {
        values = (n2-1)+includes(a1, n1-1, a2, n2-1);
    }
    else
    {
        values = (n2-1)+includes(a1, n1-1, a2, n2);
    }
        
    if (values > n2-1)
    {
        return values;
    }
    else
        return 0;
}
