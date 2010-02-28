/*
 * (c) 2010, Bernhard Walle <bernhard@bwalle.de>
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

#include <cstdio>

#include <wx/filename.h>
#include <wx/utils.h>

#include "viewer.h"

#ifdef __APPLE__
#  define VIEWER_COMMAND "open"
#else
#  define VIEWER_COMMAND "xdg-open"
#endif


/* -------------------------------------------------------------------------- */
bool Viewer::openPDF(const wxString &file)
{
    if (!wxFileName::FileExists(file)) {
        std::fprintf(stderr, "File '%s' does not exist\n", 
                     static_cast<const char *>(file.mb_str(wxConvUTF8)));
        return false;
    }

    const wxChar *args[] = {
        wxT(VIEWER_COMMAND),
        file,
        NULL
    };

    long pid = wxExecute(const_cast<wxChar **>(args));
    return pid != 0;
}


// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
