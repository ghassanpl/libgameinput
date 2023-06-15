#include "InputDevice.h"
#include "InputSystem.h"

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>

#pragma warning(error: 4061)
#pragma warning(error: 4062)

namespace libgameinput
{

	std::string_view IInputDevice::Name() const { return StringPropertyValue(StringProperty::Name); }

	OutputProperties IInputDevice::OutputProperties(DeviceOutputID input) const
	{
		ParentSystem.ErrorReporter->NewError("Input device has no output properties").Value("Device", Name()).Perform();
		std::terminate();
	}

	bool IKeyboardDevice::CanTriggerNavigation(UINavigationInput input) const
	{
		switch (input)
		{
		case UINavigationInput::Accept:
		case UINavigationInput::Cancel:
		case UINavigationInput::Left:
		case UINavigationInput::Right:
		case UINavigationInput::Up:
		case UINavigationInput::Down:
		case UINavigationInput::Home:
		case UINavigationInput::End:
		case UINavigationInput::PageUp:
		case UINavigationInput::PageDown:
			return true;
		case UINavigationInput::Back:
		case UINavigationInput::Forward:
		case UINavigationInput::Menu:
		case UINavigationInput::View:
		case UINavigationInput::PageLeft:
		case UINavigationInput::PageRight:
		case UINavigationInput::ScrollUp:
		case UINavigationInput::ScrollDown:
		case UINavigationInput::ScrollLeft:
		case UINavigationInput::ScrollRight:
			return false;
		}
		return false;
	}

	bool IKeyboardDevice::IsNavigationPressed(UINavigationInput input) const
	{
		switch (input)
		{
		case UINavigationInput::Accept: return IsInputPressed((DeviceInputID)KeyboardButton::Enter);
		case UINavigationInput::Cancel: return IsInputPressed((DeviceInputID)KeyboardButton::Escape);
		case UINavigationInput::Left: return IsInputPressed((DeviceInputID)KeyboardButton::Left);
		case UINavigationInput::Right: return IsInputPressed((DeviceInputID)KeyboardButton::Right);
		case UINavigationInput::Up: return IsInputPressed((DeviceInputID)KeyboardButton::Up);
		case UINavigationInput::Down: return IsInputPressed((DeviceInputID)KeyboardButton::Down);
		case UINavigationInput::Home: return IsInputPressed((DeviceInputID)KeyboardButton::Home);
		case UINavigationInput::End: return IsInputPressed((DeviceInputID)KeyboardButton::End);
		case UINavigationInput::PageUp: return IsInputPressed((DeviceInputID)KeyboardButton::PageUp);
		case UINavigationInput::PageDown: return IsInputPressed((DeviceInputID)KeyboardButton::PageDown);
		case UINavigationInput::Back:
		case UINavigationInput::Forward:
		case UINavigationInput::Menu:
		case UINavigationInput::View:
		case UINavigationInput::PageLeft:
		case UINavigationInput::PageRight:
		case UINavigationInput::ScrollUp:
		case UINavigationInput::ScrollDown:
		case UINavigationInput::ScrollLeft:
		case UINavigationInput::ScrollRight:
			return false;
		}
		return false;
	}

