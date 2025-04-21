#pragma once
#include <functional>
#include <map>
#include <vector>
#include <typeindex>

// Un EventBus muy básico (puedes mejorarlo según necesidad)
class EventBus {
public:
    using Callback = std::function<void()>;

    template<typename Evt>
    static void subscribe(Callback cb) {
        auto& vec = getSubscribers(std::type_index(typeid(Evt)));
        vec.push_back(std::move(cb));
    }

    template<typename Evt>
    static void publish() {
        auto& vec = getSubscribers(std::type_index(typeid(Evt)));
        for (auto& cb : vec) cb();
    }

private:
    static std::map<std::type_index, std::vector<Callback>>& getSubscribers(std::type_index idx) {
        static std::map<std::type_index, std::vector<Callback>> subs;
        return subs[idx];
    }
};
