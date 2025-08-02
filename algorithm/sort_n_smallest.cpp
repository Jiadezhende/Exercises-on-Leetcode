#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
using std::vector;

/**
 * Find the nth smallest element in an array using QuickSelect algorithm,
 * move the n smallest elements forward,
 * and return the nth smallest element value, or -1 if n is invalid.
 * Time Complexity: O(n) average, O(n^2) worst case
 * Space Complexity: O(1) - in-place algorithm
 */
int sort_nth_smallest_quickselect(vector<int> nums, int n){
    if (n <= 0 || n > nums.size()) {
        return -1;  // Invalid input
    }

    // Helper function: Partition array around pivot
    // Returns the final position of pivot element
    auto partition = [](vector<int>& arr, int low, int high) -> int {
        int pivot = arr[high];  // Choose last element as pivot
        int i = low - 1;        // Index of smaller element
        
        for (int j = low; j < high; j++) {
            // If current element is smaller than or equal to pivot
            if (arr[j] <= pivot) {
                i++;
                // Swap elements
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        
        // Place pivot in correct position
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        
        return i + 1;
    };
    
    // QuickSelect algorithm to find nth smallest element
    auto quickSelect = [&](vector<int>& arr, int low, int high, int k) -> int {
        while (low <= high) {
            int pivotIndex = partition(arr, low, high);
            
            if (pivotIndex == k - 1) {
                // Found the kth smallest element
                return arr[pivotIndex];
            }
            else if (pivotIndex > k - 1) {
                // kth smallest is in left part
                high = pivotIndex - 1;
            }
            else {
                // kth smallest is in right part
                low = pivotIndex + 1;
            }
        }
        return -1;  // Should never reach here with valid input
    };
    
    // Find and return the nth smallest element
    // The algorithm automatically moves smaller elements to the left
    return quickSelect(nums, 0, nums.size() - 1, n);
}

/**
 * Find the nth smallest element in an array using optimized Heap-based algorithm,
 * move the n smallest elements forward,
 * and return the nth smallest element value, or -1 if n is invalid.
 * Time Complexity: O(n log k + (size-k) log k) = O(size log k) - much better when k is small
 * Space Complexity: O(1) - in-place heap operations
 */
int sort_nth_smallest_heap(vector<int> nums, int n){
    if (n <= 0 || n > nums.size()) {
        return -1;  // Invalid input
    }

    int size = nums.size();
    
    // Helper function: Heapify down operation for max-heap
    auto heapifyDown = [&](vector<int>& arr, int start, int end) -> void {
        int parent = start;
        int child = 2 * parent + 1;  // Left child
        
        while (child <= end) {
            // Find the larger child (for max-heap)
            if (child + 1 <= end && arr[child + 1] > arr[child]) {
                child++;  // Right child is larger
            }
            
            // If parent is already larger than children, heap property satisfied
            if (arr[parent] >= arr[child]) {
                break;
            }
            
            // Swap parent with larger child
            int temp = arr[parent];
            arr[parent] = arr[child];
            arr[child] = temp;
            
            // Move down the tree
            parent = child;
            child = 2 * parent + 1;
        }
    };
    
    // Step 1: Build max-heap with first n elements
    // This heap will maintain the n smallest elements seen so far
    for (int i = (n - 2) / 2; i >= 0; i--) {
        heapifyDown(nums, i, n - 1);
    }
    
    // Step 2: Process remaining elements from index n to size-1
    for (int i = n; i < size; i++) {
        // If current element is smaller than the maximum in our heap (heap root)
        if (nums[i] < nums[0]) {
            // Replace the maximum with current element
            nums[0] = nums[i];
            // Restore heap property
            heapifyDown(nums, 0, n - 1);
        }
    }
    
    // Step 3: Now the first n positions contain the n smallest elements
    // The heap root (nums[0]) is the maximum among these n smallest elements
    // which is exactly the nth smallest element of the original array
    return nums[0];
}

/**
 * Main function: Choose between QuickSelect and Heap-based approaches
 * For most cases, QuickSelect is faster on average
 * For guaranteed O(n log n) performance, use heap-based approach
 */
int sort_nth_smallest(vector<int> nums, int n){
    // Default to QuickSelect for better average performance
    return sort_nth_smallest_quickselect(nums, n);
    
    // Alternative: Use heap-based approach for guaranteed performance
    // return sort_nth_smallest_heap(nums, n);
}

/**
 * Test function to verify the correctness of both algorithms
 */
void test_algorithms() {
    std::cout << "Running test cases...\n\n";
    
    // Test Case 1: Basic functionality
    {
        vector<int> test1 = {3, 1, 4, 1, 5, 9, 2, 6};
        std::cout << "Test 1: [3, 1, 4, 1, 5, 9, 2, 6]\n";
        
        for (int n = 1; n <= test1.size(); n++) {
            vector<int> copy1 = test1;
            vector<int> copy2 = test1;
            
            int result_quickselect = sort_nth_smallest_quickselect(copy1, n);
            int result_heap = sort_nth_smallest_heap(copy2, n);
            
            // Verify with standard library
            vector<int> sorted_copy = test1;
            std::sort(sorted_copy.begin(), sorted_copy.end());
            int expected = sorted_copy[n - 1];
            
            std::cout << "  n=" << n << ": QuickSelect=" << result_quickselect 
                      << ", Heap=" << result_heap << ", Expected=" << expected;
            
            assert(result_quickselect == expected);
            assert(result_heap == expected);
            std::cout << " PASS\n";
        }
        std::cout << "\n";
    }
    
    // Test Case 2: Duplicate elements
    {
        vector<int> test2 = {5, 2, 2, 1, 1, 3, 3, 3};
        std::cout << "Test 2: [5, 2, 2, 1, 1, 3, 3, 3] (with duplicates)\n";
        
        for (int n = 1; n <= test2.size(); n++) {
            vector<int> copy1 = test2;
            vector<int> copy2 = test2;
            
            int result_quickselect = sort_nth_smallest_quickselect(copy1, n);
            int result_heap = sort_nth_smallest_heap(copy2, n);
            
            vector<int> sorted_copy = test2;
            std::sort(sorted_copy.begin(), sorted_copy.end());
            int expected = sorted_copy[n - 1];
            
            std::cout << "  n=" << n << ": QuickSelect=" << result_quickselect 
                      << ", Heap=" << result_heap << ", Expected=" << expected;
            
            assert(result_quickselect == expected);
            assert(result_heap == expected);
            std::cout << " PASS\n";
        }
        std::cout << "\n";
    }
    
    // Test Case 3: Already sorted array
    {
        vector<int> test3 = {1, 2, 3, 4, 5, 6, 7, 8};
        std::cout << "Test 3: [1, 2, 3, 4, 5, 6, 7, 8] (already sorted)\n";
        
        for (int n = 1; n <= test3.size(); n++) {
            vector<int> copy1 = test3;
            vector<int> copy2 = test3;
            
            int result_quickselect = sort_nth_smallest_quickselect(copy1, n);
            int result_heap = sort_nth_smallest_heap(copy2, n);
            int expected = n;  // In this case, nth smallest is just n
            
            std::cout << "  n=" << n << ": QuickSelect=" << result_quickselect 
                      << ", Heap=" << result_heap << ", Expected=" << expected;
            
            assert(result_quickselect == expected);
            assert(result_heap == expected);
            std::cout << " PASS\n";
        }
        std::cout << "\n";
    }
    
    // Test Case 4: Reverse sorted array
    {
        vector<int> test4 = {8, 7, 6, 5, 4, 3, 2, 1};
        std::cout << "Test 4: [8, 7, 6, 5, 4, 3, 2, 1] (reverse sorted)\n";
        
        for (int n = 1; n <= test4.size(); n++) {
            vector<int> copy1 = test4;
            vector<int> copy2 = test4;
            
            int result_quickselect = sort_nth_smallest_quickselect(copy1, n);
            int result_heap = sort_nth_smallest_heap(copy2, n);
            int expected = n;  // nth smallest is n
            
            std::cout << "  n=" << n << ": QuickSelect=" << result_quickselect 
                      << ", Heap=" << result_heap << ", Expected=" << expected;
            
            assert(result_quickselect == expected);
            assert(result_heap == expected);
            std::cout << " PASS\n";
        }
        std::cout << "\n";
    }
    
    // Test Case 5: Single element
    {
        vector<int> test5 = {42};
        std::cout << "Test 5: [42] (single element)\n";
        
        vector<int> copy1 = test5;
        vector<int> copy2 = test5;
        
        int result_quickselect = sort_nth_smallest_quickselect(copy1, 1);
        int result_heap = sort_nth_smallest_heap(copy2, 1);
        int expected = 42;
        
        std::cout << "  n=1: QuickSelect=" << result_quickselect 
                  << ", Heap=" << result_heap << ", Expected=" << expected;
        
        assert(result_quickselect == expected);
        assert(result_heap == expected);
        std::cout << " PASS\n\n";
    }
    
    // Test Case 6: Edge cases - invalid input
    {
        vector<int> test6 = {1, 2, 3};
        std::cout << "Test 6: Edge cases (invalid input)\n";
        
        vector<int> copy1 = test6;
        vector<int> copy2 = test6;
        
        // Test n = 0
        int result1 = sort_nth_smallest_quickselect(copy1, 0);
        int result2 = sort_nth_smallest_heap(copy2, 0);
        assert(result1 == -1 && result2 == -1);
        std::cout << "  n=0: Both return -1 PASS\n";
        
        // Test n > size
        copy1 = test6;
        copy2 = test6;
        result1 = sort_nth_smallest_quickselect(copy1, 5);
        result2 = sort_nth_smallest_heap(copy2, 5);
        assert(result1 == -1 && result2 == -1);
        std::cout << "  n=5 (>size): Both return -1 PASS\n\n";
    }
    
    // Performance comparison hint
    std::cout << "Performance comparison:\n";
    std::cout << "- QuickSelect: O(n) average, O(n^2) worst case\n";
    std::cout << "- Heap method: O(size * log n) - better when n is small\n";
    std::cout << "\nAll tests passed!\n";
}

// Main function to run tests
int main() {
    test_algorithms();
    return 0;
}
    

    