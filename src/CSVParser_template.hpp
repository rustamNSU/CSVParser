namespace MyCSV
{

template <typename... Args>
CSVParser<Args...>::CSVParser(std::ifstream &file, size_t skip_first_lines_)
    : skip_first_lines {skip_first_lines_}
{

}


template <typename... Args>
void CSVParser<Args...>::SetSeparator(char separator_)
{

}


template <typename... Args>
void CSVParser<Args...>::SetQuote(char separator_)
{
    
}


template <typename... Args>
size_t CSVParser<Args...>::RowsNumber()
{
    return nRow;
}


template <typename... Args>
size_t CSVParser<Args...>::ColumnsNumber()
{
    return nCol;
}



} // namespace MyCSV