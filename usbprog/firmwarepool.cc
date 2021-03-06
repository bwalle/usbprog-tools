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
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <vector>
#include <cerrno>

#include <QDomDocument>
#include <QFile>
#include <QDir>

#include <sys/types.h>

#include <usbprog-core/stringutil.h>
#include <usbprog-core/util.h>
#include <usbprog-core/digest.h>
#include <usbprog-core/debug.h>
#include <usbprog/firmwarepool.h>

#define INDEX_FILE_NAME  "versions.xml"

namespace usbprog {

/* Class declaration: FirmwareXMLParser {{{ */

/**
 * @brief Parser for the firmware XML files
 *
 * This is a internal class, thus declared in an implementation file.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */
class FirmwareXMLParser {
public:
    /**
     * @brief Constructor
     *
     * Creates a new FirmwareXMLParser object.
     *
     * @param[in] pool a pointer to the firmware pool (that must be valid during the whole
     * lifetime of the FirmwareXMLParser object).
     */
    FirmwareXMLParser(Firmwarepool *pool);

public:
    /**
     * @brief Parses the whole firmware pool
     *
     * Adds the result to the firmware pool (that has been passed in the constructor) directly.
     *
     * @param[in] doc the XML document
     * @param[in] pool the root XML element
     * @exception core::ParseError if parsing failed
     */
    void parsePool(const QDomDocument &doc, const QDomElement &pool);

protected:
    /**
     * @brief Parses a firmware element
     *
     * @param[in] doc the XML document
     * @param[in] firmware the firmware XML element
     * @exception core::ParseError if parsing failed
     */
    void parseFirmware(const QDomDocument &doc, const QDomElement &firmware);

private:
    Firmwarepool *m_firmwarepool;
};

/* }}} */
/* Implementation: FirmwareXMLParser {{{ */

#define XMLCHAR(a) \
    reinterpret_cast<const xmlChar *>(a)

FirmwareXMLParser::FirmwareXMLParser(Firmwarepool *pool)
    : m_firmwarepool(pool)
{}

void FirmwareXMLParser::parsePool(const QDomDocument &doc, const QDomElement &pool)
{
    for (QDomNode node = pool.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (!node.isElement())
            continue;
        QDomElement childElement = node.toElement();
        if (childElement.tagName() == "firmware")
            parseFirmware(doc, childElement);
    }
}

void FirmwareXMLParser::parseFirmware(const QDomDocument &doc, const QDomElement &firmware)
{
    Firmware *fw;

    // set name
    QString name = firmware.attribute("name");
    if (name.isEmpty())
        throw core::ParseError("Firmware has no name");
    fw = new Firmware(name.toStdString());

    // set label
    fw->updateDevice().setLabel(firmware.attribute("label").toStdString());

    for (QDomNode node = firmware.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (!node.isElement())
            continue;

        QDomElement childElement = node.toElement();
        if (childElement.tagName() == "binary") {
            fw->setUrl(childElement.attribute("url").toStdString());
            fw->setFilename(childElement.attribute("file").toStdString());
        } else if (childElement.tagName() == "info") {
            fw->setVersion(childElement.attribute("version").toInt());
            fw->setAuthor(childElement.attribute("author").toStdString());
            fw->setDate(core::DateTime(childElement.attribute("date").toStdString(), core::DTF_ISO_DATE));
            fw->setMD5Sum(childElement.attribute("md5sum").toStdString());
        } else if (childElement.tagName() == "description") {
            fw->updateDevice().setVendor(core::parse_long(childElement.attribute("vendorid").toLocal8Bit()));
            fw->updateDevice().setProduct(core::parse_long(childElement.attribute("productid").toLocal8Bit()));
            fw->updateDevice().setBcdDevice(core::parse_long(childElement.attribute("bcddevice").toLocal8Bit()));
            fw->setDescription(core::strip(childElement.text().toStdString()));
        } else if (childElement.tagName() == "pins") {
            for (QDomNode subnode = childElement.firstChild();
                    !subnode.isNull();
                    subnode = subnode.nextSibling()) {
                if (!subnode.isElement())
                    continue;
                QDomElement subChildElement = subnode.toElement();
                fw->setPin(subChildElement.attribute("number").toStdString(),
                           subChildElement.text().toStdString());
            }
        }
    }

    m_firmwarepool->addFirmware(fw);
}

/* }}} */

/* Firmware {{{ */

Firmware::Firmware(const std::string &name)
    : m_updateDevice(name)
{}

std::string Firmware::getName() const
{
    return m_updateDevice.getName();
}

std::string Firmware::getLabel() const
{
    return m_updateDevice.getLabel();
}

void Firmware::setFilename(const std::string &filename)
{
    m_filename = filename;
}

std::string Firmware::getFilename() const
{
    return m_filename;
}

std::string Firmware::getVerFilename() const
{
    return m_filename + "." + getVersionString();
}

void Firmware::setUrl(const std::string &url)
{
    m_url = url;
}

std::string Firmware::getUrl() const
{
    return m_url;
}

void Firmware::setAuthor(const std::string &author)
{
    m_author = author;
}

std::string Firmware::getAuthor() const
{
    return m_author;
}

void Firmware::setVersion(int version)
{
    m_version = version;
}

void Firmware::setDate(const core::DateTime &date)
{
    m_date = date;
}

const core::DateTime Firmware::getDate() const
{
    return m_date;
}

void Firmware::setMD5Sum(const std::string &md5)
{
    m_md5sum = md5;
}

std::string Firmware::getMD5Sum() const
{
    return m_md5sum;
}

int Firmware::getVersion() const
{
    return m_version;
}

std::string Firmware::getVersionString() const
{
    std::stringstream ss;
    ss << m_version;
    return ss.str();
}

void Firmware::setDescription(const std::string &description)
{
    m_description = description;
}

std::string Firmware::getDescription() const
{
    return m_description;
}

void Firmware::setPin(const std::string &name, const std::string &value)
{
    m_pins[name] = value;
}

std::string Firmware::getPin(const std::string &name) const
{
    core::StringStringMap::const_iterator it = m_pins.find(name);
    if (it != m_pins.end())
        return (*it).second;
    else
        return std::string();
}

core::StringVector Firmware::getPins() const
{
    core::StringVector ret;

    for (core::StringStringMap::const_iterator it = m_pins.begin();
            it != m_pins.end(); ++it)
        ret.push_back(it->first);

    return ret;
}

void Firmware::setData(const core::ByteVector &data)
{
    m_data = data;
}

core::ByteVector &Firmware::getData()
{
    return m_data;
}

const core::ByteVector &Firmware::getData() const
{
    return m_data;
}

const core::UpdateDevice &Firmware::updateDevice() const
{
    return m_updateDevice;
}

core::UpdateDevice &Firmware::updateDevice()
{
    return m_updateDevice;
}

std::string Firmware::toString() const
{
    std::stringstream ss;

    ss << "Name            : " << getName() << std::endl;
    ss << "Label           : " << getLabel() << std::endl;
    ss << "File name       : " << m_filename << std::endl;
    ss << "URL             : " << m_url << std::endl;
    ss << "Version         : " << m_version << std::endl;
    ss << "Author          : " << m_author << std::endl;
    ss << "Date            : " << m_date.getDateTimeString(core::DTF_ISO_DATETIME) << std::endl;
    ss << "MD5sum          : " << m_md5sum << std::endl;
    ss << "Description     : " << m_description << std::endl;
    ss << "Pins      P1    : " << getPin("P1") << std::endl;
    ss << "          P2    : " << getPin("P2") << std::endl;
    ss << "          P3    : " << getPin("P3") << std::endl;
    ss << "          P4    : " << getPin("P4") << std::endl;
    ss << "          P5    : " << getPin("P5") << std::endl;
    ss << "          P6    : " << getPin("P6") << std::endl;
    ss << "          P7    : " << getPin("P7") << std::endl;
    ss << "          P8    : " << getPin("P8") << std::endl;
    ss << "          P9    : " << getPin("P9") << std::endl;
    ss << "          P10   : " << getPin("P10") << std::endl;;
    ss << "          RX    : " << getPin("RX") << std::endl;
    ss << "          TX    : " << getPin("TX") << std::endl;
    ss << "          LED   : " << getPin("LED") << std::endl;
    ss << "          JP    : " << getPin("JP") << std::endl;

    return ss.str();
}

std::string Firmware::formatDateVersion() const
{
    std::stringstream ss;

    ss << getVersion();
    ss << " [" << getDate().getDateTimeString(core::DTF_ISO_DATE) << "]";

    return ss.str();
}

/* }}} */
/* Firmwarepool {{{ */

Firmwarepool::Firmwarepool(const std::string &cacheDir)
    : m_cacheDir(cacheDir)
    , m_progressNotifier(NULL)
    , m_indexAutoUpdatetime(0)
{
    if (!core::Fileutil::isDir(cacheDir))
        if (!core::Fileutil::mkdir(cacheDir))
            throw core::IOError("Creating directory '" + cacheDir + "' failed");
}

Firmwarepool::~Firmwarepool()
{
    for (StringFirmwareMap::iterator it = m_firmware.begin();
            it != m_firmware.end(); ++it)
        delete it->second;
}

std::string Firmwarepool::getCacheDir() const
{
    return m_cacheDir;
}

void Firmwarepool::setIndexUpdatetime(int minutes)
{
    m_indexAutoUpdatetime = minutes;
}

void Firmwarepool::downloadIndex(const std::string &url)
{
    std::string newPath(core::pathconcat(m_cacheDir, std::string(INDEX_FILE_NAME) + ".new"));
    std::string oldPath(core::pathconcat(m_cacheDir, INDEX_FILE_NAME));
    std::string file(newPath);

    // don't download index if the modification time is less than 10 min
    if (m_indexAutoUpdatetime != 0) {
        try {
            core::DateTime dt = core::Fileutil::getMTime(oldPath);
            core::DateTime now;
            if (now - dt < m_indexAutoUpdatetime * 60)
                return;
        } catch (const core::IOError &e) {
            USBPROG_DEBUG_DBG("IO Error: %s", e.what());
        }
    }

    std::ofstream fout(file.c_str());
    if (fout.bad())
        throw DownloadError("Opening " + file + " failed");

    Downloader dl(fout);
    dl.setUrl(url);
    dl.setProgress(m_progressNotifier);
    try {
        dl.download();
    } catch (const DownloadError &err) {
        fout.close();
        remove(newPath.c_str());
        throw;
    }

    fout.close();

    // after the download is successful, rename new file to old file
    USBPROG_DEBUG_DBG("Renaming '%s' to '%s'\n", newPath.c_str(), oldPath.c_str());
    rename(newPath.c_str(), oldPath.c_str());
}

void Firmwarepool::readIndex()
{
    QDomDocument doc("usbprog");

    std::string filename = core::pathconcat(m_cacheDir, INDEX_FILE_NAME);
    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly))
        throw core::ParseError("Couldn't open " + filename);

    bool success = doc.setContent(&file);
    file.close();
    if (!success)
        throw core::ParseError("Unable to parse '" + filename + "'");


    QDomElement docElem = doc.documentElement();

    FirmwareXMLParser parser(this);
    for (QDomNode node = docElem.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (!node.isElement())
            continue;
        QDomElement element = node.toElement();
        if (element.tagName() == "pool")
            parser.parsePool(doc, element);
    }
}

