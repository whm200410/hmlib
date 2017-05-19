CONFIG(debug, debug|release) {
    HMLIB_NAME = hmlibD
	LIBS += -lhmlibD
}
CONFIG(release, debug|release) {
    HMLIB_NAME = hmlib
	LIBS += -lhmlib
}
QTPROPERTYBROWSER_LIBDIR = $$PWD/lib
DEPENDPATH += $$PWD
LIBS += -L$$(hmlib_QT)/bin
		
		
	
INCLUDEPATH +=  \
                $$(hmlib_QT)/include/msg   \
		$$(hmlib_QT)/include/utils \
                $$(hmlib_QT)/include/service			
