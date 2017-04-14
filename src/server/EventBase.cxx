
#include "EventBase.hxx"
EventBase::EventBase() {
  event_config* eventConfig;

#ifndef _WIN32
  eventConfig = event_config_new();
  event_config_avoid_method(eventConfig, "select");
#endif

  if (eventConfig) {
    eventBase_ = event_base_new_with_config(eventConfig);
    event_config_free(eventConfig);
  } else {
    eventBase_ = event_base_new();
  }

}
EventBase::~EventBase() {
  event_base_free(eventBase_);
}
event_base *EventBase::getEventBase() {
  return eventBase_;
}
