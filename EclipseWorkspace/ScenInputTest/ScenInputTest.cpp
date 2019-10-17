/*
 * ScenInputTest.cpp
 * Created by Igor Bender at 2018-11-13
 *
 */
#include <iostream>
#include <regex>
#include <fstream>
#include <cmath>
#include <limits>

std::vector<std::string> tokens(const std::string& InputString, const std::string& Delimiters)
{
    std::regex DelimRegex(Delimiters);
    std::sregex_token_iterator First{InputString.begin(), InputString.end(), DelimRegex, -1}, Last;
    while(First != Last && 0 == First->length())
        ++First;
    return {First, Last};
}

const uint8_t CHANNEL_1_STATE = 0x01;
const uint8_t CHANNEL_2_STATE = CHANNEL_1_STATE << 1;
const uint8_t CHANNEL_3_STATE = CHANNEL_1_STATE << 2;
const uint8_t CHANNEL_4_STATE = CHANNEL_1_STATE << 3;
const uint8_t POLAR_1_STATE = CHANNEL_1_STATE << 4;
const uint8_t POLAR_2_STATE = CHANNEL_1_STATE << 5;
const uint8_t POLAR_3_STATE = CHANNEL_1_STATE << 6;
const uint8_t POLAR_4_STATE = CHANNEL_1_STATE << 7;

struct BinaryData
{
    uint8_t ActivityBits;
    int16_t Target1Azimuth;
    int16_t Target1Elevation;
    uint16_t Target1Polarization;
    int16_t Target2Azimuth;
    int16_t Target2Elevation;
    uint16_t Target2Polarization;
    int16_t Target3Azimuth;
    int16_t Target3Elevation;
    uint16_t Target3Polarization;
    int16_t Target4Azimuth;
    int16_t Target4Elevation;
    uint16_t Target4Polarization;
} __attribute__ ((packed));

class PolarizationAngleComputer
{
public:
    enum class PolarizationType
    {
        FixAngle = 0,
        Cyrcular,
        Sweep
    };
    void init(PolarizationType Type, double StartAngle, double AngleRate = 0.0, bool Clockwise = true, double MinAngle = 0.0, double MaxAngle = 0.0)
    {
        m_Type = Type;
        m_StartAngle = StartAngle;
        m_AngleRate = AngleRate;
        m_Clockwise = Clockwise;
        m_MinAngle = MinAngle;
        m_MaxAngle = MaxAngle;
        m_CurrentAngle = m_StartAngle;
    }
    void reinit(PolarizationType Type, double AngleRate = 0.0, bool Clockwise = true, double MinAngle = 0.0, double MaxAngle = 0.0)
    {
        m_Type = Type;
        m_AngleRate = AngleRate;
        m_Clockwise = Clockwise;
        m_MinAngle = MinAngle;
        m_MaxAngle = MaxAngle;
        m_CurrentAngle = m_StartAngle;
    }
    void run(double TimeDelta)
    {
        computeAngle(m_Type, m_CurrentAngle, m_Clockwise, TimeDelta, m_AngleRate, m_MinAngle, m_MaxAngle);
    }
    double getAngle()
    {
        return m_CurrentAngle;
    }
    void setType(PolarizationType Type) { m_Type = Type; }
    void setAngleRate(double AngleRate) { m_AngleRate = AngleRate; }
    void setClockwise(bool Clockwise) { m_Clockwise = Clockwise; }
    void setMinAngle(double MinAngle) { m_MinAngle = MinAngle; }
    void setMaxAngle(double MaxAngle) { m_MaxAngle = MaxAngle; }

protected:
    double m_StartAngle = 0.0;
    double m_AngleRate = 0.0;
    double m_MaxAngle = 0.0;
    double m_MinAngle = 0.0;
    PolarizationType m_Type = PolarizationType::FixAngle;
    bool m_Clockwise = true;
    double m_CurrentAngle = 0.0;

    void computeAngle(PolarizationType Type, double& CurrentAngle, bool& Clockwise, double TimeDelta, double AngleRate, double MinAngle, double MaxAngle);
};


