#ifndef _TMB_MAIN_EVENT_HANDLER_HPP_
#define _TMB_MAIN_EVENT_HANDLER_HPP_

#include <impl/event_handler.hpp>

namespace tmb {
    
    
class GlobalEventHandler : public EventParser {
 private:
    inline static GlobalEventHandler* instance_;

 public:
    GlobalEventHandler();

    static GlobalEventHandler* getInstance();

    void remove() override;
    void call(const sf::Event& event) override;
};

enum MainEventType {
    Window = 0,
    Keyboard,
    Mouse,
    Joystick,
    Touch,
    Sensor,
    MainEventCount
};

class MainEventParser : public EventParser {
 private:
    inline static MainEventParser* instance_;

 public:
    MainEventParser();

    static MainEventParser* getInstance();

    void remove() override;

    void call(const sf::Event& event) override;
};

EventHandler* attachGlobalEventHandler(EventHandler* handler);
EventHandler* attachGlobalEventHandler(std::function<void(const sf::Event&)> callable);

EventHandler* attachMainEventHandler(uint32_t group, EventHandler* handler);
EventHandler* attachMainEventHandler(uint32_t group, std::function<void(const sf::Event&)> callable);

}

#endif