#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);
    int start(int argc, char *argv[]);

};

#endif // UPDATER_H
