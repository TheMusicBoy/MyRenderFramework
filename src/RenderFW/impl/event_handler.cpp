#include <RenderFW/impl/event_handler.hpp>

namespace tmb {

////////////////////////////////////////////////////////////
// EventHandler implementation
////////////////////////////////////////////////////////////

EventHandler::EventHandler() {}

EventHandler::~EventHandler() { container_->pop(position_); }

void EventHandler::attach(Container* container) {
    container_ = container;
    position_  = container_->push(this);
}

void EventHandler::remove() {}

////////////////////////////////////////////////////////////
// HandlerContainer implementation
////////////////////////////////////////////////////////////

HandlerContainer::HandlerContainer() {}

HandlerContainer::~HandlerContainer() {}

void HandlerContainer::call(const sf::Event& event) {
    std::lock_guard lock(lock_);
    for (auto& el : handlers_) el->call(event);
}

std::list<EventHandler*>::iterator HandlerContainer::push(
    EventHandler* handler) {
    std::lock_guard lock(lock_);
    handlers_.push_back(handler);
    HandlerPos position = handlers_.end();
    return --position;
}

void HandlerContainer::pop(HandlerPos position) {
    std::lock_guard lock(lock_);
    handlers_.erase(position);
}

void HandlerContainer::clear() {
    std::lock_guard lock(lock_);
    for (auto& el : handlers_) el->remove();
    handlers_.clear();
}

////////////////////////////////////////////////////////////
// EventParser implementation
////////////////////////////////////////////////////////////

EventParser::EventParser(size_t group_number) : handlers_(group_number) {}

EventParser::~EventParser() {}

EventHandler* EventParser::addHandler(EventHandler* handler, uint32_t group) {
    handler->attach(&handlers_[group]);
    return handler;
}

////////////////////////////////////////////////////////////
// CallableHandler implementation
////////////////////////////////////////////////////////////

CallableHandler::CallableHandler(
    std::function<void(const sf::Event&)> callable)
    : callable_(callable) {}

void CallableHandler::call(const sf::Event& event) { callable_(event); }

////////////////////////////////////////////////////////////
// UniqueHandlerList implementation
////////////////////////////////////////////////////////////

UniqueHandlerList::UniqueHandlerList() : Base() {}

UniqueHandlerList::UniqueHandlerList(UniqueHandlerList&& other) {
    move_(std::move(other));
}

UniqueHandlerList::~UniqueHandlerList() {
    for(auto& el : *this)
        delete el;
    this->clear();
}

UniqueHandlerList& UniqueHandlerList::operator=(UniqueHandlerList&& other) {
    move_(std::move(other));
    return *this;
}

std::list<EventHandler*>::iterator UniqueHandlerList::push(EventHandler* handler) {
    std::lock_guard lock(lock_);
    this->push_back(handler);
    HandlerPos position = this->end();
    return --position;
}

void UniqueHandlerList::erase(HandlerPos position) {
    std::lock_guard lock(lock_);
    this->Base::erase(position);
}

void UniqueHandlerList::remove(HandlerPos position) {
    std::lock_guard lock(lock_);
    delete *position;
    this->Base::erase(position);
}

void UniqueHandlerList::move_(UniqueHandlerList&& other) {
    other.lock_.lock();
    this->lock_.lock();
    this->Base::operator=(std::move(*static_cast<Base*>(&other)));
    this->lock_.unlock();
    other.lock_.unlock();
}

}  // namespace tmb