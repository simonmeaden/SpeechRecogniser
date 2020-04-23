QT += gui widgets

TARGET   = SpeechRecogniserTest
TEMPLATE = app
DEFINES += SPEECHRECOGNISER_LIBRARY

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# header file for common projects
INCLUDEPATH += ../include

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

unix|win32: {
    LIBS += -L/usr/local/lib -lportaudiocpp
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SpeechRecogniser/release/ -lSpeechRecogniser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SpeechRecogniser/debug/ -lSpeechRecogniser
else:unix: LIBS += -L$$OUT_PWD/../SpeechRecogniser/ -lSpeechRecogniser

INCLUDEPATH += $$PWD/../SpeechRecogniser
DEPENDPATH += $$PWD/../SpeechRecogniser
