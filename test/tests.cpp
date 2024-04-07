// Copyright 2021 GHA Test Team

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include "TimedDoor.h"

class MockTimerClient : public TimerClient {
 public:
    MOCK_METHOD(void, Timeout, ());
    MOCK_METHOD(bool, tregister, (int, TimedDoor*));
};

class TimedDoorTest : public ::testing::Test {
 protected:
    TimedDoor* door;
    MockTimerClient mockClient;
    Timer timer;
    void SetUp() override {
        door = new TimedDoor(10);
        timer = Timer();
    }

    void TearDown() override {
        delete door;
    }
};

TEST_F(TimedDoorTest, TEST_DOOR_Is_INITIALLY_CLOSED) {
    EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, TEXT_UNLOCK_DOOR) {
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, TEST_LOCK_DOOR) {
    door->unlock();
    door->lock();
    EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, TEST_LOCK_AND_UNLOCK_EXCEPTIONS) {
    EXPECT_THROW(door->lock(); , std::logic_error);
    door->unlock();
    EXPECT_THROW(door->unlock(); , std::logic_error);
}

TEST_F(TimedDoorTest, TEST_NO_THROW_STATE) {
    EXPECT_NO_THROW(door->throwState(););
}

TEST_F(TimedDoorTest, TEST_THROW_STATE) {
    door->unlock();
    EXPECT_THROW(door->throwState(); , std::runtime_error);
}

TEST_F(TimedDoorTest, TEST_ADAPTER_TIMEOUT) {
    DoorTimerAdapter adapter(*door);
    door->unlock();
    EXPECT_THROW(adapter.Timeout(), std::runtime_error);
}

TEST_F(TimedDoorTest, TEST_MOCK_TIMEOUT) {
    EXPECT_CALL(mockClient, Timeout()).Times(0);
    door->unlock();
    door->lock();
    EXPECT_NO_THROW(timer.tregister(door->getTimeOut(), door));
}

TEST_F(TimedDoorTest, TEST_MOCK_REGISTER) {
    EXPECT_CALL(mockClient, tregister(10, door)).Times(0);
    timer.tregister(10, door);
}

TEST_F(TimedDoorTest, TEST_REGISTER) {
    door->unlock();
    EXPECT_THROW(timer.tregister(10, door); , std::runtime_error);
}
