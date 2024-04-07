// Copyright 2024 Vinichuk Timofey
#include "TimedDoor.h"

#include <stdexcept>
#include <thread> // NOLINT [build/c++11]
#include <chrono> // NOLINT [build/c++11]

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& td) : door(td) {}

void DoorTimerAdapter::Timeout() {
    door.throwState();
}

TimedDoor::TimedDoor(int timout) : iTimeout(timout), isOpened(false)  {
    adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    if (isOpened)
        throw std::logic_error("the door was open before");
    isOpened = true;
}

void TimedDoor::lock() {
    if (!isOpened)
        throw std::logic_error("the door was closed before");
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isOpened) {
        throw std::runtime_error("Door is still open!");
    }
}

void Timer::sleep(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}



void Timer::tregister(int timeout, TimedDoor* client) {
    sleep(timeout);
    client->throwState();
}
