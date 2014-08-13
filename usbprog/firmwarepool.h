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

/**
 * @file firmwarepool.h
 * @brief Firmwarepool
 *
 * This file contains the implementation of the firmware pool and the firmware.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
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

/// Type definition for a map of string objects to Firmware pointers.
typedef std::map<std::string, Firmware *> StringFirmwareMap;

/// Type definition for a list of C++ strings.
typedef std::list<std::string>            StringList;

/* }}} */
/* Firmware {{{ */

/**
 * @class Firmware usbprog/firmwarepool.h
 * @brief Firmware data
 *
 * This class represents a firmware in the system.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */
class Firmware
{
    public:
        /**
         * @brief Constructor
         *
         * Creates a new firmware.
         *
         * @param[in] name the name of the firmware. The name is immutable, which means that it's
         *            not possible to change the name afterwards.
         */
        Firmware(const std::string &name);

        /**
         * @brief Returns the name
         *
         * @return the name of the firmware
         */
        std::string getName() const;

        /**
         * @brief Sets the label
         *
         * @param[in] label the firmware label
         */
        void setLabel(const std::string &label);

        /**
         * @brief Returns the label
         *
         * @return the label
         */
        std::string getLabel() const;

        /**
         * @brief Sets the file name
         *
         * @param[in] filename the new file name
         */
        void setFilename(const std::string &filename);

        /**
         * @brief Returns the file name
         *
         * @return the file name
         * @see getVerFilename()
         */
        std::string getFilename() const;

        /**
         * @brief Returns the versioned file name
         *
         * @return the versioned file name
         */
        std::string getVerFilename() const;

        /**
         * @brief Sets the URL for the firmware
         *
         * @param[in] url the new URL
         */
        void setUrl(const std::string &url);

        /**
         * @brief Returns the URL
         *
         * @return the URL
         */
        std::string getUrl() const;

        /**
         * @brief Sets the author
         *
         * @param[in] author the author
         */
        void setAuthor(const std::string &author);

        /**
         * @brief Returns the author
         *
         * @return the author name
         */
        std::string getAuthor() const;

        /**
         * @brief Sets the version
         *
         * @param[in] version the version
         */
        void setVersion(int version);

        /**
         * @brief Returns the version
         *
         * @return the version number
         */
        int getVersion() const;

        /**
         * @brief Returns the version number as string
         *
         * @return the version number as string
         */
        std::string getVersionString() const;

        /**
         * @brief Sets the md5 sum of the firmware
         *
         * @param[in] md5 the new md5sum
         */
        void setMD5Sum(const std::string &md5);

        /**
         * @brief Returns the md5 sum of the firmware
         *
         * @return the md5sum
         */
        std::string getMD5Sum() const;

        /**
         * @brief Sets the date
         *
         * @param[in] date the new date
         */
        void setDate(const core::DateTime &date);

        /**
         * @brief Returns the date
         *
         * @return the date
         */
        const core::DateTime getDate() const;

        /**
         * @brief Sets the description
         *
         * @param[in] description the description
         */
        void setDescription(const std::string &description);

        /**
         * @brief Returns the description
         *
         * @return the description
         */
        std::string getDescription() const;

        /**
         * @brief Sets the pin @p name to @p value
         *
         * @param[in] name the name of the pin that should be set
         * @param[in] value the value of the pin
         */
        void setPin(const std::string &name, const std::string &value);

        /**
         * @brief Returns the value for pin @p name
         *
         * @param[in] name the name of the pin
         * @return the pin information
         */
        std::string getPin(const std::string &name) const;

        /**
         * @brief Returns the name of all pins
         *
         * @return the name of all pins
         */
        core::StringVector getPins() const;

        /**
         * @brief Sets the data of the firmware
         *
         * @param[in] data the data bytes
         */
        void setData(const core::ByteVector &data);

        /**
         * @brief Returns the data bytes (const)
         *
         * @return the data bytes
         */
        core::ByteVector &getData();

        /**
         * @brief Returns the data bytes (non-const)
         *
         * @return the data bytes
         */
        const core::ByteVector &getData() const;

        /**
         * @brief Returns the update device (const)
         *
         * @return the update device
         */
        const core::UpdateDevice &updateDevice() const;

        /**
         * @brief Returns the update device (non-const)
         *
         * @return the update device
         */
        core::UpdateDevice &updateDevice();

        /**
         * @brief Converts the firmware to a string
         *
         * This function converts the internal representation into a string.  This function is used
         * for debugging and it returns a very long string.
         *
         * @return the firmware as string
         */
        std::string toString() const;

        /**
         * @brief Returns the version together with a date
         *
         * The return string looks like <tt>"3 [2010-01-01]"</tt>.
         *
         * @return the string as shown above
         */
        std::string formatDateVersion() const;

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

/**
 * @class Firmwarepool usbprog/firmwarepool.h
 * @brief Pool of multiple firmware files
 *
 * This class represents a pool of multiple firmwares together with management functions.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */
class Firmwarepool {
    friend class FirmwareXMLParser;

