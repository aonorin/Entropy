###########################################################
# FontTextureRanker
# Used to create font keys for the ascii shader in Entropy
###########################################################

QT       += core opengl gui

TARGET = FontTextureRanker
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    fonttexture.cpp \
    fontcharacter.cpp \
    fontanalyzer.cpp

RESOURCES += \
    FontTextureRankerResources.qrc

HEADERS += \
    fonttexture.h \
    fontcharacter.h \
    fontanalyzer.h
