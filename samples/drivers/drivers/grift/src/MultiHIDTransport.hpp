#pragma once

#ifndef _HID_TRANSPORT_H_
#define _HID_TRANSPORT_H_

#include "DataTransport.hpp"
#include "hidapi/hidapi.h"

class MultiHIDTransport : public DataTransport
{
	// Inherited via DataTransport
	int Start() override;
	void Stop() override;
	bool IsConnected() override;
	int ReadPacket(uint8_t *buffer, size_t length) override;

private:
	hid_device *gyro;
	hid_device *accel;
	hid_device *mag;

	bool HIDInit = false;
	bool HIDConnected = false;
};

#endif
