/* {{{
 * Copyright (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
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
#ifndef OS_H
#define OS_H

#include <vector>
#include <string>

namespace bw {

/**
 * \brief Flags for daemonize
 */
enum DaemonizeFlags {
    DAEMONIZE_NOCLOSE = (1<<0)   /**< don't close file descriptors */
};

/**
 * \brief Puts the current process to background.
 * \ingroup os
 *
 * Currently the function is implemented in POSIX only.
 *
 * The function is for programs wishing to detach themselves from the
 * controlling terminal and run in the background as system daemons.
 *
 * If \p flags doesn't contain DAEMONIZE_NOCLOSE, then all file descriptors
 * are closed. STDIN, STDOUT and STDERR are redirected to /dev/null, regardless
 * of DAEMONIZE_NOCLOSE.
 *
 * \param[in] flags a bitwise OR'd combination of flags as described above.
 * \return 0 on success, any standard error code on failure.
 */
int daemonize(int flags=0);

/**
 * \brief Replacement for system() without a shell
 * \ingroup os
 *
 * This function does the same as system but tries to avoid using a shell, so
 * it's not necessary to quote the arguments.
 *
 * \code
 * std::vector<std::string> args;
 * args.push_back("ls");
 * args.push_back("-l");
 * int rc = bw::system(args[0], args);
 * if (rc < 0) {
 *      std::cerr << "Unable to execute 'ls -l':" << std::strerror(errno) << std::endl;
 *      return -1;
 * }
 * \endcode
 *
 * \param[in] process the name of the program that should be executed, either the full path or just
 *            the name of the binary which assumes that the program is in <tt>$PATH</tt>.
 * \param[in] args the arguments including the 0th argument which usually is the name of the
 *            exeutable proceded in \p process
 * \return the return status of the process (between 0 and 255) on success, a negative value on
 *         failure in which case the global value errno is set accordingly.
 */
int system(const std::string &process, const std::vector<std::string> &args);

} // end namespace bw

#endif /* OS_H */

// vim: set sw=4 ts=4 et fdm=marker:
