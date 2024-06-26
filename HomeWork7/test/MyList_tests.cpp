#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MyList.hpp"
#include <algorithm>

TEST(MyList, create)
{
    MyList<int> list;
    EXPECT_EQ(list.getSize(), size_t{0});
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.begin(), list.end());
}

TEST(MyList, push_back)
{
    MyList<int> list;
    ASSERT_TRUE(list.empty());
    list.push_back(10);
    ASSERT_EQ(list.getSize(), size_t{1});
    EXPECT_EQ(*list.begin(), 10);
    list.push_back(20);
    ASSERT_EQ(list.getSize(), size_t{2});
    EXPECT_EQ(*(--list.end()), 20);
}

TEST(MyList, get_item)
{
    MyList<int> list;
    for(int i = 0; i < 10; ++i)
        list.push_back(i);
    for(size_t i = 0; i <= list.getSize(); ++i)
    {
        if(i < list.getSize())
            EXPECT_EQ(list[i], static_cast<int>(i));
        else
            ASSERT_THROW(list[i], std::out_of_range);
    }
}

TEST(MyList, insert)
{
    MyList<int> list;
    ASSERT_TRUE(list.empty());
    list.insert(list.begin(), 0);                  //{0}
    ASSERT_EQ(list.getSize(), size_t{1});
    EXPECT_EQ(list[0], 0);

    list.insert(list.begin(), 1);               //{1, 0}
    ASSERT_EQ(list.getSize(), size_t{2});
    EXPECT_EQ(list[0], 1);

    list.insert(list.end(), 2);                 //{1, 0, 2}
    ASSERT_EQ(list.getSize(), size_t{3});
    EXPECT_EQ(list[2], 2);

    list.insert(++list.begin(), 3);             //{1, 3, 0, 2}
    ASSERT_EQ(list.getSize(), size_t{4});
    EXPECT_EQ(list[1], 3);
}

TEST(MyList, erase)
{
    MyList<int> list;
    for(int i = 0; i < 10; ++i)
        list.push_back(i);
    ASSERT_EQ(list.getSize(), size_t{10});

    //удаляем последний элемент
    auto iter = list.end();
    --iter;
    EXPECT_EQ(*iter, 9);
    iter = list.erase(iter);
    ASSERT_EQ(list.getSize(), size_t{9});
    ASSERT_EQ(iter, list.end());
    --iter;
    EXPECT_EQ(*iter, 8);

    //удаляем 5-й элемент
    std::advance(iter, -4);
    EXPECT_EQ(*iter, 4);
    iter = list.erase(iter);
    ASSERT_EQ(list.getSize(), size_t{8});
    EXPECT_EQ(*iter, 5);

    //удаляем 1-й элемент    
    iter = list.erase(list.begin());
    ASSERT_EQ(list.getSize(), size_t{7});
    EXPECT_EQ(*iter, 1);
}

TEST(MyList, clear)
{
    MyList<int> list;
    for(int i = 0; i < 10; ++i)
        list.push_back(i);
    ASSERT_EQ(list.getSize(), size_t{10});
    list.clear();
    ASSERT_TRUE(list.empty());
}

TEST(MyList, find)
{
    MyList<std::string> list;
    const size_t listSize = 50;
    for(size_t i = 0; i < listSize; ++i)
        list.push_back(std::to_string(i));
    ASSERT_EQ(list.getSize(), size_t{listSize});
    EXPECT_TRUE(std::find(list.begin(), list.end(), "33") != list.end());
    EXPECT_TRUE(std::find(list.begin(), list.end(), "63") == list.end());
}

TEST(MyList, fill)
{    
    const std::string str = "new_str";
    MyList<std::string> list;
    const size_t listSize = 100;
    for(size_t i = 0; i < listSize; ++i)
        list.push_back(std::to_string(i));
    std::fill(list.begin(), list.end(), str);
    auto iter = list.begin();
    while(iter != list.end())
    {
        ASSERT_EQ(*iter, str);
        ++iter;
    } 
}

struct MyListFixture : public testing::Test
{
    const size_t listSize1 = 10;
    const size_t listSize2 = 5;
    MyList<int> list1;
    MyList<int> list2;

    void SetUp() override
    {
        for(int i = 0; i < static_cast<int>(listSize1); ++i)
            list1.push_back(i);
        for(int i = 0; i < static_cast<int>(listSize2); ++i)
            list2.push_back(i);
    }

    void TearDown() override
    {
        list1.clear();
        list2.clear();
    }
};

TEST_F(MyListFixture, copy)
{
    ASSERT_EQ(list1.getSize(), listSize1);
    ASSERT_EQ(list2.getSize(), listSize2);
    list1 = list2;
    ASSERT_EQ(list1.getSize(), listSize2);
    ASSERT_EQ(list2.getSize(), listSize2);
    for(size_t i = 0; i < listSize2; ++i)
    {
        EXPECT_EQ(list1[i], list2[i]);
    }
}

TEST_F(MyListFixture, move)
{
    ASSERT_EQ(list1.getSize(), listSize1);
    ASSERT_EQ(list2.getSize(), listSize2);
    list2 = std::move(list1);
    ASSERT_EQ(list2.getSize(), listSize1);
    EXPECT_TRUE(list1.empty());
}