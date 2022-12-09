#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include "overlay.h"
using namespace std;

// Function to generate random string
string generate_random_string(int length)
{
    string random_string;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < length; ++i) {
        random_string += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return random_string;
}
// Function to block a process from the Windows Firewall
void block_process_from_firewall(string process_name)
{
    // Create a command to block the process from the firewall
    string command = "netsh advfirewall firewall add rule name=\"";
    command += process_name + "\" dir=in action=block program=\"" + process_name + "\" enable=yes";

    // Execute the command
    system(command.c_str());
}

// Function to get process id from process name
DWORD get_process_id(string process_name)
{
    HANDLE process_snapshot;
    PROCESSENTRY32 process_entry;
    DWORD process_id = 0;

    // Take a snapshot of all running processes
    process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (process_snapshot == INVALID_HANDLE_VALUE)
        return 0;

    // Get the first process
    process_entry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(process_snapshot, &process_entry))
        return 0;

    // Find the process
    do {
        if (strcmp(process_entry.szExeFile, process_name.c_str()) == 0)
        {
            process_id = process_entry.th32ProcessID;
            break;
        }
    } while (Process32Next(process_snapshot, &process_entry));

    // Close the snapshot
    CloseHandle(process_snapshot);

    return process_id;
}

// Function to block GTA San Andreas from the Windows Firewall
void block_gta_sa_from_firewall()
{
    string process_name;
    DWORD process_id;

    // Get the process name
    process_name = "gta_sa.exe";

    // Get the process id
    process_id = get_process_id(process_name);
    if (process_id == 0)
        return;

    // Block the process from the firewall
    block_process_from_firewall(process_name);
}

// Function to block MTA San Andreas from the Windows Firewall
void block_mta_sa_from_firewall()
{
    string process_name;
    DWORD process_id;

    // Get the process name
    process_name = "proxy_sa.exe";

    // Get the process id
    process_id = get_process_id(process_name);
    if (process_id == 0)
        return;

    // Block the process from the firewall
    block_process_from_firewall(process_name);
}

// Function to block wmic diskdrive get serialnumber from the Windows Firewall
void block_wmic_diskdrive_get_serialnumber_from_firewall()
{
    string process_name;
    DWORD process_id;

    // Get the process name
    process_name = "wmic.exe";

    // Get the process id
    process_id = get_process_id(process_name);
    if (process_id == 0)
        return;

    // Block the process from the firewall
    block_process_from_firewall(process_name);
}
// Function to change MTA serial number

void change_mta_serial_no()
{
    HANDLE process_snapshot;
    PROCESSENTRY32 process_entry;
    DWORD process_id;
    HANDLE process_handle;
    BYTE* serial_number_address;
    BYTE* ip_address;
    BYTE* hwid_address;
    DWORD bytes_written;
    string random_serial_no;
    string random_ip;
    string random_hwid;

    // Generate a random serial number
    random_serial_no = generate_random_string(10);
    random_ip = generate_random_string(15);
    random_hwid = generate_random_string(20);

    // Take a snapshot of all running processes
    process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (process_snapshot == INVALID_HANDLE_VALUE)
        return;

    // Get the first process
    process_entry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(process_snapshot, &process_entry))
        return;

    // Find MTA process
    do {
        if (strcmp(process_entry.szExeFile, "proxy_sa.exe") == 0)
        {
            process_id = process_entry.th32ProcessID;
            break;
        }
    } while (Process32Next(process_snapshot, &process_entry));

    // Close the snapshot
    CloseHandle(process_snapshot);

    // Open the MTA process
    process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
    if (process_handle == NULL)
        return;

    // Find the serial number address
    serial_number_address = (BYTE*)0x00AECFC0;

    // Find the IP address
    ip_address = (BYTE*)0x00AECFF0;

    // Find the HWID address
    hwid_address = (BYTE*)0x00AED010;

    // Write the new serial number
    WriteProcessMemory(process_handle, serial_number_address, random_serial_no.c_str(), random_serial_no.length() + 1, (SIZE_T*)&bytes_written);

    // Write the new IP
    WriteProcessMemory(process_handle, ip_address, random_ip.c_str(), random_ip.length() + 1, (SIZE_T*)&bytes_written);

    // Write the new HWID
    WriteProcessMemory(process_handle, hwid_address, random_hwid.c_str(), random_hwid.length() + 1, (SIZE_T*)&bytes_written);

    // Close the MTA process
    CloseHandle(process_handle);

    cout << "Serial number and HWID changed successfully!" << endl;
}
#define OFFSET_FLY 0x8F8B90

