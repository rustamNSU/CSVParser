#pragma once
#include <tuple>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iterator>


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
    vector<tuple<Args...>> data;    ///< Tuple of columns

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

    /* Iterator and const iterator */  
    template <typename ValueType>
    class InputIterator
        : public std::iterator<std::input_iterator_tag, ValueType>
    {
        friend class CSVParser<Args...>;
    private:
        ValueType* ptr;
        InputIterator(ValueType* ptr);
    
    public:
        InputIterator(const InputIterator &other);

        bool operator==(const InputIterator &other) const;
        bool operator!=(const InputIterator &other) const;
        InputIterator& operator++();
        InputIterator& operator++(int);

        // (default) InputIterator::reference = ValueType&
        const ValueType& operator*() const;
    }; 

    typedef InputIterator<tuple<Args...>> iterator;
    typedef InputIterator<const tuple<Args...>> const_iterator;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

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
    size_t nRow;

public:
    fill(vector<string>& str_, size_t nRow_) : str(str_), nRow(nRow_) {}

    template <typename T>
    void fill_object(T &t, int index)
    {
        std::istringstream stream(str[index]);
        stream >> t;
        auto r = stream.tellg();
        if(!stream.eof())
        {
            throw std::runtime_error(
                "Different input and expected type: (" +
                std::to_string(nRow) + 
                ", " +
                std::to_string(index) +
                ") = " +
                str[index] + "\n");
        }
    }

    void fill_object(string &t, int index)
    {
        t = str[index];
    }
}; 


/****************************************** *//**
 * Iterators for printing tuple
 * *****************************************/
template<int index, typename Function>
struct print_tuple 
{
    static void next(Function& print) 
    {
        print_tuple<index - 1, Function>::next(print);
        print.print_object(std::get<index>(print.get_data()), index);
    }
};

template<typename Function>
struct print_tuple<0, Function>
{
    static void next(Function& print) 
    {
        print.print_object(std::get<0>(print.get_data()), 0);
    }
};

// For empty tuple
template<typename Function>
struct print_tuple<-1, Function>
{
    static void next(Function& print) {}
};

template<typename... Args>
class Print
{
private:
    const tuple<Args...>& data;
    std::ostream& out;
    size_t size;

public:
    Print(const tuple<Args...>& data_, size_t size_, std::ostream& out_) 
        : data(data_), size(size_), out(out_) {}

    const tuple<Args...>& get_data()
    {
        return data;
    }

    template <typename T>
    void print_object(T &t, int index)
    {
        if (index != size - 1)
        {
            out << t << ' ';
        }
        else
        {
            out << t;
        }      
    }
    
    void print_object(string &t, int index)
    {
        if (index != size - 1)
        {
            out << '\"' << t << "\", ";
        }
        else
        {
            out << '\"' << t << "\"";
        }      
    }
};

template<typename... Args>
std::ostream& operator << (std::ostream& out, const tuple<Args...>& t)
{
    int const size = sizeof...(Args);
    if (size == 0)
    {
        return out;
    }

    Print<Args...> a(t, size, out);
    print_tuple<size-1, Print<Args...>>::next(a);
    return out;
}


} // namespace MyCSV

#include "CSVParser_template.hpp"
