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

/**
 * @file tempdir.h
 * @brief Temporary directory
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */

#ifndef TEMPDIR_H_
#define TEMPDIR_H_

#include <QString>

namespace usbprog {

/* Tempdir {{{ */

/**
 * @brief Temporary directory
 *
 * This class represents a temporary directory that is created in the constructor
 * and deleted in the destructor. We cannot use QTemporaryDir since that was
 * introduced in Qt 5.3 (at least it doesn't exist in Qt 4.8).
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class Tempdir
{
public:
    /**
     * @brief Creates a temporary directory using a default name
     *
     * Constructs a QTemporaryDir using as template the application name returned by
     * QCoreApplication::applicationName(). The directory is stored in the system's temporary
     * directory, QDir::tempPath().
     */
    Tempdir();

    /**
     * @brief Constructs a QTemporaryFile with a template name of \a templateName.
     *
     * If \a templateName is a relative path, the path will be relative to the
     * current working directory. You can use QDir::tempPath() to construct \a templateName
     * if you want use the system's temporary directory.
     *
     * If the \a templateName ends with XXXXXX it will be used as the dynamic portion
     * of the directory name, otherwise it will be appended.
     *
     * Unlike QTemporaryFile, XXXXXX in the middle of the template string is not supported.
     *
     * \sa QDir::tempPath()
     */
    Tempdir(const QString &templateName);

    /**
     * @brief Destroys the Tempdir object
     */
    ~Tempdir();

public:
    /**
     * @brief Returns the path to the temporary directory.
     *
     * Empty if the QTemporaryDir could not be created.
     */
    QString path() const;

    /**
     * @brief Returns true if the Tempdir was created successfully.
     */
    bool isValid() const;

    /**
     * @brief Checks if auto-removal is enabled
     *
     * @retval @c true if auto-removal on destruction is enabled, @c false otherwise.
     */
    bool autoRemove() const;

    /**
     * @brief Enables or disables auto-removal
     *
     * @param[in] enabled @c true if auto-removal should be enabled, @c false otherwise
     */
    void setAutoRemove(bool enabled);

    /**
     * @brief Removes the temporary directory, including all contents.
     *
     * @return @c true if removing was successful.
     */
    bool remove();

private:
    void create(const QString &templateName);
    static QString defaultTemplateName();

private:
    QString m_path;
    bool m_success;
    bool m_autoRemove;
};

/* }}} */

} // end namespace usbprog

#endif // TEMPDIR_H_
