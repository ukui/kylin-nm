/*
 * Fuzz test for AppSettings
 * This file implements LLVMFuzzerTestOneInput_AppSettings
 *
 * Tests the following from pub/appsettings.h:
 * - Singleton pattern initialization
 * - Default wired/wireless device settings
 * - Firewall configuration settings
 * - Tray show settings
 * - USB hidden device pairs management
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdint>

// Avoid glib/Qt macro conflict: undef signals before including glib headers
#ifdef signals
#undef signals
#endif

// Include the appsettings header
#include "appsettings.h"

// Fuzz test for AppSettings functions
extern "C" int LLVMFuzzerTestOneInput_AppSettings(const uint8_t *data, size_t size)
{
    if (size == 0) {
        return 0;
    }

    // Data format:
    // First byte: operation type (0-9)
    // Second byte: length modifier
    // Remaining bytes: data

    uint8_t opType = data[0] % 10;
    uint8_t lenMod = size > 1 ? data[1] % 128 : 0;

    // Get singleton instance
    AppSettings &settings = AppSettings::instance();

    switch (opType) {
        case 0: {
            // Test with device name data for wired
            if (size > 2) {
                size_t nameLen = std::min(static_cast<size_t>(lenMod), size - 2);
                QString deviceName = QString::fromLatin1(reinterpret_cast<const char*>(data + 2), static_cast<int>(nameLen));
                settings.setDefaultWired(deviceName);
                QString result = settings.defaultWired();
            }
            break;
        }
        case 1: {
            // Test with device name data for wireless
            if (size > 2) {
                size_t nameLen = std::min(static_cast<size_t>(lenMod), size - 2);
                QString deviceName = QString::fromLatin1(reinterpret_cast<const char*>(data + 2), static_cast<int>(nameLen));
                settings.setDefaultWireless(deviceName);
                QString result = settings.defaultWireless();
            }
            break;
        }
        case 2: {
            // Test auto firewall permitted setting
            bool value = (data[1] % 2 == 0);
            settings.setAutoFirewallPermitted(value);
            bool result = settings.autoFirewallPermitted();
            break;
        }
        case 3: {
            // Test firewall permission show setting
            bool value = (data[1] % 2 == 0);
            settings.setFirewallPermissionShow(value);
            bool result = settings.firewallPermissionShow();
            break;
        }
        case 4: {
            // Test tray show setting
            bool value = (data[1] % 2 == 0);
            settings.setTrayShow(value);
            bool result = settings.trayShow();
            break;
        }
        case 5: {
            // Test USB hidden pairs append
            if (size > 2) {
                size_t dataLen = std::min(static_cast<size_t>(lenMod), size - 2);
                QStringList pairs;
                
                // Parse data as USB pairs (format: "xxxx:xxxx")
                for (size_t i = 0; i < dataLen && i + 9 <= dataLen; i += 10) {
                    QString pair = QString::fromLatin1(reinterpret_cast<const char*>(data + 2 + i), 9);
                    pairs.append(pair);
                }
                
                settings.appendHiddenUsbPairs(pairs);
                QStringList result = settings.getHiddenUsbPairs();
            }
            break;
        }
        case 6: {
            // Test USB hidden pairs get
            QStringList result = settings.getHiddenUsbPairs();
            break;
        }
        case 7: {
            // Test USB hidden pairs clear
            settings.clearHiddenUsbPairs();
            QStringList result = settings.getHiddenUsbPairs();
            break;
        }
        case 8: {
            // Test with various string lengths
            for (size_t i = 0; i < size && i < 16; ++i) {
                QString testStr = QString::fromLatin1(reinterpret_cast<const char*>(data), static_cast<int>(i));
                settings.setDefaultWired(testStr);
                settings.setDefaultWireless(testStr);
            }
            break;
        }
        case 9: {
            // Test boundary cases - empty and large data
            settings.setDefaultWired("");
            settings.setDefaultWireless("");
            settings.clearHiddenUsbPairs();
            
            // Test with large data
            if (size > 2) {
                size_t largeLen = std::min(static_cast<size_t>(lenMod), size - 2);
                QString largeStr = QString::fromLatin1(reinterpret_cast<const char*>(data + 2), static_cast<int>(largeLen));
                settings.setDefaultWired(largeStr);
                settings.setDefaultWireless(largeStr);
            }
            break;
        }
    }

    // Test with empty and boundary cases
    QByteArray emptyData;
    QByteArray singleByte(reinterpret_cast<const char*>(data), 1);
    QByteArray largeData(reinterpret_cast<const char*>(data),
                         static_cast<int>(std::min(size, static_cast<size_t>(4096))));

    return 0;
}