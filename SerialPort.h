#pragma once
#include <string>
#include <iostream>
#include <sstream>   // add this too
#define NOMINMAX     // prevents Windows from breaking std::min/max
#include <windows.h>
//----ChatGPT-Parser----

class SerialPort {
private:
    HANDLE handle;
    bool connected;

public:
    SerialPort(const std::string& portName, DWORD baud = CBR_9600)
        : connected(false)
    {
        handle = CreateFileA(
            portName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
        );

        if (handle == INVALID_HANDLE_VALUE) {
            connected = false;
            return;
        }

        DCB serialParams = { 0 };
        serialParams.DCBlength = sizeof(serialParams);

        if (!GetCommState(handle, &serialParams)) {
            CloseHandle(handle);
            connected = false;
            return;
        }

        serialParams.BaudRate = baud;
        serialParams.ByteSize = 8;
        serialParams.StopBits = ONESTOPBIT;
        serialParams.Parity = NOPARITY;

        if (!SetCommState(handle, &serialParams)) {
            CloseHandle(handle);
            connected = false;
            return;
        }

        connected = true;
        std::cout << connected << "\n";
    }

    bool isConnected() const {
        return connected;
    }

    bool readLine(std::string& out) {
        if (!connected) return false;

        char c;
        out.clear();

        DWORD bytesRead;
        while (true) {
            if (!ReadFile(handle, &c, 1, &bytesRead, nullptr)) {
                connected = false;
                return false;
            }

            if (bytesRead == 0)
                return false; // no data yet

            if (c == '\n')
                break;

            if (c != '\r')
                out.push_back(c);
        }

        return true;
    }

    bool writeString(const std::string& s) {
        if (!connected) return false;

        DWORD bytesSent;
        return WriteFile(handle, s.c_str(), s.size(), &bytesSent, nullptr);
    }

    ~SerialPort() {
        if (connected)
            CloseHandle(handle);
    }
};
