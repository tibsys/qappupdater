#include <QMessageBox>
#include <QTranslator>
#include <QLibraryInfo>
#include <QApplication>

#include "QAppUpdater.h"

namespace com {
namespace tibsys {

const QString QAppUpdater::LANG_ALL = "All";

QAppUpdater::QAppUpdater(QString webserviceUrl, QString appSku, int currentBranch, int currentMajor, int currentMinor, int currentOs, int currentArch, QString currentLang, int level, QWidget* parent)
    : QWidget(parent), delay_(0), appSku_(appSku), webserviceUrl_(webserviceUrl), currentBranch_(currentBranch), currentMajor_(currentMajor), currentMinor_(currentMinor)
    , currentOs_(currentOs), currentArch_(currentArch), currentLang_(currentLang), level_(level), client_(new QAppHttpClient(this))
    , showGui_(true), showAlerts_(true)
    , ctrl_(0)
{
    connect(client_, SIGNAL(signalError(QString)), this, SLOT(onUpdateError(QString)));
    connect(client_, SIGNAL(signalNewUpdate(QAppUpdateInfo)), this, SLOT(onNewUpdate(QAppUpdateInfo)));
    connect(client_, SIGNAL(signalGotUpdate(QAppUpdateFile)), this, SLOT(onGotUpdate(QAppUpdateFile)));
    connect(client_, SIGNAL(signalNoUpdate()), this, SLOT(onNoUpdate()));

    QTranslator translator;
    QString language = "en";
    switch(QLocale::system().language())
    {
    case QLocale::French: language = "fr"; break;
    default: language = "en";
    }
    bool loaded = translator.load(QString(":/i18n/QAppUpdater_%1").arg(language), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&translator);

    ctrl_ = new UpdaterDialogController(this);
}

void QAppUpdater::setLogo(QPixmap logo)
{
    logo_ = logo;
}

void QAppUpdater::start(int delayInSeconds, bool showGui, bool showAlerts)
{
    delay_ = delayInSeconds;
    showGui_ = showGui;    
    showAlerts_ = showAlerts;

    updateTimer_ = new QTimer(this);
    updateTimer_->setSingleShot(true);
    updateTimer_->setInterval(delay_*1000);
    connect(updateTimer_, SIGNAL(timeout()), this, SLOT(checkUpdatePrivate()));
    //updateTimer_->start();
    QMetaObject::invokeMethod(updateTimer_, "start", Qt::QueuedConnection);
}

void QAppUpdater::checkUpdatePrivate()
{
    ctrl_->setCurrentOperation(tr("Querying the webservice..."));

    //Call the webservice
    client_->checkUpdate(webserviceUrl_, appSku_, currentOs_, currentArch_, currentLang_, currentBranch_, currentMajor_, currentMinor_, level_);

    if(showGui_)
        ctrl_->setLogo(logo_);
}

void QAppUpdater::getUpdate(int versionId)
{
    ctrl_->setCurrentOperation(tr("Downloading the update files..."));
    ctrl_->setProgress(0);
    ctrl_->animateProgressBar();
    ctrl_->showProgressBar(true);
    ctrl_->disableCancel(true);
    client_->getUpdate(webserviceUrl_, appSku_, versionId);
}

void QAppUpdater::checkUpdate(bool showGui, bool showAlerts)
{
    showGui_ = showGui;
    showAlerts_ = showAlerts;
    checkUpdatePrivate();
}

QString QAppUpdater::getVersion()
{
    return "1.1.1";
}

void QAppUpdater::onUpdateError(QString error)
{
    ctrl_->setErrorMessage(error);
    emit signalUpdateError(error);
}

void QAppUpdater::onNewUpdate(QAppUpdateInfo info)
{
    //qDebug() << "New update found";

    ctrl_->setCurrentOperation(tr("Waiting for a user action."));
    ctrl_->setNewVersion(info);
    emit signalNewUpdate(info);

    if(showGui_)
        ctrl_->show();
}

void QAppUpdater::onGotUpdate(QAppUpdateFile file)
{
    ctrl_->stopAnimateProgressBar();
    ctrl_->setProgress(0);
    ctrl_->setCurrentOperation(tr("Installing the new files..."));    

    //We check the MD5 sum and the size before sending the signal
    if(file.getMd5hash() != file.computeMd5hash())
        emit signalUpdateError(tr("The MD5 hash is wrong !"));
    else
        emit signalGotUpdate(file);

    //qDebug() << "File received : " << file.getLocalFile();

    ctrl_->close();
}

void QAppUpdater::onNoUpdate()
{
    if(showAlerts_)
        QMessageBox::information(0, tr("Application update"), tr("There is no update available !"), QMessageBox::Ok);

    emit signalNoUpdate();
}

}
}
