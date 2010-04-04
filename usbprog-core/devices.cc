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
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

#include <unistd.h>
#include <usb.h>
#include <usbprog-core/devices.h>
#include <usbprog-core/util.h>
#include <usbprog-core/debug.h>
#include <usbprog/usbprog.h>

#define VENDOR_ID_USBPROG       0x1781
#define PRODUCT_ID_USBPROG      0x0c62
#define BCDDEVICE_UPDATE        0x0000

/* UpdateDevice {{{ */

// -----------------------------------------------------------------------------
uint16_t UpdateDevice::VENDOR_INVALID      = 0xffff;
uint16_t UpdateDevice::PRODUCT_INVALID     = 0xffff;
uint16_t UpdateDevice::BCDDEVICE_INVALID   = 0xffff;

// -----------------------------------------------------------------------------
UpdateDevice::UpdateDevice(const std::string &name)
    : m_name(name)
    , m_vendor(VENDOR_INVALID)
    , m_product(PRODUCT_INVALID)
    , m_bcddevice(BCDDEVICE_INVALID)
{}

// -----------------------------------------------------------------------------
uint16_t UpdateDevice::getVendor() const
{
    return m_vendor;
}

// -----------------------------------------------------------------------------
void UpdateDevice::setVendor(uint16_t vendor)
{
    m_vendor = vendor;
}

// -----------------------------------------------------------------------------
uint16_t UpdateDevice::getProduct() const
{
    return m_product;
}

// -----------------------------------------------------------------------------
void UpdateDevice::setProduct(uint16_t product)
{
    m_product = product;
}

// -----------------------------------------------------------------------------
uint16_t UpdateDevice::getBcdDevice() const
{
    return m_bcddevice;
}

// -----------------------------------------------------------------------------
void UpdateDevice::setBcdDevice(uint16_t bcddevice)
{
    m_bcddevice = bcddevice;
}

// -----------------------------------------------------------------------------
std::string UpdateDevice::getName() const
{
    return m_name;
}

// -----------------------------------------------------------------------------
void UpdateDevice::setName(const std::string &name)
{
    m_name = name;
}

// -----------------------------------------------------------------------------
std::string UpdateDevice::getLabel() const
{
    return m_label;
}

// -----------------------------------------------------------------------------
void UpdateDevice::setLabel(const std::string &label)
{
    m_label = label;
}

// -----------------------------------------------------------------------------
bool UpdateDevice::isValid() const
{
    return m_vendor != VENDOR_INVALID &&
           m_product != PRODUCT_INVALID &&
           m_bcddevice != BCDDEVICE_INVALID;
}

// -----------------------------------------------------------------------------
std::string UpdateDevice::formatDeviceId() const
{
    std::stringstream ss;

    if (m_vendor != VENDOR_INVALID)
        ss << "Vendor: 0x" << std::setw(4) << std::hex << std::setfill('0') << m_vendor;
    if (m_vendor != VENDOR_INVALID && m_product != PRODUCT_INVALID)
        ss << ", ";
    if (m_product != PRODUCT_INVALID)
        ss << "Product: 0x" << std::setw(4) << std::hex << m_product;
    if (m_bcddevice != BCDDEVICE_INVALID &&
            (m_vendor != VENDOR_INVALID && m_product != PRODUCT_INVALID))
        ss << ", ";
    if (m_bcddevice != BCDDEVICE_INVALID)
        ss << "BCDDevice: 0x" << std::setw(4) << std::hex << m_bcddevice;

    return ss.str();

}

/* }}} */
/* Device {{{ */

/* -------------------------------------------------------------------------- */
Device::Device(struct usb_device *handle)
    : m_handle(handle), m_updateMode(false)
{}

/* -------------------------------------------------------------------------- */
uint16_t Device::getVendor() const
{
    return m_handle->descriptor.idVendor;
}

/* -------------------------------------------------------------------------- */
uint16_t Device::getProduct() const
{
    return m_handle->descriptor.idProduct;
}

