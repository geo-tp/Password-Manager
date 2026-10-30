#include <unity.h>

#include "Services/TestCryptoService.cpp"
#include "Services/TestEntryService.cpp"
#include "Services/TestCategoryService.cpp"
#include "Services/TestSdService.cpp"
#include "Services/TestNvsService.cpp"
#include "Transformers/TestJsonTransformer.cpp"
#include "Transformers/TestModelTransformer.cpp"
#include "Transformers/TestTimeTransformer.cpp"
#include "Selectors/TestVerticalSelector.cpp"
#include "Selectors/TestHorizontalSelector.cpp"
#include "Selectors/TestFieldEditorSelector.cpp"
#include "Selectors/TestFieldActionSelector.cpp"
#include "Selectors/TestConfirmationSelector.cpp"
#include "Selectors/TestStringPromptSelector.cpp"
#include "Enums/TestActionEnumMapper.cpp"
#include "Enums/TestBaseColorEnumMapper.cpp"
#include "Enums/TestIconEnumMapper.cpp"
#include "Enums/TestKeyboardLayoutEnumMapper.cpp"
#include "Controllers/TestVaultController.cpp"
#include "Controllers/TestEntryController.cpp"
#include "Controllers/TestUtilityController.cpp"

void setup() {
    UNITY_BEGIN();

    // CryptoService
    RUN_TEST(test_generateRandomString);
    RUN_TEST(test_generateHardwareRandom);
    RUN_TEST(test_deriveKeyFromPassphrase);
    RUN_TEST(test_encrypt_decrypt_AES);
    RUN_TEST(test_encrypt_decrypt_with_passphrase);
    RUN_TEST(test_generateChecksum);
    RUN_TEST(test_generateSalt);

    // EntryService
    RUN_TEST(test_isEntryExpired);
    RUN_TEST(test_add_and_find_entry);
    RUN_TEST(test_update_entry);
    RUN_TEST(test_get_all_entries);
    RUN_TEST(test_delete_only_selected_duplicate_entry);
    RUN_TEST(test_updateField);
    RUN_TEST(test_maxSavedPasswords);

    // CategoryService
    RUN_TEST(test_validateCategory);
    RUN_TEST(test_add_and_find_category);
    RUN_TEST(test_update_category);
    RUN_TEST(test_sort_categories_by_name);
    RUN_TEST(test_get_all_categories);
    RUN_TEST(test_delete_category);

    // SdService
    RUN_TEST(test_sd_begin);
    RUN_TEST(test_sd_close);
    RUN_TEST(test_read_write_file);
    RUN_TEST(test_append_to_file);
    RUN_TEST(test_list_elements);
    RUN_TEST(test_isFile);
    RUN_TEST(test_isDirectory);
    RUN_TEST(test_getFileName);
    RUN_TEST(test_getFileExt);
    RUN_TEST(test_getParentDirectory);
    RUN_TEST(test_validateVaultFile);

    // NvsService
    RUN_TEST(test_save_and_get_string);
    RUN_TEST(test_save_and_get_int);
    RUN_TEST(test_remove_key);

    // JsonTransformer
    RUN_TEST(test_to_json_categories);
    RUN_TEST(test_from_json_to_entries);
    RUN_TEST(test_from_json_to_categories);
    RUN_TEST(test_merge_entries_and_categories_to_json);

    // ModelTransformer
    RUN_TEST(test_transform_entries_to_strings);
    RUN_TEST(test_transform_single_entry_to_strings);
    RUN_TEST(test_transform_categories_to_strings);

    // TimeTransformer
    RUN_TEST(test_to_label);
    RUN_TEST(test_to_milliseconds);
    RUN_TEST(test_get_all_time_labels);
    RUN_TEST(test_get_all_time_values);

    // VerticalSelector
    RUN_TEST(test_vertical_selector_confirm);
    RUN_TEST(test_vertical_selector_cancel);
    RUN_TEST(test_vertical_selector_delete_action);
    RUN_TEST(test_vertical_selector_delete_empty_list_is_safe);
    RUN_TEST(test_vertical_selector_selects_highlighted_duplicate);
    RUN_TEST(test_vertical_selector_shortcut);

    // HorizontalSelector
    RUN_TEST(test_horizontal_selector_confirm);
    RUN_TEST(test_horizontal_selector_cancel);

    // FieldEditorSelector
    RUN_TEST(test_field_editor_selector_confirm);
    RUN_TEST(test_field_editor_selector_cancel);

    // FieldActionSelector
    RUN_TEST(test_field_action_selector_send_to_usb);
    RUN_TEST(test_field_action_selector_send_to_usb_and_enter);
    RUN_TEST(test_field_action_selector_update_field);
    RUN_TEST(test_field_action_selector_cancel);

    // ConfirmationSelector
    RUN_TEST(test_confirmation_selector_confirm);
    RUN_TEST(test_confirmation_selector_cancel);

    // StringPromptSelector
    RUN_TEST(test_string_selector_confirm);
    RUN_TEST(test_string_selector_cancel);

    // ActionEnumMapper
    RUN_TEST(test_action_enum_to_string);
    RUN_TEST(test_action_enum_get_action_names);

    // BaseColorEnumMapper
    RUN_TEST(test_base_color_enum_to_string);
    RUN_TEST(test_base_color_enum_to_color_value);
    RUN_TEST(test_base_color_enum_get_all_color_names);

    // IconEnumMapper
    RUN_TEST(test_icon_enum_to_string);
    RUN_TEST(test_icon_enum_get_icon_names);

    // KeyboardLayoutMapper
    RUN_TEST(test_to_layout_valid_name);
    RUN_TEST(test_to_layout_invalid_name);
    RUN_TEST(test_get_all_layout_names);

    // VaultController
    RUN_TEST(test_actionNoVault);
    RUN_TEST(test_actionVaultSelected);
    RUN_TEST(test_handleVaultCreation);
    RUN_TEST(test_handleVaultSave);
    RUN_TEST(test_handleVaultSave_maxEntries);
    RUN_TEST(test_handleVaultLoading);

    // EntryController
    RUN_TEST(test_handleEntryCreation);
    RUN_TEST(test_handleEntryUpdate);
    RUN_TEST(test_handleEntryDeletion);

    // UtilityController
    RUN_TEST(test_handleGeneralSettings);

    UNITY_END();
}

void loop() {
    // Required by PlatformIO
}
