#include <cstdlib>
#include <initializer_list>
#include <stdexcept>

namespace jmch {
    template<typename T>
    class Array {
    public:
        Array();
        Array(int s);
        Array(const Array<T> &src);
        Array(const std::initializer_list<T> &l);
        const Array<T>& operator=(const Array<T> &src);
        const Array<T>& operator=(const std::initializer_list<T> &l);
        ~Array();

        void append(const T &v);
        void prepend(const T &v);
        void insert(int i, const T &v);
        inline T& operator[](int i);
        inline const T& operator[](int i) const;
        
        T pop(int i = -1);
        inline void clear();

        void sort();
        int find(const T &e) const;
        T quickSelect(int k) const;

        int length() const {return len;}
        int capacity() const {return cap;}
    private:
        void copy(const Array<T> &src);
        void copy(const std::initializer_list<T> &src);
        void grow();
        
        T *arr;
        int cap;
        int len;
        int front;
    };
};

inline int mod(int a, int b) {
    // Needed because the % operator returns negative numbers...
    int r = a % b;
    return (r < 0) ? r + b : r;
}

template<typename T>
void mergeSort(T *arr, int start, int end) {
    int size = end - start + 1;
    if (size < 2) return;
    
    // Partition and recursively sort.
    int mid = start + (size / 2);
    mergeSort(arr, start, mid - 1);
    mergeSort(arr, mid, end);

    // Collate partitions into another array.
    T merged[size];
    int i = start, j = mid, k = 0;
    
    // While there's data for both, compare directly.
    while (i < mid && j <= end) 
        merged[k++] = (arr[i] < arr[j]) ? arr[i++] : arr[j++];
    
    // When one runs out, copy the rest of the other.
    while (i < mid) merged[k++] = arr[i++];
    while (j <= end) merged[k++] = arr[j++];
    
    // Move contents of collated array into original.
    for (k = start; k <= end; k++) arr[k] = merged[k - start];
}

template<typename T>
jmch::Array<T>::Array() {    
    arr = new T[16];
    cap = 16;
    len = 0;
    front = 0;
}

template<typename T>
jmch::Array<T>::Array(int s) {
    len = s;
    cap = 16; while (cap < len) cap <<= 1;
    arr = new T[cap];
    front = 0;
}

template<typename T>
jmch::Array<T>::Array(const Array<T> &src) {
    copy(src);
}

template<typename T>
jmch::Array<T>::Array(const std::initializer_list<T> &l) {
    copy(l);
}

template<typename T>
const jmch::Array<T>& jmch::Array<T>::operator=(const Array<T> &src) {
    if (this != &src) {
        delete[] arr;
        copy(src);
    }
    return *this;
}

template<typename T>
const jmch::Array<T>& jmch::Array<T>::operator=(const std::initializer_list<T> &l) {
    delete[] arr;
    copy(l);
    return *this;
}

template<typename T>
jmch::Array<T>::~Array() {
    delete[] arr;
}

template<typename T>
void jmch::Array<T>::append(const T &v) {    
    if (len == cap) grow();
    operator[](++len - 1) = v;
}

template<typename T>
void jmch::Array<T>::prepend(const T &v) {    
    if (len == cap) grow();
    front = mod(front - 1, cap);
    arr[front] = v; len++;
}

template<typename T>
void jmch::Array<T>::insert(int i, const T &v) {
    if (i < 0) i += len;
    if (i < 0 || i >= len)
        throw std::out_of_range("jmch::Array Access OOR");

    if (i == 0) prepend(v);
    else {
        if (len++ == cap) cap *= 2;
        T* newArr = new T[cap];
        for (int j = 0, k = 0; k < len; k++) {
            if (k == i) newArr[k] = v;
            else newArr[k] = operator[](j++);
        }
        delete[] arr;
        arr = newArr;
        front = 0;
    }
}

template<typename T>
inline T& jmch::Array<T>::operator[](int i) {
    if (i < 0) i += len; // Translate negative indices to positive.
    if (i >= 0 && i < len) return arr[(front + i) % cap];
    throw std::out_of_range("jmch::Array Access OOR");
}

template<typename T>
inline const T& jmch::Array<T>::operator[](int i) const {
    if (i < 0) i += len; // Translate negative indices to positive.
    if (i >= 0 && i < len) return arr[(front + i) % cap];
    throw std::out_of_range("jmch::Array Access OOR");
}

template<typename T>
T jmch::Array<T>::pop(int i) {
    if (i < 0) i += len;
    T ret = operator[](i);

    if (i == 0 || i == len - 1) {
        if (i == 0) front = mod(front + 1, cap);
        if (--len <= cap / 4) {
            cap /= 2;
            T* newArr = new T[cap];
            for (int i = 0; i < len; i++) 
                newArr[i] = operator[](i);
            delete[] arr;
            arr = newArr;
            front = 0;
        }
    } else {
        if (len - 1 <= cap / 4) cap /= 2;
        T* newArr = new T[cap];
        for (int j = 0, k = 0; j < len; j++) 
            if (j != i) newArr[k++] = operator[](j);
        delete[] arr;
        arr = newArr;
        front = 0;
        len--;
    }

    return ret;
}

template<typename T>
inline void jmch::Array<T>::clear() {
    delete[] arr;
    arr = new T[16];
    cap = 16;
    len = 0;
    front = 0;
}

template<typename T>
void jmch::Array<T>::sort() {
    T* newArr = new T[cap];
    for (int i = 0; i < len; i++) {
        newArr[i] = operator[](i);
    }
    delete[] arr;
    arr = newArr;
    front = 0;

    mergeSort(arr, front, len - 1);
}

template<typename T>
int jmch::Array<T>::find(const T &e) const {
    int l = 0, r = len - 1, mid;
    while (l <= r) {
        mid = (l + r) / 2;
        if (operator[](mid) == e) return mid;
        else if (operator[](mid) < e) l = mid + 1;
        else r = mid - 1;
    }
    for (int i = 0; i < len; i++) {
        if (operator[](i) == e) return i;
    }
    return -1;
}

template<typename T>
T jmch::Array<T>::quickSelect(int k) const {
    if (k < 1) k = len + k + 1;
    if (k < 1 || k > len) throw std::out_of_range("QS OOR.");
    
    T pivot = operator[](rand() % len);
    Array<T> less, equal, greater;

    for (int i = 0; i < len; i++) {
        T x = operator[](i);

        if (x < pivot) less.append(x);
        else if (x == pivot) equal.append(x);
        else greater.append(x);
    }

    if (k <= less.len) return less.quickSelect(k);
    else if (k <= less.len + equal.len) return pivot;
    else return greater.quickSelect(k - less.len - equal.len);
}

template<typename T>
void jmch::Array<T>::copy(const Array<T> &src) {
    arr = new T[src.cap];
    cap = src.cap;
    len = src.len;
    front = 0;
    for (int i = 0; i < len; i++) {
        arr[i] = src[i];
    }
}

template<typename T>
void jmch::Array<T>::copy(const std::initializer_list<T> &l) {
    len = l.size();
    cap = 16; while (cap < len) cap <<= 1;
    arr = new T[cap];
    front = 0;
    int i = 0; for (const T &e : l) {
        arr[i++] = e;
    }
}

template<typename T>
void jmch::Array<T>::grow() {
    cap *= 2;
    T* newArr = new T[cap];
    for (int i = 0; i < len; i++) {
        newArr[i] = operator[](i);
    }
    delete[] arr;
    arr = newArr;
    front = 0;
}
