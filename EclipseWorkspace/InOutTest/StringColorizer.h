/*
 *  StringColorizer.h
 *
 *  Created on: 2018-01-02
 *  Author: Igor Bender
 *
 */
#ifndef STRINGCOLORIZER_H
#define STRINGCOLORIZER_H

#include <stdint.h>
#include <string>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#include <wincon.h>
#include <memory>
#endif
static void* p;

///
/// \brief The StringColorizer class
/// Provides color print in Linux console.
class  StringColorizer
{
public:
    ///
    /// \brief The Forground enum
    /// Forground color enumeration.
	enum class Forground
	{
#ifndef _WIN32
		None = 0,
		Black = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Magenta = 35,
		Cyan = 36,
		White = 37
#else
		None = -1,
		Black = 0,
		Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
		Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#endif
    };

    ///
    /// \brief The Background enum
    /// Background color enumeration.
    enum class Background
    {
#ifndef _WIN32
		None    = 0,
        Black   = 40,
        Red     = 41,
        Green   = 42,
        Yellow  = 43,
        Blue    = 44,
        Magenta = 45,
        Cyan    = 46,
        White   = 47
#else
		None = -1,
		Black = 0,
		Red = BACKGROUND_RED | BACKGROUND_INTENSITY,
		Green = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
		Yellow = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
		Blue = BACKGROUND_BLUE | BACKGROUND_INTENSITY,
		Magenta = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
		Cyan = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
		White = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY
#endif
	};

    enum class Font
    {
        None = 0,
        Bold = 1
    };


#ifdef _WIN32
	class ConsoleProperties
	{
	public:
		static ConsoleProperties* instance(bool create)
		{
			static ConsoleProperties* m_pInstance = nullptr;
			if (!m_pInstance && create)
			{
				m_pInstance = new ConsoleProperties;
			}
			if (m_pInstance && !create)
			{
				delete m_pInstance;
				m_pInstance = nullptr;
			}
			return m_pInstance;
		}
		uint16_t getProps()
		{
			return m_Props;
		}
		HANDLE getConsole()
		{
			return m_ConsoleHandle;
		}

	protected:
		HANDLE m_ConsoleHandle = NULL;
		uint16_t m_Props = 0;

		ConsoleProperties()
		{
			m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO Info;
			GetConsoleScreenBufferInfo(m_ConsoleHandle, &Info);
			m_Props = Info.wAttributes;
		}
		~ConsoleProperties()
		{
			SetConsoleTextAttribute(m_ConsoleHandle, m_Props);
		}
	};
#endif

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
        StartColorize(Forground FgColor = Forground::None, Background BgColor = Background::None, Font FontAttribute = Font::None) :
            m_FgColor(FgColor), m_BgColor(BgColor), m_FontAttribute(FontAttribute) {}

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
        EndColorize() {}

//        ///
//        /// \brief empty
//        /// Call empty functiton to eliminate unused variable warning.
//        void  empty() const
//        {}

    };
};

///
/// \brief operator <<
/// \param os - standart output stream.
/// \param sc - reference to colorizing attributes instance.
/// \return standart output stream.
///
inline std::ostream& operator <<(std::ostream& os, const StringColorizer::StartColorize& sc)
{
    if(StringColorizer::Forground::None == sc.m_FgColor && StringColorizer::Background::None == sc.m_BgColor && StringColorizer::Font::None == sc.m_FontAttribute)
        return os;
#ifndef _WIN32
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
#else
	uint16_t TextProps = sc.m_pProps->getProps();
	if(StringColorizer::Forground::None != sc.m_FgColor)
	{
		TextProps &= 0x00f0;
		TextProps |= static_cast<uint8_t>(sc.m_FgColor);
	}
	if(StringColorizer::Background::None != sc.m_BgColor)
	{
		TextProps &= 0x000f;
		TextProps |= static_cast<uint8_t>(sc.m_BgColor);
	}
	SetConsoleTextAttribute(sc.m_pProps->getConsole(), TextProps);
#endif
    return os;
}

///
/// \brief operator <<
/// \param os - standart output stream.
/// \param ec - end colorizing instance reference.
/// \return standart output stream.
///
inline std::ostream& operator <<(std::ostream& os, const StringColorizer::EndColorize& ec)
{
//    ec.empty(); // Call empty functiton to eliminate unused variable warning.
#ifndef _WIN32
    p = reinterpret_cast<void*>(const_cast<StringColorizer::EndColorize*>(&ec));
    os << "\033[0m";
#else
	SetConsoleTextAttribute(ec.m_pProps->getConsole(), ec.m_pProps->getProps());
#endif
    return os;
}

#endif // STRINGCOLORIZER_H
