#pragma once
#include "Event.h"
#include "Framework/Singleton.h"
#include <map>
#include <list>
#include <functional>

#define EVENT_SUBSCRIBE(id, function)	meow::EventManager::Instance().Subscribe(id, this, std::bind(&function, this, std::placeholders::_1));
#define EVENT_UNSUBSCRIBE(id)			meow::EventManager::Instance().Unsubscribe(id, this);
#define EVENT_DISPATCH(id, data)	    meow::EventManager::Instance().DispatchEvent(id, data)
namespace meow
{
	class IEventListener {

	};

	class EventManager : public Singleton<EventManager>
	{
	public:
		using eventFunction = std::function<void(const Event&)>;

		struct EventDispatcher
		{
			IEventListener* listener = nullptr; //obj that listens for events
			eventFunction function; //function caolled on event
		};
	public:
		void Update(float dt);

		void Subscribe(Event::id_t id, IEventListener* listener, eventFunction function);
		void Unsubscribe(Event::id_t id, IEventListener* listener);

		void DispatchEvent(Event::id_t id, Event::data_t data);

		friend class Singleton;

	private:
		EventManager() = default;
		std::map<Event::id_t, std::list<EventDispatcher>> m_dispatchers;


	};
}