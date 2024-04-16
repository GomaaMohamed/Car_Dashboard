#include "FuelService.hpp"

FuelService::FuelService() : mSomeipRuntime(vsomeip::runtime::get()),
                               mSomeipApplication(mSomeipRuntime->create_application()),
                               mFuel(0)
{
}

FuelService::~FuelService() {}

bool FuelService::initService()
{
    if (!mSomeipApplication->init())
    {
        std::cerr << "Couldn't initialize Fuel service" << std::endl;
        return false;
    }

    // Register state handler
    mSomeipApplication->register_state_handler(std::bind(&FuelService::onStateCallback, this, std::placeholders::_1));

    // Register the message handlers with the application
    mSomeipApplication->register_message_handler(FUEL_SERVICE_ID, FUEL_INSTANCE_ID,
                                                 vsomeip::ANY_METHOD, std::bind(&FuelService::onMessageCallback, this, std::placeholders::_1));

    return true;
}

void FuelService::startService()
{
    // Start event loop
    mSomeipApplication->start();
}


void FuelService::onMessageCallback(const std::shared_ptr<vsomeip::message> &Request)
{
    // Respond to requests Request/Response messages
    if (1) {
        std::cout << "Received GET FUEL request from client: " << Request->get_client() << std::endl;
    std::shared_ptr<vsomeip::message> response = mSomeipRuntime->create_response(Request);
    std::shared_ptr<vsomeip::payload> payload = mSomeipRuntime->create_payload();
    payload->set_data(reinterpret_cast<const vsomeip::byte_t *>(&mFuel), sizeof(mFuel));
    response->set_payload(payload);
    mSomeipApplication->send(response);}
}

void FuelService::onStateCallback(vsomeip::state_type_e State)
{
    if (State == vsomeip::state_type_e::ST_REGISTERED)
    {
        // Offer the fuel service
        mSomeipApplication->offer_service(FUEL_SERVICE_ID, FUEL_INSTANCE_ID);
        std::cout << "Fuel service offered" << std::endl;
        // Offer event
        mEventGroup.insert(FUEL_EVENTGROUP_ID);
        mSomeipApplication->offer_event(FUEL_SERVICE_ID, FUEL_INSTANCE_ID, FUEL_EVENT_ID, mEventGroup);
        std::cout << "Fuel event offered" << std::endl;
    }
    else if (State == vsomeip::state_type_e::ST_DEREGISTERED)
    {
        // Remove the offer for the fuel service
        mSomeipApplication->stop_offer_service(FUEL_SERVICE_ID, FUEL_INSTANCE_ID);
        std::cout << "Fuel service stopped" << std::endl;
        // Remove event
        mEventGroup.erase(FUEL_EVENTGROUP_ID);
        mSomeipApplication->stop_offer_event(FUEL_SERVICE_ID, FUEL_INSTANCE_ID, FUEL_EVENT_ID);
        std::cout << "Fuel event stopped" << std::endl;
    }
}

void FuelService::notifyFuel(int inFuel)
{
    // Update fuel
    mFuel = inFuel;
    // Create payload with fuel value
    std::shared_ptr<vsomeip::payload> payload = mSomeipRuntime->create_payload();
    payload->set_data(reinterpret_cast<const vsomeip::byte_t *>(&inFuel), sizeof(inFuel));
    // Send notification
    mSomeipApplication->notify(FUEL_SERVICE_ID, FUEL_INSTANCE_ID, FUEL_EVENT_ID, payload);
}
