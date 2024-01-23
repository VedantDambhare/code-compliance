#include <boost/spirit/include/qi_symbols.hpp>
#include <string>

using boost::spirit::qi::symbols;

struct symbol_table_val {
    std::string data_type;
    std::string next_line;
    int scope_start;
    int scope_end;
} st_1;


void main() {

    symbols<std::string,symbol_table_val> sym_global;
    int a=0;
}

