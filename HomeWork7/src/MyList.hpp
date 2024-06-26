#include <string>

template <typename T>
struct MyListNode
{
    T data;
    MyListNode* prev;
    MyListNode* next;
    MyListNode(const T& data, MyListNode* prev, MyListNode* next) : data{data}, prev{prev}, next{next} {}
    MyListNode(T&& data, MyListNode* prev, MyListNode* next) : data{std::move(data)}, prev{prev}, next{next} {}
};

template <typename T> class MyList;

template <typename T>
class MyListIterator
{
    MyListNode<T>* node;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    MyListIterator(MyListNode<T>* node) : node{node} {}     
    reference operator*() {return node->data;}
    pointer operator->() {return &node->data;}
    MyListIterator& operator++() {node = node->next; return *this;}
    MyListIterator operator++(int) {auto iter = *this; node = node->next; return iter;}
    MyListIterator& operator--() {node = node->prev; return *this;}
    MyListIterator operator--(int) {auto iter = *this; node = node->prev; return iter;}    
    friend bool operator==(const MyListIterator& lhs, const MyListIterator& rhs) {return lhs.node == rhs.node;}
    friend bool operator!=(const MyListIterator& lhs, const MyListIterator& rhs) {return lhs.node != rhs.node;}    
    friend class MyList<T>;
};

template <typename T>
class MyList
{
    using iterator = MyListIterator<T>;
    //using const_iterator = MyListIterator<const T>;
    const size_t maxSize = 1000;
    MyListNode<T>* root;
    MyListNode<T>* tail;
    size_t size;

    template <typename U> iterator _insert(iterator pos, U&& value);
public:      
    MyList();
    MyList(const MyList<T>& other);
    MyList(MyList<T>&& other);
    ~MyList();
    MyList<T>& operator=(const MyList<T>& other);
    MyList<T>& operator=(MyList<T>&& other);    
    const T& operator[](size_t pos) const;
    T& operator[](size_t pos);
    size_t getSize() const;
    bool empty() const;        
    iterator insert(iterator pos, const T& value);
    iterator insert(iterator pos, T&& value);    
    void push_back(const T& value);
    void push_back(T&& value);    
    iterator erase(iterator pos);
    void clear();
    void swap(MyList<T>& other);    
    iterator begin() {return iterator(root);}
    iterator end() {return iterator(tail);}
    // const_iterator cbegin() const {return const_iterator(root);}
    // const_iterator cend() const {return const_iterator(tail);}
};

template <typename T>
MyList<T>::MyList() : root{nullptr}, tail{nullptr}, size{0}
{
    this->tail = new MyListNode<T>(T{}, nullptr, nullptr);
    this->root = this->tail;
}

template <typename T>
MyList<T>::MyList(const MyList<T>& other) : MyList()
{
    if(other.size > 0)
    {
        MyListNode<T>* node = nullptr;
        for(MyListNode<T>* otherNode = other.root; otherNode != other.tail; otherNode = otherNode->next)
        {
            node = new MyListNode<T>(otherNode->data, node, nullptr);            
            if(otherNode == other.root)
                this->root = node;
            else
                node->prev->next = node;            
        }
        this->size = other.size;
        node->next = this->tail;
        this->tail->prev = node;

        // MyListNode<T>* node = nullptr;
        // for(auto iter = other.cbegin(); iter != other.cend(); iter++)
        // {
        //     node = new MyListNode<T>(*iter, node, nullptr);
        //     this->size++;
        //     if(this->size == 1)
        //         this->root = node;
        //     else
        //         node->prev->next = node;
        //     if(this->size == other.size)
        //     {
        //         node->next = this->tail;
        //         this->tail->prev = node;
        //     }
        // }
    }
}

template <typename T>
MyList<T>::MyList(MyList<T>&& other) : MyList()
{
    this->swap(other);
}

template <typename T>
MyList<T>::~MyList()
{
    this->clear();
    delete this->tail;    
}

template <typename T>
MyList<T>& MyList<T>::operator=(const MyList<T>& other)
{
    if(this != &other)
    {
        this->clear();
        MyList<T> temp = other;
        this->swap(temp);
    }
    return *this;
}

template <typename T>
MyList<T>& MyList<T>::operator=(MyList<T>&& other)
{
    if(this != &other)
    {
        this->clear();
        this->swap(other);
    }
    return *this;
}

template <typename T>
const T& MyList<T>::operator[](size_t pos) const
{
    if(pos >= this->size)
        throw std::out_of_range(std::string("max pos = ") + std::to_string(this->size - 1));
 
    auto iter = this->begin();
    while(pos)
    {
        iter++;
        pos--;
    }
    return *iter;
}

template <typename T>
T& MyList<T>::operator[](size_t pos)
{
    if(pos >= this->size)
        throw std::out_of_range(std::string("max pos = ") + std::to_string(this->size - 1));
    auto iter = this->begin();
    while(pos)
    {
        iter++;
        pos--;
    }
    return *iter;
}

template <typename T>
size_t MyList<T>::getSize() const
{
    return this->size;
}

template <typename T>
bool MyList<T>::empty() const
{
    return (this->size == 0);
}

template <typename T>
template <typename U>
typename MyList<T>::iterator MyList<T>::_insert(iterator pos, U&& value)
{
    if(this->size == this->maxSize)
            throw std::length_error(std::string("max myList's size = ") + std::to_string(this->maxSize));
    
    MyListNode<T>* newNode = new MyListNode<T>(std::forward<U>(value), pos.node->prev, pos.node);
    if(pos != this->begin())
        pos.node->prev->next = newNode;
    else
        this->root = newNode;
    pos.node->prev = newNode;
    this->size++;
    return iterator(newNode);    
}

template <typename T>
typename MyList<T>::iterator MyList<T>::insert(iterator pos, const T& value)
{
    return this->_insert(pos, value);
}

template <typename T>
typename MyList<T>::iterator MyList<T>::insert(iterator pos, T&& value)
{
    return this->_insert(pos, std::move(value));
}

template <typename T>
void MyList<T>::push_back(const T& value)
{
    this->_insert(this->end(), value);
}

template <typename T>
void MyList<T>::push_back(T&& value)
{
    this->_insert(this->end(), std::move(value));
}

template <typename T>
typename MyList<T>::iterator MyList<T>::erase(iterator pos)
{
    if(pos == this->end())
        throw std::out_of_range("erasing end() iterator");
    
    MyListNode<T>* nextNode = pos.node->next;
    if(pos != this->begin())
        pos.node->prev->next = nextNode;
    else
        this->root = nextNode;
    nextNode->prev = pos.node->prev;        
    delete pos.node;    
    this->size--;
    return iterator(nextNode);
}

template <typename T>
void MyList<T>::clear()
{
    if(this->size > 0)
    {
        auto iter = this->begin();
        while(iter != this->end())
        {
            ++iter;
            delete iter.node->prev;
        }
        this->tail->prev = nullptr;
        this->root = tail;
        this->size = 0;        
    }
}

template <typename T>
void MyList<T>::swap(MyList<T>& other)
{
    if(this != &other)
    {
        std::swap(this->root, other.root);
        std::swap(this->tail, other.tail);
        std::swap(this->size, other.size);
    }
}