#include <gtest/gtest.h>
#include "MyVector.hpp"

TEST(MyVector, create)
{
    MyVector<int> vector;
    EXPECT_EQ(vector.getSize(), size_t{0});
    EXPECT_TRUE(vector.empty());
    EXPECT_EQ(vector.begin(), vector.end());
}

TEST(MyVector, push_back)
{
    MyVector<int> vector;
    ASSERT_TRUE(vector.empty());
    vector.push_back(10);
    ASSERT_EQ(vector.getSize(), size_t{1});
    EXPECT_EQ(*vector.begin(), 10);
    vector.push_back(20);
    ASSERT_EQ(vector.getSize(), size_t{2});
    EXPECT_EQ(*(--vector.end()), 20);
}

TEST(MyVector, get_item)
{
    MyVector<int> vector;
    for(int i = 0; i < 10; ++i)
        vector.push_back(i);
    for(size_t i = 0; i <= vector.getSize(); ++i)
    {
        if(i < vector.getSize())
            EXPECT_EQ(vector[i], static_cast<int>(i));
        else
            ASSERT_THROW(vector[i], std::out_of_range);
    }
}

TEST(MyVector, insert)
{
    MyVector<int> vector;
    ASSERT_TRUE(vector.empty());
    vector.insert(vector.begin(), 0);                  //{0}
    ASSERT_EQ(vector.getSize(), size_t{1});
    EXPECT_EQ(vector[0], 0);

    vector.insert(vector.begin(), 1);               //{1, 0}
    ASSERT_EQ(vector.getSize(), size_t{2});
    EXPECT_EQ(vector[0], 1);

    vector.insert(vector.end(), 2);                 //{1, 0, 2}
    ASSERT_EQ(vector.getSize(), size_t{3});
    EXPECT_EQ(vector[2], 2);

    vector.insert(++vector.begin(), 3);             //{1, 3, 0, 2}
    ASSERT_EQ(vector.getSize(), size_t{4});
    EXPECT_EQ(vector[1], 3);
}

TEST(MyVector, erase)
{
    MyVector<int> vector;
    for(int i = 0; i < 10; ++i)
        vector.push_back(i);
    ASSERT_EQ(vector.getSize(), size_t{10});

    //удаляем последний элемент
    auto iter = vector.end();
    --iter;
    EXPECT_EQ(*iter, 9);
    iter = vector.erase(iter);
    ASSERT_EQ(vector.getSize(), size_t{9});
    ASSERT_EQ(iter, vector.end());
    --iter;
    EXPECT_EQ(*iter, 8);

    //удаляем 5-й элемент
    std::advance(iter, -4);
    EXPECT_EQ(*iter, 4);
    iter = vector.erase(iter);
    ASSERT_EQ(vector.getSize(), size_t{8});
    EXPECT_EQ(*iter, 5);

    //удаляем 1-й элемент    
    iter = vector.erase(vector.begin());
    ASSERT_EQ(vector.getSize(), size_t{7});
    EXPECT_EQ(*iter, 1);
}

TEST(MyVector, clear)
{
    MyVector<int> vector;
    for(int i = 0; i < 10; ++i)
        vector.push_back(i);
    ASSERT_EQ(vector.getSize(), size_t{10});
    vector.clear();
    ASSERT_TRUE(vector.empty());
}

TEST(MyVector, find)
{
    MyVector<std::string> vector;
    const size_t vectorSize = 50;
    for(size_t i = 0; i < vectorSize; ++i)
        vector.push_back(std::to_string(i));
    ASSERT_EQ(vector.getSize(), size_t{vectorSize});
    EXPECT_TRUE(std::find(vector.begin(), vector.end(), "33") != vector.end());
    EXPECT_TRUE(std::find(vector.begin(), vector.end(), "63") == vector.end());
}

TEST(MyVector, fill)
{    
    const std::string str = "new_str";
    MyVector<std::string> vector;
    const size_t vectorSize = 100;
    for(size_t i = 0; i < vectorSize; ++i)
        vector.push_back(std::to_string(i));
    std::fill(vector.begin(), vector.end(), str);
    auto iter = vector.begin();
    while(iter != vector.end())
    {
        ASSERT_EQ(*iter, str);        
        ++iter;
    } 
}

struct MyVectorFixture : public testing::Test
{
    const size_t vectorSize1 = 10;
    const size_t vectorSize2 = 5;
    MyVector<int> vector1;
    MyVector<int> vector2;

    void SetUp() override
    {
        for(int i = 0; i < static_cast<int>(vectorSize1); ++i)
            vector1.push_back(i);
        for(int i = 0; i < static_cast<int>(vectorSize2); ++i)
            vector2.push_back(i);
    }

    void TearDown() override
    {
        vector1.clear();
        vector2.clear();
    }
};

TEST_F(MyVectorFixture, copy)
{
    ASSERT_EQ(vector1.getSize(), vectorSize1);
    ASSERT_EQ(vector2.getSize(), vectorSize2);
    vector1 = vector2;
    ASSERT_EQ(vector1.getSize(), vectorSize2);
    ASSERT_EQ(vector2.getSize(), vectorSize2);
    for(size_t i = 0; i < vectorSize2; ++i)
    {
        EXPECT_EQ(vector1[i], vector2[i]);
    }
}

TEST_F(MyVectorFixture, move)
{
    ASSERT_EQ(vector1.getSize(), vectorSize1);
    ASSERT_EQ(vector2.getSize(), vectorSize2);
    vector2 = std::move(vector1);
    ASSERT_EQ(vector2.getSize(), vectorSize1);
    EXPECT_TRUE(vector1.empty());
}