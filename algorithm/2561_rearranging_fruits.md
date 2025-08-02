# 2561. Rearranging fruits

## Description

You have two fruit baskets containing n fruits each. You are given two 0-indexed integer arrays basket1 and basket2 representing the cost of fruit in each basket. You want to make both baskets equal. To do so, you can use the following operation as many times as you want:

Chose two indices i and j, and swap the ith fruit of basket1 with the jth fruit of basket2.
The cost of the swap is min(basket1[i],basket2[j]).
Two baskets are considered equal if sorting them according to the fruit cost makes them exactly the same baskets.

Return the minimum cost to make both the baskets equal or -1 if impossible.

## Analysis

### Key Insights

1. **Common Elements**: There's no need to swap common elements between the two baskets, as they are already balanced.

2. **Different Elements**: We only need to focus on elements that appear in different quantities in each basket. Let's denote the excess elements in basket1 as set A and the excess elements in basket2 as set B.

3. **Feasibility Check**: For a solution to exist, every excess element must have a corresponding pair. Specifically, if an element appears more times in one basket, the difference must be even (so we can split the excess equally). If any element has an odd difference in total count, return -1.

### Optimization Strategy

Once we've identified the elements that need to be swapped, we have two approaches:

1. **Direct Swap**: Swap the minimum cost element from A with the maximum cost element from B. This ensures we minimize the cost for each swap operation.

2. **Pivot Strategy**: Use the globally minimum element as a pivot. First swap the pivot with the expensive element we want to move, then swap the pivot back. This requires two operations but might be cheaper in some cases.

### Cost Comparison

For each pair of elements that need to be swapped:

- **Direct cost**: `min(elementA, elementB)`
- **Pivot cost**: `2 * min_global_element`

We should choose the approach that gives the minimum cost for each pair. The pivot strategy is beneficial when `2 * min_global < min(elementA, elementB)`.

## Coding

```c++
class Solution {
public:
    long long minCost(vector<int>& basket1, vector<int>& basket2) {
        // Initialize the global minimum element value
        int global_min = INT_MAX;
        
        // Use frequency map to count occurrences of each element
        // Positive count means excess in basket1, negative means excess in basket2
        unordered_map<int, int> frequency_map;
        
        // Process basket1: increment count and track global minimum
        for (int fruit : basket1) {
            frequency_map[fruit]++;
            global_min = min(global_min, fruit);
        }
        
        // Process basket2: decrement count and track global minimum
        for (int fruit : basket2) {
            frequency_map[fruit]--;
            global_min = min(global_min, fruit);
        }
        
        // Collect elements that need to be swapped
        vector<int> elements_to_swap;
        
        // Check feasibility and collect excess elements
        for (auto [element, count] : frequency_map) {
            // If count is odd, impossible to balance the baskets
            if (count % 2 != 0) {
                return -1;
            }
            
            // Add half of the excess elements to the swap list
            // These represent the elements that need to move from one basket to another
            for (int i = 0; i < abs(count) / 2; ++i) {
                elements_to_swap.push_back(element);
            }
        }

        // The function `nth_element` will partially sort the elements such that the first half is the smallest
        // This optimizes the pairing strategy (pair smallest with largest)
        nth_element(elements_to_swap.begin(), 
                   elements_to_swap.begin() + elements_to_swap.size() / 2, 
                   elements_to_swap.end());
        
        // Calculate minimum cost using optimal strategy
        return accumulate(elements_to_swap.begin(), 
                         elements_to_swap.begin() + elements_to_swap.size() / 2, 
                         0LL,   // Start with zero total cost
                         [&](long long total_cost, int element) -> long long {
                             // For each element in the smaller half, choose the cheaper option:
                             // 1. Direct swap cost: element (paired with largest remaining)
                             // 2. Pivot strategy cost: 2 * global_min
                             return total_cost + min(2 * global_min, element);
                         });
    }
};
```

***If there are less A, there must be more B. Otherwise, the arrays will have different lengths.***
