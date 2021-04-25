lessThan(QT_MAJOR_VERSION, 5) {
  QT += gui
} else {
  QT += widgets
}
DEFINES += "QAPPLICATION_CLASS=QApplication"


HEADERS       = toolwindowmanager.h
SOURCES       = toolwindowmanager.cpp \
                main.cpp

FORMS         = toolwindowmanager.ui

include (../SingleApplication/singleapplication.pri)
include (../qtterminalwidget/qtterminalwidget.pri)

LIBS += -lqtoolwindowmanager -L../libqtoolwindowmanager
LIBS +=  -lqtterminalwidget  -L../qtterminalwidget

INCLUDEPATH += ../libqtoolwindowmanager
INCLUDEPATH += ../qtterminalwidget
