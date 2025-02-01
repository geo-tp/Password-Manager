# Password Manager for ESP32

**Password Manager** designed for the ESP32. It provides **secure password storage, encryption, and USB emulation for typing passwords automatically**.

![](/image.jpg)

## Features

- **AES-128 Encryption**: All stored passwords are encrypted for security.
- **Storage on SD Card**: Encrypted password storage on an SD card for persistent data.
- **Random Password Generation**: Generate secure passwords.
- **HID Keyboard Mode**: ESP32 can act as a USB keyboard to automatically type credentials.
- **User Authentication**: Requires a master password to unlock stored credentials.
- **Auto-Lock Vault**: The vault will be automatically locked after a selected amount of time.

## Installation

- <b>M5Burner</b> : Search into M5CARDPUTER section and burn it
- <b>Github</b> : Build or take the firmware.bin from the [github release](https://github.com/geo-tp/Password-Manager/releases) and flash it

## Usage
- **Create a Vault**: Create a new encrypted vault to securely store your passwords. **Each vault is stored as an encrypted file on the SD card.**

- **Manage Password Entries**: Add, update, delete password entries to the vault. **Up to 100 passwords per vault.**

- **Auto-Type**: Select a field and press `OK`, the ESP32 type it via USB HID.

- **Update Settings**: Adjust app settings as keyboard layout, brightness and vault lock timings.

**NOTE:** You can update a vault name by modifying the filename in the `/vaults/` folder, the file extension must remains `.vault`. **The master password used to create0 a vault can't be modified.**
