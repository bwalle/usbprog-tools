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
#ifndef DEVICES_H
#define DEVICES_H

#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>

#include <usbpp/usbpp.h>

#include <usbprog-core/types.h>
#include <usbprog-core/error.h>
#include <usbprog-core/progressnotifier.h>
#include <usbprog-core/sleeper.h>

namespace usbprog {
namespace core {

/* UpdateDevice {{{ */

class UpdateDevice
{
    public:
        static uint16_t VENDOR_INVALID;
        static uint16_t PRODUCT_INVALID;
        static uint16_t BCDDEVICE_INVALID;

    public:
        UpdateDevice(const std::string &name = "");

    public:
        std::string getName() const;
        void setName(const std::string &name);

        std::string getLabel() const;
        void setLabel(const std::string &label);

        uint16_t getVendor() const;
        void setVendor(uint16_t vendor);

        uint16_t getProduct() const;
        void setProduct(uint16_t product);

        uint16_t getBcdDevice() const;
        void setBcdDevice(uint16_t bcddevice);

        bool isValid() const;

        std::string formatDeviceId() const;

    private:
        std::string m_name;
        std::string m_label;
        uint16_t m_vendor;
        uint16_t m_product;
        uint16_t m_bcddevice;
};

/* }}} */
/* Device {{{ */

class Device
{
    public:
        Device(class usb::Device *handle);
        virtual ~Device() {}

    public:
        uint16_t getVendor() const;
        uint16_t getProduct() const;
        unsigned short getDeviceNumber() const;
        unsigned short getBusNumber() const;
        std::string toString() const;
        std::string toShortString() const;

        bool isUpdateMode() const;
        void setUpdateMode(bool updateMode);

        void setName(const std::string &name);
        std::string getName() const;

        void setShortName(const std::string &shortName);
        std::string getShortName() const;

        usb::Device *getHandle() const;

    private:
        usb::Device *m_handle;
        bool m_updateMode;
        std::string m_name;
        std::string m_shortName;
        uint16_t m_vendorId;
        uint16_t m_productId;
        unsigned short m_deviceNumber;
        unsigned short m_busNumber;
};

bool operator==(const Device &a, const Device &b);
bool operator!=(const Device &a, const Device &b);
bool operator==(const DeviceVector &a, const DeviceVector &b);

/* }}} */
/* DeviceManager {{{ */

class DeviceManager {
    public:
        DeviceManager();
        DeviceManager(bool debuggingEnabled);
        virtual ~DeviceManager();

    public:
        void setCustomSleeper(Sleeper *sleeper);
        void setUsbDebugging(bool enabled);
        void discoverUpdateDevices(const std::vector<UpdateDevice> &updateDevices =  std::vector<UpdateDevice>())
        throw (IOError);
        void printDevices(std::ostream &os, bool showActive=true) const;
        void switchUpdateMode()
        throw (IOError);

        size_t getNumberUpdateDevices() const;
        Device *getDevice(size_t number) const;

        Device *getCurrentUpdateDevice() const;
        void setCurrentUpdateDevice(ssize_t number);
        void clearCurrentUpdateDevice();

    protected:
        void init(bool debuggingEnabled = false);

    private:
        DeviceVector m_updateDevices;
        ssize_t m_currentUpdateDevice;
        Sleeper *m_sleeper;
};

/* }}} */
/* UsbprogUpdater {{{ */

class UsbprogUpdater {
    public:
        UsbprogUpdater(Device *dev);
        virtual ~UsbprogUpdater();

    public:
        void setProgress(ProgressNotifier *notifier);
        void updateOpen()
            throw (IOError);
        void writeFirmware(const ByteVector &bv)
            throw (IOError);
        void startDevice()
            throw (IOError);
        void resetDevice()
            throw (IOError);
        void updateClose()
            throw (IOError);

    private:
        Device              *m_dev;
        ProgressNotifier    *m_progressNotifier;
        usb::DeviceHandle   *m_devHandle;
};

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* DEVICES_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
