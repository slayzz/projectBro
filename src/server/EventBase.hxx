#ifndef WEBSOCKET_EVENTBASE_HXX
#define WEBSOCKET_EVENTBASE_HXX

#include <event2/event.h>

class EventBase {
 public:
  EventBase();
  virtual ~EventBase();

  event_base* getEventBase();
 private:
  event_base* eventBase_;
};

#endif
