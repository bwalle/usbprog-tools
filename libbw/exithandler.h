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
#ifndef LIBBW_EXITHANDLER_H_
#define LIBBW_EXITHANDLER_H_

#include <string>

namespace bw {

/* ExitHandler {{{ */

/**
 * \class ExitHandler exithandler.h libbw/exithandler.h
 * \brief Interface for exit handlers
 *
 * This interface defines a virtual method exitCleanup() that is called for each object if
 * the program terminates. An object must be registered using the registerExitHandler() function.
 * The instance of an exit handler must be allocated on the heap since the very last action
 * is to delete the ExitHandler implementation itself.
 *
 * Example:
 *
 * \code
 * class FileDeleter : public ExitHandler
 * {
 *     public:
 *         FileDeleter(const std::string &fileName)
 *             : m_fileName(fileName)
 *         {}
 *
 *         void exitCleanup()
 *         {
 *              std::remove(m_fileName.c_str());
 *         }
 *
 *     private:
 *         std::string m_fileName;
 * };
 *
 * // later
 * void foo()
 * {
 *     registerExitHandler(new FileDeleter("/tmp/bla"));
 * }
 * \endcode
 *
 * Since deleting files is a very common operation, a FileDeleteExitHandler that does
 * the same as our example is already implemented in this library.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup misc
 */
class ExitHandler
{
public:
    /**
     * \brief Virtual destructor
     */
    virtual ~ExitHandler() {}

    /**
     * \brief Callback that is executed on application termination.
     */
    virtual void exitCleanup() = 0;
};

/* }}} */
/* FileDeleteExitHandler {{{ */

/**
 * \class FileDeleteExitHandler exithandler.h libbw/exithandler.h
 * \brief Exit handler that deletes a file
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup misc
 */
class FileDeleteExitHandler : public ExitHandler
{
public:
    /**
     * \brief Constructor
     *
     * Creates a new FileDeleteExitHandler instance with \p filename as the file name
     * that should be deleted.
     *
     * \param[in] filename the name of the file that should be deleted
     */
    FileDeleteExitHandler(const std::string &filename);

    /**
     * \brief Deletes the file
     */
    void exitCleanup();

private:
    std::string m_filename;
};

/* }}} */
/* registerExitHandler and unregisterExitHandler {{{ */

/**
 * \brief Registers an exit handler
 *
 * The ExitHandler::exitCleanup() function is executed on application termination using
 * atexit(). This means that the application must be terminated using std::exit() so that
 * the exit handlers are called. An uncaught signal doesn't call the exit handler!
 *
 * See the class documentation of ExitHandler for an example.
 *
 * \param[in] exitHandler an pointer to an exit handler object that must have been allocated
 *            on the heap
 * \ingroup misc
 */
void registerExitHandler(ExitHandler *exitHandler);

/**
 * \brief Unregisters an exit handler
 *
 * You can only unregister an exit handler if you have the address of the exit handler
 * registered, i.e. you have to call registerExitHandler() and unregisterExitHandler() with
 * the same address.
 *
 * <b>The function also deletes the \p exitHandler automatically</b>
 *
 * \param[in] exitHandler the exit handler address to unregister
 * \ingroup misc
 */
void unregisterExitHandler(ExitHandler *exitHandler);

/* }}} */

} // namespace bw

#endif // LIBBW_EXITHANDLER_H_
