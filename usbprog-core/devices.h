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
 * @file devices.h
 * @brief USBprog device functions
 *
 * This is the core of usbprog-core: Classes to detect devices and to upload firmware files.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
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

/**
 * @brief Represents an update device
 *
 * Data class that represents a USB device that may be used to upload firmware files.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class UpdateDevice
{
    public:
        /// Vendor ID which means "invalid vendor".
        static const uint16_t VENDOR_INVALID = 0xffff;

        /// Product ID which means "invalid product"
        static const uint16_t PRODUCT_INVALID = 0xffff;

        /// BCD Device ID which means "invalid device".
        static const uint16_t BCDDEVICE_INVALID = 0xffff;

    public:
        /**
         * @brief Constructor
         *
         * Creates a new update device. The vendor ID is set to UpdateDevice::VENDOR_INVALID,
         * the product ID is set to UpdateDevice::PRODUCT_INVALID and the bcd device ID is set
         * to UpdateDevice::BCDDEVICE_INVALID.
         *
         * @param[in] name the name of the device (which can be empty)
         */
        UpdateDevice(const std::string &name = "");

    public:
        /**
         * @brief Returns the name
         *
         * @return the name
         */
        std::string getName() const;

        /**
         * @brief Set the name
         *
         * @param[in] name the new name
         */
        void setName(const std::string &name);

        /**
         * @brief Returns the label
         *
         * @return the label
         */
        std::string getLabel() const;

        /**
         * @brief Sets the label
         *
         * @param[in] label the new label
         */
        void setLabel(const std::string &label);

        /**
         * @brief Returns the vendor ID
         *
         * @return the vendor ID
         */
        uint16_t getVendor() const;

        /**
         * @brief Set the vendor ID
         *
         * @param[in] vendor the new vendor ID
         */
        void setVendor(uint16_t vendor);

        /**
         * @brief Returns the product ID
         *
         * @return the product ID
         */
        uint16_t getProduct() const;

        /**
         * @brief Sets the product ID
         *
         * @param[in] product the product ID
         */
        void setProduct(uint16_t product);

        /**
         * @brief Returns the device ID
         *
         * @return the device ID as BCD
         */
        uint16_t getBcdDevice() const;

        /**
         * @brief Set the device ID
         *
         * @param[in] bcddevice the device ID as BCD
         */
        void setBcdDevice(uint16_t bcddevice);

        /**
         * @brief Checks if the update device values are valid
         *
         * A device is valid when getVendorId() is not UpdateDevice::VENDOR_INVALID,
         * getProductId() is not UpdateDevice::PRODUCT_INVALID and UpdateDevice::getBcdDevice()
         * is not UpdateDevice::BCDDEVICE_INVALID.
         *
         * @return @c true if the update device is valid, @c false otherwise.
         */
        bool isValid() const;

        /**
         * @brief Formats the device IDs
         *
         * @return a readable string for the device IDs
         */
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

/**
 * @brief USB Device
 *
 * Represents a real USB device (in hardware!).
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class Device
{
    public:
        /**
         * @brief Constructor
         *
         * Creates a new Device object.
         *
         * @param[in] handle the USB handle
         */
        Device(class usb::Device *handle);

        /**
         * @brief Destructor
         */
        virtual ~Device() {}

    public:
        /**
         * @brief Returns the vendor ID
         *
         * @return the vendor ID
         */
        uint16_t getVendor() const;

        /**
         * @brief Returns the product ID
         *
         * @return the product ID
         */
        uint16_t getProduct() const;

        /**
         * @brief Returns the device number
         *
         * @return the device number
         * @see getBusNumber()
         */
        unsigned short getDeviceNumber() const;

        /**
         * @brief Returns the bus number
         *
         * @return the bus number
         * @see getDeviceNumber()
         */
        unsigned short getBusNumber() const;

        /**
         * @brief Creates a string representation of the device
         *
         * @return the string representation
         * @see toShortString()
         */
        std::string toString() const;

        /**
         * @brief Creates a short string representation of the device
         *
         * @return the string representation
         * @see toString()
         */
        std::string toShortString() const;

        /**
         * @brief Checks if the device is in update mode
         *
         * @return @c true if the device is in update mode, @c false otherwise
         */
        bool isUpdateMode() const;

        /**
         * @brief Sets if the device is in update mode or not
         *
         * @param[in] updateMode @c true if the device is in update mode, @c false otherwise
         */
        void setUpdateMode(bool updateMode);

        /**
         * @brief Returns the device name
         *
         * @return the name of the device
         */
        std::string getName() const;

        /**
         * @brief Sets the name of the device
         *
         * @param[in] name the name of the device
         */
        void setName(const std::string &name);

        /**
         * @brief Returns the short name of the device
         *
         * @return the short name
         */
        std::string getShortName() const;

        /**
         * @brief Sets the short name of the device
         *
         * @param[in] shortName the short name
         */
        void setShortName(const std::string &shortName);

        /**
         * @brief Returns the device handle
         *
         * @return a pointer to the device handle. The pointer is still owned by the
         *         Device object and must not be freed outside.
         */
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

