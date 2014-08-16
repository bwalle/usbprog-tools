/* {{{
 * Copyright (c) 2011, Bernhard Walle <bernhard@bwalle.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. }}}
 */
#ifndef LIBBW_IO_TEMPFILE_H_
#define LIBBW_IO_TEMPFILE_H_

#include <string>
#include <stdint.h>

#include <libbw/bwerror.h>
#include <libbw/exithandler.h>

namespace bw {
namespace io {

/* TempFile {{{ */

struct TempFilePrivate;

/**
 * \class TempFile tempfile.h libbw/io/tempfile.h
 * \brief Represents a temporary file
 *
 * Since there is no standard C/C++ impelemntation of temporary files that can be called by
 * name from outside the application, this class should fill the gap. It uses the facility
 * of the operating system to find an unique name for a temporary file and open it in an
 * atomic manner (which is quite important for security).
 *
 * See also TempFile::Flags for how to close and delete temporary files automatically.
 *
 * On Unix, the <tt>TMPDIR</tt> environment is used for the temporary directory. If it isn't
 * set, the <tt>/tmp</tt> directory is used to create temporary files.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup io
 */
class TempFile {

public:
    /**
     * \brief Flags that can be applied with the constructor of the temprary file
     *
     *  - If the \c DeleteOnClose flag is set, the file deleted if the temporary file is closed
     *    or if the object is destroyed.
     *  - If the \c DeleteOnExit flag is set, the file is closed and deleted automatically after
     *    termination of the application using a bw::ExitHandler. The file is only deleted if
     *    the application is called using std::exit() or if the main function returns, not if
     *    the application crashes. This flag implies \c DeleteOnClose.
     *
     * If the <tt>LIBBW_TEMPFILE_NODELETE</tt> environment variable is set, the temporary
     * file is never deleted by this class. This is useful for debugging.
     */
    enum Flags {
        NoFlags = 0,
        DeleteOnClose = (1<<0),
        DeleteOnExit  = DeleteOnClose|(1<<1)
    };

public:
    /**
     * \brief Creates a new TempFile object
     *
     * The constructor creates the temporary file immediately.
     *
     * \param[in] namepart a part that appears in the
     * \param[in] flags the flags as specified in the documentation of TempFile::Flags
     *
     * \exception IOError
     */
    TempFile(const std::string &namepart, Flags flags=NoFlags);

    /**
     * \brief Destroys the temporary file
     *
     * Calls close() if the file is not already closed.
     */
    virtual ~TempFile();

    /**
     * \brief Returns the flags that have been set in the constructor
     * \return the flags as documented in TempFile::Flags.
     */
    Flags flags() const;

    /**
     * \brief Returns the name of the temporary file
     *
     * While the temporary file is already open and can be accessed using nativeHandle(),
     * it's valid to open the file again if portable file I/O should be used or to pass the
     * name to other applications.
     *
     * However, accessing the file by its native I/O functions and using the name and opening
     * again <b>at the same time</b> should be avoided as it causes synchronisation problems.
     *
     * \return the full path of the temporary file.
     */
    std::string name() const;

    /**
     * \brief Returns a native handle to the file descriptor
     *
     * On Unix, this is a plain file descriptor. On Windows, this is a HANDLE pointer.
     *
     * \return the handle
     */
    uint64_t nativeHandle() const;

    /**
     * \brief Closes the temporary file
     *
     * If the TempFile::Flags::DeleteOnClose flag is set, the file is deleted on disk.
     */
    void close();

protected:
    /**
     * \brief Creates the temporary file
     *
     * This function is implemented in <tt>tempfile_<i>specific</i>.cc</tt>. The implementation
     * should not modify any member variables except \c d.
     *
     * \param[in] namePart the name part as described in TempFile.
     * \return the name of the temporary file
     * \throw IOError if creation of the temporary file failed.
     */
    std::string _create(const std::string &namePart);

    /**
     * \brief Closes the temporary file handle
     *
     * This function is implemented in <tt>tempfile_<i>specific</i>.cc</tt>. The implementation
     * should not modify any member variables except \c d.
     */
    void _close();

private:
    std::string m_name;
    Flags m_flags;
    bool m_open;
    ExitHandler *m_exitHandler;
    TempFilePrivate *d;
};

/* }}} */

} // end namespace io
} // end namespace bw

#endif /* LIBBW_IO_TEMPFILE_H_ */
