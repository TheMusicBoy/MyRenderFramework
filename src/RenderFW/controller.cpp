#include <controller.hpp>
#include <main_event_handler.hpp>
#include <window_event_handler.hpp>

namespace tmb {

////////////////////////////////////////////////////////////
// Controller implementation
////////////////////////////////////////////////////////////

Controller::Controller() {
    GlobalEventHandler::getInstance()->attach(&global_handler_);
}

Controller::~Controller() {
    std::lock_guard lock(lock_);
    global_handler_.clear();
}

void Controller::attach(Render* render_instance) {
    render_instance_ = render_instance;
}

void Controller::call() {
    EventQueue event_queue_;

    lock_.lock();
    render_instance_->getEvents(&event_queue_);

    sf::Event event;
    while (event_queue_.pollEvent(&event)) {
        global_handler_.call(event);
    }

    lock_.unlock();
}

void Controller::process() {
    bool running = true;

    handlers_.push(attachWindowEventHandler(
        WindowEventType::Closed,
        new CallableHandler([&](const sf::Event& event) { running = false; })));

    while (running) this->call();
}

}  // namespace tmb
