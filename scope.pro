QT += core serialport qml gui widgets

CONFIG += serialport
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
TEMPLATE = app

SOURCES += $$files(lib/*.cpp)
SOURCES += $$files(*.c)
SOURCES += $$files(*.cpp)

HEADERS += $$files(lib/*.h)
HEADERS += $$files(*.h)

DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

binfile.files+=build/scope
binfile.path=/usr/bin

INSTALLS += binfile datafiles
