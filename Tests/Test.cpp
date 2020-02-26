// #define _USE_MATH_DEFINES
#include "gtest/gtest.h"
#include "Test.hpp"
#include "CSVParser.hpp"


using namespace MyCSV; 

TEST( CSVParserTest, constructorTest ) 
{
    std::ifstream file;
    CSVParser<double, int, std::string> csv(file);

    ASSERT_EQ(csv.ColumnsNumber(), size_t(3));
    ASSERT_EQ(csv.RowsNumber(), size_t(0));
}