/* -------------------------------------------------------------------------- */
std::string Device::getDevice() const
{
    return std::string(m_handle->filename);
}

/* -------------------------------------------------------------------------- */
std::string Device::getBus() const
{
    return std::string(m_handle->bus->dirname);
}

/* -------------------------------------------------------------------------- */
bool Device::isUpdateMode() const
{
    return m_updateMode;
}

/* -------------------------------------------------------------------------- */
void Device::setShortName(const std::string &shortName)
{
    m_shortName = shortName;
}

/* -------------------------------------------------------------------------- */
std::string Device::getShortName() const
{
    return m_shortName;
}

/* -------------------------------------------------------------------------- */
void Device::setUpdateMode(bool updateMode)
{
    m_updateMode = updateMode;
}

/* -------------------------------------------------------------------------- */
struct usb_device *Device::getHandle() const
{
    return m_handle;
}

/* -------------------------------------------------------------------------- */
void Device::setName(const std::string &name)
{
    m_name = name;
}

/* -------------------------------------------------------------------------- */
std::string Device::getName() const
{
    return m_name;
}

/* -------------------------------------------------------------------------- */
std::string Device::toString() const
{
    std::stringstream ss;

    ss << "Bus " << getBus() << " ";
    ss << "Device " << getDevice() << ": ";
    ss << std::setw(4) << std::hex << std::setfill('0') << getVendor();
    ss << std::setw(1) << ":";
    ss << std::setw(4) << std::hex << std::setfill('0') << getProduct();

    if (m_name.size() > 0)
        ss << " - " + m_name;

    return ss.str();
}

/* -------------------------------------------------------------------------- */
std::string Device::toShortString() const
{
    std::stringstream ss;

    ss << std::setw(4) << std::hex << std::setfill('0') << getVendor();
    ss << std::setw(1) << ":";
    ss << std::setw(4) << std::hex << std::setfill('0') << getProduct();

    if (m_name.size() > 0)
        ss << " - " + m_name;

    return ss.str();
}

/* -------------------------------------------------------------------------- */
bool operator==(const DeviceVector &a, const DeviceVector &b)
{
    if (a.size() != b.size())
        return false;

    for (unsigned int i = 0; i < a.size(); ++i)
        if (*(a[i]) != *(b[i]))
            return false;

    return true;
}

/* }}} */
/* DeviceManager {{{ */

/* -------------------------------------------------------------------------- */
DeviceManager::DeviceManager()
    : m_currentUpdateDevice(-1)
{
    init();
}

/* -------------------------------------------------------------------------- */
DeviceManager::~DeviceManager()
{
    for (DeviceVector::const_iterator it = m_updateDevices.begin();
            it != m_updateDevices.end(); ++it)
        delete *it;
}

/* -------------------------------------------------------------------------- */
DeviceManager::DeviceManager(int debuglevel)
    : m_currentUpdateDevice(-1)
{
    init(debuglevel);
}

/* -------------------------------------------------------------------------- */
void DeviceManager::init(int debuglevel)
{
    if (debuglevel != 0)
        setUsbDebugging(debuglevel);
    Debug::debug()->trace("usb_init()");
    usb_init();
}

/* -------------------------------------------------------------------------- */
void DeviceManager::setUsbDebugging(int debuglevel)
{
    Debug::debug()->trace("usb_set_debug(%d)", debuglevel);
    usb_set_debug(debuglevel);
}

