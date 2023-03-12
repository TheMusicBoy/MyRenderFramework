#include <keyboard_event_handler.hpp>
#include <main_event_handler.hpp>
#include <mouse_event_handler.hpp>
#include <window_event_handler.hpp>
#include <iostream>

namespace tmb {

////////////////////////////////////////////////////////////
// GlobalEventHandler implementation
////////////////////////////////////////////////////////////
GlobalEventHandler::GlobalEventHandler() : EventParser(1) {
    MainEventParser::getInstance()->attach(&handlers_[0]);
}

GlobalEventHandler* GlobalEventHandler::getInstance() {
    if (instance_ == nullptr) instance_ = new GlobalEventHandler();
    return instance_;
}

void GlobalEventHandler::remove() {
    delete MainEventParser::getInstance();
}

void GlobalEventHandler::call(const sf::Event& event) {
    handlers_[0].call(event);
}

////////////////////////////////////////////////////////////
// MainEventParser implementation
////////////////////////////////////////////////////////////
MainEventParser::MainEventParser()
    : EventParser(MainEventType::MainEventCount) {
    WindowEventParser::getInstance()->attach(&handlers_[MainEventType::Window]);
    KeyboardParser::getInstance()->attach(&handlers_[MainEventType::Keyboard]);
    MouseParser::getInstance()->attach(&handlers_[MainEventType::Mouse]);
}

void MainEventParser::remove() {
    delete WindowEventParser::getInstance();
    delete KeyboardParser::getInstance();
    delete MouseParser::getInstance();
}

MainEventParser* MainEventParser::getInstance() {
    if (instance_ == nullptr) instance_ = new MainEventParser();
    return instance_;
}

void MainEventParser::call(const sf::Event& event) {
    size_t group;

    if (event.type >= sf::Event::Closed && event.type <= sf::Event::GainedFocus)
        group = MainEventType::Window;

    else if (event.type >= sf::Event::TextEntered &&
             event.type <= sf::Event::KeyReleased)
        group = MainEventType::Keyboard;

    else if (event.type >= sf::Event::MouseWheelMoved &&
             event.type <= sf::Event::MouseLeft)
        group = MainEventType::Mouse;

    else if (event.type >= sf::Event::JoystickButtonPressed &&
             event.type <= sf::Event::JoystickDisconnected)
        group = MainEventType::Joystick;

    else if (event.type >= sf::Event::TouchBegan &&
             event.type <= sf::Event::TouchEnded)
        group = MainEventType::Touch;

    else if (event.type == sf::Event::SensorChanged)
        group = MainEventType::Sensor;

    handlers_[group].call(event);
}

EventHandler* attachGlobalEventHandler(EventHandler* handler) {
    return GlobalEventHandler::getInstance()->addHandler(handler, 0);
}

EventHandler* attachGlobalEventHandler(std::function<void(const sf::Event&)> callable) {
    return attachGlobalEventHandler(new CallableHandler(callable));
}

EventHandler* attachMainEventHandler(size_t group, EventHandler* handler) {
    return MainEventParser::getInstance()->addHandler(handler, group);
}

EventHandler* attachMainEventHandler(size_t group, std::function<void(const sf::Event&)> callable) {
    return attachMainEventHandler(group, new CallableHandler(callable));
}

}  // namespace tmb