bool polarizationSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, bool& Active, PolarizationAngleComputer::PolarizationType& Type,
                             double& StartAngle, double& AngleRate, bool& Clockwise, double& MinAngle, double& MaxAngle);
bool silenceSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, int32_t& Cycles);
bool dotSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, int32_t& Cycles, double& tAz, double& El);
bool lineSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, double& Velocity, double& StartAz, double& StartEl,
                     double& EndAz, double& EndEl);
bool arcSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, double& Velocity, double& StartAz, double& StartEl,
                    double& EndAz, double& EndEl, double& CenterAz, double& CenterEl, bool& Clockwise);
bool computeDotLeg(uint32_t Target, BinaryData* pScenario, uint32_t& Index,
                   double Azimuth, double Elevation, int32_t Cycles, bool PolarizationActive,
                   PolarizationAngleComputer* pPolar = nullptr);

bool computeArcLeg(uint32_t Target, BinaryData* pScenario, uint32_t& Index,
                   double Velocity, double BeginAzimuth, double BeginElevation,
                   double EndAzimuth, double EndElevation, double CenterAzimuth, double CenterElevation, bool Clockwise,
                   bool PolarizationActive, PolarizationAngleComputer* pPolar = nullptr);

bool computeLineLeg(uint32_t Target, BinaryData* pScenario, uint32_t& Index,
                   double Velocity, double BeginAzimuth, double BeginElevation,
                   double EndAzimuth, double EndElevation, bool PolarizationActive,
                   PolarizationAngleComputer* pPolar = nullptr);

bool silentCycles(uint32_t Target, BinaryData* pScenario, uint32_t& Index, int32_t SilentCycles);


const uint32_t MAX_NUM_OF_POINTS = 10000000;

enum class LegTypeEnum
{
    None,
    Silence,
    Dot,
    Line,
    Arc,
    Polarization
};

bool computeLeg(uint32_t Target, LegTypeEnum LegType, const std::vector<std::string>& Tokens, uint32_t LineNum, BinaryData* pScenario, uint32_t& Index);

using namespace std;

int main(int argc, char* argv[])
{
    if(4 > argc)
    {
        cout << "Usage : " << argv[0] << " target_number  trajectory file scenario_binary_file" << endl
             << "target_number: 1 - 4 or 0 for all targets." << endl;
        return 1;
    }
    uint32_t Target = 0;
    stringstream Conv;
    Conv << argv[1];
    Conv >> Target;
    if(4 < Target || !Conv)
    {
        cout << "Wrong target number." << endl;
        return 1;
    }

    BinaryData* pScenario = new BinaryData[MAX_NUM_OF_POINTS];
#ifndef _WIN32
    bzero(pScenario, sizeof (BinaryData) * MAX_NUM_OF_POINTS);
#else
    memset(pScenario, 0, sizeof (BinaryData) * MAX_NUM_OF_POINTS);
#endif
    LegTypeEnum LegType{LegTypeEnum::None};

    ifstream InFile(argv[2]);
    if(!InFile)
    {
        cerr << "No tragectory file " << argv[1] << endl;
        return 1;
    }

    uint32_t Index = 0;
    char Buffer[1024];
    const string Delims(R"([^a-zA-Z0-9#.-]+)");
    uint32_t LineCounter = 0;
    while(!InFile.eof())
    {
        ++LineCounter;
        InFile.getline(Buffer, sizeof (Buffer));
        if(!InFile)
            break;
        vector<string> Tokens = tokens(Buffer, Delims);
        if(0  >= Tokens.size())
            continue;
        if('#' == Tokens.at(0).at(0))
            continue;
        transform(Tokens.at(0).begin(), Tokens.at(0).end(), Tokens.at(0).begin(), [](char c){ return toupper(c); });
        if("POLAR" == Tokens.at(0))
            LegType = LegTypeEnum::Polarization;
        else if("SILENT" == Tokens.at(0))
            LegType = LegTypeEnum::Silence;
        else if("DOT" == Tokens.at(0))
            LegType = LegTypeEnum::Dot;
        else if("LINE" == Tokens.at(0))
            LegType = LegTypeEnum::Line;
        else if("ARC" == Tokens.at(0))
            LegType = LegTypeEnum::Arc;
        else
            LegType = LegTypeEnum::None;

        computeLeg(Target, LegType, Tokens, LineCounter, pScenario, Index);
    }
    InFile.close();

    ofstream Out(argv[3], ios::binary);
    Out.write(reinterpret_cast<char*>(pScenario), sizeof(BinaryData) * Index);
    Out.close();
    return 0;
}

