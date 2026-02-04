#include <vector>

void swap_ends(int arr[], int size)
{
    int half = size / 2;
    int second_start = size - half;

    for (int i = 0; i < half; ++i) {
        int tmp = arr[i];
        arr[i] = arr[second_start + i];
        arr[second_start + i] = tmp;
    }
}

void swap_ends_optimal(int arr[], int size)
{
    for (int i = size/2 - 1, j = size - 1; i >= 0; --i, --j)
        std::swap(arr[i], arr[j]);
}

void swap_ends(std::vector<int>& arr)
{
    for (int i = arr.size()/2 - 1, j = arr.size() - 1; i >= 0; --i, --j)
    {
        std::swap(arr[i], arr[j]);
    }
}