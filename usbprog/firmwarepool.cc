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
#include <unistd.h>

#include <QDomDocument>
#include <QFile>

#include <sys/types.h>
#include <dirent.h>

#include <usbprog-core/stringutil.h>
#include <usbprog-core/util.h>
#include <usbprog-core/digest.h>
#include <usbprog-core/debug.h>
#include <usbprog/firmwarepool.h>

using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::copy;
using std::ios;
using std::atoi;
using std::endl;
using std::stringstream;
using std::remove;
using std::back_inserter;
using std::strncmp;
using std::hex;
using std::setw;
using std::setfill;

#define INDEX_FILE_NAME  "versions.xml"
#define BUFFERSIZE       2048

/* Class declaration: FirmwareXMLParser {{{1 */

class FirmwareXMLParser {
    public:
        FirmwareXMLParser(Firmwarepool *pool);

    public:
        void parsePool(const QDomDocument &doc, const QDomElement &pool)
        throw (ParseError);

    protected:
        void parseFirmware(const QDomDocument &doc, const QDomElement &firmware)
        throw (ParseError);

        Firmware *newFirmwareFromXml(const QDomDocument &doc, const QDomElement &cur)
        throw (ParseError);

    private:
        Firmwarepool *m_firmwarepool;
};

/* Implementation: FirmwareXMLParser {{{1 */

#define XMLCHAR(a) \
    reinterpret_cast<const xmlChar *>(a)

/* -------------------------------------------------------------------------- */
FirmwareXMLParser::FirmwareXMLParser(Firmwarepool *pool)
    : m_firmwarepool(pool)
{}

/* -------------------------------------------------------------------------- */
void FirmwareXMLParser::parsePool(const QDomDocument &doc, const QDomElement &pool)
    throw (ParseError)
{
    for (QDomNode node = pool.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (!node.isElement())
            continue;
        QDomElement childElement = node.toElement();
        if (childElement.tagName() == "firmware")
            parseFirmware(doc, childElement);
    }
}

