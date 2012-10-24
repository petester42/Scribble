# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = Scribble_0.02
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui
SOURCES += scribblearea.cpp loadFileWindow.cpp mainwindow.cpp Point.cpp main.cpp inputData.cpp menu.cpp PalmRejection.cpp ColourPalette.cpp Keyboard.cpp Path.cpp SizeChooser.cpp ScreenAreas.cpp
HEADERS += menu.h inputData.h ScreenAreas.h PalmRejection.h Path.h Point.h SizeChooser.h loadFileWindow.h Keyboard.h scribblearea.h ColourPalette.h mainwindow.h defines.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += /usr/lib /usr/lib64 
LIBS += -lpoppler-qt4  
QMAKE_CXXFLAGS += -O0
