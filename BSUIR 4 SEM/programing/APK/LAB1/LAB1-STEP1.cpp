#include <windows.h>
#include <iostream>
using namespace std;

HANDLE COM_Port_1;
LPCWSTR Port_Name_1 = L"COM3";
HANDLE COM_Port_2;
LPCWSTR Port_Name_2 = L"COM4";

void Read_from_COM()
{
    DWORD Size;
    char Received_Char;
    ReadFile(COM_Port_2, &Received_Char, 1, &Size, 0);
    if (Size > 0)
    {
        cout << Received_Char;
    }
}

int main()
{
    COM_Port_1 = ::CreateFileW(Port_Name_1, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    COM_Port_2 = ::CreateFileW(Port_Name_2, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (COM_Port_1 == INVALID_HANDLE_VALUE || COM_Port_2 == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            cout << "COM-port does not exist!\n";
        }
        cout << "Some other error.\n";
        return 1;
    }

    DCB Serial_Params = { 0 };
    Serial_Params.DCBlength = sizeof(Serial_Params);
    if (!GetCommState(COM_Port_1, &Serial_Params) || !GetCommState(COM_Port_2, &Serial_Params))
    {
        cout << "Getting state error.\n";
        return 1;
    }
    Serial_Params.BaudRate = CBR_9600;
    Serial_Params.ByteSize = 8;
    Serial_Params.StopBits = ONESTOPBIT;
    Serial_Params.Parity = NOPARITY;
    if (!SetCommState(COM_Port_1, &Serial_Params) || !SetCommState(COM_Port_2, &Serial_Params))
    {
        cout << "Error setting serial port state.\n";
        return 1;
    }

    char data;
    cout << "Enter a character to send: ";
    cin >> data;

    DWORD Size = sizeof(data);
    DWORD Bytes_Written;

    BOOL Ret = WriteFile(COM_Port_1, &data, Size, &Bytes_Written, NULL);

    cout << Size << " Bytes in string. " << Bytes_Written << " Bytes sended. " << endl;

    Read_from_COM();

    CloseHandle(COM_Port_1);
    CloseHandle(COM_Port_2);

    return 0;
}
