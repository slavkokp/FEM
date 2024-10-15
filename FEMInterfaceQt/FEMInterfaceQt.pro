QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets graphs

CONFIG += c++17



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ZPData.cpp \
    converter.cpp \
    displaytablemodel.cpp \
    main.cpp \
    mainwindow.cpp \
    plot_functions.cpp \
    vertexcreator.cpp \
    vertexdata.cpp \
    zpdatacreator.cpp \
    zudata.cpp \
    zudatacreator.cpp

HEADERS += \
    ITableDisplayable.h \
    ZPData.h \
    abstractcreator.h \
    converter.h \
    displaytablemodel.h \
    mainwindow.h \
    plot_functions.h \
    vertexcreator.h \
    vertexdata.h \
    zpdatacreator.h \
    zudata.h \
    zudatacreator.h



CONFIG(debug, debug|release) {
    #LIBS += -L"C:/Users/User/source/repos/WalkWithoutLagi/x64/Debug/" -lFEM
    LIBS += -L"$$PWD/../x64/Debug/" -lFEM
} else {
    #LIBS += -L"C:/Users/User/source/repos/WalkWithoutLagi/x64/Release/" -lFEM
    LIBS += -L"$$PWD/../x64/Release/" -lFEM
}

# path to matplot++ lib
INCLUDEPATH += "C:/Program Files/Matplot++ 1.2.0/include/"
LIBS += -L"C:/Program Files/Matplot++ 1.2.0/lib/" -lmatplot
LIBS += -L"C:/Program Files/Matplot++ 1.2.0/lib/Matplot++/" -lnodesoup

LIBS += -lGdi32 -lkernel32 -luser32

INCLUDEPATH += "C:/Users/User/source/repos/WalkWithoutLagi/Source/"



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

