/**
 * @author Ali Mirmohammad
 * @file usbdefs.h
 ** This file is part of Novanix.

**Novanix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**Novanix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with Novanix. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __ALINIX_KERNEL_SYSTEM_DRIVERS_USB_USB_DEFS_HPP
#define __ALINIX_KERNEL_SYSTEM_DRIVERS_USB_USB_DEFS_HPP

#include <alinix/types.h>

#define DEV_TO_HOST     0x80
#define HOST_TO_DEV     0x00
#define REQ_TYPE_STNDRD 0x00
#define REQ_TYPE_CLASS  0x20
#define REQ_TYPE_VENDOR 0x40
#define REQ_TYPE_RESV   0x60
#define RECPT_DEVICE    0x00
#define RECPT_INTERFACE 0x01
#define RECPT_ENDPOINT  0x02
#define RECPT_OTHER     0x03
#define STDRD_GET_REQUEST   (DEV_TO_HOST | REQ_TYPE_STNDRD | RECPT_DEVICE)
#define STDRD_SET_REQUEST   (HOST_TO_DEV | REQ_TYPE_STNDRD | RECPT_DEVICE)
#define STDRD_SET_INTERFACE (HOST_TO_DEV | REQ_TYPE_STNDRD | RECPT_INTERFACE)

#define USB_TDRSTR      50   // reset on a root hub
#define USB_TDRST       10   // minimum delay for a reset
#define USB_TRHRSI      3   // No more than this between resets for root hubs
#define USB_TRSTRCY     10   // reset recovery
#define PINDC_OFF       0b00
#define PINDC_AMBER     0b01
#define PINDC_GREEN     0b10
#define ENDP_CONTROL    0

struct DEVICE_DESC {
    uint8_t  len;
    uint8_t  type;
    uint16_t usb_ver;
    uint8_t  _class;
    uint8_t  subclass;
    uint8_t  protocol;
    uint8_t  max_packet_size;
    uint16_t vendorid;
    uint16_t productid;
    uint16_t device_rel;
    uint8_t  manuf_indx;   // index value
    uint8_t  prod_indx;    // index value
    uint8_t  serial_indx;  // index value
    uint8_t  configs;      // Number of configurations
} __attribute__((packed));

struct REQUEST_PACKET {
    uint8_t  request_type;
    uint8_t  request;
    uint16_t value;
    uint16_t index;
    uint16_t length;
} __attribute__((packed));

// config descriptor
struct CONFIG_DESC {
    uint8_t  len;
    uint8_t  type;
    uint16_t tot_len;
    uint8_t  num_interfaces;
    uint8_t  config_val;
    uint8_t  config_indx;
    uint8_t  bm_attrbs;
    uint8_t  max_power;
} __attribute__((packed));

struct STRING_DESC {
    uint8_t  len;         // length of whole desc in bytes
    uint8_t  type;
    uint16_t string[127];
} __attribute__((packed));

struct INTERFACE_ASSOSIATION_DESC {
    uint8_t  len;             // len of this desc (8)
    uint8_t  type;            // type = 11
    uint8_t  interface_num;   // first interface number to start association
    uint8_t  count;           // count of continuous interfaces for association
    uint8_t  _class;          //
    uint8_t  subclass;        //
    uint8_t  protocol;        //
    uint8_t  function_indx;   // string id of this association
} __attribute__((packed));

// interface descriptor
struct INTERFACE_DESC {
    uint8_t  len;
    uint8_t  type;
    uint8_t  interface_num;
    uint8_t  alt_setting;
    uint8_t  num_endpoints;
    uint8_t  interface_class;
    uint8_t  interface_sub_class;
    uint8_t  interface_protocol;
    uint8_t  interface_indx;
} __attribute__((packed));

// endpoint descriptor
struct ENDPOINT_DESC {
    uint8_t  len;
    uint8_t  type;
    uint8_t  end_point;        // 6:0 end_point number, 7 = IN (set) or OUT (clear)
    uint8_t  bm_attrbs;        // 
    uint16_t max_packet_size;  // 10:0 = max_size, 12:11 = max transactions, 15:13 = reserved
    uint8_t  interval;
} __attribute__((packed));

struct IF_HID_DESC {
    uint8_t  len;
    uint8_t  type;
    uint16_t release;
    uint8_t  countryCode;
    uint8_t  numDescriptors;
    // Types and lenght folowing below 
} __attribute__((packed));

// setup packets
#define DEV_TO_HOST     0x80
#define HOST_TO_DEV     0x00
#define REQ_TYPE_STNDRD 0x00
#define REQ_TYPE_CLASS  0x20
#define REQ_TYPE_VENDOR 0x40
#define REQ_TYPE_RESV   0x60
#define RECPT_DEVICE    0x00
#define RECPT_INTERFACE 0x01
#define RECPT_ENDPOINT  0x02
#define RECPT_OTHER     0x03
#define STDRD_GET_REQUEST   (DEV_TO_HOST | REQ_TYPE_STNDRD | RECPT_DEVICE)
#define STDRD_SET_REQUEST   (HOST_TO_DEV | REQ_TYPE_STNDRD | RECPT_DEVICE)
#define STDRD_SET_INTERFACE (HOST_TO_DEV | REQ_TYPE_STNDRD | RECPT_INTERFACE)

// device requests
enum DeviceRequest { GET_STATUS=0, CLEAR_FEATURE, SET_FEATURE=3, SET_ADDRESS=5, GET_DESCRIPTOR=6, SET_DESCRIPTOR,
        GET_CONFIGURATION, SET_CONFIGURATION,
// interface requests
        GET_INTERFACE, SET_INTERFACE,
// standard endpoint requests
        SYNCH_FRAME,
// Device specific
        GET_MAX_LUNS = 0xFE, BULK_ONLY_RESET
};

// Descriptor types
enum DescriptorTypes { 
    DEVICE=1, 
    CONFIG, 
    STRING, 
    INTERFACE, 
    ENDPOINT, 
    DEVICE_QUALIFIER,
    OTHER_SPEED_CONFIG, 
    INTERFACE_POWER, 
    OTG, 
    DEBUG, 
    INTERFACE_ASSOSIATION,
    
    HID=0x21,
    HID_REPORT, 
    HID_PHYSICAL, 
    
    INTERFACE_FUNCTION = 0x24,
    ENDPOINT_FUNCTION,
    
    HUB = 0x29
};

#define HID_REQUEST_SET_IDLE 0x0A
#define HID_REQUEST_SET_PROTOCOL 0x0B

#endif /*__ALINIX_KERNEL_SYSTEM_DRIVERS_USB_USB_DEFS_HPP*/