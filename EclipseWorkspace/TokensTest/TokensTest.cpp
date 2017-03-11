#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <regex>

using namespace std;

int main(int argc, char* argv[])
{
    string Text = "123K";

    std::regex Numbers("[[:digit:]]+");
    std::regex Letters("[[:alpha:]]+");


//    std::copy( std::sregex_token_iterator(Text.begin(), Text.end(), Numbers, -1),
//                  std::sregex_token_iterator(),
//                  std::ostream_iterator<std::string>(std::cout, "\n"));
//    std::copy( std::sregex_token_iterator(Text.begin(), Text.end(), Letters, -1),
//                  std::sregex_token_iterator(),
//                  std::ostream_iterator<std::string>(std::cout, "\n"));
    const string Format = "";
    string Al = regex_replace(Text, Letters, Format, regex_constants::format_default);
    string Num = regex_replace(Text, Numbers, Format, regex_constants::format_default);
    cout << Al << Num << endl;
    return 0;
}
