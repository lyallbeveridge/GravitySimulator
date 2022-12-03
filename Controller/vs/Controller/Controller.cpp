#include <stdio.h>
#include <Windows.h>
#include <Dbt.h>
#include <hidsdi.h>
#include <hidpi.h>

// UDP 
#using <System.dll>
using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

void printRawInputData(LPARAM lParam)
{
	UINT size = 0;
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
	RAWINPUT* input = (RAWINPUT*)malloc(size);
	bool gotInput = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, input, &size, sizeof(RAWINPUTHEADER)) > 0;
	if (gotInput)
	{
		GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, 0, &size);
		_HIDP_PREPARSED_DATA* data = (_HIDP_PREPARSED_DATA*)malloc(size);
		bool gotPreparsedData = GetRawInputDeviceInfo(input->header.hDevice, RIDI_PREPARSEDDATA, data, &size) > 0;
		if (gotPreparsedData)
		{
			HIDP_CAPS caps;
			HidP_GetCaps(data, &caps);

			printf("Values: ");
			HIDP_VALUE_CAPS* valueCaps = (HIDP_VALUE_CAPS*)malloc(caps.NumberInputValueCaps * sizeof(HIDP_VALUE_CAPS));
			HidP_GetValueCaps(HidP_Input, valueCaps, &caps.NumberInputValueCaps, data);
			for (USHORT i = 0; i < caps.NumberInputValueCaps; ++i)
			{
				ULONG value;
				HidP_GetUsageValue(HidP_Input, valueCaps[i].UsagePage, 0, valueCaps[i].Range.UsageMin, &value, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
				printf("%d:%5d ", i, value);
			}
			free(valueCaps);

			printf("Buttons: ");
			HIDP_BUTTON_CAPS* buttonCaps = (HIDP_BUTTON_CAPS*)malloc(caps.NumberInputButtonCaps * sizeof(HIDP_BUTTON_CAPS));
			HidP_GetButtonCaps(HidP_Input, buttonCaps, &caps.NumberInputButtonCaps, data);
			for (USHORT i = 0; i < caps.NumberInputButtonCaps; ++i)
			{
				ULONG usageCount = buttonCaps->Range.UsageMax - buttonCaps->Range.UsageMin + 1;
				USAGE* usages = (USAGE*)malloc(sizeof(USAGE) * usageCount);
				HidP_GetUsages(HidP_Input, buttonCaps[i].UsagePage, 0, usages, &usageCount, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
				for (ULONG usageIndex = 0; usageIndex < usageCount; ++usageIndex) {
					printf("%d ", usages[usageIndex]);
				}
				free(usages);
			}
			free(buttonCaps);
		}
		free(data);
	}
	free(input);
	printf("\n");
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_INPUT) {
		printRawInputData(lParam);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main()
{
	// Executes the processing java successfully in a different window, the TEXT being required as a macro to convert arguments to Windows strings
	ShellExecute(NULL, TEXT("open"), TEXT("processing-java"), TEXT("--sketch=C:\\Users\\lyall\\OneDrive\\Documents\\Controller\\ProcessingBallRepo\\GravitySimulator\\Bouncing --run"), NULL, SW_SHOWNORMAL);
	
	// --- Create the UDP
	
	// UDP port number of the remote host
	int PortNumber = 6000;
	// Reference to UDP Client
	UdpClient^ Client;
	// Toggling write buffer
	int Toggle = 0;
	// reference to a buffer of uchar
	array<unsigned char>^ WriteBuf;
	array<unsigned char>^ ReadBuf;
	// Strings to send
	String^ StringPtrOFF, ^ StringPtrON;

	//Allocate space to Buf on managed heap
	WriteBuf = gcnew array<unsigned char>(128);
	ReadBuf = gcnew array<unsigned char>(128);
	// Create client object on heap and connect0
	Client = gcnew UdpClient("127.0.0.1", PortNumber);

	StringPtrOFF = gcnew String("HELLO WORLD");
	// StringPtrON is set once
	StringPtrON = gcnew String("OP85,170;");
	WriteBuf = System::Text::Encoding::ASCII->GetBytes(StringPtrOFF);

	
	
	// Create a window, as we need a window precedure to recieve raw input
	WNDCLASS wnd = { 0 };
	wnd.hInstance = GetModuleHandle(0);
	wnd.lpfnWndProc = WindowProcedure;
	wnd.lpszClassName = TEXT("Raw input test");
	RegisterClass(&wnd);
	HWND hwnd = CreateWindow(wnd.lpszClassName, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, wnd.hInstance, 0);

	// Register devices

	RAWINPUTDEVICE deviceList[2];
	deviceList[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	deviceList[0].usUsage = HID_USAGE_GENERIC_GAMEPAD;
	deviceList[0].dwFlags = RIDEV_INPUTSINK;
	deviceList[0].hwndTarget = hwnd;
	deviceList[1] = deviceList[0];
	deviceList[1].usUsage = HID_USAGE_GENERIC_JOYSTICK;

	UINT deviceCount = sizeof(deviceList) / sizeof(*deviceList);
	RegisterRawInputDevices(deviceList, deviceCount, sizeof(RAWINPUTDEVICE));

	// Message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// Send a demo UDP message
		try
		{
			//send the bytes
			Client->Send(WriteBuf, WriteBuf->Length);
		}
		catch (IO::IOException^)
		{
			Console::WriteLine("Write buffer full");
		}
	}

	Client->Close();
	return 0;
}