bool computeLeg(uint32_t Target, LegTypeEnum LegType, const std::vector<std::string>& Tokens, uint32_t LineNum, BinaryData* pScenario, uint32_t& Index)
{
    bool Result = false;
    bool PolarizationActive = false;
    PolarizationAngleComputer PolarAlgo;
    switch(LegType)
    {
    case LegTypeEnum::Silence:
    {
        int32_t Cycles;
        Result = silenceSanityCheck(Tokens, LineNum, Cycles);
        if(Result)
            silentCycles(Target, pScenario, Index, Cycles);
        break;
    }

    case LegTypeEnum::Dot:
    {
        int32_t Cycles = 0.0;
        double Az = 0.0;
        double El = 0.0;
        Result = dotSanityCheck(Tokens, LineNum, Cycles, Az, El);
        if(Result)
            computeDotLeg(Target, pScenario, Index, Az, El, Cycles, PolarizationActive, &PolarAlgo);
        break;
    }

    case LegTypeEnum::Line:
    {
        double StartAz;
        double StartEl;
        double EndAz;
        double EndEl;
        double Velocity;
        Result = lineSanityCheck(Tokens, LineNum, Velocity, StartAz, StartEl, EndAz, EndEl);
        if(Result)
            computeLineLeg(Target, pScenario, Index, Velocity, StartAz, StartEl, EndAz, EndEl, PolarizationActive, &PolarAlgo);
        break;
    }

    case LegTypeEnum::Arc:
    {
        double StartAz;
        double StartEl;
        double EndAz;
        double EndEl;
        double CenterAz;
        double CenterEl;
        double Velocity;
        bool Clockwise;
        Result = arcSanityCheck(Tokens, LineNum, Velocity, StartAz, StartEl, EndAz, EndEl, CenterAz, CenterEl, Clockwise);
        if(Result)
            computeArcLeg(Target, pScenario, Index, Velocity, StartAz, StartEl, EndAz, EndEl, CenterAz, CenterEl, Clockwise, PolarizationActive, &PolarAlgo);
        break;
    }

    case LegTypeEnum::Polarization:
    {
        PolarizationAngleComputer::PolarizationType Type = PolarizationAngleComputer::PolarizationType::FixAngle;
        double PolarAngle = 0.0;
        double AngleRate = 0.0;
        double MinAgle = 0.0;
        double MaxAngle = 0.0;
        bool Clockwise = true;
        bool Check = polarizationSanityCheck(Tokens, LineNum, PolarizationActive, Type, PolarAngle, AngleRate, Clockwise, MinAgle, MaxAngle);
        if(!Check)
            return false;
        if(PolarizationActive)
            PolarAlgo.init(Type, PolarAngle, AngleRate, Clockwise, MinAgle, MaxAngle);
        break;
    }

    case LegTypeEnum::None:
        ;
    }
    return Result;
}

