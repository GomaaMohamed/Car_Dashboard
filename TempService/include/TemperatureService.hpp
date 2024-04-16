#ifndef TEMPERATURE_SERVICE_
#define TEMPERATURE_SERVICE_

#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define TEMPERATURE_SERVICE_ID 0x1235
#define TEMPERATURE_INSTANCE_ID 0x5679
#define TEMPERATURE_EVENTGROUP_ID 0x4466
#define TEMPERATURE_EVENT_ID 0x8779

class TemperatureService
{
public:
    TemperatureService();
    ~TemperatureService();
    bool initService();
    void startService();
    void notifyTemperature(int inTemperatureValue);

private:
    std::shared_ptr<vsomeip::runtime> mSomeipRuntime;
    std::shared_ptr<vsomeip::application> mSomeipApplication;
    std::set<vsomeip::eventgroup_t> mEventGroup;
    int mTemperature;
    void onMessageCallback(const std::shared_ptr<vsomeip::message> &Request);
    void onStateCallback(vsomeip::state_type_e State);
};

#endif