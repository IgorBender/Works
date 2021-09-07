cmake_minimum_required(VERSION 3.5)

project(PThreadClassLib LANGUAGES CXX)


add_library(PThreadClassLib SHARED
CondVarClass.cpp   PRWLockClass.cpp        PThreadClassLib.h
CondVarClass.h     PRWLockClass.h          PThreadExtended.cpp
PBarrierClass.cpp  PThreadBaseException.h  PThreadExtended.h
PBarrierClass.h    PThreadClass.cpp        ThreadException.cpp
PMutexClass.cpp    PThreadClass.h          ThreadException.h
PMutexClass.h      PThreadClassLib.cpp    
)

