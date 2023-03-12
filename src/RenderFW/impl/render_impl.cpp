#include <impl/render_impl.hpp>
#include <impl/render_object.hpp>
#include <iostream>
#include <mutex>
#include <thread>

namespace tmb {

////////////////////////////////////////////////////////////
// EventQueue implementation
////////////////////////////////////////////////////////////

bool EventQueue::pollEvent(sf::Event* event) {
    std::lock_guard lock(lock_);
    if (this->empty()) return false;

    *event = this->front();
    return true;
}

void EventQueue::push(const sf::Event& event) {
    std::lock_guard lock(lock_);
    this->push_back(event);
}

void EventQueue::clear() {
    std::lock_guard lock(lock_);
    this->clear();
}

////////////////////////////////////////////////////////////
/// \brief WindowSettings implementation
////////////////////////////////////////////////////////////

Render::WindowSettings::WindowSettings()
    : video_mode(sf::VideoMode::getDesktopMode()),
      title(),
      style(sf::Style::Default),
      context(sf::ContextSettings()) {}

Render::WindowSettings::WindowSettings(sf::VideoMode video_mode,
                                       const std::wstring& title,
                                       uint32_t style,
                                       const sf::ContextSettings& context)
    : video_mode(video_mode), title(title), style(style), context(context) {}

////////////////////////////////////////////////////////////
// Render implementation
////////////////////////////////////////////////////////////

Render::Render(uint32_t layer_number) : render_mas_(layer_number) {}

Render::~Render() {
    render_lock_.lock();
    event_lock_.lock();
    event_lock_.unlock();
    render_lock_.unlock();
}

void Render::setWindowPrefs(sf::VideoMode video_mode, const std::wstring& title,
                            uint32_t style,
                            const sf::ContextSettings& context) {
    window_settings = {video_mode, title, style, context};

    render_lock_.lock();
    if (this->isOpen()) this->create(video_mode, title, style, context);
    render_lock_.unlock();
}

RenderObject* Render::addRenderObject(sf::Drawable* object, uint32_t layer,
                                      bool visible) {
    std::lock_guard lock(render_lock_);
    return new RenderObject(object, &render_mas_[layer], visible);
}

void Render::render() {
    this->clear();

    render_lock_.lock();
    for (auto& layer : render_mas_) this->draw(layer);
    render_lock_.unlock();

    this->display();
}

void Render::process() {
    if (!this->isOpen())
        this->create(window_settings.video_mode, window_settings.title,
                     window_settings.style, window_settings.context);
    while (this->isOpen()) this->render();
}

}  // namespace tmb