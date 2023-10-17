#include "EventDriven.h"
#include "Event.h"

void EventDriven::start() {
    while (true) {
        auto events = epollfd.wait();
        for (auto& event : events)
            event->handleEvent();
    }
}