// Function to enable the FLY cheat
void GiveFly()
{
    DWORD* current_value;
    // Get the current value at the FLY offset
    ReadProcessMemory(GetCurrentProcess(), (LPVOID)OFFSET_FLY, &current_value, sizeof(current_value), NULL);
    // Set the FLY value to 1 (true)
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)OFFSET_FLY, (LPVOID)(DWORD)1, sizeof(DWORD), NULL);
    std::cout << "FLY cheat enabled!\n";
}
DWORD GetProcessIdByName(string processName)
{
    HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(processSnapshot, &processEntry))
        return 0;

    do
    {
        if (strcmp(processEntry.szExeFile, processName.c_str()) == 0)
        {
            CloseHandle(processSnapshot);
            return processEntry.th32ProcessID;
        }
    } while (Process32Next(processSnapshot, &processEntry));

    CloseHandle(processSnapshot);
    return 0;
}
DWORD WINAPI GiveFlyHook(LPVOID lpParam)
{
    // Hook the GiveFly() function
    DWORD oldProtect;
    VirtualProtect((LPVOID)OFFSET_FLY, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &oldProtect);
    DWORD* originalAddress = (DWORD*)OFFSET_FLY;
    DWORD hookAddress = (DWORD)GiveFly;
    memcpy(originalAddress, &hookAddress, sizeof(DWORD));
    VirtualProtect((LPVOID)OFFSET_FLY, sizeof(DWORD), oldProtect, &oldProtect);

    return 0;
}

// Hook the code to the Multi Theft Auto process
void HookCodeToMTA()
{
    // Get the process ID of the MTA process
    DWORD mtaProcessId = GetProcessIdByName("proxy_sa.exe");
    if (mtaProcessId == 0)
    {
        std::cout << "Could not find the MTA process!\n";
        return;
    }

    // Open the MTA process
    HANDLE mtaProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mtaProcessId);
    if (mtaProcess == NULL)
    {
        std::cout << "Could not open the MTA process!\n";
        return;
    }

    // Allocate memory in the MTA process for the code
    void* memoryAddress = VirtualAllocEx(mtaProcess, NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (memoryAddress == NULL)
    {
        std::cout << "Could not allocate memory in the MTA process!\n";
        return;
    }

    // Copy the code to the MTA process
    SIZE_T bytesWritten;
    if (!WriteProcessMemory(mtaProcess, memoryAddress, (LPVOID)&GiveFlyHook, 1024, &bytesWritten))
    {
        std::cout << "Could not copy the code to the MTA process!\n";
        return;
    }

    // Create a thread in the MTA process
    HANDLE threadHandle = CreateRemoteThread(mtaProcess, NULL, 0, (LPTHREAD_START_ROUTINE)memoryAddress, NULL, 0, NULL);
    if (threadHandle == NULL)
    {
        std::cout << "Could not create a thread in the MTA process!\n";
        return;
    }

    // Wait for the thread to finish
    WaitForSingleObject(threadHandle, INFINITE);

    // Clean up
    VirtualFreeEx(mtaProcess, memoryAddress, 0, MEM_RELEASE);
    CloseHandle(threadHandle);
    CloseHandle(mtaProcess);
}
void clean_mta_traces()
{
    // Clean registry keys
    string key_name = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\MTA";
    RegDeleteKey(HKEY_LOCAL_MACHINE, key_name.c_str());

    // Clean files and folders
    string folder_name = "C:\\Program Files\\MTA";
    RemoveDirectory(folder_name.c_str());

    // Get user name
    char user_name[256];
    DWORD user_name_length = 256;
    GetUserName(user_name, &user_name_length);

    // Delete all files in the AppData folder associated with MTA
    SHFILEOPSTRUCT file_op;
    memset(&file_op, 0, sizeof(file_op));
    file_op.wFunc = FO_DELETE;
    file_op.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
    string from_string = "C:\\Users\\" + string(user_name) + "\\AppData\\Local\\MTA\\*";
    file_op.pFrom = from_string.c_str();
    SHFileOperation(&file_op);
}

