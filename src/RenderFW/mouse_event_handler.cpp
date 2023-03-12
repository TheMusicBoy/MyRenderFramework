#include <RenderFW/mouse_event_handler.hpp>

namespace tmb {

////////////////////////////////////////////////////////////
// MouseParser implementation
////////////////////////////////////////////////////////////

MouseParser::MouseParser() : EventParser(MouseEventType::MouseEventCount) {
    MouseButtonParser::getInstance()->attach(
        &handlers_[MouseEventType::MouseButton]);
    MouseMovementParser::getInstance()->attach(
        &handlers_[MouseEventType::MouseMovement]);
}

void MouseParser::remove() {
    delete MouseButtonParser::getInstance();
    delete MouseMovementParser::getInstance();
}

MouseParser* MouseParser::getInstance() {
    if (instance_ == nullptr) instance_ = new MouseParser();
    return instance_;
}

void MouseParser::call(const sf::Event& event) {
    size_t group;

    if (event.type >= sf::Event::MouseWheelMoved &&
        event.type <= sf::Event::MouseWheelScrolled)
        group = MouseEventType::MouseWheel;
    else if (event.type >= sf::Event::MouseButtonPressed &&
             event.type <= sf::Event::MouseButtonReleased)
        group = MouseEventType::MouseButton;
    else
        group = MouseEventType::MouseMovement;

    handlers_[group].call(event);
}

////////////////////////////////////////////////////////////
// MouseButtonParser
////////////////////////////////////////////////////////////

MouseButtonParser::MouseButtonParser()
    : EventParser(MouseButtonType::ButtonCount),
      button_actions_(MouseButtonType::ButtonCount) {
    for (int i = 0; i < MouseButtonType::ButtonCount; i++) {
        button_actions_[i] = new ButtonActionParser();
        button_actions_[i]->attach(&handlers_[i]);
    }
}

void MouseButtonParser::remove() {
    for (auto& el : button_actions_) delete el;
}

MouseButtonParser* MouseButtonParser::getInstance() {
    if (instance_ == nullptr) instance_ = new MouseButtonParser();
    return instance_;
}

void MouseButtonParser::call(const sf::Event& event) {
    handlers_[event.mouseButton.button].call(event);
}

////////////////////////////////////////////////////////////
// ButtonActionParser implementation
////////////////////////////////////////////////////////////

ButtonActionParser::ButtonActionParser()
    : EventParser(ButtonActionType::ButtonActionCount) {}

void ButtonActionParser::call(const sf::Event& event) {
    size_t group = event.type - sf::Event::MouseButtonPressed;
    handlers_[group].call(event);
}

////////////////////////////////////////////////////////////
// MouseMovementParser implementation
////////////////////////////////////////////////////////////

MouseMovementParser::MouseMovementParser()
    : EventParser(MouseMovementType::MouseMovementCount) {}

MouseMovementParser* MouseMovementParser::getInstance() {
    if (instance_ == nullptr) instance_ = new MouseMovementParser();
    return instance_;
}

void MouseMovementParser::call(const sf::Event& event) {
    size_t group = event.type - sf::Event::MouseMoved;
    handlers_[group].call(event);
}

////////////////////////////////////////////////////////////
// Attaching functions
////////////////////////////////////////////////////////////

EventHandler* attachMouseEventHandler(uint32_t group, EventHandler* handler) {
    return MouseParser::getInstance()->addHandler(handler, group);
}

EventHandler* attachMouseEventHandler(uint32_t group, std::function<void(const sf::Event&)> callable) {
    return attachMouseEventHandler(group, new CallableHandler(callable));
}


EventHandler* attachMouseButtonHandler(uint32_t button, EventHandler* handler) {
    return MouseButtonParser::getInstance()->addHandler(handler, button);
}

EventHandler* attachMouseButtonHandler(uint32_t button, std::function<void(const sf::Event&)> callable) {
    return attachMouseButtonHandler(button, new CallableHandler(callable));
}

EventHandler* attachMouseButtonPressHandler(uint32_t button,
                                              EventHandler* handler) {
    ButtonActionParser* parser =
        MouseButtonParser::getInstance()->button_actions_[button];
    return parser->addHandler(handler, ButtonActionType::ButtonPressed);
}

EventHandler* attachMouseButtonPressHandler(uint32_t button, std::function<void(const sf::Event&)> callable) {
    return attachMouseButtonPressHandler(button, new CallableHandler(callable));
}

EventHandler* attachMouseButtonReleaseHandler(uint32_t button,
                                              EventHandler* handler) {
    ButtonActionParser* parser =
        MouseButtonParser::getInstance()->button_actions_[button];
    return parser->addHandler(handler, ButtonActionType::ButtonReleased);
}

EventHandler* attachMouseButtonReleaseHandler(uint32_t button, std::function<void(const sf::Event&)> callable) {
    return attachMouseButtonReleaseHandler(button, new CallableHandler(callable));
}

EventHandler* attachMouseMovementHandler(uint32_t group,
                                         EventHandler* handler) {
    return MouseMovementParser::getInstance()->addHandler(handler, group);
}

EventHandler* attachMouseMovementHandler(uint32_t group, std::function<void(const sf::Event&)> callable) {
    return attachMouseMovementHandler(group, new CallableHandler(callable));
}

}  // namespace tmb
