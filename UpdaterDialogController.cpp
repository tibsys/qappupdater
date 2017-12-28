#include "UpdaterDialogController.h"
#include "ui_UpdaterDialog.h"
#include "QAppUpdater.h"

namespace com {
namespace tibsys {

UpdaterDialogController::UpdaterDialogController(QAppUpdater* updater) :
    ui(new Ui::UpdaterDialog)
  , updater_(updater)
  , versionId_(0)
  , currentProgressValue_(0)
  , progressInverted_(false)
{       
    ui->setupUi(this);

    ui->lblNewVersion->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);

    origOperation_ = ui->lblOperation->text();
}

UpdaterDialogController::~UpdaterDialogController()
{
    delete ui;
}

void UpdaterDialogController::setLogo(QPixmap logo)
{
    ui->lblLogo->setPixmap(logo);
}

void UpdaterDialogController::setSubtitle(QString subtitle)
{
    ui->lblSubtitle->setText(subtitle);
}

void UpdaterDialogController::setCurrentOperation(QString operation)
{
    ui->lblOperation->setText(origOperation_.arg(operation));
}

void UpdaterDialogController::setNewVersion(QAppUpdateInfo info)
{
    versionId_ = info.getId();
    ui->lblNewVersion->setStyleSheet("color: rgb(255, 191, 0);");
    ui->lblNewVersion->setText(ui->lblNewVersion->text().arg(QString::number(info.getBranch())).arg(QString::number(info.getMajor())).arg(QString::number(info.getMinor())));
    ui->lblNewVersion->setVisible(true);

    ui->fldComment->setText(info.getComment());
}

void UpdaterDialogController::setErrorMessage(QString message)
{
    ui->lblNewVersion->setStyleSheet("color: rgb(200, 10, 10);");
    ui->lblNewVersion->setText(message);
}

void UpdaterDialogController::showProgressBar(bool show)
{
    ui->progressBar->setVisible(show);
}

void UpdaterDialogController::setProgress(int progress)
{
    ui->progressBar->setValue(progress);
}

void UpdaterDialogController::animateProgressBar()
{
    progressAnimationTimer_ = new QTimer(this);
    progressAnimationTimer_->setInterval(10);
    connect(progressAnimationTimer_, SIGNAL(timeout()), SLOT(onAnimateProgress()));
    progressAnimationTimer_->start();
}

void UpdaterDialogController::stopAnimateProgressBar()
{
    progressAnimationTimer_->stop();
}

void UpdaterDialogController::disableCancel(bool disable)
{
    ui->btnCancel->setDisabled(disable);
}

void UpdaterDialogController::onAnimateProgress()
{
    if(currentProgressValue_ <= 99) {
        if(!progressInverted_)
            currentProgressValue_ += 1;
        else
            currentProgressValue_ -= 1;
    } else if(currentProgressValue_ >= 100) {
        progressInverted_ = true;
        ui->progressBar->setInvertedAppearance(true);
        currentProgressValue_ = 99;
    }
    if(currentProgressValue_ <= 0)
    {
        progressInverted_ = false;
        ui->progressBar->setInvertedAppearance(false);
    }

    ui->progressBar->setValue(currentProgressValue_);
}

void UpdaterDialogController::on_btnUpdate_clicked()
{
    updater_->getUpdate(versionId_);
}

void UpdaterDialogController::on_btnCancel_clicked()
{
    close();
}


}
}
