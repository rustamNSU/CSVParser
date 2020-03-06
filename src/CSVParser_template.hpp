namespace MyCSV
{

template <typename... Args>
CSVParser<Args...>::CSVParser(std::ifstream &file, size_t skip_first_lines_)
    : skip_first_lines{skip_first_lines_}
{
    std::string line;
    int skip = skip_first_lines;
    while(skip)
    {
        if (getline(file, line))
        {
            --skip;
            continue;
        }
        else
        {
            break;
        }
    }
    while (getline(file, line))
    {
        AddRow(line); 
    }
}


template <typename... Args>
void CSVParser<Args...>::AddRow(std::string str_for_stream)
{
    std::istringstream str_stream(str_for_stream);
    std::vector<std::string> result;

    char c;
    string buf;

    bool read_field          = false;
    bool quoted_field        = false;
    bool expect_double_quote = false;
    bool need_separator      = false;

    while (str_stream.get(c))
    {

        // Begin of the quated field
        if (c == quote && !read_field && !quoted_field)
        {
            quoted_field = true;
            read_field   = true;
            continue;
        }

        // This quote may mean double-quote
        // or closing quoted field
        if (c == quote && read_field && quoted_field)
        {
            if (expect_double_quote)
            {
                buf.push_back(c);
                expect_double_quote = false;
            }
            else
            {
                expect_double_quote = true;
            }
            continue;
        }

        // Separator of fields
        if (c == separator && (!quoted_field || expect_double_quote))
        {
            result.push_back(buf);
            buf.clear();

            read_field          = false;
            quoted_field        = false;
            expect_double_quote = false;
            need_separator      = false;

            continue;
        }

        // Skip space char in front of field 
        if (c == ' ' && !read_field)
        {
            expect_double_quote = false;
            quoted_field        = false;
            continue;
        }

        // Skip space char after quated field
        if (c == ' ' && (expect_double_quote || !quoted_field))
        {
            expect_double_quote = false;
            read_field          = false;
            quoted_field        = false;

            need_separator      = true;
            continue;
        }

        // Field char
        if (!need_separator)
        {
            buf.push_back(c);
            read_field = true;
            continue;
        }

        // Incorrect field
        throw std::runtime_error("Incorrect field in row = " + str_for_stream + "\n");
    }
    result.push_back(buf);

    if (result.size() != this->nCol)
    {
        throw std::runtime_error(
            "Different size of columns: expected " + 
            std::to_string(this->nCol) +
            " instead of " + 
            std::to_string(result.size()) + "\n");
    }

    this->dataStr.push_back(result);
    this->data.push_back(TransformStrToTuple(result));

    ++(this->nRow);
}


template <typename... Args>
std::tuple<Args...> CSVParser<Args...>::TransformStrToTuple(
    std::vector<std::string> data_str)
{
    std::tuple<Args...> result{};
    int const t_size = sizeof...(Args);
    iterate_tuple<t_size - 1, fill, Args...>::next(result, fill(data_str, nRow+1));
    return result;
}


/**
 * Setters
 */
template <typename... Args>
void CSVParser<Args...>::SetSeparator(char separator_)
{
    this->separator = separator_;
}


template <typename... Args>
void CSVParser<Args...>::SetQuote(char quote_)
{
    this->quote = quote_;
}


/**
 * Getters
 */
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


/**
 *  InputIterator methods 
 */ 
template <typename... Args>
template <typename ValueType>
CSVParser<Args...>::InputIterator<ValueType>::InputIterator(ValueType* ptr)
{
    this->ptr = ptr;
}


template <typename... Args>
template <typename ValueType>
CSVParser<Args...>::InputIterator<ValueType>::InputIterator(
    const InputIterator &other)
{
    this->ptr(other.ptr);
}


template <typename... Args>
template <typename ValueType>
bool CSVParser<Args...>::InputIterator<ValueType>::operator==(
    const InputIterator &other) const
{
    return this->ptr == other.ptr;
}


template <typename... Args>
template <typename ValueType>
bool CSVParser<Args...>::InputIterator<ValueType>::operator!=(
    const InputIterator &other) const
{
    return this->ptr != other.ptr;
}


template <typename... Args>
template <typename ValueType>
CSVParser<Args...>::InputIterator<ValueType>&
CSVParser<Args...>::InputIterator<ValueType>::operator++()
{
    ++(this->ptr);
    return *this;
}


template <typename... Args>
template <typename ValueType>
CSVParser<Args...>::InputIterator<ValueType>&
CSVParser<Args...>::InputIterator<ValueType>::operator++(int)
{
    InputIterator tmp(*this);
    ++(*this);
    return tmp;
}


template <typename... Args>
template <typename ValueType>
const ValueType& 
CSVParser<Args...>::InputIterator<ValueType>::operator*() const
{
    return *(this->ptr);
}


/**
 * Iterator constructors
 */
template <typename... Args>
typename CSVParser<Args...>::iterator
CSVParser<Args...>::begin()
{
    return iterator(&(*(data.begin())));
}


template <typename... Args>
typename CSVParser<Args...>::iterator
CSVParser<Args...>::end()
{
    return iterator(&(*(data.begin())) + nRow);
}

template <typename... Args>
typename CSVParser<Args...>::const_iterator
CSVParser<Args...>::begin() const
{
    return const_iterator(&(*(data.begin())));
}


template <typename... Args>
typename CSVParser<Args...>::const_iterator
CSVParser<Args...>::end() const
{
    return const_iterator(&(*(data.begin())) + nRow);
}

} // namespace MyCSV