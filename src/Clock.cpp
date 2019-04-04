#include "Clock.h"

Clock::Clock(): m_initialTime{0}, m_currentTime{0}, m_increment{1} {}

Clock::Clock(unsigned int init, unsigned int inc): m_initialTime{init}, m_currentTime{init}, m_increment{inc} {}


Clock::~Clock() {}

void Clock::tick() {
    m_currentTime += m_increment;
}