bool polarizationSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, bool& Active, PolarizationAngleComputer::PolarizationType& Type,
                             double& StartAngle, double& AngleRate, bool& Clockwise, double& MinAngle, double& MaxAngle)
{
    if(2 > Tokens.size())
    {
        cerr << "Wrong polarization definition at line " << LineNum << endl;
        return false;
    }
    int32_t IntActive = 0;
    try {
        IntActive = stoi(Tokens.at(1));
    }
    catch(invalid_argument e)
    {
        cerr << "Wrong polarization activity field at line " << LineNum << endl;
        return false;
    }
    catch(out_of_range e)
    {
        cerr << "Wrong polarization activity field at line " << LineNum << endl;
        return false;
    }
    if(!IntActive)
    {
        Active = false;
        return true;
    }
    if(2 >= Tokens.size())
    {
        cerr << "Wrong polarization parameters number at line ." << LineNum << endl;
        return false;
    }
    int32_t IntType = 0;
    try {
        IntType = stoi(Tokens.at(2));
    }
    catch(invalid_argument e)
    {
        cerr << "Wrong polarization type field at line " << LineNum << endl;
        return false;
    }
    catch(out_of_range e)
    {
        cerr << "Wrong polarization type field at line " << LineNum << endl;
        return false;
    }
    if(0 > IntType || 2 < IntType)
    {
        cerr << "Wrong polarization type field at line " << LineNum << endl;
        return false;
    }
    Type = static_cast<PolarizationAngleComputer::PolarizationType>(IntType);
    switch(Type)
    {
    case PolarizationAngleComputer::PolarizationType::FixAngle:
    {
        if(4 > Tokens.size())
        {
            cerr << "Wrong polarization parameters number at line ." << LineNum << endl;
            return false;
        }
        try {
            StartAngle = stod(Tokens.at(3));
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization angle field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization angle field at line " << LineNum << endl;
            return false;
        }
        break;
    }

    case PolarizationAngleComputer::PolarizationType::Cyrcular:
    {
        if(6 > Tokens.size())
        {
            cerr << "Wrong polarization parameters number at line ." << LineNum << endl;
            return false;
        }
        try {
            StartAngle = stod(Tokens.at(3));
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization angle field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization angle field at line " << LineNum << endl;
            return false;
        }
        try {
            AngleRate = stod(Tokens.at(4));
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization angle rate field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization angle rate field at line " << LineNum << endl;
            return false;
        }
        try {
            int32_t IntClockwise = 1;
            IntClockwise = stoi(Tokens.at(5));
            if(1 != IntClockwise && 0 != IntClockwise)
            {
                cerr << "Wrong polarization angle changing direction field at line " << LineNum << endl;
                return false;
            }
            Clockwise = 1 == IntClockwise ? true : false;
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization angle changing direction field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization angle changing direction field at line " << LineNum << endl;
            return false;
        }
        break;
    }

    case PolarizationAngleComputer::PolarizationType::Sweep:
    {
        if(8 > Tokens.size())
        {
            cerr << "Wrong polarization parameters number at line ." << LineNum << endl;
            return false;
        }
        try {
            StartAngle = stod(Tokens.at(3));
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization angle field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization angle field at line " << LineNum << endl;
            return false;
        }
        try {
            AngleRate = stod(Tokens.at(4));
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization angle rate field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization angle rate field at line " << LineNum << endl;
            return false;
        }
        try {
            int32_t IntClockwise = 1;
            IntClockwise = stoi(Tokens.at(5));
            if(1 != IntClockwise && 0 != IntClockwise)
            {
                cerr << "Wrong polarization angle changing direction field at line " << LineNum << endl;
                return false;
            }
            Clockwise = 1 == IntClockwise ? true : false;
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization angle changing direction field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization angle changing direction field at line " << LineNum << endl;
            return false;
        }
        try {
            MinAngle = stod(Tokens.at(6));
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization minimal angle field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization minimal angle field at line " << LineNum << endl;
            return false;
        }
        try {
            MaxAngle = stod(Tokens.at(7));
        }
        catch(invalid_argument e)
        {
            cerr << "Wrong polarization maximal angle field at line " << LineNum << endl;
            return false;
        }
        catch(out_of_range e)
        {
            cerr << "Wrong polarization maximal angle field at line " << LineNum << endl;
            return false;
        }
        break;
    }
    }
    return true;
}

