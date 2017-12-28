#ifndef QAPPUPDATEFILE_HPP
#define QAPPUPDATEFILE_HPP

#include <QString>
#include <QCryptographicHash>

namespace com {
namespace tibsys {

class QAppUpdateFile
{
public:
    QAppUpdateFile() { }

    /** Setters */
    void setFilename(QString filename) { filename_ = filename; }
    void setMd5hash(QString md5hash) { md5hash_ = md5hash; }
    void setData(QByteArray data) { data_ = data; }
    void setLocalFile(QString file) { localFile_ = file; }

    /** Getters */
    QString getFilename(void) { return filename_; }
    QString getMd5hash(void) { return md5hash_; }
    QByteArray getData(void) { return data_; }
    QString getLocalFile(void) { return localFile_; }

    /** Helpers */
    QString computeMd5hash(void)
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(data_);
        return hash.result().toHex();
    }

    int getSize(void)
    {
        return data_.length();
    }

private:
    QString localFile_;
    QString filename_;
    QString md5hash_;
    QByteArray data_;
};

}
}

#endif // QAPPUPDATEFILE_HPP
