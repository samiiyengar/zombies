// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>
#include <set>

// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
    public:
        // TODO: After you add add one extra pointer (see below), be sure to
        // initialize it here.
        explicit Node(const TYPE &val)
            : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
        {}

        // Description: Allows access to the element at that Node's position.
        // There are two versions, getElt() and a dereference operator, use
        // whichever one seems more natural to you.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        // The following line allows you to access any private data members of this
        // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
        // statement in PairingPQ's add_node() function).
        friend PairingPQ;

    private:
        TYPE elt;
        Node *child;
        Node *sibling;
        // if the node is a child, it points to parent
        // if it is a right sibling, it points to left sibling
        Node *parent;
    }; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        root = nullptr;
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        InputIterator it = start;
        while (it != end) {
            push(*it);
            it++;
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        BaseClass{ other.compare } {
        copy_pairing_pq(other);
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
        // self assignment
        if (this == &rhs) {
            return *this;
        }
        delete_pairing_pq();
        copy_pairing_pq(rhs);
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        delete_pairing_pq();
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // tree is empty
        if (root == nullptr) {
            return;
        }

        std::deque<Node*> queue;
        queue.push_back(root);
        root = nullptr;

        while (!queue.empty()) {
            Node* current = queue.front();
            queue.pop_front();

            if (current->child) {
                current->child->parent = nullptr;
                queue.push_back(current->child);
                current->child = nullptr;
            }

            if (current->sibling) {
                current->sibling->parent = nullptr;
                queue.push_back(current->sibling);
                current->sibling = nullptr;
            }
            root = meld(root, current);
        }
    } // updatePriorities()

    // Description: Add a new element to the priority_queue. This is almost done,
    //              in that you should implement push functionality in the addNode()
    //              function, and this function should call addNode().
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        if (root == nullptr) {
            return;
        }
        count--;
        Node *node = root->child;
        delete root;
        if (node == nullptr || node->sibling == nullptr) {
            if (node) {
                node->parent = nullptr;
            }
            root = node;
            return;
        }

        node->sibling->parent = nullptr;

        struct Record {
            Node *merged;
            Node *next_sibling;
        };

        std::deque<Record> stack;

        while (node != nullptr) {
            Node *node1 = nullptr;
            Node *node2 = nullptr;
            Node *next_sibling = nullptr;
            node1 = node;
            node1->parent = nullptr;
            if (node->sibling) {
                node2 = node->sibling;
                node2->parent = nullptr;
                if (node2->sibling) {
                    next_sibling = node2->sibling;
                    next_sibling->parent = nullptr;
                }
            }
            node1->sibling = nullptr;
            if (node2 != nullptr) {
                node2->sibling = nullptr;
            }
            Record rec;
            rec.merged = meld(node1, node2);
            rec.next_sibling = next_sibling;
            stack.push_front(rec);
            node = next_sibling;
        }

        // merge right to left
        Node *prev = !stack.empty() ? stack.front().next_sibling : nullptr;
        while (!stack.empty()) {
            Record rec = stack.front();
            stack.pop_front();
            prev = meld(rec.merged, prev);
        }
        root = prev;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return root->getElt();
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return count;
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return root == nullptr;
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
        if (node == root) {
            if (this->compare(node->getElt(), new_value)) {
                node->elt = new_value;
            }
            return;
        }
        if (node->parent->child == node) {
            if (node->sibling) {
                node->parent->child = node->sibling;
                node->sibling->parent = node->parent;
            } else {
                node->parent->child = nullptr;
            }
        }
        else {
            if (node->sibling) {
                node->parent->sibling = node->sibling;
                node->sibling->parent = node->parent;
            } else {
                node->parent->sibling = nullptr;
            }
        }
        node->elt = new_value;
        node->parent = nullptr;
        node->sibling = nullptr;
        root = meld(root, node);
    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE & val) {
        count++;
        Node *node = new Node(val);
        root = meld(root, node);
        return node;
    } // addNode()

    Node* root_node() {
        return root;
    }


private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
    Node *root = nullptr;
    size_t count = 0;

    Node *meld(Node *node1, Node *node2) {
        if (node1 == nullptr) {
            return node2;
        }
        if (node2 == nullptr) {
            return node1;
        }
        // maintain heap invariant
        if (!this->compare(node1->getElt(), node2->getElt())) {
            add_child(node1, node2);
            return node1;
        }
        else {
            add_child(node2, node1);
            return node2;
        }
    }

    void add_child(Node *parent, Node *child) {
        if (parent->child == nullptr) {
            parent->child = child;
            child->parent = parent;
        }
        else {
            Node *sibling = parent->child;

            child->sibling = sibling;
            sibling->parent = child;

            parent->child = child;
            child->parent = parent;
        }
    }

    void copy_pairing_pq(const PairingPQ &pq) {
        // tree is empty
        if (pq.root == nullptr) {
            return;
        }
        count = pq.count;
        std::deque<Node*> other;
        std::deque<Node*> ours;
        other.push_back(pq.root);
        root = new Node(pq.root->getElt());
        ours.push_back(root);
        while (!other.empty()) {
            Node *otherNode = other.front();
            other.pop_front();
            Node *ourNode = ours.front();
            ours.pop_front();
            if (otherNode->child) {
                other.push_back(otherNode->child);
                ourNode->child = new Node(otherNode->child->getElt());
                ourNode->child->parent = ourNode;
                ours.push_back(ourNode->child);
            }
            if (otherNode->sibling) {
                other.push_back(otherNode->sibling);
                ourNode->sibling = new Node(otherNode->sibling->getElt());
                ourNode->sibling->parent = ourNode;
                ours.push_back(ourNode->sibling);
            }
        }
    }

    void delete_pairing_pq() {
        // tree is empty
        if (root == nullptr) {
            return;
        }
        std::deque<Node*> queue;
        queue.push_back(root);

        while (!queue.empty()) {
            Node *node = queue.front();
            queue.pop_front();
            if (node->child) {
                queue.push_back(node->child);
            }
            if (node->sibling) {
                queue.push_back(node->sibling);
            }
            delete node;
        }
        root = nullptr;
    }
};

#endif // PAIRINGPQ_H
