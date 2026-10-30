from pathlib import Path


def test_welcome_accepts_go_and_keyboard_without_change_gate():
    source = Path("src/Inputs/CardputerInput.cpp").read_text()
    wait_press = source.split("void CardputerInput::waitPress()", 1)[1]

    assert "M5Cardputer.BtnA.isPressed()" in wait_press
    assert "M5Cardputer.Keyboard.isPressed()" in wait_press
    assert "M5Cardputer.Keyboard.isChange()" not in wait_press


if __name__ == "__main__":
    test_welcome_accepts_go_and_keyboard_without_change_gate()