bool silenceSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, int32_t& Cycles)
{
    if(2 > Tokens.size())
    {
        cerr << "Wrong silence definition at line " << LineNum << endl;
        return false;
    }
    Cycles = 0;
    try {
        Cycles = stoi(Tokens.at(1));
    }
    catch(invalid_argument e)
    {
        cerr << "Wrong silence cycles number field at line " << LineNum << endl;
        return false;
    }
    catch(out_of_range e)
    {
        cerr << "Wrong silence cycles number field at line " << LineNum << endl;
        return false;
    }
    if(0 > Cycles)
    {
        cerr << "Silence cycles number should be positive, line : " << LineNum << endl;
    }
    // Call silence generation function here.
    return true;
}

bool dotSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, int32_t& Cycles, double& Az, double& El)
{
    if(4 > Tokens.size())
    {
        cerr << "Wrong dot definition at line " << LineNum << endl;
        return false;
    }
    try {
        Cycles = stoi(Tokens.at(1));
        Az = stod(Tokens.at(2));
        El = stod(Tokens.at(3));
    }
    catch(invalid_argument e)
    {
        cerr << "Wrong dot definition field at line " << LineNum << endl;
        return false;
    }
    catch(out_of_range e)
    {
        cerr << "Wrong dot definition field at line " << LineNum << endl;
        return false;
    }

    return true;
}

bool lineSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, double& Velocity, double& StartAz, double& StartEl,
                     double& EndAz, double& EndEl)
{
    if(6 > Tokens.size())
    {
        cerr << "Wrong line definition at line " << LineNum << endl;
        return false;
    }
    try {
        Velocity = stod(Tokens.at(1));
        StartAz = stod(Tokens.at(2));
        StartEl = stod(Tokens.at(3));
        EndAz = stod(Tokens.at(4));
        EndEl = stod(Tokens.at(5));
    }
    catch(invalid_argument e)
    {
        cerr << "Wrong line definition field at line " << LineNum << endl;
        return false;
    }
    catch(out_of_range e)
    {
        cerr << "Wrong line definition field at line " << LineNum << endl;
        return false;
    }

    return true;
}

bool arcSanityCheck(const std::vector<std::string>& Tokens, uint32_t LineNum, double& Velocity, double& StartAz, double& StartEl,
                    double& EndAz, double& EndEl, double& CenterAz, double& CenterEl, bool& Clockwise)
{
    if(9 > Tokens.size())
    {
        cerr << "Wrong arc definition at line " << LineNum << endl;
        return false;
    }
    try {
        Velocity = stod(Tokens.at(1));
        StartAz = stod(Tokens.at(2));
        StartEl = stod(Tokens.at(3));
        EndAz = stod(Tokens.at(4));
        EndEl = stod(Tokens.at(5));
        CenterAz = stod(Tokens.at(6));
        CenterEl = stod(Tokens.at(7));
        int32_t IntClockwise = 1;
        IntClockwise = stoi(Tokens.at(8));
        if(1 != IntClockwise && 0 != IntClockwise)
        {
            cerr << "Wrong arc changing direction field at line " << LineNum << endl;
            return false;
        }
        Clockwise = 1 == IntClockwise ? true : false;
    }
    catch(invalid_argument e)
    {
        cerr << "Wrong arc definition field at line " << LineNum << endl;
        return false;
    }
    catch(out_of_range e)
    {
        cerr << "Wrong arc definition field at line " << LineNum << endl;
        return false;
    }

    return true;
}

