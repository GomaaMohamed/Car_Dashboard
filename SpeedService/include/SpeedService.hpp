#ifndef SPEED_SERVICE_
#define SPEED_SERVICE_

#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define SPEED_SERVICE_ID 0x1234
#define SPEED_INSTANCE_ID 0x5678
#define SPEED_EVENTGROUP_ID 0x4465
#define SPEED_EVENT_ID 0x8778

class SpeedService
{
public:
    SpeedService();
    ~SpeedService();
    bool initService();
    void startService();
    void notifySpeed(int inSpeedValue);

private:
    std::shared_ptr<vsomeip::runtime> mSomeipRuntime;
    std::shared_ptr<vsomeip::application> mSomeipApplication;
    std::set<vsomeip::eventgroup_t> mEventGroup;
    int mSpeed;
    void onMessageCallback(const std::shared_ptr<vsomeip::message> &Request);
    void onStateCallback(vsomeip::state_type_e State);
};

#endif