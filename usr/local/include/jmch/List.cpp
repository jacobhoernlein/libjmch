#ifndef JMCH_LIST_CPP
#define JMCH_LIST_CPP

#include <cstdlib>
#include <stdexcept>
#include <vector>

namespace jmch {

    /// @brief A circular dynamically allocated list structure. Supports pushing/popping from both ends and negative indexing.
    /// @tparam T The type that the list will hold.
    template<typename T>
    class List {
    public:
        class iterator;
        class const_iterator;

        List() {cap = 0; len = 0;}
        List(int s);
        List(const List<T> &src) {cap = 0; len = 0; copy(src);}
        List(const std::initializer_list<T> &l) {cap = 0; len = 0; copy(l);}
        List(const std::vector<T> &v) {cap = 0; len = 0; copy(v);}
        const List<T>& operator=(const List<T> &src) {if (&src != this) copy(src); return *this;}
        const List<T>& operator=(const std::initializer_list<T> &l) {copy(l); return *this;}
        const List<T>& operator=(const std::vector<T> &v) {copy(v); return *this;}
        ~List() {if (cap >= 16) delete[] arr;}

        /// @brief Adds an item to the back of the list.
        /// @param v The item to add.
        void pushBack(const T &v);
        
        /// @brief Adds an item to the front of the list.
        /// @param v The item to add.
        void pushFront(const T &v);
        
        /// @brief Returns and removes the item at the back of the list.
        /// @return The item.
        T popBack();
        
        /// @brief Returns and removes the item at the front of the list.
        /// @return The item.
        T popFront();

        /// @brief Returns a reference to the value at index i. Negative indexing supported.
        /// @param i The index to return a reference to.
        /// @return A reference to the item at that index.
        /// @exception std::out_of_range if accessing an element out of range.
        inline T& operator[](int i);
        inline const T& operator[](int i) const;
        
        /// @brief Clears the contents of the list, starting over with length 0.
        void clear() {if (cap >= 16) delete[] arr; cap = 0; len = 0;}
        
        /// @brief Reverses the list.
        void reverse();

        /// @brief Finds and returns the index of e, if it exists in the list.
        /// @param e The element to search for.
        /// @return The index of e, or -1 if it does not exist.
        int find(const T &e) const;
        
        /// @brief Finds and returns the kth smallest or largest (if negative) element. 
        /// @param k The number element desired. From 1 to length() or -1 to -length(), inclusive.
        /// @return A copy of the found element.
        /// @exception std::out_of_range If the absolute value of the argument is greater than length().
        T select(int k) const;

        /// @brief Returns the length of the list.
        /// @return The length of the list.
        int length() const {return len;}
        
        /// @brief Returns the capacity of the list.
        /// @return The capacity of the list.
        int capacity() const {return cap;}

        /// @brief Returns if the list is empty.
        /// @return True or false.
        bool empty() const {return !len;}

        /// @brief Returns an iterator to the front of the list.
        /// @return The iterator.
        iterator begin() {return iterator(this, 0);}
        const_iterator begin() const {return const_iterator(this, 0);}
        const_iterator cbegin() const {return const_iterator(this, 0);}
        
        /// @brief Returns an iterator to the end of the list.
        /// @return The iterator.
        iterator end() {return iterator(this, len);}
        const_iterator end() const {return const_iterator(this, len);}
        const_iterator cend() const {return const_iterator(this, len);}

        operator std::vector<T>() const {return std::vector<T>(begin(), end());}
    private:
        void copy(const List<T> &src);
        void copy(const std::initializer_list<T> &src);
        void copy(const std::vector<T> &src);
        void grow();
        void shrink();
        
        T *arr;
        int cap;
        int len;
        int front;
    };
}

template<typename T>
jmch::List<T>::List(int s) {
    len = s;
    cap = 16; while (cap < len) cap <<= 1;
    arr = new T[cap];
    front = 0;
}

template<typename T>
void jmch::List<T>::pushBack(const T &v) {    
    if (len == cap || cap < 16) grow();
    operator[](++len - 1) = v;
}

