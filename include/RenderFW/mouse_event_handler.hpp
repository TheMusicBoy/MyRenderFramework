#ifndef _TMB_MOUSE_EVENT_HANDLER_HPP_
#define _TMB_MOUSE_EVENT_HANDLER_HPP_

#include <RenderFW/impl/event_handler.hpp>

namespace tmb {

enum MouseEventType { MouseWheel = 0, MouseButton, MouseMovement, MouseEventCount };

class MouseParser : public EventParser {
 private:
    inline static MouseParser* instance_;

 public:
    MouseParser();

    static MouseParser* getInstance();

    void remove() override;

    void call(const sf::Event& event) override;
};

class ButtonActionParser;

using MouseButtonType = sf::Mouse::Button;
class MouseButtonParser : public EventParser {
 private:
    inline static MouseButtonParser* instance_;

 public:
    std::vector<ButtonActionParser*> button_actions_;

    MouseButtonParser();

    static MouseButtonParser* getInstance();

    void remove() override;

    void call(const sf::Event& event) override;
};

enum ButtonActionType { ButtonPressed = 0, ButtonReleased, ButtonActionCount };

class ButtonActionParser : public EventParser {
 public:
    ButtonActionParser();

    void call(const sf::Event& event) override;
};

enum MouseMovementType { Moved = 0, Entered, Left, MouseMovementCount };

class MouseMovementParser : public EventParser {
 private:
    inline static MouseMovementParser* instance_;

 public:
    MouseMovementParser();

    static MouseMovementParser* getInstance();

    void call(const sf::Event& event) override;
};

EventHandler* attachMouseEventHandler(uint32_t group, EventHandler* handler);
EventHandler* attachMouseEventHandler(uint32_t group, std::function<void(const sf::Event&)> callable);

EventHandler* attachMouseButtonHandler(uint32_t button, EventHandler* handler);
EventHandler* attachMouseButtonHandler(uint32_t button, std::function<void(const sf::Event&)> callable);

EventHandler* attachMouseButtonPressHandler(uint32_t button, EventHandler* handler);
EventHandler* attachMouseButtonPressHandler(uint32_t button, std::function<void(const sf::Event&)> callable);

EventHandler* attachMouseButtonReleaseHandler(uint32_t button, EventHandler* handler);
EventHandler* attachMouseButtonReleaseHandler(uint32_t button, std::function<void(const sf::Event&)> callable);

EventHandler* attachMouseMovementHandler(uint32_t group, EventHandler* handler);
EventHandler* attachMouseMovementHandler(uint32_t group, std::function<void(const sf::Event&)> callable);


}  // namespace tmb

#endif