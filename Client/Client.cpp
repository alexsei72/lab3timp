

#include <stdlib.h>
#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <wingdi.h>
#include <fstream>


std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}


#define MAX_LEN 100

#define BUFSIZE 50000

auto PIPE_NAME = TEXT("\\\\.\\pipe\\Pipe");
const int MAX_CONNECTIONS = 10;

int main(void)
{
    Sleep(2000);
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    TCHAR  chBuf[BUFSIZE];
    bool fSuccess = false;
    DWORD dwWritten;
    DWORD last_error;
    DWORD buffecount = 50000;
    LPDWORD dword = &buffecount;
    unsigned int elapsed_seconds = 0;
    const unsigned int timeout_seconds = 5;

    HANDLE hPipe = CreateFile(PIPE_NAME,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        0,
        0);

    while (INVALID_HANDLE_VALUE == hPipe &&
        elapsed_seconds < timeout_seconds)
    {
        last_error = GetLastError();

        if (last_error != ERROR_PIPE_BUSY)
        {
            break;
        }
        printf("\nPipe Is busy\n");
        Sleep(1 * 1000);
        elapsed_seconds++;

        hPipe = CreateFile(PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            0);
    }

    

    if (INVALID_HANDLE_VALUE == hPipe)
    {
        std::cerr << "Failed to connect to pipe " << PIPE_NAME <<
            ": last_error=" << last_error << "\n";
    }



    else 
    {
        dwMode = PIPE_READMODE_BYTE;
        fSuccess = SetNamedPipeHandleState(
            hPipe,    // pipe handle 
            &dwMode,  // new pipe mode 
            NULL,     // don't set maximum bytes 
            NULL);    // don't set maximum time 
        if (!fSuccess)
        {
            _tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
            int k;
            std::cin >> k;
            return -1;
        }

        fSuccess = WriteFile(
            hPipe,
            "Hello Pipe\n",
            12,   // = length of string + terminating '\0' !!!
            &dwWritten,
            NULL);


        if (!fSuccess)
        {
            _tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
            int k;
            std::cin >> k;
            return -1;
        }

        printf("\nMessage sent to server, receiving reply as follows:\n");
        do
        {
            // Read from the pipe. 

            fSuccess = ReadFile(
                hPipe,    // pipe handle 
                chBuf,    // buffer to receive reply 
                BUFSIZE,  // size of buffer 
                &cbRead,  // number of bytes read 
                
                NULL);    // not overlapped 
            char b;
            //std::cout << "םאזלטעו enter ןנט מעגועו סונגונא";
            //std::cin.get();
            if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
                break;
            //std::cout << "םאזלטעו enter ןנט מעגועו סונגונא";
            //std::cin.get();
            printf("Message arrived");

            FILE* txt;

            txt = fopen("C:\\img\\outputimage.png", "wb");
            //std::cout << "םאזלטעו enter ןנט מעגועו סונגונא";
            //std::cin.get();
            fwrite(chBuf, sizeof(TCHAR), cbRead,txt);
            int g;
            fclose(txt);


        } while (!fSuccess);

        printf("\nPipe is going to close\n");
        CloseHandle(hPipe);
    }
    int k;
    std::cin >> k;
}
