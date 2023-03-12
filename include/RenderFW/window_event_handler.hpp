#ifndef _TMB_WINDOW_EVENT_HANDLER_HPP_
#define _TMB_WINDOW_EVENT_HANDLER_HPP_

#include <RenderFW/impl/event_handler.hpp>

namespace tmb {

enum WindowEventType { Closed = 0, Resized, Focus, WindowEventCount };

class WindowEventParser : public EventParser {
 private:
    inline static WindowEventParser* instance_;

 public:
    WindowEventParser();

    static WindowEventParser* getInstance();

    void remove() override;

    void call(const sf::Event& event);
};

enum FocusEventType { LostFocus = 0, GainedFocus, FocusEventCount };

class FocusEventParser : public EventParser {
 private:
    inline static FocusEventParser* instance_;

 public:
    FocusEventParser();

    static FocusEventParser* getInstance();

    void call(const sf::Event& event);
};

////////////////////////////////////////////////////////////
// Attaching functions
////////////////////////////////////////////////////////////

EventHandler* attachWindowEventHandler(uint32_t group, EventHandler* handler);
EventHandler* attachWindowEventHandler(uint32_t group, std::function<void(const sf::Event&)> callable);

EventHandler* attachFocusEventHandler(uint32_t group, EventHandler* handler);
EventHandler* attachFocusEventHandler(uint32_t group, std::function<void(const sf::Event&)> callable);


}  // namespace tmb

#endif