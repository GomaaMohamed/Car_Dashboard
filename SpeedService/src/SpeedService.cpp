#include "SpeedService.hpp"

SpeedService::SpeedService() : mSomeipRuntime(vsomeip::runtime::get()),
                               mSomeipApplication(mSomeipRuntime->create_application()),
                               mSpeed(0)
{
}

SpeedService::~SpeedService() {}

bool SpeedService::initService()
{
    if (!mSomeipApplication->init())
    {
        std::cerr << "Couldn't initialize Speed service" << std::endl;
        return false;
    }

    // Register state handler
    mSomeipApplication->register_state_handler(std::bind(&SpeedService::onStateCallback, this, std::placeholders::_1));

    // Register the message handlers with the application
    mSomeipApplication->register_message_handler(SPEED_SERVICE_ID, SPEED_INSTANCE_ID,
                                                 vsomeip::ANY_METHOD, std::bind(&SpeedService::onMessageCallback, this, std::placeholders::_1));

    return true;
}

void SpeedService::startService()
{
    // Start event loop
    mSomeipApplication->start();
}


void SpeedService::onMessageCallback(const std::shared_ptr<vsomeip::message> &Request)
{
    // Respond to requests Request/Response messages
    if (1) {
        std::cout << "Received GET SPEED request from client: " << Request->get_client() << std::endl;
    std::shared_ptr<vsomeip::message> response = mSomeipRuntime->create_response(Request);
    std::shared_ptr<vsomeip::payload> payload = mSomeipRuntime->create_payload();
    payload->set_data(reinterpret_cast<const vsomeip::byte_t *>(&mSpeed), sizeof(mSpeed));
    response->set_payload(payload);
    mSomeipApplication->send(response);}
}

void SpeedService::onStateCallback(vsomeip::state_type_e State)
{
    if (State == vsomeip::state_type_e::ST_REGISTERED)
    {
        // Offer the speed service
        mSomeipApplication->offer_service(SPEED_SERVICE_ID, SPEED_INSTANCE_ID);
        std::cout << "Speed service offered" << std::endl;
        // Offer event
        mEventGroup.insert(SPEED_EVENTGROUP_ID);
        mSomeipApplication->offer_event(SPEED_SERVICE_ID, SPEED_INSTANCE_ID, SPEED_EVENT_ID, mEventGroup);
        std::cout << "Speed event offered" << std::endl;
    }
    else if (State == vsomeip::state_type_e::ST_DEREGISTERED)
    {
        // Remove the offer for the speed service
        mSomeipApplication->stop_offer_service(SPEED_SERVICE_ID, SPEED_INSTANCE_ID);
        std::cout << "Speed service stopped" << std::endl;
        // Remove event
        mEventGroup.erase(SPEED_EVENTGROUP_ID);
        mSomeipApplication->stop_offer_event(SPEED_SERVICE_ID, SPEED_INSTANCE_ID, SPEED_EVENT_ID);
        std::cout << "Speed event stopped" << std::endl;
    }
}

void SpeedService::notifySpeed(int inSpeed)
{
    // Update speed
    mSpeed = inSpeed;
    // Create payload with speed value
    std::shared_ptr<vsomeip::payload> payload = mSomeipRuntime->create_payload();
    payload->set_data(reinterpret_cast<const vsomeip::byte_t *>(&inSpeed), sizeof(inSpeed));
    // Send notification
    mSomeipApplication->notify(SPEED_SERVICE_ID, SPEED_INSTANCE_ID, SPEED_EVENT_ID, payload);
}
