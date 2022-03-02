QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fm_editarprodutovenda.cpp \
    fm_gestaocolaboradores.cpp \
    fm_gestaoestoque.cpp \
    fm_gestaovendas.cpp \
    fm_logar.cpp \
    fm_novavenda.cpp \
    funcoes_globais.cpp \
    main.cpp \
    fm_principal.cpp

HEADERS += \
    conexao.h \
    fm_editarprodutovenda.h \
    fm_gestaocolaboradores.h \
    fm_gestaoestoque.h \
    fm_gestaovendas.h \
    fm_logar.h \
    fm_novavenda.h \
    fm_principal.h \
    funcoes_globais.h \
    variaveis_globais.h

FORMS += \
    fm_editarprodutovenda.ui \
    fm_gestaocolaboradores.ui \
    fm_gestaoestoque.ui \
    fm_gestaovendas.ui \
    fm_logar.ui \
    fm_novavenda.ui \
    fm_principal.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Arquivo_de_recursos.qrc
