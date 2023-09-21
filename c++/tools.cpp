#include <vector>

int countOccurrences(std::vector<char> arr, int x) {
    int res = 0;

    for (std::size_t i = 0; i < arr.size(); i++)
        if (arr[i] == x)
            res++;
    return res;
}