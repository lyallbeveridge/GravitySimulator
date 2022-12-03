#include <stdio.h>
#include <Windows.h>
#include <Dbt.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <conio.h>

// UDP 
#using <System.dll>
using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

ref class ManagedUDP
{
	public:	
		static constexpr int PortNumber = 6000;	// Portnumber of the UDP system
		static UdpClient^ Client;				// UDP client reference
		static array<unsigned char>^ WriteBuf;	// reference to a buffer of uchar
		static String^ StringCommand;			// Strings to send
};


void SendButtonPress(int buttonCode)
{

	switch (buttonCode)
	{
		case 1:
			ManagedUDP::StringCommand = gcnew String("1;\n");
			break;

		case 2:
			ManagedUDP::StringCommand = gcnew String("2;\n");
			break;
		
		default:
			ManagedUDP::StringCommand = gcnew String("UNRECOGNIZED;\n");
			break;
	}


	ManagedUDP::WriteBuf = System::Text::Encoding::ASCII->GetBytes(ManagedUDP::StringCommand);

	// Send a demo UDP message
	try
	{
		//send the bytes
		ManagedUDP::Client->Send(ManagedUDP::WriteBuf, ManagedUDP::WriteBuf->Length);
	}
	catch (IO::IOException^)
	{
		Console::WriteLine("Write buffer full");
	}
}

// Only called by WindowProcedure Callback
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

			// This is the DPAD
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

			// These are the values
			printf("Buttons: ");
			HIDP_BUTTON_CAPS* buttonCaps = (HIDP_BUTTON_CAPS*)malloc(caps.NumberInputButtonCaps * sizeof(HIDP_BUTTON_CAPS));
			HidP_GetButtonCaps(HidP_Input, buttonCaps, &caps.NumberInputButtonCaps, data);
			for (USHORT i = 0; i < caps.NumberInputButtonCaps; ++i)
			{
				// This is the number of total buttons? (remains at 10)
				ULONG usageCount = buttonCaps->Range.UsageMax - buttonCaps->Range.UsageMin + 1;
				USAGE* usages = (USAGE*)malloc(sizeof(USAGE) * usageCount);
				
				// This returns a list of all HID control button usages that are set to on
				//		Also modified the usageCount value down to only buttons being used
				HidP_GetUsages(HidP_Input, buttonCaps[i].UsagePage, 0, usages, &usageCount, data, (PCHAR)input->data.hid.bRawData, input->data.hid.dwSizeHid);
				
				// After usage count has been adjusted down appropriately
				for (ULONG usageIndex = 0; usageIndex < usageCount; ++usageIndex) {
					printf("%d ", usages[usageIndex]);
					SendButtonPress(usages[usageIndex]);
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
		Console::WriteLine("BUTTON PRESSED");
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main()
{
	// Executes the processing java successfully in a different window, the TEXT being required as a macro to convert arguments to Windows strings
	ShellExecute(NULL, TEXT("open"), TEXT("processing-java"), TEXT("--sketch=C:\\Users\\lyall\\OneDrive\\Documents\\Controller\\ProcessingBallRepo\\GravitySimulator\\Bouncing --run"), NULL, SW_SHOWNORMAL);

	// Setup UDP
	ManagedUDP::WriteBuf = gcnew array<unsigned char>(128);			// Allocate space to Buf on managed heap
	ManagedUDP::Client = gcnew UdpClient("127.0.0.1", ManagedUDP::PortNumber); // Create client object on heap and connect0
	
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
	
	/**
	*  This loop gets stuck in one of the functions below,
	*	waiting for controller input
	*/
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	ManagedUDP::Client->Close();
	return 0;
}
