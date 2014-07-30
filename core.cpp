#include <core.hpp>
#include <wayland.hpp>

event_queue_t::queue_ptr::~queue_ptr()
{
  wl_event_queue_destroy(queue);
}

event_queue_t::  event_queue_t(wl_event_queue *q)
  : queue(new queue_ptr({q}))
{
}

proxy_t::proxy_ptr::~proxy_ptr()
{
  if(!display)
    wl_proxy_destroy(proxy);
  else
    wl_display_disconnect(reinterpret_cast<wl_display*>(proxy));
}

proxy_t proxy_t::marshal_single(uint32_t opcode, const wl_interface *interface, std::vector<wl_argument> v)
{
  if(interface)
    return proxy_t(wl_proxy_marshal_array_constructor(proxy->proxy, opcode, v.data(), interface));
  wl_proxy_marshal_array(proxy->proxy, opcode, v.data());
  return proxy_t();
}

proxy_t::proxy_t()
{
}

void proxy_t::add_dispatcher(wl_dispatcher_func_t dispatcher, void *data)
{
  if(proxy)
    wl_proxy_add_dispatcher(proxy->proxy, dispatcher, data, NULL);
}

proxy_t::proxy_t(wl_proxy *p, bool is_display)
  : proxy(new proxy_ptr({p, is_display}))
{
}
  
proxy_t::proxy_t(const proxy_t& p)
  : proxy(p.proxy)
{
}

uint32_t proxy_t::get_id()
{
  return wl_proxy_get_id(proxy->proxy);
}

std::string proxy_t::get_class()
{
  return wl_proxy_get_class(proxy->proxy);
}

void proxy_t::set_queue(event_queue_t queue)
{
  wl_proxy_set_queue(proxy->proxy, queue.queue->queue);
}

display_t::display_t(const proxy_t &p)
  : proxy_t(p)
{
}

display_t::display_t(int fd)
  : proxy_t(reinterpret_cast<wl_proxy*>(wl_display_connect_to_fd(fd)), true)
{
}

display_t::display_t(std::string name)
  : proxy_t(reinterpret_cast<wl_proxy*>(wl_display_connect(name == "" ? NULL : name.c_str())), true)
{
}

event_queue_t display_t::create_queue()
{
  return wl_display_create_queue(reinterpret_cast<wl_display*>(proxy->proxy));
}

int display_t::get_fd()
{
  return wl_display_get_fd(reinterpret_cast<wl_display*>(proxy->proxy));
}

int display_t::roundtrip()
{
  return wl_display_roundtrip(reinterpret_cast<wl_display*>(proxy->proxy));
}    

int display_t::read_events()
{
  return wl_display_read_events(reinterpret_cast<wl_display*>(proxy->proxy));
}    

int display_t::prepare_read()
{
  return wl_display_prepare_read(reinterpret_cast<wl_display*>(proxy->proxy));
}    

int display_t::prepare_read_queue(event_queue_t queue)
{
  return wl_display_prepare_read_queue(reinterpret_cast<wl_display*>(proxy->proxy), queue.queue->queue);
}    

void display_t::cancel_read()
{
  wl_display_cancel_read(reinterpret_cast<wl_display*>(proxy->proxy));
}    

int display_t::dispatch_queue(event_queue_t queue)
{
  return wl_display_dispatch_queue(reinterpret_cast<wl_display*>(proxy->proxy), queue.queue->queue);
}    

int display_t::dispatch_queue_pending(event_queue_t queue)
{
  return wl_display_dispatch_queue_pending(reinterpret_cast<wl_display*>(proxy->proxy), queue.queue->queue);
}    

int display_t::dispatch()
{
  return wl_display_dispatch(reinterpret_cast<wl_display*>(proxy->proxy));
}    

int display_t::dispatch_pending()
{
  return wl_display_dispatch_pending(reinterpret_cast<wl_display*>(proxy->proxy));
}    

int display_t::get_error()
{
  return wl_display_get_error(reinterpret_cast<wl_display*>(proxy->proxy));
}    

int display_t::flush()
{
  return wl_display_flush(reinterpret_cast<wl_display*>(proxy->proxy));
}    

callback_t display_t::sync()
{
  return marshal_constructor(0, &wl_callback_interface, NULL);
}

registry_t display_t::get_registry()
{
  return marshal_constructor(1, &wl_registry_interface, NULL);
}