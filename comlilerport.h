#ifndef COMPILERPORT_H
#define COMPILERPORT_H


#include <QStringList>

#include <QDebug>

class CompilerPort
{
public:
    CompilerPort();

    QStringList compiler_port(QStringList staticList,int len);

};

#endif // COMPILERPORT_H
