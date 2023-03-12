#include <RenderFW/window_event_handler.hpp>

namespace tmb {

////////////////////////////////////////////////////////////
// WindowEventParser implementation
////////////////////////////////////////////////////////////
WindowEventParser::WindowEventParser()
    : EventParser(WindowEventType::WindowEventCount) {
    FocusEventParser::getInstance()->attach(&handlers_[WindowEventType::Focus]);
}

void WindowEventParser::remove() { delete FocusEventParser::getInstance(); }

WindowEventParser* WindowEventParser::getInstance() {
    if (instance_ == nullptr) instance_ = new WindowEventParser();
    return instance_;
}

void WindowEventParser::call(const sf::Event& event) {
    size_t group;

    if (event.type == sf::Event::Closed)
        group = WindowEventType::Closed;
    else if (event.type == sf::Event::Resized)
        group = WindowEventType::Resized;
    else
        group = WindowEventType::Focus;

    handlers_[group].call(event);
}

////////////////////////////////////////////////////////////
// FocusEventParser implementation
////////////////////////////////////////////////////////////
FocusEventParser::FocusEventParser()
    : EventParser(FocusEventType::FocusEventCount) {}

FocusEventParser* FocusEventParser::getInstance() {
    if (instance_ == nullptr) instance_ = new FocusEventParser();
    return instance_;
}

void FocusEventParser::call(const sf::Event& event) {
    size_t group = event.type - sf::Event::LostFocus;
    handlers_[group].call(event);
}

EventHandler* attachWindowEventHandler(uint32_t group, EventHandler* handler) {
    return WindowEventParser::getInstance()->addHandler(handler, group);
}

EventHandler* attachWindowEventHandler(
    uint32_t group, std::function<void(const sf::Event&)> callable) {
    return attachWindowEventHandler(group, new CallableHandler(callable));
}

EventHandler* attachFocusEventHandler(uint32_t group, EventHandler* handler) {
    return FocusEventParser::getInstance()->addHandler(handler, group);
}

EventHandler* attachFocusEventHandler(uint32_t group, std::function<void(const sf::Event&)> callable) {
    return attachFocusEventHandler(group, new CallableHandler(callable));
}

}  // namespace tmb
