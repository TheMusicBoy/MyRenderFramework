#ifndef _TMB_CONTROLLER_HPP_
#define _TMB_CONTROLLER_HPP_

#include <RenderFW/impl/render_impl.hpp>
#include <RenderFW/impl/event_handler.hpp>
#include <mutex>

namespace tmb {

class Controller {
 private:
    HandlerContainer global_handler_;
    Render* render_instance_;
    UniqueHandlerList handlers_;
    std::recursive_mutex lock_;


 public:
    Controller();
    ~Controller();

    void attach(Render* render_instance_);

    void call();

    void process();
};



}  // namespace tmb

#endif