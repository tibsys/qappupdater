#ifndef QAPPUPDATEINFO_HPP
#define QAPPUPDATEINFO_HPP

#include <QString>

namespace com {
namespace tibsys {

class QAppUpdateInfo
{
public:
    QAppUpdateInfo() : exists_(false) { }

   bool hasUpdate(void) { return exists_; }

   /** Getters */
   int getId() { return id_; }
   int getBranch() { return branch_; }
   int getMajor() { return major_; }
   int getMinor() { return minor_; }
   int getOs() { return os_; }
   int getArch() { return arch_; }
   QString getOsName() { return osName_; }
   QString getArchName() { return archName_; }
   QString getFilename() { return filename_; }
   QString getMd5hash() { return md5hash_; }
   QString getComment() { return comment_; }
   QString getLanguage() { return language_; }

   /** Setters */
   void setId(int id) { id_ = id; }
   void setBranch(int branch) { branch_ = branch; }
   void setMajor(int major) { major_ = major; }
   void setMinor(int minor) { minor_ = minor; }
   void setOs(int os) { os_ = os; }
   void setArch(int arch) { arch_ = arch; }
   void setOsName(QString os) { osName_ = os; }
   void setArchName(QString arch) { archName_ = arch; }
   void setFilename(QString filename) { filename_ = filename; }
   void setMd5hash(QString md5hash) { md5hash_ = md5hash; }
   void setComment(QString comment) { comment_ = comment; }
   void setLanguage(QString language) { language_ = language; }

private:
   bool exists_;
   int id_;
   int branch_;
   int major_;
   int minor_;
   int os_;
   int arch_;
   QString osName_;
   QString archName_;
   QString filename_;
   QString md5hash_;
   QString comment_;
   QString language_;
};

}
}

#endif // QAPPUPDATEINFO_HPP
