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
private:
    size_t nRow = 0;                ///< Number of rows
    size_t nCol = sizeof...(Args);  ///< Number of columns in row (const for all table)

    vector<vector<string>> dataStr; ///< String type columns
    vector<tuple<Args...>> data;          ///< Tuple of columns

    char separator = ',';
    char quote = '\"';
    size_t skip_first_lines = 0;

public:
    CSVParser(std::ifstream &file, size_t skip_first_lines_ = 0);
    void AddRow(string str_stream);

private:
    tuple<Args...> TransformStrToTuple(vector<string> data_str);

public:    
    void SetSeparator(char separator_);
    void SetQuote(char quote_);
    
    size_t RowsNumber();
    size_t ColumnsNumber();

    ~CSVParser() = default;
};


/****************************************** *//**
 * Iterators for filling tuple
 * *****************************************/
template<int index, typename Fill, typename... Args>
struct iterate_tuple 
{
    static void next(std::tuple<Args...> &t, Fill fill) 
    {
        iterate_tuple<index - 1, Fill, Args...>::next(t, fill);
        fill.fill_object(std::get<index>(t), index);
    }
};

template<typename Fill, typename... Args>
struct iterate_tuple<0, Fill, Args...> 
{
    static void next(std::tuple<Args...> &t, Fill fill) 
    {
        fill.fill_object(std::get<0>(t), 0);
    }
};

template<typename Fill, typename... Args>
struct iterate_tuple<-1, Fill, Args...>
{
    static void next(std::tuple<Args...> &t, Fill fill) {}
};

class fill
{
private:
    vector<string> str;

public:
    fill(vector<string>& str_) : str(str_) {}

    template <typename T>
    void fill_object(T &t, int index)
    {
        std::istringstream stream(str[index]);
        stream >> t;
    }

    void fill_object(string &t, int index)
    {
        t = str[index];
    }
};


} // namespace MyCSV

#include "CSVParser_template.hpp"
