#ifndef TEST_ENTRY_SERVICE
#define TEST_ENTRY_SERVICE

#include <unity.h>
#include "../src/Services/EntryService.h"
#include "../src/Repositories/EntryRepository.h"

void test_isEntryExpired() {
    EntryRepository repository;
    EntryService service(repository);

    Entry expiredEntry("1", "Service1", "User1", "Pass1", 0);
    expiredEntry.setExpiresAt(std::time(nullptr) - 10); // Expried

    Entry validEntry("2", "Service2", "User2", "Pass2", 0);
    validEntry.setExpiresAt(std::time(nullptr) + 10); // Not expired

    TEST_ASSERT_TRUE(service.isEntryExpired(expiredEntry));
    TEST_ASSERT_FALSE(service.isEntryExpired(validEntry));
}

void test_add_and_find_entry() {
    EntryRepository repository;
    EntryService service(repository);

    Entry entry("1", "Service1", "User1", "Pass1", 0);
    service.addEntry(entry);

    Entry foundEntry = service.findEntryById("1");

    TEST_ASSERT_EQUAL_STRING("Service1", foundEntry.getServiceName().c_str());
    TEST_ASSERT_EQUAL_STRING("User1", foundEntry.getUsername().c_str());
}

void test_update_entry() {
    EntryRepository repository;
    EntryService service(repository);

    Entry entry("1", "Service1", "User1", "Pass1", 0);
    service.addEntry(entry);

    Entry updatedEntry("1", "UpdatedService", "UpdatedUser", "UpdatedPass", 1);
    bool result = service.updateEntry(entry, updatedEntry);

    TEST_ASSERT_TRUE(result);

    Entry foundEntry = service.findEntryById("1");
    TEST_ASSERT_EQUAL_STRING("UpdatedService", foundEntry.getServiceName().c_str());
    TEST_ASSERT_EQUAL_STRING("UpdatedUser", foundEntry.getUsername().c_str());
}

void test_get_all_entries() {
    EntryRepository repository;
    EntryService service(repository);

    service.addEntry(Entry("1", "Service1", "User1", "Pass1", 0));
    service.addEntry(Entry("2", "Service2", "User2", "Pass2", 0));

    std::vector<Entry> entries = service.getAllEntries();

    TEST_ASSERT_EQUAL(2, entries.size());
    TEST_ASSERT_EQUAL_STRING("Service1", entries[0].getServiceName().c_str());
    TEST_ASSERT_EQUAL_STRING("Service2", entries[1].getServiceName().c_str());
}

void test_delete_only_selected_duplicate_entry() {
    EntryRepository repository;
    EntryService service(repository);
    Entry first("1", "Same", "user", "pass-one", 0);
    Entry second("2", "Same", "user", "pass-two", 0);
    service.addEntry(first);
    service.addEntry(second);

    service.deleteEntry(second);

    auto entries = service.getAllEntries();
    TEST_ASSERT_EQUAL(1, entries.size());
    TEST_ASSERT_EQUAL_STRING("1", entries[0].getId().c_str());
}

void test_updateField() {
    EntryRepository repository;
    EntryService service(repository);

    // Add entry
    Entry entry("1", "Service1", "User1", "Pass1", 0);
    service.addEntry(entry);

    // Update user
    Field usernameField("User", "UpdatedUser");
    bool usernameResult = service.updateField(entry, usernameField);

    TEST_ASSERT_TRUE(usernameResult);
    Entry updatedEntry = service.findEntryById("1");
    TEST_ASSERT_EQUAL_STRING("UpdatedUser", updatedEntry.getUsername().c_str());

    // Update password
    Field passwordField("Pass", "UpdatedPass");
    bool passwordResult = service.updateField(entry, passwordField);

    TEST_ASSERT_TRUE(passwordResult);
    updatedEntry = service.findEntryById("1");
    TEST_ASSERT_EQUAL_STRING("UpdatedPass", updatedEntry.getPassword().c_str());

    // Update notes
    Field notesField("Note", "UpdatedNote");
    bool notesResult = service.updateField(entry, notesField);

    TEST_ASSERT_TRUE(notesResult);
    updatedEntry = service.findEntryById("1");
    TEST_ASSERT_EQUAL_STRING("UpdatedNote", updatedEntry.getNotes().c_str());

    // Invalid field
    Field invalidField("invalidField", "SomeValue");
    bool invalidResult = service.updateField(entry, invalidField);

    TEST_ASSERT_FALSE(invalidResult);
}


#endif // TEST_ENTRY_SERVICE
