#include "includes.h"
#include "functions.h"
#include <thread>
#include <chrono>
void Cleanup(const std::string message) {
    std::cout << message << std::endl;
    system("pause");
    ExitProcess(0);
}

std::string RandomString(const size_t length)
{
    std::string r;
    static const char bet[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyzZ1234567890" };
    srand((unsigned)time(NULL) * 5);
    for (int i = 0; i < length; ++i)
        r += bet[rand() % (sizeof(bet) - 1)];
    return r;
}

int main()
{
    system("color 5");
    SetConsoleTitleA(RandomString(26).c_str());

    DWORD ProcessId = Functions::GetProcessId("csgo.exe");

    if (!ProcessId) 
    {
        Cleanup("[!] No ProcessId Found. Startint CSGO dont do anything!");
        std::cout << "[+] retrying injection in 60 Seconds | 1 Minute" << std::endl;
        system("start steam://rungameid/730");
        std::this_thread::sleep_for(std::chrono::seconds(60));
        main();
    }
        
        
  


    HANDLE Game = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
    uintptr_t ModuleBase = Functions::GetModuleBaseAddress(ProcessId, "client.dll");

    std::cout << "[+] csgo.exe -> " << std::dec << ProcessId << std::endl;

    if (Functions::DoesFileExist("cheat.dll")) {

        if (!Functions::Internal::ExecuteBypass(Game)) {
            Cleanup("[!] Cannot Bypass...");
        }

        if (Functions::LoadLibraryInject(ProcessId, "cheat.dll")) {

            Functions::Internal::Backup(Game);
            system("color 2");
            std::cout << "[+] Injected into csgo.exe [" << ProcessId << "]" << std::endl;
            std::cout << "[+] Sucessfully Remapped" << std::endl;
            std::cout << "[<] Exitting in 10 Seconds..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(10));
            ExitProcess(0);
        }
        else
        {
            Functions::Internal::Backup(Game);
            Cleanup("[!] Injection Failed.");
        }

    }



    return 0;
}