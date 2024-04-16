#include "SpeedService.hpp"



int main()
{
    SpeedService speedService;
    if (!speedService.initService())
    {
        std::cerr << "Failed to initialize Speed service." << std::endl;
        return -1;
    }
    std::thread speedThread(&SpeedService::startService,&speedService);
    // Simulate speed updates
    while (true)
    {
        
        for (size_t i = 0; i < 280; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            speedService.notifySpeed(i);
        }
        for (size_t i = 280; i > 0; i--)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            speedService.notifySpeed(i);
        }
    }

    return 0;
}