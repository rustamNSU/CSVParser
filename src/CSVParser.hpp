#pragma once
#include <tuple>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>


namespace MyCSV
{

using std::string;
using std::tuple;
using std::vector;

template <typename... Args>
class CSVParser
{
public:
    using row_type = tuple<Args...>;

private:
    size_t nRow = 0;                ///< Number of rows
    size_t nCol = sizeof...(Args);  ///< Number of columns in row (const for all table)

    vector<vector<string>> dataStr; ///< String type columns
    vector<row_type> data;          ///< Tuple of columns

    char separator = ',';
    char quote = '\"';
    size_t skip_first_lines = 0;

public:
    CSVParser(std::ifstream &file, size_t skip_first_lines_ = 0);

    void AddRow(string str_stream);
    
    void SetSeparator(char separator_);
    void SetQuote(char quote_);
    
    size_t RowsNumber();
    size_t ColumnsNumber();


    ~CSVParser() = default;
};

} // namespace MyCSV

#include "CSVParser_template.hpp"