void Firmwarepool::deleteIndex()
{
    std::string file = core::pathconcat(m_cacheDir, INDEX_FILE_NAME);
    int ret = remove(file.c_str());
    if (ret < 0)
        throw core::IOError("Deleting index file failed: " + std::string(std::strerror(errno)));
}

void Firmwarepool::setProgress(core::ProgressNotifier *notifier)
{
    m_progressNotifier = notifier;
}

void Firmwarepool::downloadFirmware(const std::string &name)
{
    Firmware *fw = getFirmware(name);
    if (!fw)
        throw core::ApplicationError("Firmware doesn't exist");

    std::string url = fw->getUrl() + "/" + fw->getFilename();
    std::string file(core::pathconcat(m_cacheDir, fw->getVerFilename()));
    if (core::Fileutil::isFile(file)) {
        // check md5 if available, if the checksum is wrong, then delete
        // the file and download again. Check the checksum after that
        // download again to verify that it's now correct.
        if (fw->getMD5Sum().size() > 0) {
            if (core::check_digest(file, fw->getMD5Sum(), core::Digest::DA_MD5))
                return;
            else
                remove(file.c_str());
        } else
            return;
    }

    std::ofstream fout(file.c_str(), std::ios::binary);
    if (!fout)
        throw core::IOError("Opening " + file + " failed");

    try {
        Downloader dl(fout);
        dl.setProgress(m_progressNotifier);
        dl.setUrl(url);
        dl.download();
    } catch (const DownloadError &) {
        fout.close();
        remove(file.c_str());
        throw;
    }

    fout.close();

    // check md5 if available
    if (fw->getMD5Sum().size() > 0) {
        if (!core::check_digest(file, fw->getMD5Sum(), core::Digest::DA_MD5)) {
            remove(file.c_str());
            throw DownloadError("Bad checksum");
        }
    }
}

