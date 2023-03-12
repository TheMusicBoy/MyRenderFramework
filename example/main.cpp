#include <SFML/Graphics.hpp>
#include <RenderFW/mouse_event_handler.hpp>
#include <RenderFW/controller.hpp>
#include <thread>


int main() {
    tmb::Render render(1);
    render.setWindowPrefs(sf::VideoMode(400, 400), L"Proga");

    tmb::Controller controller;
    controller.attach(&render);

    tmb::UniqueHandlerList handlers;

    sf::CircleShape shape(30);
    shape.setRadius(40);
    shape.setPosition(200 - 30, 200 - 30);

    render.addRenderObject(&shape, 0);

    int num = 0;

    sf::Font font;
    font.loadFromFile("arialmt.ttf");
    sf::Text text(std::to_wstring(num), font);
    text.setPosition(10, 10);

    render.addRenderObject(&text, 0);

    handlers.push(tmb::attachMouseButtonReleaseHandler(
        tmb::MouseButtonType::Left, [&num, &text](const sf::Event&) {
            text.setString(std::to_wstring(++num));
        }));

    std::thread render_thread(&tmb::Render::process, std::ref(render));
    std::thread controller_thread(&tmb::Controller::process,
                                  std::ref(controller));

    render_thread.join();
    controller_thread.join();
}