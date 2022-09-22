#include "lpc_app_usbd_cfg.h"

//HID Keyboard Report Descriptor
const uint8_t Keyboard_ReportDescriptor[] = {
	// INSPIRARSE DEL DESCRIPTOR DE REPORTE DE MOUSE
	// CONSTANTES Y OTROS VALORES ESTAN EN EL ARCHIVO "usbd_hid.h"
	HID_UsagePage(HID_USAGE_PAGE_GENERIC),
	HID_Usage(HID_USAGE_PAGE_KEYBOARD),
	HID_Collection(HID_Application),
	HID_UsagePage(HID_USAGE_PAGE_KEYBOARD),
	HID_UsageMin(224),
	HID_UsageMax(231),
	HID_LogicalMin(0),
	HID_LogicalMax(1),
	HID_ReportSize(1),
	HID_ReportCount(8),
	HID_Input(HID_Data | HID_Variable | HID_Absolute),
	HID_ReportCount(1),
	HID_ReportSize(8),
	HID_Input(HID_Constant),
	HID_ReportCount(5),
	HID_ReportSize(1),
	HID_UsagePage(HID_USAGE_PAGE_LED),
	HID_UsageMin(1),
	HID_UsageMax(5),
	HID_Output(HID_Data | HID_Variable | HID_Absolute),
	HID_ReportCount(1),
	HID_ReportSize(3),
	HID_Output(HID_Constant),
	HID_ReportCount(6),
	HID_ReportSize(8),
	HID_LogicalMin(0),
	HID_LogicalMax(101),
	HID_UsagePage(HID_USAGE_PAGE_KEYBOARD),
	HID_UsageMin(0),
	HID_UsageMax(101),
	HID_Input(HID_Array),
	HID_EndCollection,
};
const uint16_t Keyboard_ReportDescSize = sizeof(Keyboard_ReportDescriptor);

//USB Standard Device Descriptor
ALIGNED(4) const uint8_t USB_DeviceDescriptor[] = {
	USB_DEVICE_DESC_SIZE,			/* bLength */
	USB_DEVICE_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0200),					/* bcdUSB: 2.00 */
	0x00,							/* bDeviceClass */
	0x00,							/* bDeviceSubClass */
	0x00,							/* bDeviceProtocol */
	USB_MAX_PACKET0,				/* bMaxPacketSize0 */
	WBVAL(0x1FC9),					/* idVendor */
	WBVAL(0x0086),					/* idProduct */
	WBVAL(0x0100),					/* bcdDevice */
	0x01,							/* iManufacturer */
	0x02,							/* iProduct */
	0x03,							/* iSerialNumber */
	0x01							/* bNumConfigurations */
};

ALIGNED(4) const uint8_t USB_DeviceQualifier[] = {
	USB_DEVICE_QUALI_SIZE,					/* bLength */
	USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE,	/* bDescriptorType */
	WBVAL(0x0200),							/* bcdUSB: 2.00 */
	0x00,									/* bDeviceClass */
	0x00,									/* bDeviceSubClass */
	0x00,									/* bDeviceProtocol */
	USB_MAX_PACKET0,						/* bMaxPacketSize0 */
	0x01,									/* bNumOtherSpeedConfigurations */
	0x00									/* bReserved */
};

//USB HSConfiguration Descriptor
//All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
ALIGNED(4) uint8_t USB_HsConfigDescriptor[] = {
	/* Configuration 1 */
	USB_CONFIGURATION_DESC_SIZE,			/* bLength */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(									/* wTotalLength */
		USB_CONFIGURATION_DESC_SIZE +
		USB_INTERFACE_DESC_SIZE     +
		HID_DESC_SIZE               +
		USB_ENDPOINT_DESC_SIZE
		),
	0x01,							/* bNumInterfaces */
	0x01,							/* bConfigurationValue */
	0x00,							/* iConfiguration */
	USB_CONFIG_SELF_POWERED,		/* bmAttributes */
	USB_CONFIG_POWER_MA(2),			/* bMaxPower */

	/* Interface 0, Alternate Setting 0, HID Class */
	USB_INTERFACE_DESC_SIZE,		/* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,	/* bDescriptorType */
	0x00,							/* bInterfaceNumber */
	0x00,							/* bAlternateSetting */
	0x01,							/* bNumEndpoints */
	USB_DEVICE_CLASS_HUMAN_INTERFACE,	/* bInterfaceClass */
	HID_SUBCLASS_BOOT,				/* bInterfaceSubClass */
	HID_PROTOCOL_KEYBOARD,			/* bInterfaceProtocol */
	0x04,							/* iInterface */
	/* HID Class Descriptor */
	/* HID_DESC_OFFSET = 0x0012 */
	HID_DESC_SIZE,					/* bLength */
	HID_HID_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0111),					/* bcdHID : 1.11*/
	0x00,							/* bCountryCode */
	0x01,							/* bNumDescriptors */
	HID_REPORT_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(sizeof(Keyboard_ReportDescriptor)),	/* wDescriptorLength */
	/* Endpoint, HID Interrupt In */
	USB_ENDPOINT_DESC_SIZE,			/* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType */
	HID_EP_IN,						/* bEndpointAddress */
	USB_ENDPOINT_TYPE_INTERRUPT,	/* bmAttributes */
	WBVAL(0x0008),					/* wMaxPacketSize */
	0x08,							/* bInterval */
	/* Terminator */
	0								/* bLength */
};

