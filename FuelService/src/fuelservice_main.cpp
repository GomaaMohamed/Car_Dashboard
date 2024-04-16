#include "FuelService.hpp"


int main()
{
    FuelService fuelService;
    if (!fuelService.initService())
    {
        std::cerr << "Failed to initialize Fuel service." << std::endl;
        return -1;
    }
    std::thread fuelThread(&FuelService::startService,&fuelService);
    // Simulate fuel updates
    while (true)
    {
        
        for (size_t i = 0; i < 100; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            fuelService.notifyFuel(i);
        }
        for (size_t i = 100; i > 0; i--)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            fuelService.notifyFuel(i);
        }
    }

    return 0;
}