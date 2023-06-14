#pragma once

#include "Common.h"

namespace libgameinput
{
	enum class MouseButton
	{
		Left,
		Right,
		Middle,
		Button4,
		Button5
	};

	enum class InputFlags
	{
		Digital,
		ReturnsToNeutral,
		HasDeadzone,
		/// If axis, whether this axis influences or is influenced by another axis (e.g. between two axis on a pad stick, or between the absolute position of the mouse, and the delta)
		/// If button, whether this button cannot be pressed with/without another button
		Correlated,
		/// If axis, whether this axis is emulated from buttons
		/// If button, whether this button is emulated from an axis
		Emulated,
		/// Whether this button can repeat its 'pressed' events
		CanRepeat,
		/// Whether this input can have its polling frequency changed
		CanChangeUpdateFrequency,
		HasForceFeedback,
	};

	enum class InputDeviceFlags
	{
		Composite, /// e.g. Mouse+Keyboard combo, or two paired joy-cons
		Wireless,
		CanBeReset,
		CanBeDisabled,
		UniquePerSystem, /// Only one available in the system
		InputsSequential,
		OutputsSequential,
	};

	enum class InputDeviceOutputType
	{
		Flag, /// E.g. for LEDs on keyboards
		Value, /// E.g. for progress bars, odometers, number displays, etc.
		Color,
		Vibration,
		Image,
		Video,
		Sound,
		Text,
		PersistentData, /// E.g. for saves
		Other
	};

	struct InputDeviceComponentProperties
	{
		std::string Name;
		std::string ShortName;

		std::string GlyphURI;

		Seconds UpdateFrequency = std::numeric_limits<Seconds>::min();

		vec3 PhysicalPositionOnDevice = {};
		vec3 PositionOnDevicePreview = {}; /// On image or model

		std::vector<uint16_t> HIDID{};
	};

	struct InputProperties : InputDeviceComponentProperties
	{
		enum_flags<InputFlags> Flags{};

		double PressedThreshold = 0.5;

		double DeadZoneMin = -0.1;
		double DeadZoneMax = 0.1;

		double MinValue = -1;
		double NeutralValue = 0;
		double MaxValue = 1;
		double StepSize = 0;

		float Effort = 0.0f; /// A relative value indicating how much effort the user must expend to activate this input

		std::string Unit;
		std::string Dimension;
		vec3 RepresentsDirection = {};

		/// TODO: InputControlType -> enum { PushButton, MomentaryTrigger, Switch, Hat, Slider, Dial, Wheel, };
		/// TODO: ValueFunction -> enum { Linear, Quadratic, Sinusoidal, etc. }
	};

	struct ButtonInputProperties : InputProperties
	{
		ButtonInputProperties(std::string_view name, std::string glyph_url = {})
		{
			Name = name;
			Flags.set(InputFlags::Digital);
			Flags.set(InputFlags::ReturnsToNeutral); /// Most buttons return to neutral
			DeadZoneMin = DeadZoneMax = MinValue = 0;
			StepSize = 1;
			GlyphURI = std::move(glyph_url);
		}
	};

	/// TODO: This
	enum class OutputFlags
	{
		Continuous, /// vs triggered requests
		CanBeDisabled,
	};

	struct OutputProperties : InputDeviceComponentProperties
	{
		InputDeviceOutputType Type = InputDeviceOutputType::Other;

		vec3 Resolution = { 1,1,1 };

		vec3 PhysicalPositionOnDevice = {};
		vec3 PositionOnDevicePreview = {}; /// On image or model
	};

/*
	/// TODO
	enum class InputDataType
	{
		Audio,
		Video,
		PersistentData,
		Identity,
		Other
	};

	/// TODO
	struct InputDataProperties
	{
		std::string Name;
		InputDataType Type = InputDataType::Other;
		vec3 Resolution = { 1,1,1 };
	};
*/

	enum class UINavigationInput
	{
		Accept, Cancel, Menu, View,
		Left, Right, Up, Down,
		Home, End, Back, Forward,
		PageUp, PageDown, PageLeft, PageRight,
		ScrollUp, ScrollDown, ScrollLeft, ScrollRight,
	};

	vec2 UINavigationInputToDirection(UINavigationInput input);

	enum class Handedness
	{
		Left,
		Right,
		Both,
		Either,
	};

	struct IInputDevice
	{
		virtual ~IInputDevice() = default;

		IInputSystem& ParentSystem;
		IInputDevice(IInputSystem& sys) : ParentSystem(sys) {}

		TimePoint LastActiveTime() const { return mLastActiveTime; }
		void SetLastActiveTime(TimePoint time) { mLastActiveTime = time; }

