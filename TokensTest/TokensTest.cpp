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
        first{input.begin(), input.end(), re, -1};
    std::sregex_token_iterator  last;
    std::vector<std::string> Results({first, last});
    if(0 == Results.begin()->length())
        ++first;
    return {first, last};
}

inline std::vector<std::string> tokens(const std::string& InputString, const std::string& Delimiters)
{
    std::regex DelimRegex(Delimiters);
    std::sregex_token_iterator First{InputString.begin(), InputString.end(), DelimRegex, -1}, Last;
    while(First != Last && 0 == First->length())
        ++First;
    return {First, Last};
}

class Tokenizer
{
public:
    Tokenizer(const std::string& InputString, const std::string& Delimiters) : m_DelimRegex(Delimiters)
    {
        std::sregex_token_iterator First{InputString.begin(), InputString.end(), m_DelimRegex, -1};
        while(First != m_Last && 0 == First->length())
            ++First;
        m_Current = First;
    }
    Tokenizer() {}
    virtual ~Tokenizer() {}
    void initialize(const std::string& InputString, const std::string& Delimiters)
    {
        m_DelimRegex = std::regex(Delimiters);
        std::sregex_token_iterator First{InputString.begin(), InputString.end(), m_DelimRegex, -1};
        while(First != m_Last && 0 == First->length())
            ++First;
        m_Current = First;
    }

    std::string getToken() // Deleivers next token of empty string after last.
    {
        if(m_Current == m_Last)
            return std::string("");
        std::sregex_token_iterator Itr{m_Current};
        ++m_Current;
        return *Itr;
    }
    void getTokens(std::vector<std::string>& Tokens)
    {
        std::vector<std::string> Results({m_Current, m_Last});
        Tokens = Results;
    }

protected:
    std::sregex_token_iterator m_Current;
    std::sregex_token_iterator m_Last;
    std::regex m_DelimRegex;
};

int main()
{
    string Text = "       Dot    -12.44\t1 1 2.3 2.4 # comment";

//    std::regex Numbers("[[:digit:]]+");
//    std::regex Letters("[[:alpha:]]+");
    std::string Delimiters("[[:blank:]]+"); // Any number of blanks (space, tab etc.)
//    std::string Delimiters("[^a-zA-Z0-9#.-]+");

//    vector<string> Tokens = split(Text, Delimiters);
//    for(auto& s : Tokens)
//        cout << s << endl;

//    cout << endl << endl;
//    Tokens.clear();
//    Tokens = tokens(Text, Delimiters);
//    for(auto& s : Tokens)
//        cout << s << endl;

//    cout << endl << endl;

    Tokenizer Tok(Text, Delimiters);
    std::string s{ Tok.getToken() };
    do
    {
        cout << s << endl;
        s =  Tok.getToken();
    }
    while(0 < s.length());
    cout << "========================" << endl;

    std::string Delimiters2(" ");
    string Text2 = "-12.44\t1 1 2.3 2.4 # comment";
    Tok.initialize(Text2, Delimiters2);
    s =  Tok.getToken();
    do
    {
        cout << s << endl;
        s =  Tok.getToken();
    }
    while(0 < s.length());
    cout << "========================" << endl;

    Tokenizer* pTok= new Tokenizer(Text, Delimiters);
    std::vector<std::string> Tokens;
    pTok->getTokens(Tokens);
    delete pTok;
    for(auto& s : Tokens)
        cout << s << endl;
    cout << "========================" << endl;

    return 0;
}
