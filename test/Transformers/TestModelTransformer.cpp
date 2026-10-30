#ifndef TEST_MODEL_TRANSFORMER_H
#define TEST_MODEL_TRANSFORMER_H

#include <unity.h>
#include "../src/Transformers/ModelTransformer.h"
#include "../src/Models/Entry.h"
#include "../src/Models/Category.h"

void test_transform_entries_to_strings() {
    std::vector<Entry> entries = {
        Entry("1", "Service1", "User1", "Pass1", 0),
        Entry("2", "Service2", "User2", "Pass2", 1)
    };

    auto strings = ModelTransformer::toStrings<Entry>(
        entries,
        [](const Entry& entry) { return entry.getServiceName(); }
    );

    TEST_ASSERT_EQUAL(2, strings.size());
    TEST_ASSERT_EQUAL_STRING("Service1", strings[0].c_str());
    TEST_ASSERT_EQUAL_STRING("Service2", strings[1].c_str());
}

void test_transform_categories_to_strings() {
    std::vector<Category> categories = {
        Category(1, "Personal", "#FF5733", "icon1.png"),
        Category(2, "Work", "#33FF57", "icon2.png")
    };

    auto strings = ModelTransformer::toStrings<Category>(
        categories,
        [](const Category& category) { return category.getName(); }
    );

    TEST_ASSERT_EQUAL(2, strings.size());
    TEST_ASSERT_EQUAL_STRING("Personal", strings[0].c_str());
    TEST_ASSERT_EQUAL_STRING("Work", strings[1].c_str());
}

void test_transform_single_entry_to_strings() {
    Entry entry("1", "Service1", "User1", "encryptedPass1", 0);
    entry.setNotes("This is a note");
    entry.setCreatedAt(1672531200);
    entry.setUpdatedAt(1672617600);

    auto strings = ModelTransformer::toStrings(entry);

    TEST_ASSERT_EQUAL(3, strings.size());
    TEST_ASSERT_EQUAL_STRING("encryptedPass1", strings[0].c_str());
    TEST_ASSERT_EQUAL_STRING("User1", strings[1].c_str());
    TEST_ASSERT_EQUAL_STRING("This is a note", strings[2].c_str());
}

#endif // TEST_MODEL_TRANSFORMER_H
