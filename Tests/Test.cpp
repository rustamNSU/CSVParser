// #define _USE_MATH_DEFINES
#include "gtest/gtest.h"
#include "Test.hpp"
#include "CSVParser.hpp"


using namespace MyCSV;

void openFile(std::ifstream &file, std::string filename)
{
    file.open("Tests/Input/" + filename);
    if (!file.is_open())
    {
        std::cout << "File  " + filename + "  didn\'t open\n";
    }
}

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
    std::ifstream file;
    openFile(file, "Simple.csv");
    CSVParser< int, int> csv(file);

    ASSERT_EQ(csv.ColumnsNumber(), size_t(2));
    ASSERT_EQ(csv.RowsNumber(), size_t(3));
}

TEST( CSVParserTest, hardTest ) 
{
    std::ifstream file;
    openFile(file, "Hard.csv");
    CSVParser< int, std::string> csv(file);

    ASSERT_EQ(csv.ColumnsNumber(), size_t(2));
    ASSERT_EQ(csv.RowsNumber(), size_t(3));
}

// TEST( CSVParserTest, incorrectExceptionTest ) 
// {
//     std::ifstream file;
//     openFile(file, "Incorrect1.csv");

//     EXPECT_ANY_THROW({CSVParser<int, int>{file}});
// }