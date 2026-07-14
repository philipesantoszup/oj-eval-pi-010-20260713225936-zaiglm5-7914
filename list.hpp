#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}

        node(const T &value, node *p = nullptr, node *n = nullptr) 
            : data(new T(value)), prev(p), next(n) {}

        ~node() {
            if (data) {
                delete data;
                data = nullptr;
            }
        }
    };

protected:
    node *head;  // Sentinel node at the beginning
    node *tail;  // Sentinel node at the end
    size_t len;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        len++;
        return cur;
    }
    
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        len--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        node *ptr;
        const list *container;

    public:
        iterator(node *p = nullptr, const list *c = nullptr) 
            : ptr(p), container(c) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        /**
         * ++iter
         */
        iterator & operator++() {
            if (ptr == nullptr || ptr == container->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }
        
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        /**
         * --iter
         */
        iterator & operator--() {
            if (ptr == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }
        
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr == container->tail || ptr == container->head) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr == container->tail || ptr == container->head) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
        friend class const_iterator;
    };
    
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const node *ptr;
        const list *container;

    public:
        const_iterator(const node *p = nullptr, const list *c = nullptr) 
            : ptr(p), container(c) {}
        
        const_iterator(const iterator &it) 
            : ptr(it.ptr), container(it.container) {}

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        const_iterator & operator++() {
            if (ptr == nullptr || ptr == container->tail) {
                throw invalid_iterator();
            }
            ptr = ptr->next;
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        const_iterator & operator--() {
            if (ptr == nullptr || ptr->prev == container->head) {
                throw invalid_iterator();
            }
            ptr = ptr->prev;
            return *this;
        }
        
        const T & operator *() const {
            if (ptr == nullptr || ptr == container->tail || ptr == container->head) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }
        
        const T * operator ->() const {
            if (ptr == nullptr || ptr == container->tail || ptr == container->head) {
                throw invalid_iterator();
            }
            return ptr->data;
        }
        
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
    };
    
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        len = 0;
    }
    
    list(const list &other) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        len = 0;
        
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->data));
        }
    }
    
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }
    
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;
        
        clear();
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*(p->data));
        }
        return *this;
    }
    
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) throw container_is_empty();
        return *(head->next->data);
    }
    
    const T & back() const {
        if (empty()) throw container_is_empty();
        return *(tail->prev->data);
    }
    
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head->next, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }
    
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(tail, this);
    }
    
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return len == 0;
    }
    
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return len;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (!empty()) {
            pop_front();
        }
    }
    
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) {
            throw invalid_iterator();
        }
        node *newNode = new node(value);
        insert(pos.ptr, newNode);
        return iterator(newNode, this);
    }
    
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty()) throw container_is_empty();
        if (pos.container != this || pos.ptr == tail || pos.ptr == head) {
            throw invalid_iterator();
        }
        node *nextNode = pos.ptr->next;
        erase(pos.ptr);
        delete pos.ptr;
        return iterator(nextNode, this);
    }
    
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        insert(end(), value);
    }
    
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) throw container_is_empty();
        erase(--end());
    }
    
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        insert(begin(), value);
    }
    
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) throw container_is_empty();
        erase(begin());
    }
    
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (len <= 1) return;
        
        // Collect pointers to data for sorting (no default construction needed)
        T **arr = new T*[len];
        size_t idx = 0;
        node *p = head->next;
        while (p != tail) {
            arr[idx++] = p->data;
            p = p->next;
        }
        
        // Merge sort for pointers
        mergeSort(arr, 0, len);
        
        // Reassign data pointers to nodes in sorted order
        p = head->next;
        idx = 0;
        while (p != tail) {
            p->data = arr[idx++];
            p = p->next;
        }
        
        delete[] arr;
    }

private:
    void mergeSort(T **arr, size_t left, size_t right) {
        if (right - left <= 1) return;
        size_t mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid, right);
        merge(arr, left, mid, right);
    }
    
    void merge(T **arr, size_t left, size_t mid, size_t right) {
        T **tmp = new T*[right - left];
        size_t i = left, j = mid, k = 0;
        while (i < mid && j < right) {
            if (*(arr[i]) < *(arr[j]) || !(*(arr[j]) < *(arr[i]))) {
                // arr[i] <= arr[j], for stability use arr[i] first
                if (!(*(arr[i]) < *(arr[j])) && !(*(arr[j]) < *(arr[i]))) {
                    // Equal, keep original order
                    tmp[k++] = arr[i++];
                } else {
                    tmp[k++] = arr[i++];
                }
            } else {
                tmp[k++] = arr[j++];
            }
        }
        while (i < mid) tmp[k++] = arr[i++];
        while (j < right) tmp[k++] = arr[j++];
        for (i = left, k = 0; i < right; i++, k++) {
            arr[i] = tmp[k];
        }
        delete[] tmp;
    }

public:
    
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator<
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;
        
        node *p1 = head->next;
        node *p2 = other.head->next;
        
        while (p1 != tail && p2 != other.tail) {
            if (*(p1->data) < *(p2->data)) {
                p1 = p1->next;
            } else {
                node *next2 = p2->next;
                // Remove p2 from other
                other.erase(p2);
                // Insert p2 before p1 in this list
                insert(p1, p2);
                p2 = next2;
            }
        }
        
        // Move remaining nodes from other
        while (p2 != other.tail) {
            node *next2 = p2->next;
            other.erase(p2);
            insert(tail, p2);
            p2 = next2;
        }
    }
    
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (len <= 1) return;
        
        node *p = head;
        while (p != nullptr) {
            node *next = p->next;
            node *tmp = p->prev;
            p->prev = p->next;
            p->next = tmp;
            p = next;
        }
        
        // Swap head and tail
        node *tmp = head;
        head = tail;
        tail = tmp;
    }
    
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (len <= 1) return;
        
        node *p = head->next->next;
        while (p != tail) {
            if (*(p->data) == *(p->prev->data)) {
                node *next = p->next;
                erase(p);
                delete p;
                p = next;
            } else {
                p = p->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
