#include <RenderFW/impl/render_impl.hpp>
#include <RenderFW/impl/render_object.hpp>
#include <RenderFW/window_event_handler.hpp>
#include <iostream>
#include <mutex>
#include <thread>

namespace tmb {

////////////////////////////////////////////////////////////
// EventQueue implementation
////////////////////////////////////////////////////////////

EventQueue::EventQueue() : Base() {}

EventQueue::EventQueue(EventQueue&& other) {
    other.lock_.lock();
    this->lock_.lock();

    this->Base::operator=(std::move(*static_cast<Base*>(&other)));

    this->lock_.unlock();
    other.lock_.unlock();
}

EventQueue::~EventQueue() { clear(); }

EventQueue& EventQueue::operator=(EventQueue&& other) {
    clear();

    std::lock_guard lock(lock_);
    this->Base::operator=(std::move(*static_cast<Base*>(&other)));
    return *this;
}

bool EventQueue::pollEvent(sf::Event* event) {
    std::lock_guard lock(lock_);
    if (this->empty()) return false;

    if (this->size() > 100) int a = 0;

    *event = this->front();
    this->pop_front();
    return true;
}

void EventQueue::push(const sf::Event& event) {
    std::lock_guard lock(lock_);
    this->push_back(event);
}

void EventQueue::clear() {
    std::lock_guard lock(lock_);
    this->Base::clear();
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

void Render::update() {
    event_lock_.lock();
    render_lock_.lock();
    sf::Event event;
    while (this->pollEvent(event)) event_queue_.push(event);
    render_lock_.unlock();
    event_lock_.unlock();
}

void Render::getEvents(EventQueue* event_queue) {
    std::lock_guard lock(event_lock_);
    *event_queue = std::move(event_queue_);
}

void Render::render() {
    this->clear();

    render_lock_.lock();
    for (auto& layer : render_mas_) this->draw(layer);
    render_lock_.unlock();

    this->display();
}

void Render::process() {
    bool running = true;

    handlers_.push(attachWindowEventHandler(
        WindowEventType::Closed,
        new CallableHandler(
            [&running](const sf::Event&) { running = false; })));

    if (!this->isOpen())
        this->create(window_settings.video_mode, window_settings.title,
                     window_settings.style, window_settings.context);

    while (running) {
        this->update();
        this->render();
    }

    this->close();
}

}  // namespace tmb