#ifndef _MY_VECTOR_
#define _MY_VECTOR_
#include <string>

template <typename T>
class MyVectorIterator
{
    T* ptr;
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    MyVectorIterator(T* ptr) : ptr{ptr} {} 
    //определены не все операторы для random_access_iterator_tag, пытался добится, чтобы работали std::sort() и str::find()   
    reference operator*() {return *ptr;}
    const reference operator*() const {return *ptr;}
    pointer operator->() {return ptr;}
    MyVectorIterator& operator++() {ptr++; return *this;}
    MyVectorIterator operator++(int) {auto iter = *this; ptr++; return iter;}
    MyVectorIterator& operator--() {ptr--; return *this;}
    MyVectorIterator operator--(int) {auto iter = *this; ptr--; return iter;}
    MyVectorIterator& operator+=(difference_type n) {ptr += n; return *this;}
    MyVectorIterator& operator-=(difference_type n) {ptr -= n; return *this;}
    MyVectorIterator operator+(difference_type n) {auto iter = *this; iter += n; return iter;}
    MyVectorIterator operator-(difference_type n) {auto iter = *this; iter -= n; return iter;}
    friend bool operator==(const MyVectorIterator& lhs, const MyVectorIterator& rhs) {return lhs.ptr == rhs.ptr;}
    friend bool operator!=(const MyVectorIterator& lhs, const MyVectorIterator& rhs) {return lhs.ptr != rhs.ptr;}
    friend difference_type operator-(const MyVectorIterator& lhs, const MyVectorIterator& rhs) {return (lhs.ptr - rhs.ptr);}
    friend bool operator<(const MyVectorIterator& lhs, const MyVectorIterator& rhs) {return (lhs - rhs) < 0;}
};

template <typename T>
class MyVector
{    
    const float allocCoef = 1.5f;
    const size_t maxSize = 1000;
    size_t realSize;
    size_t size;
    T* values;
    template <typename U> void _insert(size_t pos, U&& value);
public:
    using iterator = MyVectorIterator<T>;
    //using const_iterator = MyVectorIterator<const T>;    
    MyVector() : realSize{0}, size{0}, values{nullptr} {}
    MyVector(const MyVector<T>& other);
    MyVector(MyVector<T>&& other);
    ~MyVector() {delete[] this->values;}
    MyVector<T>& operator=(const MyVector<T>& other);
    MyVector<T>& operator=(MyVector<T>&& other);
    const T& operator[](size_t pos) const;
    T& operator[](size_t pos);
    size_t getSize() const;
    bool empty() const;    
    void insert(size_t pos, const T& value);
    void insert(size_t pos, T&& value);        
    iterator insert(iterator pos, const T& value);
    iterator insert(iterator pos, T&& value);
    // void push_front(const T& value);
    // void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);
    void erase(size_t pos);
    iterator erase(iterator pos);
    void clear();
    void swap(MyVector<T>& other);    
    iterator begin() {return iterator(values);}
    iterator end() {return iterator(values + size);}
    // const_iterator cbegin() const {return const_iterator(values);}
    // const_iterator cend() const {return const_iterator(values + size);}
};

template <typename T>
MyVector<T>::MyVector(const MyVector<T>& other)
{
    this->realSize = other.realSize;
    this->size = other.size;
    this->values = new T[realSize];
    for(size_t i = 0; i < other.size; i++)
        this->values[i] = other.values[i];    
}

template <typename T>
MyVector<T>::MyVector(MyVector&& other) : realSize{0}, size{0}, values{nullptr}
{
    this->swap(other);    
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other)
{
    if(this != &other)
    {
        this->realSize = other.realSize;
        this->size = other.size;
        delete[] values;
        this->values = new T[realSize];
        for(size_t i = 0; i < other.size; i++)
            this->values[i] = other.values[i]; 
    }
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other)
{
    if(this != &other)
    {
        this->clear();
        this->swap(other);
    }
    return *this;
}

