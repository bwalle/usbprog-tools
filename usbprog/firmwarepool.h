/*
 * (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
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
#ifndef FIRMWAREPOOL_H
#define FIRMWAREPOOL_H

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <list>

#include <usbprog-core/date.h>
#include <usbprog-core/inifile.h>
#include <usbprog-core/error.h>
#include <usbprog-core/types.h>
#include <usbprog-core/devices.h>
#include <usbprog/downloader.h>

namespace usbprog {

/* Typedefs {{{ */

class Firmware;
typedef std::map<std::string, Firmware *> StringFirmwareMap;
typedef std::list<std::string>            StringList;

/* }}} */
/* Firmware {{{ */

class Firmware
{
    public:
        Firmware(const std::string &name);

        /* name is immutable */
        std::string getName() const;

        void setLabel(const std::string &label);
        std::string getLabel() const;

        void setFilename(const std::string &filename);
        std::string getFilename() const;
        std::string getVerFilename() const;

        void setUrl(const std::string &url);
        std::string getUrl() const;

        void setAuthor(const std::string &author);
        std::string getAuthor() const;

        void setVersion(int version);
        int getVersion() const;
        std::string getVersionString() const;

        void setMD5Sum(const std::string &md5);
        std::string getMD5Sum() const;

        void setDate(const core::DateTime &date);
        const core::DateTime getDate() const;

        void setDescription(const std::string &description);
        std::string getDescription() const;

        void setPin(const std::string &name, const std::string &value);
        std::string getPin(const std::string &name) const;
        core::StringVector getPins() const;

        void setData(const core::ByteVector &data);
        core::ByteVector &getData();
        const core::ByteVector &getData() const;

        const core::UpdateDevice &updateDevice() const;
        core::UpdateDevice &updateDevice();

        std::string toString() const;
        std::string formatDateVersion() const;
        std::string formatDeviceId() const;

    private:
        std::string           m_filename;
        std::string           m_url;
        std::string           m_author;
        int                   m_version;
        core::DateTime        m_date;
        std::string           m_description;
        core::StringStringMap m_pins;
        core::ByteVector      m_data;
        std::string           m_md5sum;
        core::UpdateDevice    m_updateDevice;
};

/* }}} */
/* Firmwarepool {{{ */

class FirmwareXMLParser;

class Firmwarepool {
    friend class FirmwareXMLParser;

    public:
        static void readFromFile(const std::string  &filename,
                                 core::ByteVector   &bv)
            throw (core::IOError);

    public:
        Firmwarepool(const std::string &cacheDir)
            throw (core::IOError);
        virtual ~Firmwarepool();

    public:
        /* m_cacheDir is immutable */
        std::string getCacheDir() const;

        void downloadIndex(const std::string &url)
            throw (DownloadError);
        void readIndex()
            throw (core::IOError, core::ParseError);
        void deleteIndex()
            throw (core::IOError);

        StringList getFirmwareNameList() const;
        Firmware *getFirmware(const std::string &name) const;
        std::vector<Firmware *> getFirmwareList() const;

        std::vector<core::UpdateDevice> getUpdateDeviceList() const;

        void setProgress(core::ProgressNotifier *notifier);
        void setIndexUpdatetime(int minutes);

        void downloadFirmware(const std::string &name)
            throw (DownloadError, core::ApplicationError);
        void fillFirmware(const std::string &name)
            throw (core::IOError, core::ApplicationError);
        bool isFirmwareOnDisk(const std::string &name)
            throw (core::IOError);

        void deleteCache()
            throw (core::IOError);
        void cleanCache()
            throw (core::IOError);

    protected:
        std::string getFirmwareFilename(Firmware *fw) const;
        void addFirmware(Firmware *fw);

    private:
        const std::string       m_cacheDir;
        StringFirmwareMap       m_firmware;
        core::ProgressNotifier  *m_progressNotifier;
        int                     m_indexAutoUpdatetime;
};

/* }}} */

} // end namespace usbprog

#endif /* FIRMWAREPOOL_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
