#include <impl/render_object.hpp>
#include <iostream>

namespace tmb {

////////////////////////////////////////////////////////////
// RenderList implementation
////////////////////////////////////////////////////////////

RenderList::RenderList() : object_list_() {}

RenderList::~RenderList() { std::lock_guard lock(lock_); }

std::list<RenderObject*>::iterator RenderList::addRenderObject(
    RenderObject* object, bool visible) {
    std::lock_guard lock(lock_);
    object_list_.push_back(object);
    ObjectPos position = object_list_.end();
    return --position;
}

void RenderList::removeRenderObject(ObjectPos position) {
    std::lock_guard lock(lock_);
    object_list_.erase(position);
}

void RenderList::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    std::lock_guard lock(lock_);
    for(auto& el : object_list_)
        el->draw(target, states);
}

////////////////////////////////////////////////////////////
// RenderObject implementation
////////////////////////////////////////////////////////////

RenderObject::RenderObject(ObjectPtr object, RenderList* container,
                           bool visible)
    : object_(object), container_(container), visible_(visible) {
    position_ = container_->addRenderObject(this, visible);
}

RenderObject::~RenderObject() { container_->removeRenderObject(position_); }

void RenderObject::set_visible(bool visible) { visible_ = visible; }

bool RenderObject::get_visible() { return visible_; }

void RenderObject::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
    if (visible_) target.draw(*object_, states);
}

bool detach(RenderObject** object) {
    if (*object == nullptr) return false;

    try {
        delete *object;
        object = nullptr;
    } catch (...) {
        throw;
    }

    return true;
}

}  // namespace tmb