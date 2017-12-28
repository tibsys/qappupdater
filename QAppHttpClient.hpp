#ifndef QAPPHTTPCLIENT_HPP
#define QAPPHTTPCLIENT_HPP

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QDomDocument>
#include <QDir>
#include <QFile>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QUrlQuery>
#else
#include <QUrl>
#endif

#include "QAppUpdateInfo.hpp"
#include "QAppUpdateFile.hpp"

namespace com {
namespace tibsys {

class QAppHttpClient : public QObject
{
    Q_OBJECT
public:
    QAppHttpClient(QObject* parent)
        : QObject(parent), nam_(new QNetworkAccessManager(this)), step_(STEP_NONE), isDownloading_(false)
    {
        connect(nam_, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReplyFinished(QNetworkReply*)));
    }

    void checkUpdate(QString webserviceUrl, QString appSku, int currentOs, int currentArch, QString lang, int branch, int major, int minor, int level)
    {
        step_ = STEP_CHECK;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QUrlQuery url(webserviceUrl);
#else
        QUrl url(webserviceUrl);
#endif
        url.addQueryItem("checkUpdate", "");
        url.addQueryItem("os", QString::number(currentOs));
        url.addQueryItem("arch", QString::number(currentArch));
        url.addQueryItem("lang", lang);
        url.addQueryItem("appsku", appSku);
        url.addQueryItem("level", QString::number(level));
        url.addQueryItem("branch", QString::number(branch));
        url.addQueryItem("major", QString::number(major));
        url.addQueryItem("minor", QString::number(minor));
       // qDebug() << "Updater query : " << url.toString();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        nam_->get(QNetworkRequest(QUrl(url.toString())));
#else
        nam_->get(QNetworkRequest(url));
#endif
    }

    void getUpdate(QString webserviceUrl, QString appSku, int versionId)
    {
        if(isDownloading_)
            return;

        step_ = STEP_GET;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QUrlQuery url(webserviceUrl);
#else
        QUrl url(webserviceUrl);
#endif
        url.addQueryItem("getUpdate", "");
        url.addQueryItem("versionId", QString::number(versionId));
        url.addQueryItem("appsku", appSku);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        nam_->get(QNetworkRequest(QUrl(url.toString())));
#else
        nam_->get(QNetworkRequest(url));
#endif
        isDownloading_ = true;
    }

signals:
    void signalError(QString);
    void signalNewUpdate(QAppUpdateInfo);
    void signalGotUpdate(QAppUpdateFile);
    void signalNoUpdate();

private slots:
    void onReplyFinished(QNetworkReply* reply)
    {        
        QString error = "";
        QString resp = reply->readAll();

        //First of all we have to clean the response to remove eventual empty lines
        QStringList respList = resp.split("\n");
        QStringListIterator it(respList);
        resp = QString("");
        while(it.hasNext())
        {
            QString l = it.next();
            if(l.trimmed() != "")
                resp.append(l);
        }

        //qDebug() << "Update reply : " << resp.left(100);

        QAppUpdateInfo info;
        QAppUpdateFile file;
        bool noUpdate = false;

        if(resp != "")
        {
            QDomDocument doc;
            doc.setContent(resp);
            QDomElement root = doc.documentElement();

            //qDebug() << "root elt : " << root.tagName();
            if(root.tagName() == "update")
            {
                QDomElement next = root.firstChild().toElement();
                QDomElement before;
                while(!next.isNull())
                {
                    //qDebug() << "tag : " << next.tagName();

                    if(next.tagName() == "error") {
                        error = next.text();
                        noUpdate = true;
                    } else {
                        if(step_ == STEP_CHECK)
                        {
                            //We received a valid check response
                            if(next.tagName() == "no-version")
                            {
                                emit signalNoUpdate();
                                noUpdate = true;
                                break;
                            } else if(next.tagName() == "version")
                            {
                                info.setId(next.attribute("id", "0").toInt());
                                info.setBranch(next.attribute("branch", "0").toInt());
                                info.setMajor(next.attribute("major", "0").toInt());
                                info.setMinor(next.attribute("minor", "0").toInt());
                                info.setOs(next.attribute("os", "0").toInt());
                                info.setArch(next.attribute("arch", "0").toInt());
                                info.setOsName(next.attribute("osName", tr("unknown")));
                                info.setArchName(next.attribute("archName", tr("unknown")));
                                info.setLanguage(next.attribute("language", tr("unknown")));
                            } else if(next.tagName() == "file")
                            {
                                info.setFilename(next.attribute("filename", ""));
                                info.setMd5hash(next.attribute("md5hash", ""));
                            } else if(next.tagName() == "comment")
                                info.setComment(next.text().replace("\\\'", "'"));
                        } else if(step_ == STEP_GET) {
                            if(next.tagName() == "file")
                            {
                                file.setFilename(next.attribute("filename", ""));
                                file.setMd5hash(next.attribute("md5hash", ""));
                            } else if(next.tagName() == "data") {
                                QByteArray b64data;
                                b64data.append(next.text());
                                file.setData(QByteArray::fromBase64(b64data));

                                if(file.getMd5hash() != file.getMd5hash())
                                    emit signalError("The file downloaded is incorrect ! Please retry");
                                else {
                                    //Write the file down
                                    QFile updateFile(QString("%1/%2").arg(QDir::tempPath()).arg(file.getFilename()));
                                    if(updateFile.open(QIODevice::ReadWrite))
                                    {
                                        updateFile.write(file.getData());
                                        updateFile.close();
                                        file.setLocalFile(updateFile.fileName());
                                        //qDebug() << "wrote file : " << updateFile.fileName();

                                        emit signalGotUpdate(file);
                                    } else
                                        emit signalError("An error occured while writing the file locally.");
                                }
                            }

                            isDownloading_ = false;
                        }
                    }

                    before = next;
                    next = next.firstChild().toElement();
                    if(next.isNull())
                        next = before.nextSibling().toElement();
                }

                if(!noUpdate) {
                    if(step_ == STEP_CHECK)
                        emit signalNewUpdate(info);
                    /*else if(step_ == STEP_GET)
                        emit signalGotUpdate(file);*/
                }
            } else
                error = tr("The XML file format is wrong !");
        } else
            error = tr("We received en empty response !");

        if(error != "")
            emit signalError(error);
    }

private:
    enum Steps { STEP_NONE, STEP_CHECK, STEP_GET };
    QNetworkAccessManager* nam_;
    int     step_;
    bool    isDownloading_;
};

}
}

#endif // QAPPHTTPCLIENT_HPP