void Firmwarepool::fillFirmware(const std::string &name)
{
    Firmware *fw = getFirmware(name);
    if (!fw)
        throw core::ApplicationError("Firmware doesn't exist");

    std::string file = getFirmwareFilename(fw);
    fw->setData(core::Fileutil::readBytesFromFile(file));
}

std::string Firmwarepool::getFirmwareFilename(Firmware *fw) const
{
    return core::pathconcat(m_cacheDir, fw->getVerFilename());
}

StringList Firmwarepool::getFirmwareNameList() const
{
    StringList ret;
    for (StringFirmwareMap::const_iterator it = m_firmware.begin();
            it != m_firmware.end(); ++it)
        ret.push_back(it->first);

    return ret;
}

Firmware *Firmwarepool::getFirmware(const std::string &name) const
{
    StringFirmwareMap::const_iterator it = m_firmware.find(name);
    if (it == m_firmware.end())
        return NULL;
    else
        return it->second;
}

std::vector<Firmware *> Firmwarepool::getFirmwareList() const
{
    std::vector<Firmware *> ret;

    for (StringFirmwareMap::const_iterator it = m_firmware.begin();
            it != m_firmware.end(); ++it)
        ret.push_back(it->second);

    return ret;
}

std::vector<core::UpdateDevice> Firmwarepool::getUpdateDeviceList() const
{
    std::vector<core::UpdateDevice> ret;

    for (StringFirmwareMap::const_iterator it = m_firmware.begin(); it != m_firmware.end(); ++it) {
        const Firmware *fw = it->second;
        if (fw->updateDevice().isValid())
            ret.push_back(fw->updateDevice());
    }

    return ret;
}

