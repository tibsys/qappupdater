#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QStringList>

class Launcher : public QObject
{
    Q_OBJECT
public:
    explicit Launcher(QObject *parent = 0);
    int start(int argc, char *argv[]);

};

#endif // LAUNCHER_H
