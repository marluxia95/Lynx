/**
 * @file eventManager.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief This file contains all the event handling.
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include <queue>
#include "Events/event.h"
#include "lynx_common.h"

namespace Lynx {

    using EventCallbackFunc = std::function<void(const Event&)>;

    class LYNXENGINE_API EventListener {
        public:
            EventListener() {}
            EventListener(EventCallbackFunc function, unsigned int id) : m_callbackfunc(function), id(id) {}

            unsigned int GetID() { return id; }

            bool operator==(const EventListener& obj2) {
                if(id == obj2.id)
                    return true;

                return false;
            }

            void operator()(const Event& ev) {
                m_callbackfunc(ev);
            }

        private:
            unsigned int id = -1;
            EventCallbackFunc m_callbackfunc;
    };

    class LYNXENGINE_API EventManager {

        public:

            /**
             * @brief Adds a listener
             *
             * @param type Listener type
             * @param func Callback function
             * @return EventListener
             */
            static EventListener AddListener(const EventType& type, EventCallbackFunc&& func);

            /**
             * @brief Sends an event signal
             *
             * @param event
             */
            static void SendEvent(const Event& event);

            /**
             * @brief Removes a listener
             *
             * @param type Listener type
             * @param listener Listener
             */
            static void RemoveListener(const EventType& type, EventListener listener);

        private:
            friend class Application;
            friend class GameApplication;
#ifdef LYNX_MULTITHREAD
            /**
             * @brief Updates the event_queue, ONLY FOR MULTITHREAD
             *
             */
            static void UpdateListeners();
            static std::queue<Event> event_queue;
            static std::mutex queue_mutex;
#endif
            static std::map<EventType, std::vector<EventListener>> listeners;
            static unsigned int last_ev_id;
    };

}

#endif
