#ifndef _TMB_KEYBOARD_EVENT_HANDLER_HPP_
#define _TMB_KEYBOARD_EVENT_HANDLER_HPP_

#include <impl/event_handler.hpp>

namespace tmb {

enum KeyboardEventType { TextEntered = 0, Keys, KeyboardEventCount };

class KeyboardParser : public EventParser {
 private:
    inline static KeyboardParser* instance_;

 public:
    KeyboardParser();

    static KeyboardParser* getInstance();

    void remove() override;

    void call(const sf::Event& event) override;
};

class KeyActionParser;

using KeyCodes = sf::Keyboard;
class KeyParser : public EventParser {
 private:
    inline static KeyParser* instance_;

 public:
    std::vector<KeyActionParser*> key_actions_;

    KeyParser();

    static KeyParser* getInstance();

    void remove() override;

    void call(const sf::Event& event) override;
};

enum KeyActionType { KeyPressed = 0, KeyReleased, KeyActionCount };

class KeyActionParser : public EventParser {
 public:
    KeyActionParser();

    void call(const sf::Event& event) override;
};

EventHandler* attachKeyboardHandler(uint32_t group, EventHandler* handler);
EventHandler* attachKeyboardHandler(uint32_t group, std::function<void(const sf::Event&)> callable);

EventHandler* attachKeyHandler(uint32_t key, EventHandler* handler);
EventHandler* attachKeyHandler(uint32_t key, std::function<void(const sf::Event&)> callable);

EventHandler* attachKeyPressHandler(uint32_t key, EventHandler* handler);
EventHandler* attachKeyPressHandler(uint32_t key, std::function<void(const sf::Event&)> callable);

EventHandler* attachKeyReleaseHandler(uint32_t key, EventHandler* handler);
EventHandler* attachKeyReleaseHandler(uint32_t key, std::function<void(const sf::Event&)> callable);


}  // namespace tmb

#endif