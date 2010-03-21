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

        void setDate(const DateTime &date);
        const DateTime getDate() const;

        void setDescription(const std::string &description);
        std::string getDescription() const;

        void setPin(const std::string &name, const std::string &value);
        std::string getPin(const std::string &name) const;
        StringVector getPins() const;

        void setData(const ByteVector &data);
        ByteVector &getData();
        const ByteVector &getData() const;

        const UpdateDevice &updateDevice() const;
        UpdateDevice &updateDevice();

        std::string toString() const;
        std::string formatDateVersion() const;
        std::string formatDeviceId() const;

    private:
        std::string           m_filename;
        std::string           m_url;
        std::string           m_author;
        int                   m_version;
        DateTime              m_date;
        std::string           m_description;
        StringStringMap       m_pins;
        ByteVector            m_data;
        std::string           m_md5sum;
        UpdateDevice          m_updateDevice;
};

/* }}} */
/* Firmwarepool {{{ */

class FirmwareXMLParser;

class Firmwarepool {
    friend class FirmwareXMLParser;

    public:
        static void readFromFile(const std::string &filename,
                ByteVector &bv) throw (IOError);

    public:
        Firmwarepool(const std::string &cacheDir)
            throw (IOError);
        virtual ~Firmwarepool();

    public:
        /* m_cacheDir is immutable */
        std::string getCacheDir() const;

        void downloadIndex(const std::string &url)
            throw (DownloadError);
        void readIndex()
            throw (IOError, ParseError);
        void deleteIndex()
            throw (IOError);

        StringList getFirmwareNameList() const;
        Firmware *getFirmware(const std::string &name) const;
        std::vector<Firmware *> getFirmwareList() const;

        std::vector<UpdateDevice> getUpdateDeviceList() const;

        void setProgress(ProgressNotifier *notifier);
        void setIndexUpdatetime(int minutes);

        void downloadFirmware(const std::string &name)
            throw (DownloadError, ApplicationError);
        void fillFirmware(const std::string &name)
            throw (IOError, ApplicationError);
        bool isFirmwareOnDisk(const std::string &name)
            throw (IOError);

        void deleteCache()
            throw (IOError);
        void cleanCache()
            throw (IOError);

    protected:
        std::string getFirmwareFilename(Firmware *fw) const;
        void addFirmware(Firmware *fw);

    private:
        const std::string m_cacheDir;
        StringFirmwareMap m_firmware;
        ProgressNotifier *m_progressNotifier;
        int m_indexAutoUpdatetime;
};

/* }}} */

#endif /* FIRMWAREPOOL_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