/* -------------------------------------------------------------------------- */
void DeviceManager::discoverUpdateDevices(const std::vector<UpdateDevice> &updateDevices)
{
    Debug::debug()->trace("usb_find_busses()");
    usb_find_busses();
    Debug::debug()->trace("usb_find_devices()");
    usb_find_devices();

    DeviceVector oldDevices = m_updateDevices;
    m_updateDevices.clear();

    for (struct usb_bus *bus = usb_get_busses(); bus; bus = bus->next) {
        for (struct usb_device *dev = bus->devices; dev; dev = dev->next) {
            uint16_t vendorid = dev->descriptor.idVendor;
            uint16_t productid = dev->descriptor.idProduct;
            uint16_t bcddevice = dev->descriptor.bcdDevice;
            Device *d = NULL;

            Debug::debug()->dbg("Found USB device [%04x:%04x:%04x]",
                    int(vendorid), int(productid), int(bcddevice));

            if (vendorid == VENDOR_ID_USBPROG &&
                    productid == PRODUCT_ID_USBPROG &&
                    bcddevice == BCDDEVICE_UPDATE) {
                d = new Device(dev);
                d->setUpdateMode(true);
                d->setName("USBprog in update mode");
                d->setShortName("usbprog");
            } else {
                for (std::vector<UpdateDevice>::const_iterator it = updateDevices.begin(); it != updateDevices.end(); ++it)
                    if (vendorid != 0 && productid != 0 &&
                            it->getVendor() == vendorid &&
                            it->getProduct() == productid &&
                            it->getBcdDevice() == bcddevice) {
                        d = new Device(dev);
                        d->setName("USBprog with \"" + it->getLabel() + "\" firmware");
                        d->setShortName(it->getName());
                        break;
                    }
            }

            if (d)
                m_updateDevices.push_back(d);
        }
    }

    // reset update device only when something has changed
    if (oldDevices != m_updateDevices)
        m_currentUpdateDevice = -1;

    // free memory
    for (DeviceVector::const_iterator it = oldDevices.begin(); it != oldDevices.end(); ++it)
        delete *it;
}

/* -------------------------------------------------------------------------- */
void DeviceManager::printDevices(std::ostream &os) const
{
    int i = 0;
    Device *up = getCurrentUpdateDevice();
    for (DeviceVector::const_iterator it = m_updateDevices.begin();
            it != m_updateDevices.end(); ++it) {

        Device *dev = *it;
        os << " [" << std::setw(2) << std::right << i++ << "] " << std::left;
        if (up != NULL && *up == *dev)
            os << " *  ";
        else
            os << "    ";
        os << "Bus " << dev->getBus() << " "
           << "Device " << dev->getDevice() << ": "
           << std::setw(4) << std::hex << std::setfill('0') << dev->getVendor()
           << std::setw(1) << ":"
           << std::setw(4) << std::hex << std::setfill('0') << dev->getProduct()
           << std::endl;

        if (dev->getName().size() > 0)
            os << "          " + dev->getShortName() << ": "
               << dev->getName() << std::endl;

        // reset fill character
        os << std::setfill(' ');
    }
}

/* -------------------------------------------------------------------------- */
void DeviceManager::switchUpdateMode()
    throw (IOError)
{
    Device *dev = getCurrentUpdateDevice();
    if (dev->isUpdateMode())
        return;

    Debug::debug()->dbg("DeviceManager::switchUpdateMode()");
    Debug::debug()->trace("usb_open(%p)", dev->getHandle());
    struct usb_dev_handle *usb_handle = usb_open(dev->getHandle());
    if (!usb_handle)
        throw IOError("Could not open USB device: " + std::string(usb_strerror()));

    Debug::debug()->trace("usb_set_configuration(%p, %d)",
            usb_handle, dev->getHandle()->config[0].bConfigurationValue);
    usb_set_configuration(usb_handle, dev->getHandle()->config[0].bConfigurationValue);

    int usb_interface = dev->getHandle()->config[0].interface[0].
        altsetting[0].bInterfaceNumber;
    Debug::debug()->trace("usb_claim_interface(%p, %d)",
            usb_handle, usb_interface);
    int ret = usb_claim_interface(usb_handle, usb_interface);
    if (ret < 0) {
        usb_close(usb_handle);
        throw IOError(usb_strerror());
    }

    /* needed ?*/
    usb_set_altinterface(usb_handle, 0);
    Debug::debug()->trace("usb_set_altinterface(%p, 0)", usb_handle);
    if (ret < 0) {
        usb_release_interface(usb_handle, usb_interface);
        usb_close(usb_handle);
        throw IOError("Error when setting altinterface to 0: " + std::string(usb_strerror()));
    }

    int timeout = 6;

    Debug::debug()->trace("usb_control_msg (multiple times)");
    while (usb_control_msg(usb_handle, 0xC0, 0x01, 0, 0, NULL, 8, 1000) < 0){
        if (--timeout == 0)
            break;
        usbprog_sleep(1);
    }

    Debug::debug()->trace("usb_release_interface(%p, %d)", usb_handle, usb_interface);
    usb_release_interface(usb_handle, usb_interface);

    Debug::debug()->trace("usb_close(%p)", usb_handle);
    usb_close(usb_handle);

    // set again the update device
    int updatedev = m_currentUpdateDevice;
    discoverUpdateDevices();

    // TODO: Verify that the m_currentUpdateDevice is still valid!
    setCurrentUpdateDevice(updatedev);
}

