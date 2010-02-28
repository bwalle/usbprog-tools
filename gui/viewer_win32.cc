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

#include <memory>

#include <wx/filename.h>
#include <wx/mimetype.h>

#include "viewer.h"

/* -------------------------------------------------------------------------- */
bool Viewer::openPDF(const wxString &file)
{
    if (!wxFileName::FileExists(file))
        return false;

    std::auto_ptr<wxFileType> ft(
        wxTheMimeTypesManager->GetFileTypeFromExtension(wxT("pdf")));
    if (ft.get() == NULL)
        return false;

    wxString cmd;
    bool ok = ft->GetOpenCommand(
        &cmd, wxFileType::MessageParameters(file, wxEmptyString));
    if (!ok)
        return false;

    long pid = wxExecute(cmd);
    return pid != 0;
}


// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
