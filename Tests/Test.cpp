// #define _USE_MATH_DEFINES
#include "gtest/gtest.h"
#include "Test.hpp"
#include "CSVParser.hpp"


using namespace MyCSV; 

TEST( CSVParserTest, constructorTest1 ) 
{
    std::ifstream file;
    CSVParser<double, int, std::string> csv1(file);
    CSVParser<int, int, int, size_t, int, double> csv2(file);

    ASSERT_EQ(csv1.ColumnsNumber(), size_t(3));
    ASSERT_EQ(csv1.RowsNumber(), size_t(0));
    ASSERT_EQ(csv2.ColumnsNumber(), size_t(6));
}

TEST( CSVParserTest, constructorTest2 ) 
{
    std::ifstream file("Tests/Simple.csv");
    if (!file.is_open())
    {
        std::cout << "File didn\'t open";
    }
    CSVParser< int, int> csv(file);

    ASSERT_EQ(csv.ColumnsNumber(), size_t(2));
    ASSERT_EQ(csv.RowsNumber(), size_t(3));
}