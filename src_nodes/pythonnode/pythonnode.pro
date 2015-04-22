QT += core
QT -= gui

TARGET = pythonnode
TEMPLATE = lib
CONFIG += plugin
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../src_framework \
               ../../src_data \
               extras/pythonqt/include

# Include the PythonQt libraries.
LIBS += -L./extras/pythonqt/
# Include the Python3 bindings using 'pkg-conf'.
CONFIG += link_pkgconfig
PKGCONFIG += python3

CONFIG(debug,debug|release) {
  # Debug...
  DESTDIR = ../../bin/debug/nodes
  OBJECTS_DIR = build/debug
  MOC_DIR = build/debug/moc
  RCC_DIR = build/debug/rcc
  # PythonQt libraries
  LIBS +=  -lPythonQt_d
  # Copy the PythonQt library.
  pythonqt.path = $$DESTDIR/extras
  pythonqt.files = extras/pythonqt/libPythonQt_d.so.1
  INSTALLS += pythonqt
} else {
  # Release...
  DESTDIR = ../../bin/release/nodes
  OBJECTS_DIR = build/release
  MOC_DIR = build/release/moc
  RCC_DIR = build/release/rcc
  #DEFINES += QT_NO_DEBUG_OUTPUT
  DEFINES += QT_MESSAGELOGCONTEXT
  # PythonQt libraries
  LIBS +=  -lPythonQt
  # Copy the PythonQt library.
  pythonqt.path = $$DESTDIR/extras
  pythonqt.files = extras/pythonqt/libPythonQt.so.1
  INSTALLS += pythonqt
}


QMAKE_CLEAN += $$DESTDIR/*$$TARGET*

HEADERS += \
    pythonnode.h \
    interface.h \
    tabledatadecor.h \
    datawrapfactory.h \
    tabledatawrap.h \
    flowsdatawrap.h \
    flowwrap.h

SOURCES += \
    pythonnode.cpp \
    interface.cpp \
    tabledatadecor.cpp \
    datawrapfactory.cpp \
    tabledatawrap.cpp \
    flowsdatawrap.cpp \
    flowwrap.cpp