		virtual std::string_view Name() const;
		virtual enum_flags<InputDeviceFlags> Flags() const = 0;

		virtual DeviceInputID MaxInputID() const = 0;
		virtual bool IsReceivingAnyInput() const = 0;
		virtual bool IsInputValid(DeviceInputID input) const { return input < MaxInputID(); }
		virtual double InputValue(DeviceInputID input) const = 0;
		virtual vec2 InputValue(DeviceInputID input1, DeviceInputID input2) const { return { this->InputValue(input1), this->InputValue(input2) }; }
		virtual bool IsInputPressed(DeviceInputID input) const = 0;
		virtual double InputValueLastFrame(DeviceInputID input) const = 0;
		virtual bool WasInputPressedLastFrame(DeviceInputID input) const = 0;
		virtual bool SetInputUpdateFrequency(Seconds freq) { return false; }
		virtual void ResetInput(DeviceInputID input) {} /// used, for example, to set a delta-based input to an origin value

		/// TODO: Force feedback per input

		virtual auto PropertiesOf(DeviceInputID input) const -> std::optional<InputProperties> = 0;

		virtual bool CanTriggerNavigation(UINavigationInput input) const = 0;
		virtual bool IsNavigationPressed(UINavigationInput input) const = 0;
		virtual bool WasNavigationPressedLastFrame(UINavigationInput input) const = 0;

		virtual bool IsActive() const { return true; }

		virtual size_t SubDeviceCount() const { return 0; }
		virtual IInputDevice* SubDevice(size_t index) const { return nullptr; }
		virtual IInputDevice* ParentDevice() const { return nullptr; }

		virtual DeviceOutputID MaxOutputID() const { return {}; }
		virtual bool IsOutputValid(DeviceOutputID input) const { return input < MaxOutputID(); }
		virtual OutputProperties OutputProperties(DeviceOutputID input) const;
		virtual bool SetOutput(DeviceOutputID index, vec3 value) { return false; }
		virtual bool ResetOutput(DeviceOutputID index) { return false; }
		virtual bool SendOutputData(DeviceOutputID index, std::span<const uint8_t> data) { return false; }
		virtual bool SetOutputCallback(DeviceOutputID index, std::function<void(std::span<uint8_t>)> callback) { return false; }
		virtual bool EnableOutput(DeviceOutputID index, bool enable) { return false; }

		/// TODO: Data
		// virtual size_t DataCount() const = 0;
		// virtual bool DataMeaningful(size_t index) const= 0;
		// virtual InputDataProperties const& DataProperties(size_t index) const = 0;
		// virtual json ReadData(size_t index, size_t max_data) = 0;

		enum class ConnectionType
		{
			Attached,
			Wired,
			Wireless,
		};

		virtual enum_flags<ConnectionType> SupportedConnectionTypes() const { return { ConnectionType::Attached }; }
		virtual ConnectionType CurrentConnectionType() const { return ConnectionType::Attached; }

		enum class PowerSourceType
		{
			None,
			Wire,
			Batteries,
			InternalAccu,
			ExternalAccu,
			Unknown
		};

		virtual PowerSourceType CurrentPowerSource() const { return IInputDevice::PowerSourceType::Unknown; }
		virtual enum_flags<PowerSourceType> ConnectedPowerSources() const { return enum_flags<PowerSourceType>{}; }
		virtual enum_flags<PowerSourceType> SupportedPowerSources() const { return enum_flags<PowerSourceType>{}; }

		enum class StringProperty
		{
			Name,
			Company,
			ImageURL,
			Website,
			SerialNumber,
			VendorProductVersionID,
			SupportedLanguages,
		};

		virtual bool IsStringPropertyValid(StringProperty property) const = 0;
		virtual std::string_view StringPropertyValue(StringProperty property, std::string_view lang = {}) const = 0;

		/// TODO: Get Current Handedness
		/// TODO: Get Available Handednesses

		enum class NumberProperty
		{
			PhysicalSize, /// vec3, in centimeters
			AutoOffTime, /// float, in seconds
			Range, /// float, in cm
			Charge, /// float, 0 - 1
			PowerDraw, /// float, in amperes
			InternalTemperature, /// float, in Celsius
			HIDUsage, /// vec3, first is usage page, second is usage id
		};

		virtual bool IsNumberPropertyValid(NumberProperty property) const = 0;
		virtual vec3 NumberPropertyValue(NumberProperty property) const = 0;

		virtual void ForceRefresh() = 0;
		virtual void NewFrame() = 0;