#include <fstream>
#include <string>

void execute_mta_lua_file(string lua_file_path)
{
    // Open the file
    std::basic_ifstream<char, std::char_traits<char>> lua_file(lua_file_path);

    // Check if the file is opened successfully
    if (!lua_file.is_open())
        return;

    // Get the contents of the file
    string lua_file_contents;
    lua_file.seekg(0, ios::end);
    lua_file_contents.reserve(lua_file.tellg());
    lua_file.seekg(0, ios::beg);
    lua_file_contents.assign((istreambuf_iterator<char>(lua_file)),
        istreambuf_iterator<char>());

    // Execute the file
    system(("proxy_fn.exe -f \"" + lua_file_path + "\"").c_str());

    // Close the file
    lua_file.close();
}

void lua() {
    string lua_file_path;

    // Ask for the path of the LUA file
    cout << "Enter the path of the LUA file: ";
    cin >> lua_file_path;

    // Execute the file
    execute_mta_lua_file(lua_file_path);
}


DWORD WINAPI InjectThread(LPVOID lpParam)
{
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
    LPVOID lpAddr;
    LPCSTR lpDllName;

    dwProcessId = (DWORD)(lpParam);
    lpDllName = "mta.dll";

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    if (hProcess == NULL)
    {
        return 0;
    }

    lpAddr = (LPVOID)VirtualAllocEx(hProcess, NULL, strlen(lpDllName) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (lpAddr == NULL)
    {
        CloseHandle(hProcess);
        return 0;
    }

    if (WriteProcessMemory(hProcess, lpAddr, lpDllName, strlen(lpDllName) + 1, NULL) == 0)
    {
        VirtualFreeEx(hProcess, lpAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 0;
    }

    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, lpAddr, 0, &dwThreadId);
    if (hThread == NULL)
    {
        VirtualFreeEx(hProcess, lpAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 0;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, lpAddr, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}

BOOL Inject(DWORD dwProcessId)
{
    HANDLE hThread;
    DWORD dwThreadId;

    hThread = CreateThread(NULL, 0, InjectThread, (LPVOID)dwProcessId, 0, &dwThreadId);
    if (hThread == NULL)
    {
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return TRUE;
}

void inj() {
    DWORD dwProcessId;

    cout << "Make sure the DLL it's in the folder where this app it's.\n";
    system("pause");
    // Get the process ID of the target process
    dwProcessId = GetProcessIdByName("proxy_gta.exe");

    // Inject the DLL into the target process
    if (Inject(dwProcessId))
    {
        printf("DLL injected successfully!\n");
    }
    else
    {
        printf("Failed to inject DLL!\n");
    }

}
BOOL SetVolumeNameForVolumeMountPoint(LPCTSTR lpszVolumeMountPoint, LPCTSTR lpszVolumeName, DWORD dwFlags)
{
    return SetVolumeMountPoint(lpszVolumeMountPoint, lpszVolumeName);
}
void diskspoofer() {
    // Get the current diskdrive serial number
    char currentSerial[32];
    GetVolumeNameForVolumeMountPoint("C:\\", currentSerial, 32);

    // Generate a new, random serial number
    char newSerial[32];
    for (int i = 0; i < 32; i++)
    {
        newSerial[i] = (char)(rand() % 256);
    }

    // Set the new serial number
    SetVolumeNameForVolumeMountPoint("C:\\", newSerial, 32);

    // Confirm that the serial has changed
    char updatedSerial[32];
    GetVolumeNameForVolumeMountPoint("C:\\", updatedSerial, 32);

    if (strcmp(updatedSerial, newSerial) == 0)
    {
        cout << "Serial successfully changed!" << endl;
    }
    else
    {
        cout << "Error changing serial!" << endl;
    }

    // Restore the original serial
    SetVolumeNameForVolumeMountPoint("C:\\", currentSerial, 32);
}
void basespoofer() {
    string baseboardSerialNumber;

    cout << "Please enter the Baseboard Serial Number: ";
    getline(cin, baseboardSerialNumber);

    string newBaseboardSerialNumber;

    for (int i = 0; i < baseboardSerialNumber.length(); i++)
    {
        if (baseboardSerialNumber[i] == '0')
        {
            newBaseboardSerialNumber += '1';
        }
        else if (baseboardSerialNumber[i] == '1')
        {
            newBaseboardSerialNumber += '2';
        }
        else if (baseboardSerialNumber[i] == '2')
        {
            newBaseboardSerialNumber += '3';
        }
        else
        {
            newBaseboardSerialNumber += baseboardSerialNumber[i];
        }
    }

    cout << "The new Baseboard Serial Number is: " << newBaseboardSerialNumber << endl;
}


// Define namespace 
using namespace std;

// Wallhack class
class Wallhack
{
public:
    // Constructor
    Wallhack();

    // Methods
    void Hook();
    void Unhook();
    void ReadData();
    void WriteData();
};

// Constructor
Wallhack::Wallhack()
{

}

// Method to hook into MTA (C++)
void Wallhack::Hook()
{
    // Get data from the game
    ReadData();

    // Hook into the game
    HMODULE hMod = GetModuleHandle(NULL);
    DWORD dwAddr = (DWORD)GetProcAddress(hMod, "proxy_sa.exe");
    if (!dwAddr)
    {
        cout << "Error: Failed to hook into the game!" << endl;
        return;
    }

    // Enable wallhack
    WriteData();

    cout << "Wallhack enabled!" << endl;
}

// Method to unhook from MTA (C++)
void Wallhack::Unhook()
{
    // Restore original data
    WriteData();

    cout << "Wallhack disabled!" << endl;
}

// Method to read data from the game
void Wallhack::ReadData()
{
    // Get the pointer to the game memory
    DWORD dwBaseAddr = (DWORD)GetModuleHandle(NULL);
    DWORD dwOffset = 0x100;

    // Read the data from the game memory
    string data;
    ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddr + dwOffset), &data, data.size(), NULL);
}

// Method to write data to the game
void Wallhack::WriteData()
{
    // Get the pointer to the game memory
    DWORD dwBaseAddr = (DWORD)GetModuleHandle(NULL);
    DWORD dwOffset = 0x100;

    // Write the data to the game memory
    string data = "MTA: San Andreas Wallhack";
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddr + dwOffset), &data, data.size(), NULL);
}
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