bool computeDotLeg(uint32_t Target, BinaryData* pScenario, uint32_t& Index,
                   double Azimuth, double Elevation, int32_t Cycles, bool PolarizationActive,
                   PolarizationAngleComputer* pPolAlgo)
{
    for(int32_t i = 0; i < Cycles; ++i)
    {
        switch(Target)
        {
            case 0:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits = CHANNEL_1_STATE | CHANNEL_2_STATE | CHANNEL_3_STATE | CHANNEL_4_STATE |
                            POLAR_1_STATE | POLAR_2_STATE | POLAR_3_STATE | POLAR_4_STATE;
                else
                    pScenario[Index].ActivityBits = CHANNEL_1_STATE | CHANNEL_2_STATE | CHANNEL_3_STATE | CHANNEL_4_STATE;
                pScenario[Index].Target1Azimuth = static_cast<int16_t>(round(Azimuth * 10.0));
                pScenario[Index].Target1Elevation = static_cast<int16_t>(round(Elevation * 10.0));
                pScenario[Index].Target2Azimuth = static_cast<int16_t>(round(Azimuth * 10.0));
                pScenario[Index].Target2Elevation = static_cast<int16_t>(round(Elevation * 10.0));
                pScenario[Index].Target3Azimuth = static_cast<int16_t>(round(Azimuth * 10.0));
                pScenario[Index].Target3Elevation = static_cast<int16_t>(round(Elevation * 10.0));
                pScenario[Index].Target4Azimuth = static_cast<int16_t>(round(Azimuth * 10.0));
                pScenario[Index].Target4Elevation = static_cast<int16_t>(round(Elevation * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target1Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target2Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target3Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target4Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 1:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_1_STATE | POLAR_1_STATE;
                else
                    pScenario[Index].ActivityBits |= CHANNEL_1_STATE;
                pScenario[Index].Target1Azimuth = static_cast<int16_t>(round(Azimuth * 10.0));
                pScenario[Index].Target1Elevation = static_cast<int16_t>(round(Elevation * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target1Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 2:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_2_STATE | POLAR_2_STATE;
                else
                    pScenario[Index].ActivityBits |= CHANNEL_2_STATE;
                pScenario[Index].Target2Azimuth = static_cast<int16_t>(round(Azimuth * 10.0));
                pScenario[Index].Target2Elevation = static_cast<int16_t>(round(Elevation * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target2Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 3:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_3_STATE | POLAR_3_STATE;
                else
                    pScenario[Index].ActivityBits |= CHANNEL_3_STATE;
                pScenario[Index].Target3Azimuth = static_cast<int16_t>(round(Azimuth * 10.0));
                pScenario[Index].Target3Elevation = static_cast<int16_t>(round(Elevation * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target3Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 4:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_4_STATE | POLAR_4_STATE;
                else
                    pScenario[Index].ActivityBits |= CHANNEL_4_STATE;
                pScenario[Index].Target4Azimuth = static_cast<int16_t>(round(Azimuth * 10.0));
                pScenario[Index].Target4Elevation = static_cast<int16_t>(round(Elevation * 10.0));
                pScenario[Index].Target4Polarization = 0;
                if(PolarizationActive)
                {
                    pScenario[Index].Target4Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }
        }
        ++Index;
    }
    return true;
}

bool computeArcLeg(uint32_t Target, BinaryData* pScenario, uint32_t& Index,
                   double Velocity, double BeginAzimuth, double BeginElevation,
                   double EndAzimuth, double EndElevation, double CenterAzimuth, double CenterElevation, bool Clockwise,
                   bool PolarizationActive,  PolarizationAngleComputer* pPolAlgo)
{
    // Compute radius
    double Radius = std::sqrt((EndAzimuth - CenterAzimuth) * (EndAzimuth - CenterAzimuth) +
            (EndElevation - CenterElevation) * (EndElevation - CenterElevation));
    // Compute arc length
    double Length = 0.0;
    double BeginAngle;
    double Angle = 0.0;
    if(std::numeric_limits<double>::epsilon() > std::abs(EndAzimuth - BeginAzimuth) &&
            std::numeric_limits<double>::epsilon() > std::abs(EndElevation - BeginElevation)) // full circle
    {
        Length = 2.0 * M_PI * Radius;
        Angle = (!Clockwise) ? (2.0 * M_PI) : (-2.0 * M_PI);
        BeginAngle = atan2(BeginElevation - CenterElevation, BeginAzimuth - CenterAzimuth);
        BeginAngle += (0.0 > BeginAngle ? 2.0 * M_PI : 0.0);
    }
    else
    {
        double EndAngle;
        BeginAngle = atan2(BeginElevation - CenterElevation, BeginAzimuth - CenterAzimuth);
        BeginAngle += (0.0 > BeginAngle ? 2.0 * M_PI : 0.0);
        EndAngle = atan2(EndElevation - CenterElevation, EndAzimuth - CenterAzimuth);
        EndAngle += (0.0 > EndAngle ? 2.0 * M_PI : 0.0);
        if(!Clockwise)
            Angle = (EndAngle > BeginAngle) ? (EndAngle - BeginAngle) : (2.0 * M_PI - (BeginAngle - EndAngle));
        else
            Angle = (BeginAngle > EndAngle) ? (EndAngle - BeginAngle) : (-2.0 * M_PI + (EndAngle - BeginAngle));
        Length = Radius * abs(Angle);
    }
    // Compute arc delta
    double TimeSec = Length / Velocity;
    uint32_t NumOfSteps = static_cast<uint32_t>(TimeSec * 1000.0);
    double AngleDelta = Angle / NumOfSteps;
    // Iterate through steps and compute coordinates.
    double CurrentAz = BeginAzimuth;
    double CurrentEl = BeginElevation;
    double CurrentAngle = BeginAngle;
    for(uint32_t i = 0; i < NumOfSteps; ++i)
    {
        switch(Target)
        {
            case 0:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits = CHANNEL_1_STATE | CHANNEL_2_STATE | CHANNEL_3_STATE | CHANNEL_4_STATE |
                            POLAR_1_STATE | POLAR_2_STATE | POLAR_3_STATE | POLAR_4_STATE;
                else
                pScenario[Index].ActivityBits = CHANNEL_1_STATE | CHANNEL_2_STATE | CHANNEL_3_STATE | CHANNEL_4_STATE;
                pScenario[Index].Target1Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target1Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                pScenario[Index].Target2Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target2Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                pScenario[Index].Target3Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target3Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                pScenario[Index].Target4Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target4Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target1Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target2Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target3Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target4Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 1:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_1_STATE | POLAR_1_STATE;
                else
                pScenario[Index].ActivityBits |= CHANNEL_1_STATE;
                pScenario[Index].Target1Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target1Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target1Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 2:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_2_STATE | POLAR_2_STATE;
                else
                pScenario[Index].ActivityBits |= CHANNEL_2_STATE;
                pScenario[Index].Target2Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target2Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target2Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 3:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_3_STATE | POLAR_3_STATE;
                else
                pScenario[Index].ActivityBits |= CHANNEL_3_STATE;
                pScenario[Index].Target3Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target3Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target3Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 4:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_4_STATE | POLAR_4_STATE;
                else
                pScenario[Index].ActivityBits |= CHANNEL_4_STATE;
                pScenario[Index].Target4Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target4Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target4Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }
        }
        CurrentAngle += AngleDelta;
        CurrentAz = cos(CurrentAngle) * Radius + CenterAzimuth;
        CurrentEl = sin(CurrentAngle) * Radius + CenterElevation;
        ++Index;
    }
    return true;
}

bool computeLineLeg(uint32_t Target, BinaryData* pScenario, uint32_t& Index,
                   double Velocity, double BeginAzimuth, double BeginElevation,
                   double EndAzimuth, double EndElevation, bool PolarizationActive, PolarizationAngleComputer* pPolAlgo)
{
    double Length = std::sqrt((EndAzimuth - BeginAzimuth) * (EndAzimuth - BeginAzimuth) +
                              (EndElevation - BeginElevation) * (EndElevation - BeginElevation));
//    double Angle = asin(BeginAzimuth - BeginAzimuth) / Length;
    double TimeSec = Length / Velocity;
    uint32_t NumOfSteps = static_cast<uint32_t>(TimeSec * 1000);
    double AzDelta = (EndAzimuth - BeginAzimuth) / NumOfSteps;
    double ElDelta = (EndElevation - BeginElevation) / NumOfSteps;

    double CurrentAz = BeginAzimuth;
    double CurrentEl = BeginElevation;
    for(uint32_t i = 0; i < NumOfSteps; ++i)
    {
        switch(Target)
        {
            case 0:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits = CHANNEL_1_STATE | CHANNEL_2_STATE | CHANNEL_3_STATE | CHANNEL_4_STATE |
                            POLAR_1_STATE | POLAR_2_STATE | POLAR_3_STATE | POLAR_4_STATE;
                else
                pScenario[Index].ActivityBits = CHANNEL_1_STATE | CHANNEL_2_STATE | CHANNEL_3_STATE | CHANNEL_4_STATE;
                pScenario[Index].Target1Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target1Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                pScenario[Index].Target2Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target2Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                pScenario[Index].Target3Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target3Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                pScenario[Index].Target4Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target4Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target1Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target2Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target3Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pScenario[Index].Target4Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 1:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_1_STATE | POLAR_1_STATE;
                else
                pScenario[Index].ActivityBits |= CHANNEL_1_STATE;
                pScenario[Index].Target1Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target1Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target1Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 2:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_2_STATE | POLAR_2_STATE;
                else
                pScenario[Index].ActivityBits |= CHANNEL_2_STATE;
                pScenario[Index].Target2Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target2Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target2Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 3:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_3_STATE | POLAR_3_STATE;
                else
                pScenario[Index].ActivityBits |= CHANNEL_3_STATE;
                pScenario[Index].Target3Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target3Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target3Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }

            case 4:
            {
                if(PolarizationActive)
                    pScenario[Index].ActivityBits |= CHANNEL_4_STATE | POLAR_4_STATE;
                else
                pScenario[Index].ActivityBits |= CHANNEL_4_STATE;
                pScenario[Index].Target4Azimuth = static_cast<int16_t>(round(CurrentAz * 10.0));
                pScenario[Index].Target4Elevation = static_cast<int16_t>(round(CurrentEl * 10.0));
                if(PolarizationActive)
                {
                    pScenario[Index].Target4Polarization = static_cast<uint16_t>(pPolAlgo->getAngle());
                    pPolAlgo->run(0.001);
                }
                break;
            }
        }
        CurrentAz += AzDelta;
        CurrentEl += ElDelta;
        ++Index;
    }

    return true;
}

bool silentCycles(uint32_t Target, BinaryData* pScenario, uint32_t& Index, int32_t SilentCycles)
{
    for(int32_t i = 0; i < SilentCycles; ++i)
    {
        switch(Target)
        {
            case 0:
            {
                pScenario[Index].ActivityBits = 0;
                break;
            }

            case 1:
            {
                pScenario[Index].ActivityBits &= ~(CHANNEL_1_STATE | POLAR_1_STATE);
                break;
            }

            case 2:
            {
                pScenario[Index].ActivityBits &= ~(CHANNEL_2_STATE | POLAR_2_STATE);
                break;
            }

            case 3:
            {
                pScenario[Index].ActivityBits &= ~(CHANNEL_3_STATE | POLAR_3_STATE);
                break;
            }

            case 4:
            {
                pScenario[Index].ActivityBits &= ~(CHANNEL_4_STATE | POLAR_4_STATE);
                break;
            }
        }
        ++Index;
    }
    return true;
}

void PolarizationAngleComputer::computeAngle(PolarizationType Type, double& CurrentAngle, bool& Clockwise, double TimeDelta, double AngleRate, double MinAngle, double MaxAngle)
{
    switch(Type)
    {
    case PolarizationType::Cyrcular:
        if(Clockwise)
        {
            CurrentAngle += AngleRate * TimeDelta;
        }
        else
        {
            CurrentAngle -= AngleRate * TimeDelta;
        }
        // Normalize
        CurrentAngle = CurrentAngle + 360.0 - 360.0 * (floor(CurrentAngle / 360.0) + 1.0);
        break;

    case PolarizationType::Sweep:
        if(MinAngle >= MaxAngle)
        {
            MaxAngle += 360.0;
            if(MinAngle > CurrentAngle)
                CurrentAngle += 360.0;
        }
        if(Clockwise)
        {
            CurrentAngle += AngleRate * TimeDelta;
            if(CurrentAngle >= MaxAngle)
                Clockwise = false;
        }
        else
        {
            CurrentAngle -= AngleRate * TimeDelta;
            if(CurrentAngle <= MinAngle)
                Clockwise = true;
        }
        // Normalize
        CurrentAngle = CurrentAngle + 360.0 - 360.0 * (floor(CurrentAngle / 360.0) + 1.0);
        break;

    default:
        ;
    }
}