/**
 * @brief Check if two Device objects are equal
 *
 * The devices are compared according following criteria:
 *
 *  - the bus number (Device::getBusNumber())
 *  - the device number (Device::getDeviceNumber())
 *  - the vendor ID (Device::getVendor())
 *  - the product ID (Device::getProduct())
 *
 * @param[in] a the first device to compare
 * @param[in] b the second device to compare
 * @return @c true if the devices are equal, @c false otherwise
 * @ingroup core
 */
bool operator==(const Device &a, const Device &b);

/**
 * @brief Check if two Device objects are not equal
 *
 * The devices are compared according following criteria:
 *
 *  - the bus number (Device::getBusNumber())
 *  - the device number (Device::getDeviceNumber())
 *  - the vendor ID (Device::getVendor())
 *  - the product ID (Device::getProduct())
 *
 * @param[in] a the first device to compare
 * @param[in] b the second device to compare
 * @return @c true if the devices are not equal, @c false otherwise
 * @ingroup core
 */
bool operator!=(const Device &a, const Device &b);

/**
 * @brief Compares two device vectors
 *
 * Two device vectors are equal if they have the same length and every element in the
 * first vector is equal to the corresponding element in the second vector. That means
 * that also the order must match.
 *
 * @param[in] a the first device vector
 * @param[in] b the second device vector
 * @return @c true if the device vectors are equal, @c false otherwise
 * @ingroup core
 */
bool operator==(const DeviceVector &a, const DeviceVector &b);

/* }}} */
/* DeviceManager {{{ */