/* -------------------------------------------------------------------------- */
size_t DeviceManager::getNumberUpdateDevices() const
{
    return m_updateDevices.size();
}

/* -------------------------------------------------------------------------- */
Device *DeviceManager::getCurrentUpdateDevice() const
{
    if (m_currentUpdateDevice < -1 ||
            m_currentUpdateDevice >= (ssize_t)m_updateDevices.size())
        return NULL;

    if (m_currentUpdateDevice == (ssize_t)-1) {
        for (DeviceVector::const_iterator it = m_updateDevices.begin();
                it != m_updateDevices.end(); ++it)
            if ((*it)->isUpdateMode())
                return *it;

        return NULL;
    } else
        return m_updateDevices[m_currentUpdateDevice];
}

// -----------------------------------------------------------------------------
void DeviceManager::clearCurrentUpdateDevice()
{
    m_currentUpdateDevice = -1;
}

/* -------------------------------------------------------------------------- */
Device *DeviceManager::getDevice(size_t number) const
{
    if (number >= m_updateDevices.size())
        return NULL;

    return m_updateDevices[number];
}

/* -------------------------------------------------------------------------- */
void DeviceManager::setCurrentUpdateDevice(ssize_t number)
{
    if (number < 0 || number >= int(m_updateDevices.size()))
        return;
    m_currentUpdateDevice = number;
}

/* -------------------------------------------------------------------------- */
bool operator==(const Device &a, const Device &b)
{
    return a.getBus() == b.getBus() &&
        a.getDevice() == b.getDevice() &&
        a.getProduct() == b.getProduct() &&
        a.getVendor() == b.getVendor();
}

/* -------------------------------------------------------------------------- */
bool operator!=(const Device &a, const Device &b)
{
    return a.getBus() != b.getBus() ||
        a.getDevice() != b.getDevice() ||
        a.getProduct() != b.getProduct() ||
        a.getVendor() != b.getVendor();
}

/* }}} */
/* UsbprogUpdater {{{ */

#define USB_PAGESIZE 64
#define WRITEPAGE      0x02
#define STARTAPP       0x01

/* -------------------------------------------------------------------------- */
UsbprogUpdater::UsbprogUpdater(Device *dev)
    : m_dev(dev), m_progressNotifier(NULL), m_devHandle(NULL)
{}

/* -------------------------------------------------------------------------- */
UsbprogUpdater::~UsbprogUpdater()
{
    if (m_devHandle)
        updateClose();
}

/* -------------------------------------------------------------------------- */
void UsbprogUpdater::setProgress(ProgressNotifier *progress)
{
    m_progressNotifier = progress;
}

