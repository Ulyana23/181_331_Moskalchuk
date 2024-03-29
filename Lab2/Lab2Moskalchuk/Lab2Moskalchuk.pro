QT += quick core widgets gui

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        controller.cpp \
        cryptocontroller.cpp \
        main.cpp \
        model.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data.json

HEADERS += \
    controller.h \
    cryptocontroller.h \
    model.h

include (C:/Users/User/Documents/SortFilterProxyModel/SortFilterProxyModel.pri)


INCLUDEPATH += C:\Qt\Tools\OpenSSL\Win_x64\include
LIBS += -L"C:\Qt\Tools\OpenSSL\Win_x64\bin"
LIBS += -L"C:\Qt\Tools\OpenSSL\Win_x64\lib" -llibssl
LIBS += -L"C:\Qt\Tools\OpenSSL\Win_x64\lib" -llibcrypto
