#ifndef TEST_FIELD_ACTION_SELECTOR_H
#define TEST_FIELD_ACTION_SELECTOR_H

#include <unity.h>
#include "../src/Selectors/FieldActionSelector.h"
#include "../src/Managers/InactivityManager.h"
#include "../Views/MockView.h"
#include "../Inputs/MockInput.h"

void test_field_action_selector_send_to_usb() {
    MockView mockView;
    MockInput mockInput;
    InactivityManager inactivityManager(mockView);
    FieldActionSelector fieldActionSelector(mockView, mockInput, inactivityManager);

    std::string label = "Test Label";
    std::string value = "Test Value";

    mockInput.enqueueKey(KEY_OK);

    ActionEnum action = fieldActionSelector.select(label, value);

    TEST_ASSERT_EQUAL(ActionEnum::SendToUsb, action);
    TEST_ASSERT_TRUE(mockView.topBarCalled);
    TEST_ASSERT_TRUE(mockView.valueCalled);
}

void test_field_action_selector_send_to_usb_and_enter() {
    MockView mockView;
    MockInput mockInput;
    InactivityManager inactivityManager(mockView);
    FieldActionSelector fieldActionSelector(mockView, mockInput, inactivityManager);

    mockInput.enqueueKey(KEY_OK_LONG);

    ActionEnum action = fieldActionSelector.select("Pass", "secret");

    TEST_ASSERT_EQUAL(ActionEnum::SendToUsbAndEnter, action);
}

void test_field_action_selector_update_field() {
    MockView mockView;
    MockInput mockInput;
    InactivityManager inactivityManager(mockView);
    FieldActionSelector fieldActionSelector(mockView, mockInput, inactivityManager);

    std::string label = "Test Label";
    std::string value = "Test Value";

    // M is Update shortcut
    mockInput.enqueueKey('m');

    ActionEnum action = fieldActionSelector.select(label, value);

    TEST_ASSERT_EQUAL(ActionEnum::UpdateField, action);
    TEST_ASSERT_TRUE(mockView.topBarCalled);
    TEST_ASSERT_TRUE(mockView.valueCalled);
}

void test_field_action_selector_cancel() {
    MockView mockView;
    MockInput mockInput;
    InactivityManager inactivityManager(mockView);
    FieldActionSelector fieldActionSelector(mockView, mockInput, inactivityManager);

    std::string label = "Test Label";
    std::string value = "Test Value";

    mockInput.enqueueKey(KEY_ARROW_LEFT);

    ActionEnum action = fieldActionSelector.select(label, value);

    TEST_ASSERT_EQUAL(ActionEnum::None, action);
    TEST_ASSERT_TRUE(mockView.topBarCalled);
    TEST_ASSERT_TRUE(mockView.valueCalled);
}

#endif // TEST_FIELD_ACTION_SELECTOR_H
