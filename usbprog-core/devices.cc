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
#include <memory>

#include <usbpp/usbpp.h>

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
Device::Device(USB::Device *handle)
    : m_handle(handle)
    , m_updateMode(false)
    , m_vendorId(handle->getDescriptor().getVendorId())
    , m_productId(handle->getDescriptor().getProductId())
    , m_deviceNumber(handle->getDeviceNumber())
    , m_busNumber(handle->getBusNumber())
{}

/* -------------------------------------------------------------------------- */
uint16_t Device::getVendor() const
{
    return m_vendorId;
}

/* -------------------------------------------------------------------------- */
uint16_t Device::getProduct() const
{
    return m_productId;
}

/* -------------------------------------------------------------------------- */
unsigned short Device::getDeviceNumber() const
{
    return m_deviceNumber;
}

/* -------------------------------------------------------------------------- */
unsigned short Device::getBusNumber() const
{
    return m_busNumber;
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
USB::Device *Device::getHandle() const
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

    ss << "Bus " << getBusNumber() << " ";
    ss << "Device " << getDeviceNumber() << ": ";
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
    , m_sleeper(NULL)
{
    init();
}

/* -------------------------------------------------------------------------- */
DeviceManager::DeviceManager(bool debuggingEnabled)
    : m_currentUpdateDevice(-1)
    , m_sleeper(NULL)
{
    init(debuggingEnabled);
}

/* -------------------------------------------------------------------------- */
DeviceManager::~DeviceManager()
{
    for (DeviceVector::const_iterator it = m_updateDevices.begin(); it != m_updateDevices.end(); ++it)
        delete *it;
    delete m_sleeper;
}

/* -------------------------------------------------------------------------- */
void DeviceManager::init(bool debuggingEnabled)
{
    USB::UsbManager &usbManager = USB::UsbManager::instance();
    Debug::debug()->trace("USB::UsbManager::init()");
    setUsbDebugging(debuggingEnabled);
    m_sleeper = new BlockingSleeper;
}

/* -------------------------------------------------------------------------- */
void DeviceManager::setUsbDebugging(bool enabled)
{
    Debug::debug()->trace("usb_set_debug(%s)", enabled ? "true" : "false");
    USB::UsbManager::instance().setDebug(enabled);
}

// -----------------------------------------------------------------------------
void DeviceManager::setCustomSleeper(Sleeper *sleeper)
{
    delete m_sleeper;
    m_sleeper = sleeper;
}

/* -------------------------------------------------------------------------- */
void DeviceManager::discoverUpdateDevices(const std::vector<UpdateDevice> &updateDevices)
    throw (IOError)
{
    try {
        USB::UsbManager &usbManager = USB::UsbManager::instance();
        Debug::debug()->trace("USB::UsbManager::detectDevices()");
        usbManager.detectDevices();

        DeviceVector oldDevices = m_updateDevices;
        m_updateDevices.clear();

        for (size_t deviceNumber = 0; deviceNumber < usbManager.getNumberOfDevices(); ++deviceNumber) {
            USB::Device *dev = usbManager.getDevice(deviceNumber);

            uint16_t vendorid = dev->getDescriptor().getVendorId();
            uint16_t productid = dev->getDescriptor().getProductId();
            uint16_t bcddevice = dev->getDescriptor().getBcdDevice();
            Device *d = NULL;

            Debug::debug()->dbg("Found USB device [%04x:%04x:%04x]", int(vendorid), int(productid), int(bcddevice));

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

        // reset update device only when something has changed
        if (oldDevices != m_updateDevices)
            m_currentUpdateDevice = -1;

        // free memory
        for (DeviceVector::const_iterator it = oldDevices.begin(); it != oldDevices.end(); ++it)
            delete *it;
    } catch (const USB::Error &err) {
        throw IOError("USB error: " + std::string(err.what()));
    }
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
        os << "Bus "    << std::setw(3) << std::setfill('0') << dev->getBusNumber() << " "
           << "Device " << std::setw(3) << std::setfill('0') << dev->getDeviceNumber() << ": "
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


    std::auto_ptr<USB::DeviceHandle> usb_handle;
    try {
        usb_handle.reset(dev->getHandle()->open());
    } catch (const USB::Error &err) {
        throw IOError("Could not open USB device: " + std::string(err.what()));
    }

    try {
        std::auto_ptr<USB::ConfigDescriptor> configDescriptor(dev->getHandle()->getConfigDescriptor(0));
        Debug::debug()->trace("USB::DeviceHandle::setConfiguration(%d)", configDescriptor->getConfigurationValue());
        usb_handle->setConfiguration(configDescriptor->getConfigurationValue());
    } catch (const USB::Error &err) {
        throw IOError("Unable to set configuration: " + std::string(err.what()));
    }

    unsigned int interfaceNumber;
    try {
        std::auto_ptr<USB::ConfigDescriptor> configDescriptor(dev->getHandle()->getConfigDescriptor(0));
        std::auto_ptr<USB::InterfaceDescriptor> interfaceDescriptor(configDescriptor->getInterfaceDescriptor(0, 0));
        interfaceNumber = interfaceDescriptor->getInterfaceNumber();
        Debug::debug()->trace("USB::DeviceHandle::claimInterface(%d)", interfaceNumber);
        usb_handle->claimInterface(interfaceNumber);
    } catch (const USB::Error &err) {
        throw IOError("Unable to claim interface: " + std::string(err.what()));
    }

    /* needed ?*/
    try {
        Debug::debug()->trace("USB::DeviceHandle::setInterfaceAltSetting(interfaceNumber, 0)", interfaceNumber);
        usb_handle->setInterfaceAltSetting(interfaceNumber, 0);
    } catch (const USB::Error &err) {
        throw IOError("Unable to set interface alternative setting: " + std::string(err.what()));
    }


    Debug::debug()->trace("USB::DeviceHandle::controlTransfer() (multiple times)");

    int timeout = 6;
    bool successfully = false;
    while (!successfully && --timeout > 0) {
        try {
            usb_handle->controlTransfer(0xC0, 0x01, 0, 0, NULL, 8, 1000);
            successfully = true;
        } catch (const USB::Error &) {}

        m_sleeper->sleep(1);
    }

    // Calling the d'tor of USB::DeviceHandle also releases the claimed interface
    // That behaviour is needed for RAII.
    Debug::debug()->trace("Delete USB::DeviceHandle");
    usb_handle.reset(NULL);

    m_sleeper->sleep(2000);

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
    return a.getBusNumber() == b.getBusNumber() &&
        a.getDeviceNumber() == b.getDeviceNumber() &&
        a.getProduct() == b.getProduct() &&
        a.getVendor() == b.getVendor();
}

/* -------------------------------------------------------------------------- */
bool operator!=(const Device &a, const Device &b)
{
    return !(a == b);
}

/* }}} */
/* UsbprogUpdater {{{ */

#define USB_PAGESIZE 64
#define WRITEPAGE      0x02
#define STARTAPP       0x01

/* -------------------------------------------------------------------------- */
UsbprogUpdater::UsbprogUpdater(Device *dev)
    : m_dev(dev)
    , m_progressNotifier(NULL)
    , m_devHandle(NULL)
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
    unsigned char buf[USB_PAGESIZE];
    unsigned char cmd[USB_PAGESIZE];

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

        Debug::debug()->trace("USB::DeviceHandle::bulkTransfer(2, %p, %d, NULL, 100)", 2, cmd, USB_PAGESIZE);

        try {
            m_devHandle->bulkTransfer(2, cmd, USB_PAGESIZE, NULL, 100);
        } catch (const USB::Error &err) {
            updateClose();
            if (m_progressNotifier)
                m_progressNotifier->finished();
            throw IOError("Error while writing to USB device: "+
                    std::string(err.what()));
        }

        // data message
        Debug::debug()->trace("USB::DeviceHandle::bulkTransfer(2, %p, %d, NULL, 100)", buf, USB_PAGESIZE);
        try {
            m_devHandle->bulkTransfer(2, buf, USB_PAGESIZE, NULL, 100);
        } catch (const USB::Error &err) {
            updateClose();
            if (m_progressNotifier)
                m_progressNotifier->finished();
            throw IOError("Error while writing to USB device: "+
                    std::string(err.what()));
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
    USB::Device *dev = m_dev->getHandle();

    Debug::debug()->dbg("UsbprogUpdater::updateOpen()");

    if (m_devHandle)
        throw IOError("Device still opened. Close first.");

    try {
        Debug::debug()->trace("usb_open(%p)", dev);
        m_devHandle = dev->open();
    } catch (const USB::Error &err) {
        throw IOError("usb_open failed " + std::string(err.what()));
    }

    try {
        std::auto_ptr<USB::ConfigDescriptor> configDescriptor(dev->getConfigDescriptor(0));
        Debug::debug()->trace("USB::DeviceHandle::setConfiguration(%d)", configDescriptor->getConfigurationValue());
        m_devHandle->setConfiguration(configDescriptor->getConfigurationValue());
    } catch (const USB::Error &err) {
        throw IOError("Unable to set configuration: " + std::string(err.what()));
    }

    unsigned int interfaceNumber;
    try {
        std::auto_ptr<USB::ConfigDescriptor> configDescriptor(dev->getConfigDescriptor(0));
        std::auto_ptr<USB::InterfaceDescriptor> interfaceDescriptor(configDescriptor->getInterfaceDescriptor(0, 0));
        interfaceNumber = interfaceDescriptor->getInterfaceNumber();
        Debug::debug()->trace("USB::DeviceHandle::claimInterface(%d)", interfaceNumber);
        m_devHandle->claimInterface(interfaceNumber);
    } catch (const USB::Error &err) {
        throw IOError("Unable to claim interface: " + std::string(err.what()));
    }
}

/* -------------------------------------------------------------------------- */
void UsbprogUpdater::updateClose()
    throw (IOError)
{
    Debug::debug()->dbg("UsbprogUpdater::updateClose()");

    if (!m_devHandle)
        throw IOError("Device already closed");

    Debug::debug()->trace("Closing USB::Device");
    delete m_devHandle;
    m_devHandle = NULL;
}

/* -------------------------------------------------------------------------- */
void UsbprogUpdater::startDevice()
    throw (IOError)
{
    if (!m_devHandle)
        throw IOError("Device not opened");

    unsigned char buf[USB_PAGESIZE];
    std::memset(buf, 0, USB_PAGESIZE);

    Debug::debug()->dbg("Starting device");

    buf[0] = STARTAPP;
    Debug::debug()->trace("USB::DeviceHandle::bulkTransfer(2, %p, %d, NULL, 100)",
            buf, USB_PAGESIZE);
    try {
        m_devHandle->bulkTransfer(2, buf, USB_PAGESIZE, NULL, 100);
    } catch (const USB::Error &err) {
        throw IOError("Error in bulk write: " + std::string(err.what()));
    }
}

/* }}} */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