/* -------------------------------------------------------------------------- */
void FirmwareXMLParser::parseFirmware(const QDomDocument &doc, const QDomElement &firmware)
    throw (ParseError)
{
    Firmware *fw;

    // set name
    QString name = firmware.attribute("name");
    if (name.isEmpty())
        throw ParseError("Firmware has no name");
    fw = new Firmware(name.toStdString());

    // set label
    fw->setLabel(firmware.attribute("label").toStdString());

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
            fw->setDate(DateTime(childElement.attribute("date").toStdString(), DTF_ISO_DATE));
            fw->setMD5Sum(childElement.attribute("md5sum").toStdString());
        } else if (childElement.tagName() == "description") {
            fw->setVendorId(parse_long(childElement.attribute("vendorid").toAscii()));
            fw->setProductId(parse_long(childElement.attribute("productid").toAscii()));
            fw->setBcdDevice(parse_long(childElement.attribute("bcddevice").toAscii()));
            fw->setDescription(strip(childElement.text().toStdString()));
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

/* Firmware {{{1 */

/* -------------------------------------------------------------------------- */
Firmware::Firmware(const string &name)
    : m_name(name)
{}

/* -------------------------------------------------------------------------- */
string Firmware::getName() const
{
    return m_name;
}

/* -------------------------------------------------------------------------- */
void Firmware::setLabel(const string &label)
{
    m_label = label;
}

/* -------------------------------------------------------------------------- */
string Firmware::getLabel() const
{
    return m_label;
}

/* -------------------------------------------------------------------------- */
void Firmware::setFilename(const string &filename)
{
    m_filename = filename;
}

/* -------------------------------------------------------------------------- */
string Firmware::getFilename() const
{
    return m_filename;
}

/* -------------------------------------------------------------------------- */
string Firmware::getVerFilename() const
{
    return m_filename + "." + getVersionString();
}

/* -------------------------------------------------------------------------- */
void Firmware::setUrl(const string &url)
{
    m_url = url;
}

/* -------------------------------------------------------------------------- */
string Firmware::getUrl() const
{
    return m_url;
}

/* -------------------------------------------------------------------------- */
void Firmware::setAuthor(const string &author)
{
    m_author = author;
}

/* -------------------------------------------------------------------------- */
string Firmware::getAuthor() const
{
    return m_author;
}

/* -------------------------------------------------------------------------- */
void Firmware::setVersion(int version)
{
    m_version = version;
}

/* -------------------------------------------------------------------------- */
void Firmware::setDate(const DateTime &date)
{
    m_date = date;
}

/* -------------------------------------------------------------------------- */
const DateTime Firmware::getDate() const
{
    return m_date;
}

/* -------------------------------------------------------------------------- */
void Firmware::setMD5Sum(const std::string &md5)
{
    m_md5sum = md5;
}

/* -------------------------------------------------------------------------- */
std::string Firmware::getMD5Sum() const
{
    return m_md5sum;
}

/* -------------------------------------------------------------------------- */
int Firmware::getVersion() const
{
    return m_version;
}

/* -------------------------------------------------------------------------- */
string Firmware::getVersionString() const
{
    stringstream ss;
    ss << m_version;
    return ss.str();
}

/* -------------------------------------------------------------------------- */
void Firmware::setDescription(const string &description)
{
    m_description = description;
}

/* -------------------------------------------------------------------------- */
string Firmware::getDescription() const
{
    return m_description;
}

/* -------------------------------------------------------------------------- */
void Firmware::setPin(const string &name, const string &value)
{
    m_pins[name] = value;
}

/* -------------------------------------------------------------------------- */
string Firmware::getPin(const string &name) const
{
    StringStringMap::const_iterator it = m_pins.find(name);
    if (it != m_pins.end())
        return (*it).second;
    else
        return string();
}

/* -------------------------------------------------------------------------- */
StringVector Firmware::getPins() const
{
    StringVector ret;

    for (StringStringMap::const_iterator it = m_pins.begin();
            it != m_pins.end(); ++it)
        ret.push_back(it->first);

    return ret;
}

/* -------------------------------------------------------------------------- */
void Firmware::setData(const ByteVector &data)
{
    m_data = data;
}

/* -------------------------------------------------------------------------- */
ByteVector &Firmware::getData()
{
    return m_data;
}

/* -------------------------------------------------------------------------- */
const ByteVector &Firmware::getData() const
{
    return m_data;
}

/* -------------------------------------------------------------------------- */
void Firmware::setVendorId(uint16_t vendorid)
{
    m_vendorId = vendorid;
}

/* -------------------------------------------------------------------------- */
uint16_t Firmware::getVendorId() const
{
    return m_vendorId;
}

/* -------------------------------------------------------------------------- */
void Firmware::setProductId(uint16_t productid)
{
    m_productId = productid;
}

/* -------------------------------------------------------------------------- */
uint16_t Firmware::getProductId() const
{
    return m_productId;
}

/* -------------------------------------------------------------------------- */
void Firmware::setBcdDevice(uint16_t bcdDevice)
{
    m_bcdDevice = bcdDevice;
}

/* -------------------------------------------------------------------------- */
uint16_t Firmware::getBcdDevice() const
{
    return m_bcdDevice;
}

/* -------------------------------------------------------------------------- */
string Firmware::toString() const
{
    stringstream ss;

    ss << "Name            : " << m_name << endl;
    ss << "Label           : " << m_label << endl;
    ss << "File name       : " << m_filename << endl;
    ss << "URL             : " << m_url << endl;
    ss << "Version         : " << m_version << endl;
    ss << "Author          : " << m_author << endl;
    ss << "Date            : " << m_date.getDateTimeString(DTF_ISO_DATETIME) << endl;
    ss << "MD5sum          : " << m_md5sum << endl;
    ss << "Description     : " << m_description << endl;
    ss << "Pins      P1    : " << getPin("P1") << endl;
    ss << "          P2    : " << getPin("P2") << endl;
    ss << "          P3    : " << getPin("P3") << endl;
    ss << "          P4    : " << getPin("P4") << endl;
    ss << "          P5    : " << getPin("P5") << endl;
    ss << "          P6    : " << getPin("P6") << endl;
    ss << "          P7    : " << getPin("P7") << endl;
    ss << "          P8    : " << getPin("P8") << endl;
    ss << "          P9    : " << getPin("P9") << endl;
    ss << "          P10   : " << getPin("P10") << endl;;
    ss << "          RX    : " << getPin("RX") << endl;
    ss << "          TX    : " << getPin("TX") << endl;
    ss << "          LED   : " << getPin("LED") << endl;
    ss << "          JP    : " << getPin("JP") << endl;

    return ss.str();
}

/* -------------------------------------------------------------------------- */
string Firmware::formatDateVersion() const
{
    stringstream ss;

    ss << getVersion();
    ss << " [" << getDate().getDateTimeString(DTF_ISO_DATE) << "]";

    return ss.str();
}

/* -------------------------------------------------------------------------- */
bool Firmware::hasDeviceId() const
{
    return getVendorId() != 0 || getProductId() != 0 || getBcdDevice() != 0;
}

/* -------------------------------------------------------------------------- */
string Firmware::formatDeviceId() const
{
    stringstream ss;

    if (getVendorId() != 0)
        ss << "Vendor: 0x" << setw(4) << hex << setfill('0') << getVendorId();
    if (getVendorId() != 0 && getProductId() != 0)
        ss << ", ";
    if (getProductId() != 0)
        ss << "Product: 0x" << setw(4) << hex << getProductId();
    if (getBcdDevice() != 0 && (getProductId() != 0
                || getVendorId() != 0))
        ss << ", ";
    if (getBcdDevice() != 0)
        ss << "BCDDevice: 0x" << setw(4) << hex << getBcdDevice();

    return ss.str();
}

/* Firmwarepool {{{1 */

/* -------------------------------------------------------------------------- */
void Firmwarepool::readFromFile(const string &file,
        ByteVector &bv) throw (IOError)
{
    char buffer[BUFFERSIZE];

    ifstream fin(file.c_str(), ios::binary);
    if (!fin)
        throw IOError("Opening " + file + " failed");

    bv.clear();
    while (!fin.eof()) {
        fin.read(buffer, BUFFERSIZE);
        if (fin.bad())
            throw IOError("Error while reading data from " + file);

        copy(buffer, buffer + fin.gcount(), back_inserter(bv));
    }

    fin.close();
}

/* -------------------------------------------------------------------------- */
Firmwarepool::Firmwarepool(const string &cacheDir)
      throw (IOError)
    : m_cacheDir(cacheDir), m_progressNotifier(NULL),
      m_indexAutoUpdatetime(0)
{
    if (!Fileutil::isDir(cacheDir))
        if (!Fileutil::mkdir(cacheDir))
            throw IOError("Creating " + cacheDir + " failed");
}

/* -------------------------------------------------------------------------- */
Firmwarepool::~Firmwarepool()
{
    for (StringFirmwareMap::iterator it = m_firmware.begin();
            it != m_firmware.end(); ++it)
        delete it->second;
}

/* -------------------------------------------------------------------------- */
string Firmwarepool::getCacheDir() const
{
    return m_cacheDir;
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::setIndexUpdatetime(int minutes)
{
    m_indexAutoUpdatetime = minutes;
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::downloadIndex(const string &url)
    throw (DownloadError)
{
    string newPath(pathconcat(m_cacheDir, string(INDEX_FILE_NAME) + ".new"));
    string oldPath(pathconcat(m_cacheDir, INDEX_FILE_NAME));
    string file(newPath);

    // don't download index if the modification time is less than 10 min
    if (m_indexAutoUpdatetime != 0) {
        try {
            DateTime dt = Fileutil::getMTime(oldPath);
            DateTime now;
            if (now - dt < m_indexAutoUpdatetime * 60)
                return;
        } catch (const IOError &e)
        {}
    }

    ofstream fout(file.c_str());
    if (fout.bad())
        throw DownloadError("Opening " + file + " failed");

    Downloader dl(fout);
    dl.setUrl(url);
    dl.download();
    fout.close();

    // after the download is successful, rename new file to old file
    Debug::debug()->dbg("Renaming '%s' to '%s'\n",
            newPath.c_str(), oldPath.c_str());
    rename(newPath.c_str(), oldPath.c_str());
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::readIndex()
    throw (IOError, ParseError)
{
    QDomDocument doc("usbprog");

    string filename = pathconcat(m_cacheDir, INDEX_FILE_NAME);
    QFile file(filename.c_str());
    if (!file.open(QIODevice::ReadOnly))
        throw ParseError("Couldn't open " + filename);

    bool success = doc.setContent(&file);
    file.close();
    if (!success)
        throw ParseError("Unable to parse '" + filename + "'");


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

/* -------------------------------------------------------------------------- */
void Firmwarepool::deleteIndex()
    throw (IOError)
{
    string file = pathconcat(m_cacheDir, INDEX_FILE_NAME);
    int ret = remove(file.c_str());
    if (ret < 0)
        throw IOError("Deleting index file failed: " + string(strerror(errno)));
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::setProgress(ProgressNotifier *notifier)
{
    m_progressNotifier = notifier;
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::downloadFirmware(const string &name)
    throw (DownloadError, GeneralError)
{
    Firmware *fw = getFirmware(name);
    if (!fw)
        throw GeneralError("Firmware doesn't exist");

    string url = fw->getUrl() + "/" + fw->getFilename();
    string file(pathconcat(m_cacheDir, fw->getVerFilename()));
    if (Fileutil::isFile(file)) {
        // check md5 if available, if the checksum is wrong, then delete
        // the file and download again. Check the checksum after that
        // download again to verify that it's now correct.
        if (fw->getMD5Sum().size() > 0) {
            if (check_digest(file, fw->getMD5Sum(), Digest::DA_MD5))
                return;
            else
                remove(file.c_str());
        } else
            return;
    }

    ofstream fout(file.c_str(), ios::binary);
    if (!fout)
        throw IOError("Opening " + file + " failed");

    try {
        Downloader dl(fout);
        dl.setProgress(m_progressNotifier);
        dl.setUrl(url);
        dl.download();
    } catch (const DownloadError &err) {
        fout.close();
        remove(file.c_str());
        throw;
    }

    fout.close();

    // check md5 if available
    if (fw->getMD5Sum().size() > 0) {
        if (!check_digest(file, fw->getMD5Sum(), Digest::DA_MD5)) {
            remove(file.c_str());
            throw DownloadError("Bad checksum");
        }
    }
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::fillFirmware(const string &name)
    throw (IOError, GeneralError)
{
    Firmware *fw = getFirmware(name);
    if (!fw)
        throw GeneralError("Firmware doesn't exist");

    string file = getFirmwareFilename(fw);
    readFromFile(file, fw->getData());
}

/* -------------------------------------------------------------------------- */
string Firmwarepool::getFirmwareFilename(Firmware *fw) const
{
    return pathconcat(m_cacheDir, fw->getVerFilename());
}

/* -------------------------------------------------------------------------- */
StringList Firmwarepool::getFirmwareNameList() const
{
    StringList ret;
    for (StringFirmwareMap::const_iterator it = m_firmware.begin();
            it != m_firmware.end(); ++it)
        ret.push_back(it->first);

    return ret;
}

/* -------------------------------------------------------------------------- */
Firmware *Firmwarepool::getFirmware(const string &name) const
{
    StringFirmwareMap::const_iterator it = m_firmware.find(name);
    if (it == m_firmware.end())
        return NULL;
    else
        return it->second;
}

/* -------------------------------------------------------------------------- */
vector<Firmware *> Firmwarepool::getFirmwareList() const
{
    vector<Firmware *> ret;

    for (StringFirmwareMap::const_iterator it = m_firmware.begin();
            it != m_firmware.end(); ++it)
        ret.push_back(it->second);

    return ret;
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::deleteCache()
    throw (IOError)
{
    struct dirent *de;

    DIR *dir = opendir(m_cacheDir.c_str());
    if (!dir)
        throw IOError("opendir on " + m_cacheDir + " failed");

    while ((de = readdir(dir)) != NULL) {
        if (strcmp(de->d_name, INDEX_FILE_NAME) == 0 ||
                strcmp(de->d_name, ".") == 0 ||
                strcmp(de->d_name, "..") == 0)
            continue;

        string file = pathconcat(m_cacheDir, de->d_name);
        int ret = remove(file.c_str());
        if (ret < 0) {
            closedir(dir);
            throw IOError("Deletion of " +file+ " failed ");
        }
    }

    closedir(dir);
}

/* -------------------------------------------------------------------------- */
bool Firmwarepool::isFirmwareOnDisk(const string &name)
    throw (IOError)
{
    Firmware *fw = getFirmware(name);
    if (!fw)
        return false;

    return Fileutil::isFile(getFirmwareFilename(fw));
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::cleanCache()
    throw (IOError)
{
    struct dirent *de;

    DIR *dir = opendir(m_cacheDir.c_str());
    if (!dir)
        throw IOError("opendir on " + m_cacheDir + " failed");

    while ((de = readdir(dir)) != NULL) {
        if (strcmp(de->d_name, INDEX_FILE_NAME) == 0 ||
                strcmp(de->d_name, ".") == 0 ||
                strcmp(de->d_name, "..") == 0)
            continue;

        string name = de->d_name;
        size_t last_dot = name.rfind('.', name.length());
        if (last_dot == string::npos)
            continue;

        string firmware = name.substr(0, last_dot);
        if (firmware.rfind(".bin") == firmware.size() - 4)
            firmware.erase(firmware.size() - 4);
        string version = name.substr(last_dot+1);

        Firmware *fw = getFirmware(firmware);
        if (!fw)
            continue;

        if (isFirmwareOnDisk(firmware) && version != fw->getVersionString()) {
            int ret;

            string file(pathconcat(m_cacheDir, name).c_str());
            ret = remove(file.c_str());
            if (ret < 0) {
                closedir(dir);
                throw IOError("Deletion of file " + file + " failed.");
            }
        }
    }

    closedir(dir);
}

/* -------------------------------------------------------------------------- */
void Firmwarepool::addFirmware(Firmware *fw)
{
    m_firmware[fw->getName()] = fw;
}

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