		virtual void ResetDevice() {}
		virtual void EnableDevice(bool enable) {}

		virtual PlayerID AssociatedPlayer() const { return mAssociatedPlayer; }
		virtual bool AssociatePlayer(PlayerID player) { mAssociatedPlayer = player; return true; }

	protected:

		void ReportInvalidInput(DeviceInputID input) const;

	private:

		TimePoint mLastActiveTime = {};
		PlayerID mAssociatedPlayer = {};

	};

	/// NOTE: Keyboard DIDs are basically equivalent to scancodes
	/// That is, physical buttons regardless of keyboard language layout
	/// 
	enum class KeyboardButton
	{
		None = 0,
		A = 4, B = 5, C = 6, D = 7, E = 8, F = 9, G = 10, H = 11, I = 12, J = 13, K = 14, L = 15, M = 16, 
		N = 17, O = 18, P = 19, Q = 20, R = 21, S = 22, T = 23, U = 24, V = 25, W = 26, X = 27, Y = 28, Z = 29,
		_1 = 30, _2 = 31, _3 = 32, _4 = 33, _5 = 34, _6 = 35, _7 = 36, _8 = 37, _9 = 38, _0 = 39,
		Return = 40, Enter = Return,
		Escape = 41, Backspace = 42, Tab = 43, Space = 44,
			Minus = 45, Equals = 46, LeftBracket = 47, RightBracket = 48, Backslash = 49, Semicolon = 51, Apostrophe = 52, Grave = 53, Comma = 54, Period = 55, Slash = 56,
		Capslock = 57,
		F1 = 58, F2 = 59, F3 = 60, F4 = 61, F5 = 62, F6 = 63, F7 = 64, F8 = 65, F9 = 66, F10 = 67, F11 = 68, F12 = 69,
		PrintScreen = 70, ScrollLock = 71, Pause = 72, Insert = 73, Home = 74, PageUp = 75, Delete = 76, End = 77, PageDown = 78, Right = 79, Left = 80, Down = 81, Up = 82,
		NumLock = 83, KP_Divide = 84, KP_Multiply = 85, KP_Minus = 86, KP_Plus = 87, KP_Enter = 88, 
			KP_1 = 89, KP_2 = 90, KP_3 = 91, KP_4 = 92, KP_5 = 93, KP_6 = 94, KP_7 = 95, KP_8 = 96, KP_9 = 97, KP_0 = 98, KP_Period = 99,
		NonUSBackslash = 100, Application = 101, KP_Equals = 103, F13 = 104, F14 = 105, F15 = 106, F16 = 107, F17 = 108, F18 = 109, F19 = 110, F20 = 111,
			F21 = 112, F22 = 113, F23 = 114, F24 = 115, Execute = 116, Help = 117, Menu = 118, Select = 119, Stop = 120, Again = 121, Undo = 122, 
			Cut = 123, Copy = 124, Paste = 125, Find = 126, Mute = 127, VolumeUp = 128, VolumeDown = 129, KP_Comma = 133, KP_Equals2 = 134,
		International1 = 135, International2 = 136, International3 = 137, International4 = 138, International5 = 139, International6 = 140, International7 = 141, International8 = 142, International9 = 143, 
		Lang1 = 144, Lang2 = 145, Lang3 = 146, Lang4 = 147, Lang5 = 148, Lang6 = 149, Lang7 = 150, Lang8 = 151, Lang9 = 152,
		AltErase = 153, SysReq = 154, Cancel = 155, Clear = 156, Prior = 157, Return2 = 158, Separator = 159, Out = 160, Oper = 161, ClearAgain = 162, CrSel = 163, ExSel = 164,
		KP_00 = 176, KP_000 = 177, ThousandsSeparator = 178, DecimalSeparator = 179, CurrencyUnit = 180, CurrencySubunit = 181, KP_LeftParen = 182, KP_RightParen = 183, 
			KP_LeftBrace = 184, KP_RightBrace = 185, KP_Tab = 186, KP_Backspace = 187, KP_A = 188, KP_B = 189, KP_C = 190, KP_D = 191, KP_E = 192, KP_F = 193, KP_Xor = 194, 
			KP_Power = 195, KP_Percent = 196, KP_Less = 197, KP_Greater = 198, KP_Ampersand = 199, KP_DblAmpersand = 200, KP_VerticalBar = 201, KP_DblVerticalBar = 202, 
			KP_Colon = 203, KP_Hash = 204, KP_Space = 205, KP_At = 206, KP_Exclam = 207, KP_MemStore = 208, KP_MemRecall = 209, KP_MemClear = 210, KP_MemAdd = 211, 
			KP_MemSubtract = 212, KP_MemMultiply = 213, KP_MemDivide = 214, KP_PlusMinus = 215, KP_Clear = 216, KP_ClearEntry = 217, KP_Binary = 218, KP_Octal = 219, 
			KP_Decimal = 220, KP_Hexadecimal = 221,
		LeftCtrl = 224, LeftShift = 225, LeftAlt = 226, LeftGUI = 227, RightCtrl = 228, RightShift = 229, RightAlt = 230, RightGUI = 231
	};

