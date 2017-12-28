#ifndef QAPPUPDATERHELPER_HPP
#define QAPPUPDATERHELPER_HPP

#include <QSettings>
#include <QString>

namespace com {
namespace tibsys {

class QAppUpdaterHelper
{
public:
    static void setUpdaterFilename(QString appName, QString filename)
    {
        QSettings settings("tibSys", "QAppUpdater");
        settings.setValue(appName, filename);
    }

    static QString getUpdaterFilename(QString appName)
    {
        QSettings settings("tibSys", "QAppUpdater");
        return settings.value(appName).toString();
    }

    static void resetUpdaterFilename(QString appName)
    {
        QSettings settings("tibSys", "QAppUpdater");
        settings.setValue(appName, "");
    }

    static bool hasUpdaterFilename(QString appName)
    {
        return !getUpdaterFilename(appName).isEmpty();
    }
};

}
}

#endif // QAPPUPDATERHELPER_HPP
