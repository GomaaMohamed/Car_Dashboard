#include "TemperatureService.hpp"

TemperatureService::TemperatureService() : mSomeipRuntime(vsomeip::runtime::get()),
                               mSomeipApplication(mSomeipRuntime->create_application()),
                               mTemperature(0)
{
}

TemperatureService::~TemperatureService() {}

bool TemperatureService::initService()
{
    if (!mSomeipApplication->init())
    {
        std::cerr << "Couldn't initialize Temperature service" << std::endl;
        return false;
    }

    // Register state handler
    mSomeipApplication->register_state_handler(std::bind(&TemperatureService::onStateCallback, this, std::placeholders::_1));

    // Register the message handlers with the application
    mSomeipApplication->register_message_handler(TEMPERATURE_SERVICE_ID, TEMPERATURE_INSTANCE_ID,
                                                 vsomeip::ANY_METHOD, std::bind(&TemperatureService::onMessageCallback, this, std::placeholders::_1));

    return true;
}

void TemperatureService::startService()
{
    // Start event loop
    mSomeipApplication->start();
}


void TemperatureService::onMessageCallback(const std::shared_ptr<vsomeip::message> &Request)
{
    // Respond to requests Request/Response messages
    if (1) {
        std::cout << "Received GET TEMPERATURE request from client: " << Request->get_client() << std::endl;
    std::shared_ptr<vsomeip::message> response = mSomeipRuntime->create_response(Request);
    std::shared_ptr<vsomeip::payload> payload = mSomeipRuntime->create_payload();
    payload->set_data(reinterpret_cast<const vsomeip::byte_t *>(&mTemperature), sizeof(mTemperature));
    response->set_payload(payload);
    mSomeipApplication->send(response);}
}

void TemperatureService::onStateCallback(vsomeip::state_type_e State)
{
    if (State == vsomeip::state_type_e::ST_REGISTERED)
    {
        // Offer the temperature service
        mSomeipApplication->offer_service(TEMPERATURE_SERVICE_ID, TEMPERATURE_INSTANCE_ID);
        std::cout << "Temperature service offered" << std::endl;
        // Offer event
        mEventGroup.insert(TEMPERATURE_EVENTGROUP_ID);
        mSomeipApplication->offer_event(TEMPERATURE_SERVICE_ID, TEMPERATURE_INSTANCE_ID, TEMPERATURE_EVENT_ID, mEventGroup);
        std::cout << "Temperature event offered" << std::endl;
    }
    else if (State == vsomeip::state_type_e::ST_DEREGISTERED)
    {
        // Remove the offer for the temperature service
        mSomeipApplication->stop_offer_service(TEMPERATURE_SERVICE_ID, TEMPERATURE_INSTANCE_ID);
        std::cout << "Temperature service stopped" << std::endl;
        // Remove event
        mEventGroup.erase(TEMPERATURE_EVENTGROUP_ID);
        mSomeipApplication->stop_offer_event(TEMPERATURE_SERVICE_ID, TEMPERATURE_INSTANCE_ID, TEMPERATURE_EVENT_ID);
        std::cout << "Temperature event stopped" << std::endl;
    }
}

void TemperatureService::notifyTemperature(int inTemperature)
{
    // Update temperature
    mTemperature = inTemperature;
    // Create payload with temperature value
    std::shared_ptr<vsomeip::payload> payload = mSomeipRuntime->create_payload();
    payload->set_data(reinterpret_cast<const vsomeip::byte_t *>(&inTemperature), sizeof(inTemperature));
    // Send notification
    mSomeipApplication->notify(TEMPERATURE_SERVICE_ID, TEMPERATURE_INSTANCE_ID, TEMPERATURE_EVENT_ID, payload);
}
