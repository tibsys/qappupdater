#ifndef UPDATERDIALOGCONTROLLER_H
#define UPDATERDIALOGCONTROLLER_H

#include <QDialog>
#include "QAppUpdateInfo.hpp"

namespace Ui {
class UpdaterDialog;
}

namespace com {
namespace tibsys {

class QAppUpdater;

class UpdaterDialogController : public QDialog
{
    Q_OBJECT
    
public:
    explicit UpdaterDialogController(QAppUpdater* updater);
    ~UpdaterDialogController();

    void setLogo(QPixmap logo);
    void setSubtitle(QString);
    void setCurrentOperation(QString);
    void setNewVersion(QAppUpdateInfo);
    void setErrorMessage(QString);
    void showProgressBar(bool show);
    void setProgress(int progress);
    void animateProgressBar();
    void stopAnimateProgressBar();
    void disableCancel(bool);

private slots:
    void on_btnUpdate_clicked();
    void on_btnCancel_clicked();
    void onAnimateProgress();

private:
    Ui::UpdaterDialog *ui;
    QString origOperation_;
    QAppUpdater* updater_;
    int versionId_;
    QTimer* progressAnimationTimer_;
    int currentProgressValue_;
    bool progressInverted_;
};

}
}

#endif // UPDATERDIALOGCONTROLLER_H
