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

TEST( CSVParserTest, bigDataTest ) 
{
    std::ifstream file;
    openFile(file, "StandartSalesJan2009.csv");

    CSVParser<
        std::string,
        std::string,
        int,
        std::string,
        std::string,
        std::string,
        std::string,
        std::string,
        std::string,
        std::string,
        double,
        double
    > csv(file, 1);

    ASSERT_EQ(csv.ColumnsNumber(), size_t(12));
    ASSERT_EQ(csv.RowsNumber(), size_t(998));

    int count = 0;
    for (auto i : csv)
    {
        std::cout << count << ") " << std::get<5>(i) << ", ";
        ++count;
    }
    std::cout << "\ncount = " << count << "\n";
}


TEST( CSVParserTest, outputOperator ) 
{
    std::ifstream file;
    openFile(file, "Hard.csv");
    CSVParser< int, std::string> csv(file);

    ASSERT_EQ(csv.ColumnsNumber(), size_t(2));
    ASSERT_EQ(csv.RowsNumber(), size_t(3));

    int count = 1;
    for (auto i : csv)
    {
        std::cout << count << ") " << i << std::endl;
        ++count;
    }
}