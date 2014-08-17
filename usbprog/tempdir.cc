/*
 * (c) 2014, Bernhard Walle <bernhard@bwalle.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QtGlobal>

#ifdef Q_OS_UNIX
#  include <unistd.h> // mkdtemp
#endif

#include "tempdir.h"

namespace usbprog {

/* Tempdir {{{ */

#if defined(Q_OS_QNX ) || defined(Q_OS_WIN)
static char *mkdtemp(char *templateName)
{
    static const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t length = strlen(templateName);

    char *XXXXXX = templateName + length - 6;

    if ((length < 6u) || strncmp(XXXXXX, "XXXXXX", 6))
        return 0;

    for (int i = 0; i < 256; ++i) {
        int v = qrand();

        /* Fill in the random bits.  */
        XXXXXX[0] = letters[v % 62];
        v /= 62;
        XXXXXX[1] = letters[v % 62];
        v /= 62;
        XXXXXX[2] = letters[v % 62];
        v /= 62;
        XXXXXX[3] = letters[v % 62];
        v /= 62;
        XXXXXX[4] = letters[v % 62];
        v /= 62;
        XXXXXX[5] = letters[v % 62];

        QString templateNameStr = QFile::decodeName(templateName);

        QFileSystemEntry fileSystemEntry(templateNameStr);
        if (QFileSystemEngine::createDirectory(fileSystemEntry, false)) {
            QSystemError error;
            QFileSystemEngine::setPermissions(fileSystemEntry,
                                              QFile::ReadOwner |
                                              QFile::WriteOwner |
                                              QFile::ExeOwner, error);
            if (error.error() != 0)
                continue;
            return templateName;
        }
    }
    return 0;
}
#elif defined(Q_OS_LINUX_ANDROID)
extern char *mkdtemp(char *);
#endif

Tempdir::Tempdir() :
    m_success(false),
    m_autoRemove(false)
{
    create(defaultTemplateName());
}

Tempdir::Tempdir(const QString &templateName) :
    m_success(false),
    m_autoRemove(false)
{
    if (templateName.isEmpty())
        create(defaultTemplateName());
    else
        create(templateName);
}

Tempdir::~Tempdir()
{
    if (m_autoRemove && m_success)
        remove();
}

QString Tempdir::path() const
{
    return m_path;
}

bool Tempdir::isValid() const
{
    return m_success;
}

bool Tempdir::autoRemove() const
{
    return m_autoRemove;
}

void Tempdir::setAutoRemove(bool enabled)
{
    m_autoRemove = enabled;
}

bool Tempdir::remove()
{
    if (!m_success)
        return false;

    Q_ASSERT(!path().isEmpty());
    Q_ASSERT(path() != QLatin1String("."));

    return removeRecursively(m_path);
}

bool Tempdir::removeRecursively(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
       Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
           if (info.isDir())
               result = removeRecursively(info.absoluteFilePath());
           else
               result = QFile::remove(info.absoluteFilePath());

           if (!result)
               return result;
       }
       result = dir.rmdir(dirName);
   }

   return result;
}

void Tempdir::create(const QString &templateName)
{
    QByteArray buffer = QFile::encodeName(templateName);
    if (!buffer.endsWith("XXXXXX"))
        buffer += "XXXXXX";
    if (mkdtemp(buffer.data())) { // modifies buffer
        m_success = true;
        m_path = QFile::decodeName(buffer.constData());
    }
}

QString Tempdir::defaultTemplateName()
{
    QString baseName; 
    baseName = QCoreApplication::applicationName();
    if (baseName.isEmpty())
        baseName = QLatin1String("qt_temp");

    return QDir::tempPath() + QLatin1Char('/') + baseName + QLatin1String("-XXXXXX");
}

/* }}} */

} // end namespace usbprog
