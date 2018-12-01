#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <regex>

using namespace std;

#define toUpper(Str) for_each(Str.begin(), Str.end(), [](char& c) { c = static_cast<char>(toupper(c)); })

std::vector<std::string> split(const string& input, const string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    std::vector<std::string> Results({first, last});
    if(0 == Results.begin()->length())
        ++first;
    return {first, last};
}

int main(int argc, char* argv[])
{
    string Text = "       Dot    -12.44  1 1 2.3 2.4 # comment";

//    std::regex Numbers("[[:digit:]]+");
//    std::regex Letters("[[:alpha:]]+");
    std::string Delimiters("[[:blank:]]+");
//    std::string Delimiters("[^a-zA-Z0-9#.-]+");



//    std::copy( std::sregex_token_iterator(Text.begin(), Text.end(), All, -1),
//                  std::sregex_token_iterator(),
//                  std::ostream_iterator<std::string>(std::cout, "\n"));
//    std::copy( std::sregex_token_iterator(Text.begin(), Text.end(), Letters, -1),
//                  std::sregex_token_iterator(),
//                  std::ostream_iterator<std::string>(std::cout, "\n"));
//    const string Format = "";
//    string Num = regex_replace(Text, Numbers, Format, regex_constants::format_default);
//    string Al = regex_replace(Text, Letters, Format, regex_constants::format_default);
//    string Num = regex_match(Text, Numbers);
//    string Al = regex_match(Text, Letters);
//    cout << Al << Num << endl;
//    string a = regex_replace(Text, All, Format, regex_constants::format_default);

    vector<int> aaa;
    aaa.push_back(2);
    aaa.push_back(3);
    vector<string> Tokens = split(Text, Delimiters);
    for(auto s : Tokens)
        cout << s << endl;
    enum class Type
    {
        Silent,
        Dot,
        Line,
        Arc,
        Error
    } LegType;

    toUpper(Tokens.at(0));
    if("SILENT" == Tokens.at(0))
        LegType = Type::Silent;
    else if("DOT" == Tokens.at(0))
        LegType = Type::Dot;
    else if("LINE" == Tokens.at(0))
        LegType = Type::Line;
    else if("ARC" == Tokens.at(0))
        LegType = Type::Arc;
    else
        LegType = Type::Error;

    switch(LegType)
    {
    case Type::Silent:
        break;

    case Type::Dot:
    {
        double Speed, StartAz, StartEl, EndAz, EndEl;
        try
        {
            Speed = stod(Tokens.at(1));
        }
        catch(invalid_argument& e)
        {

        }
        break;
    }

    case Type::Line:
        break;

    case Type::Arc:
            break;

    case Type::Error:
        break;
    }
//    double Speed, StartAz, StartEl, EndAz, EndEl;
//    if("Dot == Tokens[0]")
//    {
//        stringstream Conv;
//        Conv << Tokens[1] << " " << Tokens[2] << " " << Tokens[3] << " " << Tokens[4] << " " << Tokens[5] << ends;
//        Conv >> Speed >> StartAz >> StartEl >> EndAz >> EndEl;
//        cout << StartAz << " " << StartEl << " " << EndAz << " " << EndEl << endl;
//    }
    return 0;
}
