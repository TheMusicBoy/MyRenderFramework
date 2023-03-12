#ifndef _TMB_RENDER_IMPL_HPP_
#define _TMB_RENDER_IMPL_HPP_

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <impl/render_object.hpp>
#include <impl/values.hpp>
#include <impl/event_handler.hpp>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace tmb {

class Controller;

////////////////////////////////////////////////////////////
/// \brief Object that contains events.
////////////////////////////////////////////////////////////
class EventQueue : protected std::list<sf::Event> {
 protected:
    using Base = std::list<sf::Event>;

    std::recursive_mutex lock_;

 public:
    EventQueue();
    EventQueue(const EventQueue&) = delete;
    EventQueue(EventQueue&&);
    ~EventQueue();

    EventQueue& operator=(EventQueue&& other);

    bool pollEvent(sf::Event* event);
    void push(const sf::Event& event);

    void clear();
};

////////////////////////////////////////////////////////////
/// \brief Object used to draw objects and record window
/// events.
////////////////////////////////////////////////////////////
class Render : public sf::RenderWindow {
 protected:
    using RenderMas = std::vector<RenderList>;

    RenderMas render_mas_;
    EventQueue event_queue_;
    UniqueHandlerList handlers_;

    std::recursive_mutex event_lock_;
    std::recursive_mutex render_lock_;

    struct WindowSettings {
        sf::VideoMode video_mode;
        std::wstring title;
        uint32_t style;
        sf::ContextSettings context;

        WindowSettings();
        WindowSettings(sf::VideoMode video_mode, const std::wstring& title,
                       uint32_t style, const sf::ContextSettings& context);
    };

    WindowSettings window_settings;

 public:
    Render(uint32_t layer_number);

    ~Render();

    void setWindowPrefs(
        sf::VideoMode video_mode, const std::wstring& title,
        uint32_t style                     = sf::Style::Default,
        const sf::ContextSettings& context = sf::ContextSettings());

    RenderObject* addRenderObject(sf::Drawable* object, uint32_t layout,
                                  bool visible = true);

    void removeRenderObject(const typename RenderList::ObjectPos& iterator);

    void update();

    void getEvents(EventQueue* event_queue);

    void render();

    void process();
};

}  // namespace tmb

#endif