    public:
        /**
         * @brief Constructor
         *
         * Creates a new Firmwarepool object.
         *
         * @param[in] cacheDir the directory that contains the firmware as cached files on disk.
         *            The cache directory is immutable and cannot be changed afterwards.
         */
        Firmwarepool(const std::string &cacheDir);

        /**
         * @brief Destructor
         */
        virtual ~Firmwarepool();

    public:
        /**
         * @brief Returns the cache directory
         *
         * @return the cache directory as set in the constructor
         */
        std::string getCacheDir() const;

        /**
         * @brief Downloads the index file from @p url
         *
         * @param[in] url the URL where the index file should be downloaded from
         * @exception DownloadError if downloading the firmware file failed
         * @see setIndexUpdatetime()
         */
        void downloadIndex(const std::string &url);

        /**
         * @brief Parses the index file
         *
         * Reads the index file from disk and parses it.
         *
         * @exception core::IOError if reading the firmware file failed
         * @exception core::ParseError if parsing the firmware file failed
         */
        void readIndex();

        /**
         * @brief Deletes the index from disk
         *
         * @exception core::IOError if deleting the file failed
         */
        void deleteIndex();

        /**
         * @brief Returns a list of firmware names
         *
         * @return the list
         */
        StringList getFirmwareNameList() const;

        /**
         * @brief Returns the firmware @p name
         *
         * @param[in] name the name of the firmware
         * @return a pointer to the firmware object. The object is still owned by the Firmwarepool,
         *         which means that it's only valid during the life time of the firmware pool and
         *         must not be freed by the caller.
         */
        Firmware *getFirmware(const std::string &name) const;

        /**
         * @brief Returns a list of all firmwares
         *
         * @return a vector of pointers to Firmware objects. The objects are still owned by the
         *         Firmwarepool, which means that it's only valid during the life time of the
         *         firmware pool and must not be freed by the caller.
         */
        std::vector<Firmware *> getFirmwareList() const;

        /**
         * @brief Returns a list of update devices
         *
         * This function is needed to return a list of update devices that are considered by the
         * core::DeviceManager as USBprog devices.
         *
         * @return the list as described above
         */
        std::vector<core::UpdateDevice> getUpdateDeviceList() const;

        /**
         * @brief Sets the progress notifier for the download operation
         *
         * @param[in] notifier the notifier object. The object is still owned by the caller
         *            but must be valid during the whole lifetime of the Firmwarepool, or until
         *            this function is called again with @p NULL.
         */
        void setProgress(core::ProgressNotifier *notifier);

        /**
         * @brief Sets the index update time
         *
         * This value determines the difference between now and the last modification time of the
         * firmware file on disk which is needed to let the downloadFirmware() function really
         * download the firmware again.
         *
         * @param[in] minutes the time in minutes
         * @see downloadIndex()
         */
        void setIndexUpdatetime(int minutes);

        /**
         * @brief Downloads the firmware @p name
         *
         * To notify the caller about the progress about the download operation, the
         * ProgressNotifier that has been set with setProgress() is called in regular intervals.
         *
         * @param[in] name the name of the firmware that should be downloaded
         * @see setProgress()
         * @exception DownloadError if downloading failed
         * @exception ApplicationError if @p name is invalid
         */
        void downloadFirmware(const std::string &name);

        /**
         * @brief Fills the content bytes of firmware @p name
         *
         * While normally the Firmware object only contains the meta information, this call makes
         * sure that firmware @p name also contains the data bytes that can be retrieved with
         * Firmware::getData().
         *
         * @param[in] name the name of the firmware
         * @exception core::IOError if it's not possible to read the data bytes for the firmware
         * @exception ApplicationError if @p name is invalid
         */
        void fillFirmware(const std::string &name);

        /**
         * @brief Checks if the firmware @p name is on disk
         *
         * @param[in] name the name of the firmware
         * @return @c true if the firmware is on disk, @c false otherwise
         */
        bool isFirmwareOnDisk(const std::string &name);

        /**
         * @brief Deletes the firmware cache
         *
         * Deleting means to delete everything, not only the old files.
         *
         * @exception core::IOError if deletion of files failed
         * @see cleanCache()
         */
        void deleteCache();

        /**
         * @brief Cleans up the firmware cache
         *
         * Cleaning up means to delete old versions of firmwares that cannot even be used by normal
         * operations.
         *
         * @exception core::IOError if deletion of files failed
         * @see deleteCache()
         */
        void cleanCache();

    protected:
        /**
         * @brief Returns the firmware file name
         *
         * Returns the name of the file that contains the firmware @p fw.
         *
         * @param[in] fw a pointer to the firmware object
         * @return the file name as string
         */
        std::string getFirmwareFilename(Firmware *fw) const;

        /**
         * @brief Adds the firmware @p fw to the list
         *
         * @param[in] fw the pointer to the firmware. The caller is no longer responsible for @p fw,
         *            it will be freed by the Firmwarepool.
         */
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
