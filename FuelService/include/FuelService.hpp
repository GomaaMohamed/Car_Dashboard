#ifndef FUEL_SERVICE_
#define FUEL_SERVICE_

#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define FUEL_SERVICE_ID 0x1233
#define FUEL_INSTANCE_ID 0x5677
#define FUEL_EVENTGROUP_ID 0x4463
#define FUEL_EVENT_ID 0x8773

class FuelService
{
public:
    FuelService();
    ~FuelService();
    bool initService();
    void startService();
    void notifyFuel(int inFuelValue);

private:
    std::shared_ptr<vsomeip::runtime> mSomeipRuntime;
    std::shared_ptr<vsomeip::application> mSomeipApplication;
    std::set<vsomeip::eventgroup_t> mEventGroup;
    int mFuel;
    void onMessageCallback(const std::shared_ptr<vsomeip::message> &Request);
    void onStateCallback(vsomeip::state_type_e State);
};

#endif