//Defines
#define WM_PERSIST_OVERLAY WM_USER + 1
#define WM_ACTIVATE_OVERLAY WM_USER + 2
#define WM_DEACTIVATE_OVERLAY WM_USER + 3

//Global variables
HWND hwnd_overlay;
HWND hwnd_proxy_sa;
HDC hdc_overlay;
PAINTSTRUCT ps;
RECT rect;
bool showMenu = false;

//Prototypes
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HINSTANCE hInstance = GetModuleHandle(NULL);
int nCmdShow = SW_SHOWDEFAULT;
HMENU hMenu;

void overlay_v2()
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "SteamOverlay";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);

    //Create window
    hwnd_overlay = CreateWindowEx(WS_EX_COMPOSITED | WS_EX_TRANSPARENT | WS_EX_TOPMOST, "SteamOverlay", "SteamOverlay", WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    SetWindowLong(hwnd_overlay, GWL_EXSTYLE, GetWindowLong(hwnd_overlay, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd_overlay, RGB(255, 255, 255), 255, LWA_ALPHA);
    ShowWindow(hwnd_overlay, nCmdShow);
    UpdateWindow(hwnd_overlay);

    //Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

  //  return (int)msg.wParam;

}


//Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PERSIST_OVERLAY:
        //Get handle to proxy_sa.exe
        hwnd_proxy_sa = FindWindow("Proxy_SA", NULL);
        //Get rect of proxy_sa.exe
        GetWindowRect(hwnd_proxy_sa, &rect);
        //Set size and position of overlay
        SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE);
        //Activate overlay
        PostMessage(hwnd, WM_ACTIVATE_OVERLAY, 0, 0);
        break;

    case WM_ACTIVATE_OVERLAY:
        //Show overlay
        ShowWindow(hwnd, SW_SHOW);
        //Create a device context
        hdc_overlay = BeginPaint(hwnd, &ps);
        //Draw overlay
        DrawText(hdc_overlay, "Cheat Enabled", -1, &rect, DT_CENTER | DT_VCENTER);
        //Draw menu
        if (hwnd == hwnd_proxy_sa) {

        }
        //Clean up
        EndPaint(hwnd, &ps);
        break;

    case WM_DEACTIVATE_OVERLAY:
        //Hide overlay
        ShowWindow(hwnd, SW_HIDE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
        break;
    }

    return 0;
}

