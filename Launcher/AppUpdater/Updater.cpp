#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "Updater.h"

Updater::Updater(QObject *parent) :
    QObject(parent)
{
}

int Updater::start(int argc, char *argv[])
{
    if(argc < 2)
    {
        QMessageBox::critical(0, tr("Arguments"), tr("Missing arguments !"), QMessageBox::Abort);
        return 1;
    }

    QString filename = argv[1];
    if(!filename.isEmpty())
    {
        //This is a complete filename, we divide it
        QString updateFilepath;
        QString updateFilename;

        QFile newFile(filename);
        if(newFile.exists())
        {
            QFileInfo fi(newFile);
            updateFilename = fi.fileName();
            updateFilepath = fi.absolutePath();

            QString localFilename = QString("%1\\%2").arg(QDir::currentPath()).arg(updateFilename);
            QFile localFile(localFilename);
            if(localFile.exists())
            {
                //1 - Backup existing file
                bool ok = localFile.rename(QString("%1\\%2_backup").arg(QDir::currentPath()).arg(updateFilename));
                if(!ok)
                {
                    QMessageBox::critical(0, tr("Upgrade process"), tr("Unable to backup file !"), QMessageBox::Abort);
                    qDebug() << "Unable to backup file";
                    return 1;
                }
            }

            //2 - Copy the new file
            bool ok = newFile.rename(localFilename);
            if(ok)
            {
                QMessageBox::information(0, tr("Upgrade process"), tr("The upgrade was successfull ! Please restart the application."), QMessageBox::Ok);
                return 0;
            } else {
                QMessageBox::critical(0, tr("Upgrade process"), tr("Unable to install the new file !"), QMessageBox::Abort);
                qDebug() << "Unable to move new file";
                return 1;
            }
        } else {
            QMessageBox::critical(0, tr("Upgrade process"), tr("The file %1 does not exist !").arg(filename), QMessageBox::Abort);
            qDebug() << "The file does not exist";
            return 1;
        }
    }

    return 0;
}