	struct IKeyboardDevice : virtual IInputDevice
	{
		using IInputDevice::IInputDevice;

		/// TODO: Getting/setting layout or keycode mappings

		virtual bool CanTriggerNavigation(UINavigationInput input) const override;
		virtual bool IsNavigationPressed(UINavigationInput input) const override;
		virtual bool WasNavigationPressedLastFrame(UINavigationInput input) const override;

	protected:

		struct KeyboardButtonDescriptor
		{
			std::string Name{};
			vec2 Position{};
			std::string URI{};
		};

		static std::map<DeviceInputID, KeyboardButtonDescriptor> mISOUSKeyboardButtons;
	};

	enum class MouseCursorShape
	{
		None,
		Default,
		Arrow,
		Busy, /// or 'Wait'
		Question, /// or 'Help'
		Edit, /// or 'IBeam'
		Move,
		ResizeN,
		ResizeW,
		ResizeS,
		ResizeE,
		ResizeNW,
		ResizeSW,
		ResizeSE,
		ResizeNE,
		Progress,
		Precision, /// or 'Crosshair'
		Link, /// or 'Hand'
		AltSelect, /// or 'UpArrow'
		Unavailable,

		Drag,
		CanDrop,
		CannotDrop,
	};

	struct IMouseDevice : virtual IInputDevice
	{
		using IInputDevice::IInputDevice;

		virtual DeviceInputID VerticalWheelInputID() const = 0;
		virtual DeviceInputID HorizontalWheelInputID() const = 0;

		virtual DeviceInputID XAxisInputID() const = 0;
		virtual DeviceInputID YAxisInputID() const = 0;

		virtual vec2 Position() const { return { this->InputValue(XAxisInputID()), this->InputValue(YAxisInputID()) }; }
		virtual double Wheel() const { return this->InputValue(VerticalWheelInputID()); }

		virtual void ShowCursor(bool show) {}
		virtual bool IsCursorVisible() const { return true; }
		virtual bool IsCursorShapeAvailable(MouseCursorShape shape) const { return shape == MouseCursorShape::Default; }
		virtual void SetCursorShape(MouseCursorShape shape) {}
		virtual bool IsCustomCursorShapeAvailable(std::string_view shape) const { return false; }
		virtual void SetCustomCursorShape(std::string_view name) {}
		virtual void SetCustomCursorShape(void* resource, MouseCursorShape replace = {}, vec2 hotspot = {}) {}

		virtual bool CanWarp() const { return false; }
		virtual void Warp(vec2 pos) {}

		virtual auto ValidRegions() const -> std::vector<rec2> = 0;
		virtual void SetValidRegions(std::span<rec2 const>) = 0;

		/// TODO: Double-click support

		virtual bool CanTriggerNavigation(UINavigationInput input) const override;
		virtual bool IsNavigationPressed(UINavigationInput input) const override;
		virtual bool WasNavigationPressedLastFrame(UINavigationInput input) const override;
	};

	struct IGamepadDevice : virtual IInputDevice
	{
		using IInputDevice::IInputDevice;

		virtual uint8_t StickCount() const = 0;
		virtual uint8_t StickAxisCount(uint8_t stick_num) const = 0;
		virtual uint8_t ButtonCount() const = 0;

		virtual bool IsButtonPressed(uint8_t button_num) const = 0;
		virtual vec3 StickValue(uint8_t stick_num) const
		{
			vec3 result{};
			for (uint8_t a = 0; a < std::min(uint8_t(3), StickAxisCount(stick_num)); ++a)
				result[a] = StickAxisValue(stick_num, a);
			return result;
		}
		virtual float StickAxisValue(uint8_t stick_num, uint8_t axis_num) const = 0;

		virtual bool WasButtonPressedLastFrame(uint8_t button_num) const = 0;
		virtual vec3 StickValueLastFrame(uint8_t stick_num) const
		{
			vec3 result{};
			for (uint8_t a = 0; a < std::min(uint8_t(3), StickAxisCount(stick_num)); ++a)
				result[a] = StickAxisValueLastFrame(stick_num, a);
			return result;
		}
		virtual float StickAxisValueLastFrame(uint8_t stick_num, uint8_t axis_num) const = 0;

