#include <iostream>
#include "MyVector.h"
#include "MyList.h"
//#include <algorithm>

template <typename T>
void showContainerContent(T& container)
{
    std::cout << "Container content: ";
    for(auto iter = container.begin(); iter != container.end(); iter++)
    {
        if(iter != container.begin())
            std::cout << ", ";
        std::cout << (*iter);
    }    
    std::cout << std::endl;
}

template <typename T>
void testContainer(T& container, const std::string& containerName)
{
    std::cout << "Test " << containerName << std::endl;

    //заполняем контейнер
    for(int i = 0; i < 10; ++i)
        container.push_back(i);
    showContainerContent(container);
    std::cout << "Container size = " << container.getSize() << std::endl;

    //удаляем 3, 5 и 7 элементы
    auto iter = container.begin();
    std::advance(iter, 2);
    iter = container.erase(iter);
    std::advance(iter, 1);
    iter = container.erase(iter);
    std::advance(iter, 1);
    iter = container.erase(iter);
    showContainerContent(container);

    //добавляем 10 в начало
    container.insert(container.begin(), 10);
    showContainerContent(container);

    //добавляем 20 в серидину
    iter = container.begin();
    std::advance(iter, 4);
    container.insert(iter, 20);
    showContainerContent(container);

    //добавляем 30 в конец
    container.push_back(30);
    showContainerContent(container);
    
    //выводим содержимое контейнера, используя operator[]    
    for(size_t i = 0; i < container.getSize(); ++i)
        std::cout << "container[" << i << "] = " << container[i] << std::endl;
    std::cout << std::endl;
}

int main()
{
    MyList<int> l;
    testContainer(l, "MyList");
    MyVector<int> v;
    testContainer(v, "MyVector");
    return 0;
}

/*
int main()
{
    MyVector<std::string> v1;
    v1.push_back("1");
    v1.push_back("2");
    v1.push_back("3");
    MyVector<std::string> v2;
    v2.push_back("21");
    v2.push_back("22");
    v2.push_back("23");
    v2.push_back("24");
    std::string str = "666";
    v2.push_back(std::move(str));
    showContainerContent(v1);
    showContainerContent(v2);
    v1 = v2;
    showContainerContent(v1);
    showContainerContent(v2);
    v1 = std::move(v2);
    showContainerContent(v1);
    showContainerContent(v2);   
    std::cout << std::endl; 

    MyList<int> l;
    testContainer(l, "MyList");
    MyVector<int> v;
    testContainer(v, "MyVector");

    {        
        MyVector<std::string> c;
        c.push_back("sdff");
        c.push_back("azc");
        c.push_back("142");
        c.push_back("adf");
        c.push_back("zcvv");
        showContainerContent(c);
        std::sort(c.begin(), c.end());//только для вектора, тк у него итератор подходящего типа / определены нужные операторы
        showContainerContent(c);
        if(std::find(c.begin(), c.end(), "azc") != c.end())
            std::cout << "element found" << std::endl;
        else
            std::cout << "element not found" << std::endl;
        std::fill(++c.begin(), --c.end(), "la-la-la");
        showContainerContent(c);
    }    
    return 0;
}
*/