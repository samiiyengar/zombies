// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {} // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        for (InputIterator it = start; it != end; ++it) {
            data.push_back(*it);
        }
        updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        for (size_t i = data.size() - 1; static_cast<int>(i) >= 0; --i) {
            bubble_down(i);
        }
    } // updatePriorities()


    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    virtual void push(const TYPE & val) {
        data.push_back(val);
        if (data.size() > 1) {
            bubble_up(data.size() - 1);
        }
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        data[0] = data[data.size() - 1];
        data.pop_back();
        bubble_down(0);
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return data[0];
    } // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;

    void bubble_up(size_t index) {
        // reached the root
        if (index == 0) {
            return;
        }
        
        size_t parent_index = (index - 1) / 2;
        
        if (BaseClass::compare(data[parent_index], data[index])) {
            std::swap(data[parent_index], data[index]);
            bubble_up(parent_index);
        }
    }
    
    void bubble_down(size_t index) {
        size_t left_child_index = 2 * index + 1;
        size_t right_child_index = 2 * index + 2;
        
        // out of bounds check
        if (left_child_index >= data.size()) {
            return;
        }
        
        size_t current_index = index;
        if (BaseClass::compare(data[current_index], data[left_child_index])) {
            current_index = left_child_index;
        }
        
        
        if (right_child_index < data.size()) {
            if (BaseClass::compare(data[current_index], data[right_child_index])) {
                current_index = right_child_index;
            }
        }
        
        if (current_index != index) {
            std::swap(data[current_index], data[index]);
            bubble_down(current_index);
        }
    }
}; // BinaryPQ


#endif // BINARYPQ_H
