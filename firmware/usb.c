/**
 * Controller Converter for Nintendo Switch
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2023 T.T
 */

#include "RP2040Version.h"
#include "usb.h"

static const uint8_t report_descriptor[] =
{
	0x05, 0x01,			//	Usage Page (Generic Desktop Ctrls)
	0x09, 0x05,			//	Usage (Game Pad)
	0xA1, 0x01,			//	Collection (Application)

	0x15, 0x00,			//		Logical Minimum (0)
	0x25, 0x01,			//		Logical Maximum (1)
	0x35, 0x00,			//		Physical Minimum (0)
	0x45, 0x01,			//		Physical Maximum (1)
	0x75, 0x01,			//		Report Size (1)
	0x95, 0x10,			//		Report Count (16)
	0x05, 0x09,			//		Usage Page (Button)
	0x19, 0x01,			//		Usage Minimum (0x01)
	0x29, 0x10,			//		Usage Maximum (0x10)
	0x81, 0x02,			//		Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)

	0x05, 0x01,			//		Usage Page (Generic Desktop Ctrls)
	0x25, 0x07,			//		Logical Maximum (7)
	0x46, 0x3B, 0x01,	//		Physical Maximum (315)
	0x75, 0x04,			//		Report Size (4)
	0x95, 0x01,			//		Report Count (1)
	0x65, 0x14,			//		Unit (System: English Rotation, Length: Centimeter)
	0x09, 0x39,			//		Usage (Hat switch)
	0x81, 0x42,			//		Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)

	0x65, 0x00,			//		Unit (None)
	0x95, 0x01,			//		Report Count (1)
	0x81, 0x01,			//		Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

	0x26, 0xFF, 0x00,	//		Logical Maximum (255)
	0x46, 0xFF, 0x00,	//		Physical Maximum (255)
	0x09, 0x30,			//		Usage (X)
	0x09, 0x31,			//		Usage (Y)
	0x09, 0x32,			//		Usage (Z)
	0x09, 0x35,			//		Usage (Rz)
	0x75, 0x08,			//		Report Size (8)
	0x95, 0x04,			//		Report Count (4)
	0x81, 0x02,			//		Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)

	0x06, 0x00, 0xFF,	//		Usage Page (Vendor Defined 0xFF00)
	0x09, 0x20,			//		Usage (0x20)
	0x95, 0x01,			//		Report Count (1)
	0x81, 0x02,			//		Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)

	0x0A, 0x21, 0x26,	//		Usage (0x2621)
	0x95, 0x08,			//		Report Count (8)
	0x91, 0x02,			//		Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

	0xC0,				//	End Collection
};

static const uint8_t device_descriptor[] =
{
	18,											// bLength
	TUSB_DESC_DEVICE,							// bDescriptorType
	U16_TO_U8S_LE(0x0200),						// bcdUSB
	TUSB_CLASS_UNSPECIFIED,						// bDeviceClass
	0x00,										// bDeviceSubClass
	0x00,										// bDeviceProtocol
	CFG_TUD_ENDPOINT0_SIZE,						// bMaxPacketSize0
	U16_TO_U8S_LE(0x0F0D),						// idVendor
	U16_TO_U8S_LE(0x0092),						// idProduct
	U16_TO_U8S_LE(0x0100),						// bcdDevice
	1,											// iManufacturer
	2,											// iProduct
	0,											// iSerialNumber
	1,											// bNumConfigurations
};

static const uint8_t configuration_descriptor[] =
{
	9,											// bLength
	TUSB_DESC_CONFIGURATION,					// bDescriptorType
	U16_TO_U8S_LE(0x0029),						// wTotalLength
	1,											// bNumInterfaces
	1,											// bConfigurationValue
	0,											// iConfiguration
	0x80,										// bmAttributes
	500 >> 1,									// bMaxPower

	9,											// bLength
	TUSB_DESC_INTERFACE,						// bDescriptorType
	0,											// bInterfaceNumber
	0,											// bAlternateSetting
	2,											// bNumEndpoints
	TUSB_CLASS_HID,								// bInterfaceClass
	HID_SUBCLASS_NONE,							// bInterfaceSubClass
	HID_ITF_PROTOCOL_NONE,						// bInterfaceProtocol
	0,											// iInterface

	9,											// bLength
	HID_DESC_TYPE_HID,							// bDescriptorType
	U16_TO_U8S_LE(0x0111),						// bcdHID
	0,											// bCountryCode
	1,											// bNumDescriptors
	HID_DESC_TYPE_REPORT,						// bDescriptorType[0]
	U16_TO_U8S_LE(sizeof(report_descriptor)),	// wDescriptorLength[0]

	7,											// bLength
	TUSB_DESC_ENDPOINT,							// bDescriptorType
	0x02,										// bEndpointAddress
	TUSB_XFER_INTERRUPT,						// bmAttributes
	U16_TO_U8S_LE(CFG_TUD_HID_EP_BUFSIZE),		// wMaxPacketSize
	1,											// bInterval

	7,											// bLength
	TUSB_DESC_ENDPOINT,							// bDescriptorType
	0x81,										// bEndpointAddress
	TUSB_XFER_INTERRUPT,						// bmAttributes
	U16_TO_U8S_LE(CFG_TUD_HID_EP_BUFSIZE),		// wMaxPacketSize
	1,											// bInterval
};

static const uint16_t string_language[] =
{
	TUSB_DESC_STRING << 8 | 4,
	0x0409
};

static const uint16_t string_manufacturer[] =
{
	TUSB_DESC_STRING << 8 | sizeof("HORI CO.,LTD.") << 1,
	'H','O','R','I',' ','C','O','.',',','L','T','D','.'
};

static const uint16_t string_product[] =
{
	TUSB_DESC_STRING << 8 | sizeof("POKKEN CONTROLLER") << 1,
	'P','O','K','K','E','N',' ','C','O','N','T','R','O','L','L','E','R'
};

static const uint16_t* const string_descriptors[] =
{
	string_language,
	string_manufacturer,
	string_product,
};

static report_t sendBuffer;

uint8_t const * tud_descriptor_device_cb(void)
{
	return device_descriptor;
}

uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
	return configuration_descriptor;
}

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
	if (index < TU_ARRAY_SIZE(string_descriptors))
	{
		return string_descriptors[index];
	}
	else
	{
		return NULL;
	}
}

uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
{
	return report_descriptor;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
	if ((report_id != 0) || (report_type != HID_REPORT_TYPE_INPUT))
	{
		return 0;
	}

	memcpy(buffer, &sendBuffer, sizeof(report_t));
	return sizeof(report_t);
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
}

void send_report(const report_t* report)
{
	// Wait for previous transmission to complete.
	while (tud_ready() && !tud_hid_ready())
	{
		tud_task();
	}

	memcpy(&sendBuffer, report, sizeof(report_t));
	if (tud_hid_ready())
	{
		tud_hid_report(0, &sendBuffer, sizeof(report_t));
	}
}

#if ((ARDUINO_PICO_MAJOR << 16) | (ARDUINO_PICO_MINOR << 8) | ARDUINO_PICO_REVISION) >= 0x020605

int32_t tud_msc_read10_cb (uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
	return -1;
}

int32_t tud_msc_write10_cb (uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize)
{
	return -1;
}

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4])
{
}

bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
	return false;
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size)
{
}

int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize)
{
	return -1;
}

#endif
