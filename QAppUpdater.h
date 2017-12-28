#ifndef QAPPUPDATER_H
#define QAPPUPDATER_H

#include <QTimer>
#include <QWidget>
#include <QTranslator>
#include <QObject>

#include "QAppHttpClient.hpp"
#include "QAppUpdateFile.hpp"
#include "QAppUpdateInfo.hpp"
#include "UpdaterDialogController.h"
#include "QAppPlatform.h"

namespace com {
namespace tibsys {

class QAPPUPDATER_API QAppUpdater : public QWidget {
    Q_OBJECT

public:
    QAppUpdater(QString webserviceUrl, QString appSku, int currentBranch, int currentMajor, int currentMinor, int currentOs, int currentArch, QString currentLang, int level = PRODUCTION, QWidget* parent = 0);
    void start(int delayInSeconds = 120, bool showGui = true, bool showAlerts = true);
    void getUpdate(int versionId);
    void checkUpdate(bool showGui = true, bool showAlerts = true);
    void setLogo(QPixmap);
    static QString getVersion();

signals:
    void signalNewUpdate(QAppUpdateInfo);
    void signalUpdateError(QString);
    void signalGotUpdate(QAppUpdateFile);
    void signalNoUpdate();

private slots:
    void checkUpdatePrivate();

private slots:
    void onUpdateError(QString);
    void onNewUpdate(QAppUpdateInfo);
    void onGotUpdate(QAppUpdateFile);
    void onNoUpdate();

public:
    enum OsTypes { OS_WINDOWS = 1, OS_MACOS, OS_GNULINUX };
    enum ArchTypes { ARCH_X86 = 1, ARCH_X64 };
    enum Levels { PRODUCTION = 0, PREPRODUCTION = 1, DEBUG = 2 };
    static const QString LANG_ALL;

private:
    int     delay_;
    QTimer* updateTimer_;
    QString appSku_;
    QString webserviceUrl_;
    int     currentBranch_;
    int     currentMajor_;
    int     currentMinor_;
    int     currentOs_;
    int     currentArch_;
    QString currentLang_;
    int     level_;

    QAppHttpClient* client_;
    bool showGui_;
    //bool silent_;
    bool showAlerts_;

    UpdaterDialogController*  ctrl_;
    QPixmap     logo_;
};

}
}


#endif // QAPPUPDATER_H