	protected:

		virtual auto StickAxisInputs(uint8_t stick_num) -> std::array<DeviceInputID, 3> { return {InvalidDeviceInputID,InvalidDeviceInputID,InvalidDeviceInputID}; } 
		virtual auto InputForButton(uint8_t button_num) -> DeviceInputID { return InvalidDeviceInputID; }
	};

	enum class XboxGamepadButton
	{
		A, B, X, Y,
		RightBumper,
		LeftBumper,
		RightStickButton,
		LeftStickButton,
		Back,
		Start,
		Right, Left, Down, Up,
	};

	struct IXboxGamepadDevice : virtual IGamepadDevice
	{
		using IGamepadDevice::IGamepadDevice;

		virtual DeviceInputID MaxInputID() const override;

		virtual auto PropertiesOf(DeviceInputID input) const ->std::optional<InputProperties> override;

		virtual uint8_t StickCount() const override;
		virtual uint8_t StickAxisCount(uint8_t stick_num) const override;
		virtual uint8_t ButtonCount() const override;

		virtual bool CanTriggerNavigation(UINavigationInput input) const override;
		virtual bool IsNavigationPressed(UINavigationInput input) const override;
		virtual bool WasNavigationPressedLastFrame(UINavigationInput input) const override;

		static constexpr uint64_t DefaultButtonCount = 14;

		enum class Axes
		{
			LeftStick_XAxis = DefaultButtonCount,
			LeftStick_YAxis,
			RightStick_XAxis,
			RightStick_YAxis,
			LeftTrigger,
			RightTrigger,
		};
	};

	/// TODO: api for this
	struct ITextInputDevice : public IInputDevice
	{
		void StartTextInput(rec2 const& input_area);
		void CancelTextInput();
		bool IsTextInputActive();

		IKeyboardDevice* GetScreenKeyboard();
		bool HasScreenKeyboardSupport();

		auto GetCurrentText() -> std::string;
		auto GetCurrentSelection() -> std::pair<int, int>;

		void OnTextCompositionStarted();
		void OnTextCompositionFinished();
		void OnTextCompositionCanceled();
	};

	/// Represents the entire machine and OS
	struct ISystemDevice : public IInputDevice
	{
		enum class ExampleInputs /// These can change during session
		{
			LidState, /// open vs closed
			OnBattery,
			BatteryCharge,
			Docked,
			Sleep, /// the device is in sleep mode, don't do GPU or sound, don't do any processing except for events that should wake up the device or notify the user
			Background, /// if true, the application should be in the "in-background" mode, e.g. minimized, paused, etc.
			GracefulShutdown,
			ImmediateShutdown,
			DebugBreak,
			DisplayChange, /// Anything about a connected display has changed (new display, display lost, display shape changed, etc.)
			StereographyRequested, /// If true, a stereographic 3D image is requested to be rendered
			TakeScreenshot,
			StartRecording,
			StopRecording,
			CopyToClipboard,
			CutToClipboard,
			Play,
			Pause,
			Stop,
			NextTrack,
			PrevTrack,
			FastForward,
			Rewind,
			Mute,
			VolumeUp,
			VolumeDown,
			
			CPUTemperature,
			CPUFanUtilization,
			CPUUtilization,
			GPUTemperature,
			GPUUtilization,
			GPUFanUtilization,
			ChassisFanUtilization,
			
			/// TODO: Should this be its own device?
			InternetConnected,
			WirelessNetworkStrength,
			NetworkConnectionMetered,
		};

		enum class SystemConfig /// These are usually static per-session
		{
			PreferredUIScale,
			PrimaryAccentColor,
			SecondaryAccentColor,
			KeyRepeatFrequency,
			KeyRepeatDelay,
			DPI,
			PreferredTextSize,
			LimitAnimations, /// 0 - no/minimal animations, 1 - full animations
			AdminPrivileges,
			ChassisFanCount,
		};

		enum class ColorblindnessType
		{
			RedGreenDeuteranopia,
			RedGreenProtanopia,
			BlueYellowTritanopia,
			Grayscale,
			GrayscaleInverted,
			Inverted,
		};

		enum class SystemConfigFlags
		{
			DarkMode,
			PreferDyslexiaFriendlyFont,
			LimitFlashing, /// for photosensitive epilepsy
			IncreasePointerSize,
			HighContrast,
		};

		/// TODO: Listing connected devices, listing connection slots and their physical locations
		/// Getting offset from machine to device if available
	};
}