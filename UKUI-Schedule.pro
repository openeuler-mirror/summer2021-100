QT       += core gui     \
            network      \
            printsupport \
            sql          \
            concurrent   \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbmanager.cpp \
    main.cpp \
    mainCalendar.cpp \
    new_page.cpp \
    scheduledata.cpp \
    singleApplication.cpp \
    widget.cpp

HEADERS += \
    dbmanager.h \
    mainCalendar.h \
    new_page.h \
    scheduledata.h \
    singleApplication.h \
    widget.h

FORMS += \
    new_page.ui \
    widget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
