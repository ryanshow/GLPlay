#pragma once

#include <functional>
#include <fmt/format.h>
#include <map>

namespace GLPlay {

class EventData {
};

template<typename T>
class EventSource {
public:
    EventSource() {};

    void GenerateEvent(const T event_type, const EventData & event_data);

    void RegisterHandler(const T event_type, std::function<void(const EventData&)> &&handler);

private:
    std::map<T, std::vector<std::function<void(const EventData&)>>> registered_handlers_;
};

template<typename T>
void EventSource<T>::GenerateEvent(const T event_type, const EventData & event_data) {
    for (auto handler : registered_handlers_[event_type]) {
        handler(event_data);
    }
}

template<typename T>
void EventSource<T>::RegisterHandler(const T event_type, std::function<void(const EventData&)> &&handler) {
    registered_handlers_[event_type].emplace_back(std::move(handler));
}

}