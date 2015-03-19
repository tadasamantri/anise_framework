QT += core
QT -= gui

TARGET = pythonnode
TEMPLATE = lib
CONFIG += plugin
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../src_framework \
               ../../src_data \
               extras/pythonqt/include

# Include the PythonQt libraries
LIBS += -L./extras/pythonqt/ -lPythonQt
# Include the Python3 bindings
CONFIG += link_pkgconfig
PKGCONFIG += python3

CONFIG(debug,debug|release) {
  # Debug...
  DESTDIR = ../../bin/debug/nodes
  OBJECTS_DIR = build/debug
  MOC_DIR = build/debug/moc
  RCC_DIR = build/debug/rcc
} else {
  # Release...
  DESTDIR = ../../bin/release/nodes
  OBJECTS_DIR = build/release
  MOC_DIR = build/release/moc
  RCC_DIR = build/release/rcc
  #DEFINES += QT_NO_DEBUG_OUTPUT
}

# Copy the PythonQt library.
pythonqt.path = $$DESTDIR/extras
pythonqt.files = extras/pythonqt/libPythonQt.so.1

INSTALLS += pythonqt

QMAKE_CLEAN += $$DESTDIR/*$$TARGET*

HEADERS += \
    pythonnode.h \
    interface.h

SOURCES += \
    pythonnode.cpp \
    interface.cpp
