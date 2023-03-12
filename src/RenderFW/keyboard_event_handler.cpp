#include <RenderFW/keyboard_event_handler.hpp>

namespace tmb {

////////////////////////////////////////////////////////////
// KeyboardParser implementation
////////////////////////////////////////////////////////////
KeyboardParser::KeyboardParser()
    : EventParser(KeyboardEventType::KeyboardEventCount) {
    KeyParser::getInstance()->attach(&handlers_[KeyboardEventType::Keys]);
}

void KeyboardParser::remove() {}

KeyboardParser* KeyboardParser::getInstance() {
    if (instance_ == nullptr) instance_ = new KeyboardParser();
    return instance_;
}

void KeyboardParser::call(const sf::Event& event) {
    size_t group;

    if (event.type == sf::Event::TextEntered)
        group = KeyboardEventType::TextEntered;
    else
        group = KeyboardEventType::Keys;

    handlers_[group].call(event);
}

////////////////////////////////////////////////////////////
// KeyParser implementation
////////////////////////////////////////////////////////////
KeyParser::KeyParser()
    : EventParser(KeyCodes::KeyCount), key_actions_(KeyCodes::KeyCount) {
    for (int i = 0; i < KeyCodes::KeyCount; i++) {
        key_actions_[i] = new KeyActionParser();
        key_actions_[i]->attach(&handlers_[i]);
    }
}

void KeyParser::remove() {
    for (auto& el : key_actions_) delete el;
}

KeyParser* KeyParser::getInstance() {
    if (instance_ == nullptr) instance_ = new KeyParser();
    return instance_;
}

void KeyParser::call(const sf::Event& event) {
    handlers_[event.key.code].call(event);
}

////////////////////////////////////////////////////////////
// KeyActionParser implementation
////////////////////////////////////////////////////////////
KeyActionParser::KeyActionParser()
    : EventParser(KeyActionType::KeyActionCount) {}

void KeyActionParser::call(const sf::Event& event) {
    size_t group = event.type - sf::Event::KeyPressed;
    handlers_[group].call(event);
}

////////////////////////////////////////////////////////////
// Attaching functions
////////////////////////////////////////////////////////////

EventHandler* attachKeyboardHandler(uint32_t group, EventHandler* handler) {
    return KeyboardParser::getInstance()->addHandler(handler, group);
}

EventHandler* attachKeyboardHandler(uint32_t group, std::function<void(const sf::Event&)> callable) {
    return attachKeyboardHandler(group, new CallableHandler(callable));
}

EventHandler* attachKeyHandler(uint32_t key, EventHandler* handler) {
    return KeyParser::getInstance()->addHandler(handler, key);
}

EventHandler* attachKeyHandler(uint32_t key, std::function<void(const sf::Event&)> callable) {
    return attachKeyHandler(key, new CallableHandler(callable));
}

EventHandler* attachKeyPressHandler(uint32_t key, EventHandler* handler) {
    KeyActionParser* parser = KeyParser::getInstance()->key_actions_[key];
    return parser->addHandler(handler, KeyActionType::KeyPressed);
}

EventHandler* attachKeyPressHandler(uint32_t key, std::function<void(const sf::Event&)> callable) {
    return attachKeyPressHandler(key, new CallableHandler(callable));
}

EventHandler* attachKeyReleaseHandler(uint32_t key, EventHandler* handler) {
    KeyActionParser* parser = KeyParser::getInstance()->key_actions_[key];
    return parser->addHandler(handler, KeyActionType::KeyReleased);
}

EventHandler* attachKeyReleaseHandler(uint32_t key, std::function<void(const sf::Event&)> callable) {
    return attachKeyPressHandler(key, new CallableHandler(callable));
}

}  // namespace tmb