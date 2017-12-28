#include <QMessageBox>
#include <QStringList>
#include <QSettings>
#include <QProcess>
#include <QDir>
#include <Windows.h>
#include <ShellAPI.h>

#include "Launcher.h"
#include "QAppUpdaterHelper.hpp"

Launcher::Launcher(QObject *parent) :
    QObject(parent)
{
}

int Launcher::start(int argc, char *argv[])
{
    //We need an application key name for the update key
    if(argc < 3)
    {
        QMessageBox::critical(0, tr("Missing arguments"), tr("Arguments are missing !"), QMessageBox::Ok);
        return 1;
    }

    //Then we check if there is an update pending
    QString updateFilename;

    QString appName = argv[1];
    QString exeName = argv[2];
    if(appName != "")
    {
        if(com::tibsys::QAppUpdaterHelper::hasUpdaterFilename(appName))
            updateFilename = com::tibsys::QAppUpdaterHelper::getUpdaterFilename(appName);        

        if(updateFilename != "")
        {
            //We have an update file, we start the updater
            QStringList args;
            args << updateFilename;

            int ret = QMessageBox::information(0, tr("Upgrade process"), tr("We have an upgrade for your application. The upgrade process will launch as administrator, the system way ask your password."), QMessageBox::Ok, QMessageBox::Abort);
            if(ret == QMessageBox::Ok)
            {
                SHELLEXECUTEINFO shInfo = {0};
                shInfo.cbSize = sizeof(SHELLEXECUTEINFO);
                shInfo.fMask = NULL;
                shInfo.hwnd = NULL;
                shInfo.lpVerb = L"runas";
                shInfo.lpFile = L"AppUpdater.exe";
                std::wstring wUpdateFilename = updateFilename.toStdWString();
                shInfo.lpParameters = wUpdateFilename.c_str();
                shInfo.nShow = SW_SHOWNORMAL;
                shInfo.hInstApp = NULL;
                ShellExecuteEx(&shInfo); //Execute the updater
                WaitForSingleObject(shInfo.hProcess, INFINITE);                
            }

            com::tibsys::QAppUpdaterHelper::resetUpdaterFilename(appName);
        }

        //Launch app
        if(exeName != "")
        {
            QStringList pargs = QStringList();
            bool ok = QProcess::startDetached(exeName, pargs, QDir::currentPath());
            if(!ok)
                QMessageBox::critical(0, tr("AppLauncher"), tr("Impossible to start the application !"), QMessageBox::Abort);
        } else
            QMessageBox::critical(0, tr("AppLauncher"), tr("Missing exe name"), QMessageBox::Abort);
    } else {
        QMessageBox::critical(0, tr("AppLauncher"), tr("Missing app name"), QMessageBox::Abort);
    }

    return 0;
}