/**
 * @brief Manager of all USB devices that should be used by USBprog
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class DeviceManager {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new DeviceVector.
         */
        DeviceManager();

        /**
         * @brief Constructor that sets the debug flag
         *
         * @param[in] debuggingEnabled @c true if USB debugging should be enabled, @c false otherwise (which is
         *            the default)
         * @see setUsbDebugging()
         */
        DeviceManager(bool debuggingEnabled);

        /**
         * @brief Destructor
         */
        virtual ~DeviceManager();

    public:
        /**
         * @brief Sets a custom sleeper
         *
         * The default sleeper implementation is an instance of BlockingSleeper.
         *
         * @param[in] sleeper the new sleeper object that gets freed by the DeviceManager
         */
        void setCustomSleeper(Sleeper *sleeper);

        /**
         * @brief Enables or diables USB debugging
         *
         * @param[in] enabled @c true if USB debugging should be enabled, @c false otherwise
         *            (which is the default)
         */
        void setUsbDebugging(bool enabled);

        /**
         * @brief Discover update devices
         *
         * @param[in] updateDevices a vector with devices that should be treated as update devices
         *            in addition to USBprog in base mode. That vector is normally retrieved from the
         *            firmware pool. This is needed to have a weak coupling between the FirmwarePool
         *            (that is not in the core) and the DeviceManager (that is in the core).
         * @throw IOError on any I/O error when communicating with USB device(s)
         */
        void discoverUpdateDevices(const std::vector<UpdateDevice> &updateDevices =  std::vector<UpdateDevice>())
        throw (IOError);

        /**
         * @brief Prints the list of devices
         *
         * @param[in,out] os the stream to which the device list should be printed
         * @param[in] showActive @c true if the active device should be marked with a star
         */
        void printDevices(std::ostream &os, bool showActive=true) const;

        /**
         * @brief Switch to update mode
         *
         * Switches the current update device to update mode.
         *
         * @throw IOError on any I/O error when communicating with USB device(s)
         * @see setCurrentUpdateDevice()
         */
        void switchUpdateMode()
        throw (IOError);

        /**
         * @brief Returns the number of update devices
         *
         * It's necessary to call discoverUpdateDevices() before.
         *
         * @return the number of update devices
         */
        size_t getNumberUpdateDevices() const;

        /**
         * @brief Returns the update device number @p number
         *
         * @param[in] number the number which must be between 0 (inclusive) and
         *            getNumberUpdateDevices() (exclusive)
         * @return a pointer to the device object that is still owned by the DeviceManager and only
         *         valid until the DeviceManager is valid and until discoverUpdateDevices() is called the
         *         next time
         */
        Device *getDevice(size_t number) const;

        /**
         * @brief Returns the current update device
         *
         * @return a pointer to the device object that is still owned by the DeviceManager and only
         *         valid until the DeviceManager is valid and until discoverUpdateDevices() is called the
         *         next time
         * @see setCurrentUpdateDevice()
         */
        Device *getCurrentUpdateDevice() const;

        /**
         * @brief Sets the current update device to @p number
         *
         * @param[in] number the number which must be between 0 (inclusive) and
         *            getNumberUpdateDevices() (exclusive)
         */
        void setCurrentUpdateDevice(int number);

        /**
         * @brief Rests the current update device
         */
        void clearCurrentUpdateDevice();

    protected:
        /**
         * @brief Init function
         *
         * This function exists because C++ in the current release cannot call another constructor
         * from a constructor.
         *
         * @param[in] debuggingEnabled @c true if USB debugging should be enabled, @c false otherwise (which is
         *            the default)
         */
        void init(bool debuggingEnabled = false);

    private:
        DeviceVector m_updateDevices;
        int m_currentUpdateDevice;
        Sleeper *m_sleeper;
};

/* }}} */
/* UsbprogUpdater {{{ */

/**
 * @brief Updater for USBprog devices
 *
 * Class that implements the firmware update functionality for USBprog devices.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class UsbprogUpdater {
    public:
        /**
         * @brief Constructor
         *
         * Creates a updater for @p dev.
         *
         * @param[in] dev the device which should be updated. The device pointer is still owned by the
         *            caller and must be valid until the whole life time of UsbprogUpdater.
         */
        UsbprogUpdater(Device *dev);

        /**
         * @brief Destructor
         */
        virtual ~UsbprogUpdater();

    public:
        /**
         * @brief Sets a progress notifier
         *
         * @param[in] notifier an implementation of the ProgressNotifier interface. The pointer is still
         *            owned by the caller but must be valid during the whole life time of UsbprogUpdater.
         */
        void setProgress(ProgressNotifier *notifier);

        /**
         * @brief Opens the update device for updating
         *
         * @exception IOError on any error when communicating with the USBprog device.
         */
        void updateOpen()
        throw (IOError);

        /**
         * @brief Writes the firmware to the device.
         *
         * It's necessary that updateOpen() has been called before.
         *
         * @param[in] bv the firmware bytes
         * @exception IOError on any error when communicating with the USBprog device.
         */
        void writeFirmware(const ByteVector &bv)
        throw (IOError);

        /**
         * @brief Starts the firmware of the device.
         *
         * It's necessary that updateOpen() has been called before.
         *
         * @exception IOError on any error when communicating with the USBprog device.
         */
        void startDevice()
        throw (IOError);

        /**
         * @brief Resets the device.
         *
         * @exception IOError on any error when communicating with the USBprog device.
         */
        void resetDevice()
        throw (IOError);

        /**
         * @brief Closes the update device.
         *
         * This is also done by the destructor automatically.
         *
         * @exception IOError if the device is already closed
         */
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