void Firmwarepool::deleteCache()
{
    QDir cacheDir(QString::fromStdString(m_cacheDir));
    if (!cacheDir.exists())
        throw core::IOError("opendir on " + m_cacheDir + " failed");

    QStringList entries = cacheDir.entryList(QDir::Files);
    Q_FOREACH (QString entry, entries) {
        if (entry == INDEX_FILE_NAME)
            continue;
        if (!cacheDir.remove(entry))
            throw core::IOError("Deletion of " +entry.toStdString()+" in directory " + m_cacheDir + " failed ");
    }
}

bool Firmwarepool::isFirmwareOnDisk(const std::string &name)
{
    Firmware *fw = getFirmware(name);
    if (!fw)
        return false;

    return core::Fileutil::isFile(getFirmwareFilename(fw));
}

void Firmwarepool::cleanCache()
{
    QDir cacheDir(QString::fromStdString(m_cacheDir));
    if (!cacheDir.exists())
        throw core::IOError("opendir on " + m_cacheDir + " failed");

    QStringList entries = cacheDir.entryList(QDir::Files);
    Q_FOREACH (QString entry, entries) {
        if (entry == INDEX_FILE_NAME)
            continue;

        std::string name = entry.toStdString();
        std::string::size_type last_dot = name.rfind('.', name.length());
        if (last_dot == std::string::npos)
            continue;

        std::string firmware = name.substr(0, last_dot);
        if (firmware.rfind(".bin") == firmware.size() - 4)
            firmware.erase(firmware.size() - 4);
        std::string version = name.substr(last_dot+1);

        Firmware *fw = getFirmware(firmware);
        if (!fw)
            continue;


        if (isFirmwareOnDisk(firmware) && version != fw->getVersionString()) {
            if (!cacheDir.remove(entry))
                throw core::IOError("Deletion of " +entry.toStdString()+" in directory " + m_cacheDir + " failed ");
        }
    }
}

void Firmwarepool::addFirmware(Firmware *fw)
{
    m_firmware[fw->getName()] = fw;
}

/* }}} */

} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
