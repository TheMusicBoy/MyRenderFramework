#ifndef _TMB_RENDER_OBJECT_HPP_
#define _TMB_RENDER_OBJECT_HPP_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <RenderFW/impl/values.hpp>
#include <list>
#include <mutex>

namespace tmb {

class RenderObject;

////////////////////////////////////////////////////////////
/// \brief Object used for containing RenderObjects
////////////////////////////////////////////////////////////
class RenderList : public sf::Drawable {
 public:
    using ObjectList = std::list<RenderObject*>;
    using ObjectPos  = typename ObjectList::iterator;

 protected:
    mutable std::recursive_mutex lock_;
    ObjectList object_list_;

 public:
    RenderList();
    ~RenderList();

    ObjectPos addRenderObject(RenderObject* object, bool visible = true);
    void removeRenderObject(ObjectPos position);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

////////////////////////////////////////////////////////////
/// \brief Object used control rendering of object.
////////////////////////////////////////////////////////////
class RenderObject : public sf::Drawable {
 protected:
    using RenderPos = typename RenderList::ObjectPos;
    using ObjectPtr = sf::Drawable*;

    RenderList* container_;
    RenderPos position_;
    ObjectPtr object_;
    bool visible_;

 public:
    RenderObject(ObjectPtr object, RenderList* container, bool visible);

    ~RenderObject();

    void set_visible(bool visible);

    bool get_visible();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

/**
 * \brief Safely detaches object and turning pointer to nullptr.
 */
bool detach(RenderObject** object);

}  // namespace tmb

#endif