template <typename T>
const T& MyVector<T>::operator[](std::size_t pos) const
{
    if(pos >= this->size)
        throw std::out_of_range(std::string("max pos = ") + std::to_string(this->size - 1));        
    return this->values[pos];
}

template <typename T>
T& MyVector<T>::operator[](std::size_t pos)
{
    if(pos >= this->size)
        throw std::out_of_range(std::string("max pos = ") + std::to_string(this->size - 1));        
    return this->values[pos];
}

template <typename T>
size_t MyVector<T>::getSize() const
{
    return this->size;
}

template <typename T>
bool MyVector<T>::empty() const
{
    return (this->size == 0);
}

template <typename T>
template <typename U>
void MyVector<T>::_insert(std::size_t pos, U&& value)
{         
    if(pos > this->size)
        throw std::out_of_range(std::string("max pos = ") + std::to_string(this->size));        
    
    if(this->size == this->realSize)
    {        
        if(this->realSize == this->maxSize)
            throw std::length_error(std::string("max myVector's size = ") + std::to_string(this->maxSize));

        this->realSize = static_cast<size_t>(std::ceil((this->realSize + 1) * this->allocCoef));
        this->realSize = std::min(this->realSize, this->maxSize);
        T* newValues = new T[this->realSize];

        for(size_t i = 0; i <= this->size; i++)
        {
            if(i < pos)
                newValues[i] = std::move(this->values[i]);
            else if(i == pos)
                newValues[i] = std::forward<U>(value);
            else
                newValues[i] = std::move(this->values[i - 1]);
        }
        
        delete[] this->values;   
        this->values = newValues;
        this->size++;
    }
    else
    {
        for(size_t i = size; i > pos; i--)
        {
            this->values[i] = std::move(this->values[i - 1]);
        }
        this->values[pos] = std::forward<U>(value);
        this->size++;
    }    
}

template <typename T>
void MyVector<T>::insert(std::size_t pos, const T& value)
{
    this->_insert(pos, value);
}

template <typename T>
void MyVector<T>::insert(std::size_t pos, T&& value)
{
    this->_insert(pos, std::move(value));
}

template <typename T>
typename MyVector<T>::iterator MyVector<T>::insert(iterator iter, const T& value)
{
    size_t pos = std::distance(this->begin(), iter);
    this->_insert(pos, value);
    return iterator(this->values + pos);//обновляем итератор на случай, если перевыделяли память
}

template <typename T>
typename MyVector<T>::iterator MyVector<T>::insert(iterator iter, T&& value)
{
    size_t pos = std::distance(this->begin(), iter);
    this->_insert(pos, std::move(value));
    return iterator(this->values + pos);//обновляем итератор на случай, если перевыделяли память
}

template <typename T>
void MyVector<T>::erase(std::size_t pos)
{
    if(pos >= this->size)
        throw std::out_of_range(std::string("max pos = ") + std::to_string(this->size - 1));        

    this->size--;
    for(size_t i = pos; i < this->size; i++)
    {
        this->values[i] = std::move(this->values[i + 1]);
    }
}

template <typename T>
typename MyVector<T>::iterator MyVector<T>::erase(iterator iter)
{
    size_t pos = std::distance(this->begin(), iter);
    this->erase(pos);
    return iter;
}

// template <typename T>
// void MyVector<T>::push_front(const T& value)
// {
//     this->_insert(0, value);
// }

// template <typename T>
// void MyVector<T>::push_front(T&& value)
// {
//     this->insert(0, std::move(value));
// }

template <typename T>
void MyVector<T>::push_back(const T& value)
{
    this->_insert(this->size, value);
}

template <typename T>
void MyVector<T>::push_back(T&& value)
{
    this->_insert(this->size, std::move(value));
}

template <typename T>
void MyVector<T>::clear()
{
    this->size = 0;
    this->realSize = 0;
    delete[] this->values;
    this->values = nullptr;
}

template <typename T>
void MyVector<T>::swap(MyVector<T>& other)
{
    if(this != &other)
    {
        std::swap(this->size, other.size);
        std::swap(this->realSize, other.realSize);
        std::swap(this->values, other.values);
    }
}

#endif