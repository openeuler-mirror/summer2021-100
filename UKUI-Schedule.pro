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
    adderrorwidget.cpp \
    customSystemTrayicon.cpp \
    dbmanager.cpp \
    main.cpp \
    mainCalendar.cpp \
    monthScheduleCheck.cpp \
    monthscheduleBuddy.cpp \
    new_page.cpp \
    scheduleExitWindow.cpp \
    scheduledata.cpp \
    singleApplication.cpp \
    widget.cpp

HEADERS += \
    adderrorwidget.h \
    customSystemTrayicon.h \
    dbmanager.h \
    mainCalendar.h \
    monthScheduleCheck.h \
    monthscheduleBuddy.h \
    new_page.h \
    scheduleExitWindow.h \
    scheduledata.h \
    singleApplication.h \
    widget.h

FORMS += \
    adderrorwidget.ui \
    monthScheduleCheck.ui \
    monthscheduleBuddy.ui \
    new_page.ui \
    scheduleExitWindow.ui \
    widget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    myImage.qrc