/* -------------------------------------------------------------------------- */
void UsbprogUpdater::writeFirmware(const ByteVector &bv)
    throw (IOError)
{
    char buf[USB_PAGESIZE];
    char cmd[USB_PAGESIZE];
    int ret;

    Debug::debug()->dbg("UsbprogUpdater::writeFirmware, size=%d", bv.size());

    if (!m_devHandle)
        throw IOError("Device not opened");

    int page = 0;
    memset(cmd, 0, USB_PAGESIZE);

    for (unsigned int i = 0; i < bv.size(); i += 64) {
        size_t sz = std::min(USB_PAGESIZE, int(bv.size()-i));
        memset(buf, 0, USB_PAGESIZE);
        copy(bv.begin() + i, bv.begin() + i + sz, buf);

        cmd[0] = WRITEPAGE;
        cmd[1] = (char)page;
        cmd[2] = (char)(page++ >> 8);

        Debug::debug()->trace("usb_bulk_write(%p, 2, %p, %d, 100)",
                m_devHandle, 2, cmd, USB_PAGESIZE);

        ret = usb_bulk_write(m_devHandle,2,cmd,USB_PAGESIZE, 100);
        if (ret < 0) {
            updateClose();
            if (m_progressNotifier)
                m_progressNotifier->finished();
            throw IOError("Error while writing to USB device: "+
                    std::string(usb_strerror()));
        }

        // data message
        Debug::debug()->trace("usb_bulk_write(%p, 2, %p, %d, 100)",
                m_devHandle, buf, USB_PAGESIZE);
        ret = usb_bulk_write(m_devHandle,2,buf,USB_PAGESIZE, 100);
        if (ret < 0) {
            updateClose();
            if (m_progressNotifier)
                m_progressNotifier->finished();
            throw IOError("Error while writing to USB device: "+
                    std::string(usb_strerror()));
        }

        if (m_progressNotifier)
            m_progressNotifier->progressed(bv.size(), i);
    }

    if (m_progressNotifier)
        m_progressNotifier->finished();
}

/* -------------------------------------------------------------------------- */
void UsbprogUpdater::updateOpen()
    throw (IOError)
{
    struct usb_device *dev = m_dev->getHandle();

    int ret;
    Debug::debug()->dbg("UsbprogUpdater::updateOpen()");

    if (m_devHandle)
        throw IOError("Device still opened. Close first.");

    Debug::debug()->trace("usb_open(%p)", dev);
    m_devHandle = usb_open(dev);
    if (!m_devHandle)
        throw IOError("usb_open failed " + std::string(usb_strerror()));

    Debug::debug()->trace("usb_set_configuration(handle, %d)",
            dev->config[0].bConfigurationValue);
    usb_set_configuration(m_devHandle, dev->config[0].bConfigurationValue);

    int usb_interface = dev->config[0].interface[0].altsetting[0].bInterfaceNumber;
    Debug::debug()->trace("usb_claim_interface(handle, %d)", usb_interface);
    ret = usb_claim_interface(m_devHandle, usb_interface);
    if (ret < 0) {
        updateClose();
        throw IOError("Claiming interface failed: " + std::string(usb_strerror()));
    }
}

/* -------------------------------------------------------------------------- */
void UsbprogUpdater::updateClose()
    throw (IOError)
{
    Debug::debug()->dbg("UsbprogUpdater::updateClose()");

    if (!m_devHandle)
        throw IOError("Device already closed");

    int usb_interface = m_dev->getHandle()->config[0].interface[0]
        .altsetting[0].bInterfaceNumber;

    Debug::debug()->trace("usb_release_interface(%p, %d)",
            m_devHandle, usb_interface);
    usb_release_interface(m_devHandle, usb_interface);

    Debug::debug()->trace("usb_close(%p)", m_devHandle);
    usb_close(m_devHandle);
    m_devHandle = NULL;
}

/* -------------------------------------------------------------------------- */
void UsbprogUpdater::startDevice()
    throw (IOError)
{
    if (!m_devHandle)
        throw IOError("Device not opened");

    char buf[USB_PAGESIZE];
    memset(buf, 0, USB_PAGESIZE);

    Debug::debug()->dbg("Starting device");

    buf[0] = STARTAPP;
    Debug::debug()->trace("usb_bulk_write(%p, 2, %p, %d, 100)",
            m_devHandle, buf, USB_PAGESIZE);
    int ret = usb_bulk_write(m_devHandle, 2, buf, USB_PAGESIZE, 100);
    if (ret < 0)
        throw IOError("Error in bulk write: " + std::string(usb_strerror()));
}

/* }}} */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
