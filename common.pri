CONFIG(debug, debug|release) {
    HMLIB_NAME = hmlibD
}
CONFIG(release, debug|release) {
    HMLIB_NAME = hmlib
}
QTPROPERTYBROWSER_LIBDIR = $$PWD/lib
DEPENDPATH += $$PWD
        
        
    
INCLUDEPATH +=  \
                $$(hmlib_QT)/include/msg   \
                $$(hmlib_QT)/include/utils \
                $$(hmlib_QT)/include/log \
                $$(hmlib_QT)/include/ipc \
                $$(hmlib_QT)/include/service
