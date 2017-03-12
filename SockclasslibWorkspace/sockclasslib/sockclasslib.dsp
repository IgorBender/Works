# Microsoft Developer Studio Project File - Name="sockclasslib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=sockclasslib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sockclasslib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sockclasslib.mak" CFG="sockclasslib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sockclasslib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "sockclasslib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sockclasslib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "sockclasslib___Win32_Release"
# PROP BASE Intermediate_Dir "sockclasslib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SOCKCLASSLIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SOCKCLASSLIB_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40d /d "NDEBUG"
# ADD RSC /l 0x40d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "sockclasslib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "sockclasslib___Win32_Debug"
# PROP BASE Intermediate_Dir "sockclasslib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SOCKCLASSLIB_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SOCKCLASSLIB_EXPORTS" /FD /GZ  /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40d /d "_DEBUG"
# ADD RSC /l 0x40d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "sockclasslib - Win32 Release"
# Name "sockclasslib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BoundSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\BroadcastBound.cpp
# End Source File
# Begin Source File

SOURCE=.\BroadcastSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ClentBound.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSimple.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectedSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\DatagramBound.cpp
# End Source File
# Begin Source File

SOURCE=.\DatagramSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\InternetSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\MulticastBound.cpp
# End Source File
# Begin Source File

SOURCE=.\MulticastSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerReuse.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSimple.cpp
# End Source File
# Begin Source File

SOURCE=.\sockclasslib.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketClass.cpp
# End Source File
# Begin Source File

SOURCE=.\SockException.cpp
# End Source File
# Begin Source File

SOURCE=.\StreamBound.cpp
# End Source File
# Begin Source File

SOURCE=.\StreamSocket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseException.h
# End Source File
# Begin Source File

SOURCE=.\BoundSocket.h
# End Source File
# Begin Source File

SOURCE=.\BroadcastBound.h
# End Source File
# Begin Source File

SOURCE=.\BroadcastSocket.h
# End Source File
# Begin Source File

SOURCE=.\ClientBound.h
# End Source File
# Begin Source File

SOURCE=.\ClientSimple.h
# End Source File
# Begin Source File

SOURCE=.\ConnectedSocket.h
# End Source File
# Begin Source File

SOURCE=.\DatagramBound.h
# End Source File
# Begin Source File

SOURCE=.\DatagramSocket.h
# End Source File
# Begin Source File

SOURCE=.\DisconnectException.h
# End Source File
# Begin Source File

SOURCE=.\InternetSocket.h
# End Source File
# Begin Source File

SOURCE=.\MulticastBound.h
# End Source File
# Begin Source File

SOURCE=.\MulticastSocket.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ServerReuse.h
# End Source File
# Begin Source File

SOURCE=.\ServerSimple.h
# End Source File
# Begin Source File

SOURCE=.\sockclasslib.h
# End Source File
# Begin Source File

SOURCE=.\SocketClass.h
# End Source File
# Begin Source File

SOURCE=.\SockException.h
# End Source File
# Begin Source File

SOURCE=.\StreamBound.h
# End Source File
# Begin Source File

SOURCE=.\StreamSocket.h
# End Source File
# Begin Source File

SOURCE=.\TraceException.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\errtab.rc
# End Source File
# End Group
# End Target
# End Project
