/*
 *  StringColorizer.h
 *
 *  Created on: 2018-01-02
 *  Author: Igor Bender
 *
 */
#ifndef STRINGCOLORIZER_H
#define STRINGCOLORIZER_H

#include <sstream>
#include <stdint.h>
#include <string>

///
/// \brief The StringColorizer class
/// Provides color print in Linux console.
class StringColorizer
{
  public:
    ///
    /// \brief The Forground enum
    /// Forground color enumeration.
	enum class Forground
	{
		None = 0,
		Black = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Magenta = 35,
		Cyan = 36,
		White = 37
    };

    ///
    /// \brief The Background enum
    /// Background color enumeration.
    enum class Background
    {
        None = 0,
        Black = 40,
        Red = 41,
        Green = 42,
        Yellow = 43,
        Blue = 44,
        Magenta = 45,
        Cyan = 46,
        White = 47
	};

    enum class Font
    {
        None = 0,
        Bold = 1
    };

    ///
    /// \brief The StartColorize class
    /// class contains colorizing attributes to be inserted into standart output stream.
    class StartColorize
    {
      public:
        ///
        /// \brief StartColorize - constructor.
        /// \param FgColor - Forground color.
        /// \param BgColor - Background color.
        /// \param FontAttribute - bold or regular font.
        ///
        StartColorize(Forground FgColor = Forground::None, Background BgColor = Background::None,
                      Font FontAttribute = Font::None)
            : m_FgColor(FgColor), m_BgColor(BgColor), m_FontAttribute(FontAttribute)
        {
        }

        std::string colorize()
        {
            if(StringColorizer::Forground::None == m_FgColor &&
               StringColorizer::Background::None == m_BgColor &&
               StringColorizer::Font::None == m_FontAttribute)
                return std::string("");
            std::string Out = "\033[";
            std::stringstream Number;
            bool PutSemicolon = false;
            if(StringColorizer::Forground::None != m_FgColor)
            {
                Number << static_cast<uint32_t>(m_FgColor) << std::ends;
                Out += Number.str().substr(0, Number.str().length() - 1);
                PutSemicolon = true;
                Number.seekp(0);
                Number.seekg(0);
                Number.str("");
                Number.clear();
            }
            if(StringColorizer::Background::None != m_BgColor)
            {
                if(PutSemicolon)
                    Out += ";";
                Number << static_cast<uint32_t>(m_BgColor) << std::ends;
                Out += Number.str().substr(0, Number.str().length() - 1);
                PutSemicolon = true;
                Number.seekp(0);
                Number.seekg(0);
                Number.str("");
                Number.clear();
            }
            if(StringColorizer::Font::None != m_FontAttribute)
            {
                if(PutSemicolon)
                    Out += ";";
                Number << static_cast<uint32_t>(m_FontAttribute) << std::ends;
                std::string Tmp = Number.str();
                Tmp = Tmp.substr(0, Tmp.length() - 1);
                Out += Number.str().substr(0, Number.str().length() - 1);
            }

            Out += "m";

            return Out;
        }

        Forground m_FgColor = Forground::None; ///< Forground color.
        Background m_BgColor = Background::None; ///< Background color.
        Font m_FontAttribute = Font::None; ///< bold or regular font.
    };

    ///
    /// \brief The EndColorize class
    /// class to be inserted into standart output stream in order to return default colors and font.
    class EndColorize
    {
    public:
        std::string ColorEnd = "\033[0m";
    };
};

///
/// \brief operator <<
/// \param os - standart output stream.
/// \param sc - reference to colorizing attributes instance.
/// \return standart output stream.
///
inline std::ostream& operator<<(std::ostream& os, const StringColorizer::StartColorize& sc)
{
    if(StringColorizer::Forground::None == sc.m_FgColor &&
       StringColorizer::Background::None == sc.m_BgColor &&
       StringColorizer::Font::None == sc.m_FontAttribute)
        return os;
    std::string Out = "\033[";
    std::stringstream Number;
    bool PutSemicolon = false;
    if(StringColorizer::Forground::None != sc.m_FgColor)
    {
        Number << static_cast<uint32_t>(sc.m_FgColor) << std::ends;
        Out += Number.str().substr(0, Number.str().length() - 1);
        PutSemicolon = true;
        Number.seekp(0);
        Number.seekg(0);
        Number.str("");
        Number.clear();
    }
    if(StringColorizer::Background::None != sc.m_BgColor)
    {
        if(PutSemicolon)
            Out += ";";
        Number << static_cast<uint32_t>(sc.m_BgColor) << std::ends;
        Out += Number.str().substr(0, Number.str().length() - 1);
        PutSemicolon = true;
        Number.seekp(0);
        Number.seekg(0);
        Number.str("");
        Number.clear();
    }
    if(StringColorizer::Font::None != sc.m_FontAttribute)
    {
        if(PutSemicolon)
            Out += ";";
        Number << static_cast<uint32_t>(sc.m_FontAttribute) << std::ends;
        std::string Tmp = Number.str();
        Tmp = Tmp.substr(0, Tmp.length() - 1);
        Out += Number.str().substr(0, Number.str().length() - 1);
    }

    Out += "m";
    os << Out;
    return os;
}

///
/// \brief operator <<
/// \param os - standart output stream.
/// \param ec - end colorizing instance reference.
/// \return standart output stream.
///
inline std::ostream& operator<<(std::ostream& os, const StringColorizer::EndColorize& ec)
{
//    ec.empty(); // Call empty functiton to eliminate unused variable warning.
    os << ec.ColorEnd;//"\033[0m";
    return os;
}

#endif // STRINGCOLORIZER_H
