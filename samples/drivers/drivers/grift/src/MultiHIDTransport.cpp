#include "MultiHIDTransport.hpp"
#include "driverlog.h"
#include "hidapi/hidapi.h"
#include "openvr/openvr_driver.h"
#include "settingsAPIKeys.h"

int MultiHIDTransport::Start()
{
	DriverLog("[MultiHIDTransport] Initializing HID transport");

	if (hid_init() != 0)
	{
		DriverLog("[MultiHIDTransport] HID init failed.");
		return 1;
	}

	const unsigned short pid = (const unsigned short)vr::VRSettings()->GetInt32(k_pch_Driver_Section, k_pch_HID_PID_Int32);
	const unsigned short vid = (const unsigned short)vr::VRSettings()->GetInt32(k_pch_Driver_Section, k_pch_HID_VID_Int32);

	hid_device_info *hid_list = hid_enumerate((unsigned short)vid, (unsigned short)pid);

	while (hid_list->next)
	{
		switch (hid_list->usage_page)
		{
		case 0x0973:
			accel = hid_open_path(hid_list->path);
			break;
		case 0x0976:
			gyro = hid_open_path(hid_list->path);
			break;
		case 0x0983:
			mag = hid_open_path(hid_list->path);
			break;
		}
	}

	if (!accel || !gyro)
	{
		DriverLog("[MultiHIDTransport] Unable to start data stream of device with pid=%d and vid=%d.\n", pid, vid);
		HIDConnected = false;
		return 1;
	}

	HIDInit = true;
	HIDConnected = true;

	DriverLog("[DataStream] HID value PID = %d , VID = %d\n", pid, vid);

	return 0;
}

void MultiHIDTransport::Stop()
{
	hid_close(accel);
	hid_close(gyro);
	if (mag != NULL) {
		hid_close(mag);
	}

	hid_exit();
	HIDConnected = false;
	HIDInit = false;
}

bool MultiHIDTransport::IsConnected()
{
	return HIDConnected;
}

int MultiHIDTransport::ReadPacket(uint8_t *buffer, size_t length)
{
	return hid_read(accel, buffer, length); // Result should be greater than 0.
}
