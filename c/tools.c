int countOccurrences(int *arr, int n, int x) {
    int res = 0;

    for (int i = 0; i < n; i++)
        if (arr[i] == x)
            res++;
    return res;
}