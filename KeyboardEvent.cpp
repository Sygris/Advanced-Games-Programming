#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
    : m_eventType(EventType::Invalid), m_key{0u}
{
}

KeyboardEvent::KeyboardEvent(const EventType eventType, const unsigned char key)
    : m_eventType{eventType}, m_key{key}
{
}

bool KeyboardEvent::IsPressed() const
{
    return m_eventType == EventType::Press;
}

bool KeyboardEvent::IsReleased() const
{
    return m_eventType == EventType::Release;
}

bool KeyboardEvent::IsInvalid() const
{
    return m_eventType == EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const
{
    return m_key;
}
