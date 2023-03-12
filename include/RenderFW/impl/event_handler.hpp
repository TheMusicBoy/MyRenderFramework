#ifndef _TMB_EVENT_HANDLER_HPP_
#define _TMB_EVENT_HANDLER_HPP_

#include <SFML/Window/Event.hpp>
#include <functional>
#include <list>
#include <mutex>
#include <vector>

namespace tmb {

class HandlerContainer;

class EventHandler {
 protected:
    using Container = HandlerContainer;

 private:
    using Position = typename std::list<EventHandler*>::iterator;

    Container* container_;
    Position position_;

 public:
    EventHandler();
    ~EventHandler();

    void attach(Container* container);

    virtual void remove();
    virtual void call(const sf::Event& event) = 0;
};

class HandlerContainer {
 protected:
    using HandlerList = std::list<EventHandler*>;
    using HandlerPos = typename HandlerList::iterator;

    std::recursive_mutex lock_;
    HandlerList handlers_;

 public:
    HandlerContainer();
    ~HandlerContainer();

    void call(const sf::Event& event);

    HandlerPos push(EventHandler* handler);
    void pop(HandlerPos position);
    void clear();
};

class EventParser : public EventHandler {
 protected:
    using HandlerList = std::vector<HandlerContainer>;

 public:
    HandlerList handlers_;

    EventParser(size_t group_number);
    ~EventParser();

    virtual void call(const sf::Event& event) = 0;

    EventHandler* addHandler(EventHandler* handler, uint32_t group);
};

class CallableHandler : public EventHandler {
 private:
    std::function<void(const sf::Event&)> callable_;

 public:
    CallableHandler(std::function<void(const sf::Event&)> callable);

    void call(const sf::Event& event) override;
};

class UniqueHandlerList : protected std::list<EventHandler*> {
 protected:
    using Base = std::list<EventHandler*>;
    using HandlerPos = typename Base::iterator;

    std::recursive_mutex lock_;

    void move_(UniqueHandlerList&& other);

 public:
    UniqueHandlerList();
    UniqueHandlerList(const UniqueHandlerList&) = delete;
    UniqueHandlerList(UniqueHandlerList&&);
    ~UniqueHandlerList();

    UniqueHandlerList& operator=(const UniqueHandlerList&) = delete;
    UniqueHandlerList& operator=(UniqueHandlerList&&);

    HandlerPos push(EventHandler* handler);
    void erase(HandlerPos position);
    void remove(HandlerPos position);
};

}  // namespace tmb

#endif