template<typename T>
void jmch::List<T>::pushFront(const T &v) {    
    if (len == cap || cap < 16) grow();
    front = (front - 1) % cap;
    if (front < 0) front += cap;
    arr[front] = v; len++;
}

template<typename T>
T jmch::List<T>::popBack() {    
    T ret = operator[](-1);
    if (--len <= cap / 4) shrink();
    return ret;
}

template<typename T>
T jmch::List<T>::popFront() {    
    T ret = operator[](0);
    front = (front + 1) % cap;
    if (--len <= cap / 4) shrink();
    return ret;
}

template<typename T>
inline T& jmch::List<T>::operator[](int i) {
    if (i < 0) i += len; // Translate negative indices to positive.
    if (i >= 0 && i < len) return arr[(front + i) % cap];
    else throw std::out_of_range("jmch::List Access OOR");
}

template<typename T>
inline const T& jmch::List<T>::operator[](int i) const {
    if (i < 0) i += len; // Translate negative indices to positive.
    if (i >= 0 && i < len) return arr[(front + i) % cap];
    else throw std::out_of_range("jmch::List Access OOR");
}

template<typename T>
void jmch::List<T>::reverse() {
    if (len < 2) return;
    List<T> newArr(len);
    for (int i = 0; i < len; i++) 
        newArr[-1 - i] = operator[](i);
    delete[] arr; copy(newArr);
}

template<typename T>
int jmch::List<T>::find(const T &e) const {
    if (!len) return -1;
    if (len == 1) return (operator[](1) == e) ? 0 : -1;
    
    int l = 0, r = len - 1, mid;
    while (l <= r) {
        mid = (l + r) / 2;
        if (operator[](mid) == e) return mid;
        else if (operator[](mid) < e) l = mid + 1;
        else r = mid - 1;
    }
    for (int i = 0; i < len; i++) 
        if (operator[](i) == e) return i;
    return -1;
}

template<typename T>
T jmch::List<T>::select(int k) const {
    if (k < 1) k = len + k + 1;
    if (k < 1 || k > len) throw std::out_of_range("jmch::List Access OOR");
    
    T pivot = operator[](rand() % len);
    List<T> less, equal, greater;

    for (int i = 0; i < len; i++) {
        T x = operator[](i);

        if (x < pivot) less.pushBack(x);
        else if (x == pivot) equal.pushBack(x);
        else greater.pushBack(x);
    }

    if (k <= less.len) return less.select(k);
    else if (k <= less.len + equal.len) return pivot;
    else return greater.select(k - less.len - equal.len);
}

template<typename T>
void jmch::List<T>::copy(const List<T> &src) {
    if (cap >= 16) delete[] arr;
    cap = src.cap;
    len = src.len;
    arr = new T[src.cap];
    front = 0;
    for (int i = 0; i < len; i++)
        arr[i] = src[i];
}

template<typename T>
void jmch::List<T>::copy(const std::initializer_list<T> &l) {
    if (cap >= 16) delete[] arr;
    len = l.size();
    cap = 16; while (cap < len) cap <<= 1;
    arr = new T[cap];
    front = 0;
    int i = 0; for (const T &e : l)
        arr[i++] = e;
}

template<typename T>
void jmch::List<T>::copy(const std::vector<T> &v) {
    if (cap >= 16) delete[] arr;
    len = v.size();
    cap = 16; while (cap < len) cap <<= 1;
    arr = new T[cap];
    front = 0;
    int i = 0; for (const T &e : v)
        arr[i++] = e;
}

template<typename T>
void jmch::List<T>::grow() {
    // Capacity < 16 implies arr is not yet 
    // initialized (via default constructor)
    if (cap < 16) {
        cap = 16;
        arr = new T[cap];
        front = 0;
    } else {
        T* newArr = new T[cap * 2];
        for (int i = 0; i < len; i++) 
            newArr[i] = operator[](i);
        delete[] arr;
        arr = newArr;
        cap *= 2;
        front = 0;
    }
}

