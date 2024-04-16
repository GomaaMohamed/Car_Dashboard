#include "TemperatureService.hpp"



int main()
{
    TemperatureService temperatureService;
    if (!temperatureService.initService())
    {
        std::cerr << "Failed to initialize temperature service." << std::endl;
        return -1;
    }
    std::thread temperatureThread(&TemperatureService::startService,&temperatureService);
    // Simulate temperature updates
    while (true)
    {
        
        for (size_t i = 0; i < 100; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            temperatureService.notifyTemperature(i);
        }
        for (size_t i = 100; i > 0; i--)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            temperatureService.notifyTemperature(i);
        }
    }

    return 0;
}