void caca() {
    // Wallhack wh;
//  DiscordOverlay overlay; 
  /*You can choose between Discord Overlay and Caca Overlay, i personally choose the Caca Overlay one.*/
    // overlay_v2();
   // wh.Hook();
 diskspoofer();
 basespoofer();
 HookCodeToMTA();
 block_gta_sa_from_firewall();
 block_mta_sa_from_firewall();
change_mta_serial_no();
 clean_mta_traces();
  block_wmic_diskdrive_get_serialnumber_from_firewall();
  GiveFly();
   inj();
   cout << "\u00A9totoware 2021-2022\n";
  lua();
}
//Defines
#define WM_PERSIST_OVERLAY WM_USER + 1
#define WM_ACTIVATE_OVERLAY WM_USER + 2
#define WM_DEACTIVATE_OVERLAY WM_USER + 3

//Global variables

int main()
{
    //Hide the ransomware process from antivirus
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tp;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

    /*const int WIDTH = 30;
    const int HEIGHT = 10;

    int x = 0;
    int y = 0;
    int prevX;
    int prevY;

    srand(time(NULL));

        // Clear console
        std::cout << "\033[2J\033[1;1H";

        // Randomize position
        prevX = x;
        prevY = y;
        x = rand() % WIDTH;
        y = rand() % HEIGHT;

        // Draw character
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (i == y && j == x)
                {
                    std::cout << "*";
                }
                else if (i == prevY && j == prevX)
                {
                    std::cout << " ";
                }
                else
                {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }*/
    

    /*****************************************************************************************************************************/
    SetConsoleTitleA("TotoWare");
    cout << endl;
    cout << "\t\t\t\t  This app it's in beta, by continuing you accept to all our Terms & Services.\n";
    cout << "\t\t\t\t  THIS APP IT'S FOR LEARNING PURPSES\n";
    cout << "\t\t\t\t  Any videos with this application on YouTube it will be taken down within hours.\n";
    cout << "\t\t\t\t  \u00A9totoware 2022" << endl << endl;
    system("pause");
    cout << "\t\t\t\t  Thank you for accepting our terms. You may now use our services." << endl;
    system("cls");
    /*****************************************************************************************************************************/
  
    caca();
    // going where everything it's

    //Register window class
   //Register window class
    


    std::cin.ignore();
    std::cin.ignore();
}