//USB FSConfiguration Descriptor
//All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
ALIGNED(4) uint8_t USB_FsConfigDescriptor[] = {
	/* Configuration 1 */
	USB_CONFIGURATION_DESC_SIZE,			/* bLength */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(									/* wTotalLength */
		USB_CONFIGURATION_DESC_SIZE   +
		USB_INTERFACE_DESC_SIZE     +
		HID_DESC_SIZE               +
		USB_ENDPOINT_DESC_SIZE
		),
	0x01,							/* bNumInterfaces */
	0x01,							/* bConfigurationValue */
	0x00,							/* iConfiguration */
	USB_CONFIG_SELF_POWERED,		/* bmAttributes */
	USB_CONFIG_POWER_MA(2),			/* bMaxPower */

	/* Interface 0, Alternate Setting 0, HID Class */
	USB_INTERFACE_DESC_SIZE,		/* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,	/* bDescriptorType */
	0x00,							/* bInterfaceNumber */
	0x00,							/* bAlternateSetting */
	0x01,							/* bNumEndpoints */
	USB_DEVICE_CLASS_HUMAN_INTERFACE,	/* bInterfaceClass */
	HID_SUBCLASS_BOOT,				/* bInterfaceSubClass */
	HID_PROTOCOL_KEYBOARD,			/* bInterfaceProtocol */
	0x04,							/* iInterface */
	/* HID Class Descriptor */
	/* HID_DESC_OFFSET = 0x0012 */
	HID_DESC_SIZE,					/* bLength */
	HID_HID_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0111),					/* bcdHID : 1.11*/
	0x00,							/* bCountryCode */
	0x01,							/* bNumDescriptors */
	HID_REPORT_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(sizeof(Keyboard_ReportDescriptor)),	/* wDescriptorLength */
	/* Endpoint, HID Interrupt In */
	USB_ENDPOINT_DESC_SIZE,			/* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType */
	HID_EP_IN,						/* bEndpointAddress */
	USB_ENDPOINT_TYPE_INTERRUPT,	/* bmAttributes */
	WBVAL(0x0008),					/* wMaxPacketSize */
	0x0A,										/* bInterval */
	/* Terminator */
	0								/* bLength */
};

//USB String Descriptor (optional)
const uint8_t USB_StringDescriptor[] = {
	// INSPIRARSE DEL DESCRIPTOR DE STRING DE MOUSE
	0x04,							/* bLength */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0409),					/* wLANGID  0x0409 = US English*/
	/* Index 0x01: Manufacturer */
	(13 * 2 + 2),					/* bLength (13 Char + Type + length) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'T', 0,
	'D', 0,
	' ', 0,
	'2', 0,
	'0', 0,
	'2', 0,
	'1', 0,
	' ', 0,
	'-', 0,
	' ', 0,
	'U', 0,
	'N', 0,
	'T', 0,
	/* Index 0x02: Product */
	(16 * 2 + 2),					/* bLength (16 Char + Type + length) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'E', 0,
	'D', 0,
	'U', 0,
	'C', 0,
	'I', 0,
	'A', 0,
	'A', 0,
	' ', 0,
	'K', 0,
	'E', 0,
	'Y', 0,
	'B', 0,
	'O', 0,
	'A', 0,
	'R', 0,
	'D', 0,
	/* Index 0x03: Serial Number */
	(11 * 2 + 2),					/* bLength (13 Char + Type + length) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'G', 0,
	'B', 0,
	'E', 0,
	'R', 0,
	'A', 0,
	'R', 0,
	'D', 0,
	'U', 0,
	'C', 0,
	'C', 0,
	'I', 0,
	/* Index 0x04: Interface 0, Alternate Setting 0 */
	(11 * 2 + 2),					/* bLength (12 Char + Type + length) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'H', 0,
	'I', 0,
	'D', 0,
	' ', 0,
	'K', 0,
	'E', 0,
	'Y', 0,
	'B', 0,
	'O', 0,
	'A', 0,
	'R', 0,
	'D', 0,	
};