template<typename T>
void jmch::List<T>::shrink() {
    // Ensure cap never goes below 16.
    T* newArr = new T[(cap >= 32) ? cap / 2 : cap];
    for (int i = 0; i < len; i++) 
        newArr[i] = operator[](i);
    delete[] arr;
    arr = newArr;
    if (cap >= 32) cap /= 2;
    front = 0;
}

/// @brief An iterator that points to an element in the list.
template<typename T>
class jmch::List<T>::iterator : public std::iterator<std::random_access_iterator_tag, T> {
public:    
    friend class jmch::List<T>::const_iterator;
    
    iterator() {list = nullptr; i = 0;}
    iterator(List<T> *l, int j) {list = l; i = j;}

    iterator& operator++() {i++; return *this;}
    iterator operator++(int)
        {iterator it = *this; ++(*this); return it;} 
    iterator& operator--() {i--; return *this;};
    iterator operator--(int)
        {iterator it = *this; --(*this); return it;}

    iterator operator+(int n) const
        {iterator ret = *this; ret.i += n; return ret;}
    friend iterator operator+(int n, const iterator &it)
        {return it + n;}
    iterator operator-(int n) const
        {iterator ret = *this; ret.i -= n; return ret;}
    iterator operator-(const iterator &r) const 
        {iterator ret = *this; ret.i -= r.i; return ret;}
    iterator& operator+=(int n) {i += n; return *this;}
    iterator& operator-=(int n) {i -= n; return *this;}

    bool operator<(const iterator &r) const {return (i < r.i);}
    bool operator>(const iterator &r) const {return (r < *this);}
    bool operator<=(const iterator &r) const {return !(r < *this);}
    bool operator>=(const iterator &r) const {return !(*this < r);}
    bool operator==(const iterator& r) const {return (i == r.i);}
    bool operator !=(const iterator &r) const {return !(*this == r);}
    operator int() const {return i;}

    T& operator*() const {return list->operator[](i);}
    T* operator->() const {return &(list->operator[](i));}
protected:
    List<T> *list;
    int i;
};

/// @brief A constant iterator to the list.
template<typename T>
class jmch::List<T>::const_iterator : public std::iterator<std::random_access_iterator_tag, T> {
public:   
    const_iterator() {list = nullptr; i = 0;}
    const_iterator(const iterator &r) {list = r.list; i = r.i;}
    const_iterator(const List<T> *l, int j) {list = l; i = j;}
    
    const_iterator& operator++() {i++; return *this;}
    const_iterator operator++(int)
        {const_iterator it = *this; ++(*this); return it;} 
    const_iterator& operator--() {i--; return *this;};
    const_iterator operator--(int)
        {const_iterator it = *this; --(*this); return it;}

    const_iterator operator+(int n) const
        {const_iterator ret = *this; ret.i += n; return ret;}
    friend const_iterator operator+(int n, const const_iterator &it)
        {return it + n;}
    const_iterator operator-(int n) const
        {const_iterator ret = *this; ret.i -= n; return ret;}
    const_iterator operator-(const const_iterator &r) const 
        {const_iterator ret = *this; ret.i -= r.i; return ret;}
    const_iterator& operator+=(int n) {i += n; return *this;}
    const_iterator& operator-=(int n) {i -= n; return *this;}

    bool operator<(const const_iterator &r) const {return (i < r.i);}
    bool operator>(const const_iterator &r) const {return (r < *this);}
    bool operator<=(const const_iterator &r) const {return !(r < *this);}
    bool operator>=(const const_iterator &r) const {return !(*this < r);}
    bool operator==(const const_iterator& r) const {return (i == r.i);}
    bool operator !=(const const_iterator &r) const {return !(*this == r);}
    operator int() const {return i;}

    const T& operator*() const {return list->operator[](i);}
    const T* operator->() const {return &(list->operator[](i));}
protected:
    const List<T> *list;
    int i;
};

#endif