	bool IKeyboardDevice::WasNavigationPressedLastFrame(UINavigationInput input) const
	{
		switch (input)
		{
		case UINavigationInput::Accept: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::Enter);
		case UINavigationInput::Cancel: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::Escape);
		case UINavigationInput::Left: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::Left);
		case UINavigationInput::Right: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::Right);
		case UINavigationInput::Up: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::Up);
		case UINavigationInput::Down: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::Down);
		case UINavigationInput::Home: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::Home);
		case UINavigationInput::End: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::End);
		case UINavigationInput::PageUp: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::PageUp);
		case UINavigationInput::PageDown: return WasInputPressedLastFrame((DeviceInputID)KeyboardButton::PageDown);
		case UINavigationInput::Back:
		case UINavigationInput::Forward:
		case UINavigationInput::Menu:
		case UINavigationInput::View:
		case UINavigationInput::PageLeft:
		case UINavigationInput::PageRight:
		case UINavigationInput::ScrollUp:
		case UINavigationInput::ScrollDown:
		case UINavigationInput::ScrollLeft:
		case UINavigationInput::ScrollRight:
			return false;
		}
		return false;
	}

	bool IMouseDevice::CanTriggerNavigation(UINavigationInput input) const
	{
		switch (input)
		{
			/// SM_MOUSEHORIZONTALWHEELPRESENT
			/// SM_MOUSEWHEELPRESENT
			/// SM_CMOUSEBUTTONS
		case UINavigationInput::Accept:
		case UINavigationInput::Cancel:
			return true;
		case UINavigationInput::ScrollUp:
		case UINavigationInput::ScrollDown:
			return VerticalWheelInputID() != InvalidDeviceInputID;
		case UINavigationInput::ScrollLeft:
		case UINavigationInput::ScrollRight:
			return HorizontalWheelInputID() != InvalidDeviceInputID;

		case UINavigationInput::Left:
		case UINavigationInput::Right:
		case UINavigationInput::Up:
		case UINavigationInput::Down:
		case UINavigationInput::Home:
		case UINavigationInput::End:
		case UINavigationInput::Back:
		case UINavigationInput::Forward:
		case UINavigationInput::Menu:
		case UINavigationInput::View:
		case UINavigationInput::PageLeft:
		case UINavigationInput::PageRight:
		case UINavigationInput::PageUp:
		case UINavigationInput::PageDown:
			return false;
		}
		return false;
	}

	bool IMouseDevice::IsNavigationPressed(UINavigationInput input) const
	{
		switch (input)
		{
		case UINavigationInput::Accept: return IsInputPressed((DeviceInputID)MouseButton::Left);
		case UINavigationInput::Cancel: return IsInputPressed((DeviceInputID)MouseButton::Right);
		case UINavigationInput::ScrollUp: return InputValue(VerticalWheelInputID()) < 0;
		case UINavigationInput::ScrollDown: return InputValue(VerticalWheelInputID()) > 0;
		case UINavigationInput::ScrollLeft: return InputValue(HorizontalWheelInputID()) < 0;
		case UINavigationInput::ScrollRight: return InputValue(HorizontalWheelInputID()) > 0;

		case UINavigationInput::Left:
		case UINavigationInput::Right:
		case UINavigationInput::Up:
		case UINavigationInput::Down:
		case UINavigationInput::Home:
		case UINavigationInput::End:
		case UINavigationInput::Back:
		case UINavigationInput::Forward:
		case UINavigationInput::Menu:
		case UINavigationInput::View:
		case UINavigationInput::PageLeft:
		case UINavigationInput::PageRight:
		case UINavigationInput::PageUp:
		case UINavigationInput::PageDown:
			return false;
		}
		return false;
	}

	bool IMouseDevice::WasNavigationPressedLastFrame(UINavigationInput input) const
	{
		switch (input)
		{
		case UINavigationInput::Accept: return WasInputPressedLastFrame((DeviceInputID)MouseButton::Left);
		case UINavigationInput::Cancel: return WasInputPressedLastFrame((DeviceInputID)MouseButton::Right);
		case UINavigationInput::ScrollUp: return InputValueLastFrame(VerticalWheelInputID()) < 0;
		case UINavigationInput::ScrollDown: return InputValueLastFrame(VerticalWheelInputID()) > 0;
		case UINavigationInput::ScrollLeft: return InputValueLastFrame(HorizontalWheelInputID()) < 0;
		case UINavigationInput::ScrollRight: return InputValueLastFrame(HorizontalWheelInputID()) > 0;

		case UINavigationInput::Left:
		case UINavigationInput::Right:
		case UINavigationInput::Up:
		case UINavigationInput::Down:
		case UINavigationInput::Home:
		case UINavigationInput::End:
		case UINavigationInput::Back:
		case UINavigationInput::Forward:
		case UINavigationInput::Menu:
		case UINavigationInput::View:
		case UINavigationInput::PageLeft:
		case UINavigationInput::PageRight:
		case UINavigationInput::PageUp:
		case UINavigationInput::PageDown:
			return false;
		}
		return false;
	}

	DeviceInputID IXboxGamepadDevice::MaxInputID() const
	{
		return 20;
	}

	struct XboxButtonInputProperties : InputProperties
	{
		XboxButtonInputProperties(std::string_view name)
		{
			Name = name;
			Flags.set(InputFlags::Digital);
			DeadZoneMin = DeadZoneMax = MinValue = 0;
			StepSize = 1;
		}
	};

	struct XboxStickAxisInputProperties : InputProperties
	{
		XboxStickAxisInputProperties(std::string_view name, double min = -1.0, double max = 1.0)
		{
			Name = name;
			Flags.unset(InputFlags::Digital);
			Flags.set(InputFlags::ReturnsToNeutral);
			Flags.set(InputFlags::HasDeadzone);
			MinValue = min;
			MaxValue = max;
		}
	};

	std::optional<InputProperties> IXboxGamepadDevice::PropertiesOf(DeviceInputID input) const
	{
		static const std::map<XboxGamepadButton, XboxButtonInputProperties> button_properties = {
			{ XboxGamepadButton::A, XboxButtonInputProperties("A") },
			{ XboxGamepadButton::B, XboxButtonInputProperties("B") },
			{ XboxGamepadButton::X, XboxButtonInputProperties("X") },
			{ XboxGamepadButton::Y, XboxButtonInputProperties("Y") },
			{ XboxGamepadButton::RightBumper, XboxButtonInputProperties("Right Bumper") },
			{ XboxGamepadButton::LeftBumper, XboxButtonInputProperties("Left Bumper") },
			{ XboxGamepadButton::RightStickButton, XboxButtonInputProperties("Right Stick") },
			{ XboxGamepadButton::LeftStickButton, XboxButtonInputProperties("Left Stick") },
			{ XboxGamepadButton::Back, XboxButtonInputProperties("Back") },
			{ XboxGamepadButton::Start, XboxButtonInputProperties("Start") },
			{ XboxGamepadButton::Right, XboxButtonInputProperties("Right") },
			{ XboxGamepadButton::Left, XboxButtonInputProperties("Left") },
			{ XboxGamepadButton::Down, XboxButtonInputProperties("Down") },
			{ XboxGamepadButton::Up, XboxButtonInputProperties("Up") },
		};

		if (!IsInputValid(input)) return {};
		
		if (input < DefaultButtonCount)
		{
			const auto input_button = (XboxGamepadButton)input;
			if (auto it = button_properties.find(input_button); it != button_properties.end())
				return it->second;
		}
		else
		{
			static const std::array<XboxStickAxisInputProperties, 6> axis_properties = {
				XboxStickAxisInputProperties("Left Stick X Axis"),
				XboxStickAxisInputProperties("Left Stick Y Axis"),
				XboxStickAxisInputProperties("Right Stick X Axis"),
				XboxStickAxisInputProperties("Right Stick Y Axis"),
				XboxStickAxisInputProperties("Left Trigger"),
				XboxStickAxisInputProperties("Right Trigger"),
			};

			const auto axis = input - DefaultButtonCount;
			if (axis < 6)
				return axis_properties[axis];
		}
		
		return {};
	}

	uint8_t IXboxGamepadDevice::StickCount() const
	{
		return 2;
	}

	uint8_t IXboxGamepadDevice::StickAxisCount(uint8_t stick_num) const
	{
		return 2;
	}

	uint8_t IXboxGamepadDevice::ButtonCount() const
	{
		return DefaultButtonCount;
	}

	bool IXboxGamepadDevice::CanTriggerNavigation(UINavigationInput input) const
	{
		switch (input)
		{
		case UINavigationInput::Accept:
		case UINavigationInput::Cancel:
		case UINavigationInput::Menu:
		case UINavigationInput::View:
		case UINavigationInput::Left:
		case UINavigationInput::Right:
		case UINavigationInput::Up:
		case UINavigationInput::Down:
		case UINavigationInput::Back:
		case UINavigationInput::Forward:
		case UINavigationInput::PageUp:
		case UINavigationInput::PageDown:
		case UINavigationInput::PageLeft:
		case UINavigationInput::PageRight:
		case UINavigationInput::ScrollUp:
		case UINavigationInput::ScrollDown:
		case UINavigationInput::ScrollLeft:
		case UINavigationInput::ScrollRight:
			return true;

		case UINavigationInput::Home:
		case UINavigationInput::End:
			return false;
		}
		return false;
	}

	bool IXboxGamepadDevice::IsNavigationPressed(UINavigationInput input) const
	{
		switch (input)
		{
		case UINavigationInput::Accept: return IsInputPressed((DeviceInputID)XboxGamepadButton::A);
		case UINavigationInput::Cancel: return IsInputPressed((DeviceInputID)XboxGamepadButton::B);
		case UINavigationInput::Menu: return IsInputPressed((DeviceInputID)XboxGamepadButton::Start);
		case UINavigationInput::View: return IsInputPressed((DeviceInputID)XboxGamepadButton::Back);
		case UINavigationInput::Left: return IsInputPressed((DeviceInputID)XboxGamepadButton::Left);
		case UINavigationInput::Right: return IsInputPressed((DeviceInputID)XboxGamepadButton::Right);
		case UINavigationInput::Up: return IsInputPressed((DeviceInputID)XboxGamepadButton::Up);
		case UINavigationInput::Down: return IsInputPressed((DeviceInputID)XboxGamepadButton::Down);
		case UINavigationInput::Back: return IsInputPressed((DeviceInputID)XboxGamepadButton::LeftBumper);
		case UINavigationInput::Forward: return IsInputPressed((DeviceInputID)XboxGamepadButton::RightBumper);
		case UINavigationInput::PageUp: return StickAxisValue(0, 0) < -0.1;
		case UINavigationInput::PageDown: return StickAxisValue(0, 0) > 0.1;
		case UINavigationInput::PageLeft: return StickAxisValue(0, 1) < -0.1;
		case UINavigationInput::PageRight: return StickAxisValue(0, 1) > 0.1;
		case UINavigationInput::ScrollUp: return StickAxisValue(1, 0) < -0.1;
		case UINavigationInput::ScrollDown:	return StickAxisValue(1, 0) > 0.1;
		case UINavigationInput::ScrollLeft:	return StickAxisValue(1, 1) < -0.1;
		case UINavigationInput::ScrollRight: return StickAxisValue(1, 1) > 0.1;

		case UINavigationInput::Home:
		case UINavigationInput::End:
			return false;
		}
		return false;
	}

	bool IXboxGamepadDevice::WasNavigationPressedLastFrame(UINavigationInput input) const
	{
		switch (input)
		{
		case UINavigationInput::Accept: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::A);
		case UINavigationInput::Cancel: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::B);
		case UINavigationInput::Menu: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::Start);
		case UINavigationInput::View: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::Back);
		case UINavigationInput::Left: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::Left);
		case UINavigationInput::Right: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::Right);
		case UINavigationInput::Up: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::Up);
		case UINavigationInput::Down: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::Down);
		case UINavigationInput::Back: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::LeftBumper);
		case UINavigationInput::Forward: return WasInputPressedLastFrame((DeviceInputID)XboxGamepadButton::RightBumper);
		case UINavigationInput::PageUp: return StickAxisValueLastFrame(0, 0) < -0.1;
		case UINavigationInput::PageDown: return StickAxisValueLastFrame(0, 0) > 0.1;
		case UINavigationInput::PageLeft: return StickAxisValueLastFrame(0, 1) < -0.1;
		case UINavigationInput::PageRight: return StickAxisValueLastFrame(0, 1) > 0.1;
		case UINavigationInput::ScrollUp: return StickAxisValueLastFrame(1, 0) < -0.1;
		case UINavigationInput::ScrollDown:	return StickAxisValueLastFrame(1, 0) > 0.1;
		case UINavigationInput::ScrollLeft:	return StickAxisValueLastFrame(1, 1) < -0.1;
		case UINavigationInput::ScrollRight: return StickAxisValueLastFrame(1, 1) > 0.1;

		case UINavigationInput::Home:
		case UINavigationInput::End:
			return false;
		}
		return false;
	}

	vec2 UINavigationInputToDirection(UINavigationInput input)
	{
		switch (input)
		{
		case UINavigationInput::Accept: return {};
		case UINavigationInput::Cancel: return {};
		case UINavigationInput::Left: return { -1.0f, 0.0f };
		case UINavigationInput::Right: return { 1.0f, 0.0f };
		case UINavigationInput::Up: return { 0.0f, -1.0f };
		case UINavigationInput::Down: return { 0.0f, 1.0f };
		case UINavigationInput::Home: return { 0.0f, -INFINITY };
		case UINavigationInput::End: return { 0.0f, INFINITY };
		case UINavigationInput::PageUp: return { 0.0f, -10.0f };
		case UINavigationInput::PageDown: return { 0.0f, 10.0f };
		case UINavigationInput::Back: return { -1.0f, 0.0f };
		case UINavigationInput::Forward: return { 1.0f, 0.0f };
		case UINavigationInput::Menu: return {};
		case UINavigationInput::View: return {};
		case UINavigationInput::PageLeft: return { -1.0f, 0.0f };
		case UINavigationInput::PageRight: return { 1.0f, 0.0f };
		case UINavigationInput::ScrollUp: return { 0.0f, -1.0f };
		case UINavigationInput::ScrollDown: return { 0.0f, 1.0f };
		case UINavigationInput::ScrollLeft: return { -1.0f, 0.0f };
		case UINavigationInput::ScrollRight: return { 1.0f, 0.0f };
		}
		return {};
	}

	void IInputDevice::ReportInvalidInput(DeviceInputID input) const
	{
		ParentSystem.ErrorReporter->NewError("Input is not valid")
			.Value("Input", input)
			.Value("Device", Name())
			.Value("ValidRange", MaxInputID() - 1)
			.Perform();
	}

	std::map<DeviceInputID, IKeyboardDevice::KeyboardButtonDescriptor> IKeyboardDevice::mISOUSKeyboardButtons = [] {
		using enum KeyboardButton;
		static constexpr auto valid_keys = { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,  _1, _2, _3, _4, _5, _6, _7, _8, _9, _0,  Return, Escape, Backspace, Tab, Space,  
			Minus, Equals, LeftBracket, RightBracket, Backslash, Semicolon, Apostrophe, Grave, Comma, Period, Slash,  Capslock,  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,  
			PrintScreen, ScrollLock, Pause, Insert, Home, PageUp, Delete, End, PageDown, Right, Left, Down, Up,  NumLock, KP_Divide, KP_Multiply, KP_Minus, KP_Plus, KP_Enter, 
			KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9, KP_0, KP_Period,  NonUSBackslash, Application, KP_Equals, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, 
			Execute, Help, Menu, Select, Stop, Again, Undo, Cut, Copy, Paste, Find, Mute, VolumeUp, VolumeDown, KP_Comma, KP_Equals2,  International1, International2, International3, 
			International4, International5, International6, International7, International8, International9, Lang1, Lang2, Lang3, Lang4, Lang5, Lang6, Lang7, Lang8, Lang9,  
			AltErase, SysReq, Cancel, Clear, Prior, Return2, Separator, Out, Oper, ClearAgain, CrSel, ExSel,  KP_00, KP_000, ThousandsSeparator, DecimalSeparator, CurrencyUnit, 
			CurrencySubunit, KP_LeftParen, KP_RightParen, KP_LeftBrace, KP_RightBrace, KP_Tab, KP_Backspace, KP_A, KP_B, KP_C, KP_D, KP_E, KP_F, KP_Xor, KP_Power, KP_Percent, 
			KP_Less, KP_Greater, KP_Ampersand, KP_DblAmpersand, KP_VerticalBar, KP_DblVerticalBar, KP_Colon, KP_Hash, KP_Space, KP_At, KP_Exclam, KP_MemStore, KP_MemRecall, 
			KP_MemClear, KP_MemAdd, KP_MemSubtract, KP_MemMultiply, KP_MemDivide, KP_PlusMinus, KP_Clear, KP_ClearEntry, KP_Binary, KP_Octal, KP_Decimal, KP_Hexadecimal,  
			LeftCtrl, LeftShift, LeftAlt, LeftGUI, RightCtrl, RightShift, RightAlt, RightGUI
		};

		std::map<DeviceInputID, IKeyboardDevice::KeyboardButtonDescriptor> result;
		for (auto key : valid_keys)
			result[DeviceInputID(key)] = {};

		result[4].Name = "A";
		result[5].Name = "B";
		result[6].Name = "C";
		result[7].Name = "D";
		result[8].Name = "E";
		result[9].Name = "F";
		result[10].Name = "G";
		result[11].Name = "H";
		result[12].Name = "I";
		result[13].Name = "J";
		result[14].Name = "K";
		result[15].Name = "L";
		result[16].Name = "M";
		result[17].Name = "N";
		result[18].Name = "O";
		result[19].Name = "P";
		result[20].Name = "Q";
		result[21].Name = "R";
		result[22].Name = "S";
		result[23].Name = "T";
		result[24].Name = "U";
		result[25].Name = "V";
		result[26].Name = "W";
		result[27].Name = "X";
		result[28].Name = "Y";
		result[29].Name = "Z";
		result[30].Name = "1";
		result[31].Name = "2";
		result[32].Name = "3";
		result[33].Name = "4";
		result[34].Name = "5";
		result[35].Name = "6";
		result[36].Name = "7";
		result[37].Name = "8";
		result[38].Name = "9";
		result[39].Name = "0";
		result[40].Name = "Return";
		result[41].Name = "Escape";
		result[42].Name = "Backspace";
		result[43].Name = "Tab";
		result[44].Name = "Space";
		result[45].Name = "-";
		result[46].Name = "=";
		result[47].Name = "[";
		result[48].Name = "]";
		result[49].Name = "\\";
		result[50].Name = "#";
		result[51].Name = ";";
		result[52].Name = "'";
		result[53].Name = "`";
		result[54].Name = ",";
		result[55].Name = ".";
		result[56].Name = "/";
		result[57].Name = "CapsLock";
		result[58].Name = "F1";
		result[59].Name = "F2";
		result[60].Name = "F3";
		result[61].Name = "F4";
		result[62].Name = "F5";
		result[63].Name = "F6";
		result[64].Name = "F7";
		result[65].Name = "F8";
		result[66].Name = "F9";
		result[67].Name = "F10";
		result[68].Name = "F11";
		result[69].Name = "F12";
		result[70].Name = "PrintScreen";
		result[71].Name = "ScrollLock";
		result[72].Name = "Pause";
		result[73].Name = "Insert";
		result[74].Name = "Home";
		result[75].Name = "PageUp";
		result[76].Name = "Delete";
		result[77].Name = "End";
		result[78].Name = "PageDown";
		result[79].Name = "Right";
		result[80].Name = "Left";
		result[81].Name = "Down";
		result[82].Name = "Up";
		result[83].Name = "Numlock";
		result[84].Name = "Keypad /";
		result[85].Name = "Keypad *";
		result[86].Name = "Keypad -";
		result[87].Name = "Keypad +";
		result[88].Name = "Keypad Enter";
		result[89].Name = "Keypad 1";
		result[90].Name = "Keypad 2";
		result[91].Name = "Keypad 3";
		result[92].Name = "Keypad 4";
		result[93].Name = "Keypad 5";
		result[94].Name = "Keypad 6";
		result[95].Name = "Keypad 7";
		result[96].Name = "Keypad 8";
		result[97].Name = "Keypad 9";
		result[98].Name = "Keypad 0";
		result[99].Name = "Keypad .";
		
		result[101].Name = "Application";
		result[102].Name = "Power";
		result[103].Name = "Keypad =";
		result[104].Name = "F13";
		result[105].Name = "F14";
		result[106].Name = "F15";
		result[107].Name = "F16";
		result[108].Name = "F17";
		result[109].Name = "F18";
		result[110].Name = "F19";
		result[111].Name = "F20";
		result[112].Name = "F21";
		result[113].Name = "F22";
		result[114].Name = "F23";
		result[115].Name = "F24";
		result[116].Name = "Execute";
		result[117].Name = "Help";
		result[118].Name = "Menu";
		result[119].Name = "Select";
		result[120].Name = "Stop";
		result[121].Name = "Again";
		result[122].Name = "Undo";
		result[123].Name = "Cut";
		result[124].Name = "Copy";
		result[125].Name = "Paste";
		result[126].Name = "Find";
		result[127].Name = "Mute";
		result[128].Name = "VolumeUp";
		result[129].Name = "VolumeDown";
		
		result[133].Name = "Keypad ,";
		result[134].Name = "Keypad = (AS400)";
		
		result[153].Name = "AltErase";
		result[154].Name = "SysReq";
		result[155].Name = "Cancel";
		result[156].Name = "Clear";
		result[157].Name = "Prior";
		result[158].Name = "Return";
		result[159].Name = "Separator";
		result[160].Name = "Out";
		result[161].Name = "Oper";
		result[162].Name = "Clear / Again";
		result[163].Name = "CrSel";
		result[164].Name = "ExSel";
		
		result[176].Name = "Keypad 00";
		result[177].Name = "Keypad 000";
		result[178].Name = "ThousandsSeparator";
		result[179].Name = "DecimalSeparator";
		result[180].Name = "CurrencyUnit";
		result[181].Name = "CurrencySubUnit";
		result[182].Name = "Keypad (";
		result[183].Name = "Keypad )";
		result[184].Name = "Keypad {";
		result[185].Name = "Keypad }";
		result[186].Name = "Keypad Tab";
		result[187].Name = "Keypad Backspace";
		result[188].Name = "Keypad A";
		result[189].Name = "Keypad B";
		result[190].Name = "Keypad C";
		result[191].Name = "Keypad D";
		result[192].Name = "Keypad E";
		result[193].Name = "Keypad F";
		result[194].Name = "Keypad XOR";
		result[195].Name = "Keypad ^";
		result[196].Name = "Keypad %";
		result[197].Name = "Keypad <";
		result[198].Name = "Keypad >";
		result[199].Name = "Keypad &";
		result[200].Name = "Keypad &&";
		result[201].Name = "Keypad |";
		result[202].Name = "Keypad ||";
		result[203].Name = "Keypad :";
		result[204].Name = "Keypad #";
		result[205].Name = "Keypad Space";
		result[206].Name = "Keypad @";
		result[207].Name = "Keypad !";
		result[208].Name = "Keypad MemStore";
		result[209].Name = "Keypad MemRecall";
		result[210].Name = "Keypad MemClear";
		result[211].Name = "Keypad MemAdd";
		result[212].Name = "Keypad MemSubtract";
		result[213].Name = "Keypad MemMultiply";
		result[214].Name = "Keypad MemDivide";
		result[215].Name = "Keypad +/-";
		result[216].Name = "Keypad Clear";
		result[217].Name = "Keypad ClearEntry";
		result[218].Name = "Keypad Binary";
		result[219].Name = "Keypad Octal";
		result[220].Name = "Keypad Decimal";
		result[221].Name = "Keypad Hexadecimal";
		
		result[224].Name = "Left Ctrl";
		result[225].Name = "Left Shift";
		result[226].Name = "Left Alt";
		result[227].Name = "Left GUI";
		result[228].Name = "Right Ctrl";
		result[229].Name = "Right Shift";
		result[230].Name = "Right Alt";
		result[231].Name = "Right GUI";

		return result;
	}();

	vec3 IEyeTrackingDevice::EyeFocusPosition() const
	{
		const auto left = LeftEye();
		const auto right = RightEye();
		if (left && right)
			return ClosestPointBetween(left->Ray(), right->Ray());
		return { NAN, NAN, NAN };
	}
}