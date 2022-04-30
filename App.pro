QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    edits.cpp \
    flags.cpp \
    frames.cpp \
    ipcam.cpp \
    main.cpp \
    mainwindow.cpp \
    observer.cpp \
    path.cpp \
    preframes.cpp \
    result_form.cpp \
    settings.cpp \
    thread_fire.cpp \
    thread_general.cpp \
    thread_smoke.cpp \
    usbcam.cpp \
    video.cpp


HEADERS += \
    edits.h \
    flags.h \
    frames.h \
    ipcam.h \
    mainwindow.h \
    observer.h \
    path.h \
    preframes.h \
    result_form.h \
    settings.h \
    thread_fire.h \
    thread_general.h \
    thread_smoke.h \
    usbcam.h \
    video.h

FORMS += \
    fire_mask.ui \
    ipcam.ui \
    mainwindow.ui \
    result_form.ui \
    settings.ui \
    smoke_mask.ui \
    usbcam.ui \
    video.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += C:/opencv/release/install/include

LIBS += C:/opencv/release/install/x64/mingw/lib/libopencv_core420.dll.a
LIBS += C:/opencv/release/install/x64/mingw/lib/libopencv_highgui420.dll.a
LIBS += C:/opencv/release/install/x64/mingw/lib/libopencv_imgcodecs420.dll.a
LIBS += C:/opencv/release/install/x64/mingw/lib/libopencv_video420.dll.a
LIBS += C:/opencv/release/install/x64/mingw/lib/libopencv_videoio420.dll.a
LIBS += C:/opencv/release/install/x64/mingw/lib/libopencv_imgproc420.dll.a

