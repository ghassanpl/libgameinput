#include "InputSystem.h"
//#include "../Debugger.h"

namespace libgameinput
{
	IInputSystem::IInputSystem(std::shared_ptr<IErrorReporter> error_reporter) noexcept
		: ErrorReporter{ std::move(error_reporter) }
	{

	}

	void IInputSystem::SetLastActiveDevice(IInputDevice* device, TimePoint current_time)
	{
		if (device)
			device->SetLastActiveTime(current_time);
		mLastActiveDevice = device;
	}

	void IInputSystem::Init()
	{
		SetLastActiveDevice(Keyboard(), {});
	}

	void IInputSystem::Update()
	{
		for (auto& device : mInputDevices)
		{
			if (device) device->NewFrame();
		}
	}


	void IInputSystem::MapButton(DeviceInputID physical_button, InputDeviceIndex of_device, Input to_input)
	{
		//if (of_device >= mInputDevices.size())
			//Game->Warning("Input device index {} does not represent a connected device", of_device);
		mPlayers[to_input.Player].Mappings[to_input.ActionID].push_back(Mapping{ of_device, {physical_button, InvalidDeviceInputID} });
	}

	bool IInputSystem::IsButtonPressed(Input input_id)
	{
		if (auto player = GetPlayer(input_id.Player))
		{
			if (auto it = player->Mappings.find(input_id.ActionID); it != player->Mappings.end())
			{
				for (auto& mapping : it->second)
				{
					if (auto device = InputDevice(mapping.DeviceID))
					{
						if (device->IsInputPressed(mapping.Inputs[0]))
							return true;
					}
				}
			}
		}
		return false;
	}

	bool IInputSystem::IsButtonPressed(MouseButton but)
	{
		return Mouse()->IsInputPressed((DeviceInputID)but);
	}

	bool IInputSystem::IsKeyPressed(KeyboardKey key)
	{
		return Keyboard()->IsInputPressed((DeviceInputID)key);
	}

	bool IInputSystem::WasButtonPressed(Input input_id)
	{
		if (auto player = GetPlayer(input_id.Player))
		{
			if (auto it = player->Mappings.find(input_id.ActionID); it != player->Mappings.end())
			{
				for (auto& mapping : it->second)
				{
					if (auto device = InputDevice(mapping.DeviceID))
					{
						if (device->IsInputPressed(mapping.Inputs[0]) && !device->WasInputPressedLastFrame(mapping.Inputs[0]))
							return true;
					}
				}
			}
		}
		return false;
	}

	bool IInputSystem::WasButtonPressed(MouseButton but)
	{
		return Mouse()->IsInputPressed((DeviceInputID)but) && !Mouse()->WasInputPressedLastFrame((DeviceInputID)but);
	}

	bool IInputSystem::WasKeyPressed(KeyboardKey key)
	{
		return Keyboard()->IsInputPressed((DeviceInputID)key) && !Keyboard()->WasInputPressedLastFrame((DeviceInputID)key);
	}

	bool IInputSystem::WasButtonReleased(Input input_id)
	{
		if (auto player = GetPlayer(input_id.Player))
		{
			if (auto it = player->Mappings.find(input_id.ActionID); it != player->Mappings.end())
			{
				for (auto& mapping : it->second)
				{
					if (auto device = InputDevice(mapping.DeviceID))
					{
						if (!device->IsInputPressed(mapping.Inputs[0]) && device->WasInputPressedLastFrame(mapping.Inputs[0]))
							return true;
					}
				}
			}
		}
		return false;
	}

	bool IInputSystem::WasKeyReleased(KeyboardKey key)
	{
		return !Keyboard()->IsInputPressed((DeviceInputID)key) && Keyboard()->WasInputPressedLastFrame((DeviceInputID)key);
	}

	float IInputSystem::AxisValue(Input of_input)
	{
		auto player = GetPlayer(of_input.Player);
		if (!player)
		{
			ErrorReporter->NewWarning("Player not found for input")
				.Value("PlayerID", of_input.Player.value)
				.Value("ActionID", of_input.ActionID)
				.Perform();
			return {};
		}

		if (auto it = player->Mappings.find(of_input.ActionID); it != player->Mappings.end())
		{
			for (auto& mapping : it->second)
			{
				if (auto device = InputDevice(mapping.DeviceID))
				{
					return (float)device->InputValue(mapping.Inputs[0]);
				}
			}
		}
		return 0.0f;
	}

	vec2 IInputSystem::Axis2DValue(Input of_input)
	{
		auto player = GetPlayer(of_input.Player);
		if (!player)
		{
			ErrorReporter->NewWarning("Player not found for input")
				.Value("PlayerID", of_input.Player.value)
				.Value("ActionID", of_input.ActionID)
				.Perform();
			return {};
		}

		if (auto it = player->Mappings.find(of_input.ActionID); it != player->Mappings.end())
		{
			for (auto& mapping : it->second)
			{
				if (auto device = InputDevice(mapping.DeviceID))
				{
					return { (float)device->InputValue(mapping.Inputs[0]), (float)device->InputValue(mapping.Inputs[1]) };
				}
			}
		}
		return {};
	}

	vec2 IInputSystem::MousePosition() const
	{
		return { (float)Mouse()->InputValue(Mouse()->XAxisInputID()), (float)Mouse()->InputValue(Mouse()->YAxisInputID()) };
	}

	IInputDevice* IInputSystem::InputDevice(InputDeviceIndex id)
	{
		if (id < mInputDevices.size())
			return mInputDevices[id].get();
		return nullptr;
	}

	std::string IInputSystem::InputDeviceName(InputDeviceIndex id)
	{
		if (auto dev = InputDevice(id))
			return (std::string)dev->Name();
		else
			return std::format("Disconnected Device {}", id); /// TODO: Cache device names so we can add ("(Previously {})", OldDeviceName)
	}

	IInputSystem::PlayerInformation* IInputSystem::GetPlayer(PlayerID id)
	{
		auto player_it = mPlayers.find(id);
		if (player_it == mPlayers.end())
			return {};
		return &player_it->second;
	}

	std::string IInputSystem::ButtonNamesForInput(Input button, std::string_view button_format)
	{
		std::string buttons;
		for (auto& mapping : GetPlayer(button.Player)->Mappings[button.ActionID])
		{
			if (auto device = InputDevice(mapping.DeviceID))
			{
				if (auto props = device->PropertiesOf(mapping.Inputs[0]))
				{
					if (!buttons.empty()) buttons += ", ";
					buttons += std::vformat(button_format, std::make_format_args(props->Name));
				}
			}
			else
			{
				if (!buttons.empty()) buttons += ", ";
				buttons += std::vformat(button_format, std::make_format_args(std::format("#{}@{}#", mapping.Inputs[0], mapping.DeviceID)));
			}
		}
		return buttons;
	}

	std::string IInputSystem::ButtonNameForInput(Input input, std::string_view button_format)
	{
		/// Find the correct mapping from the device that was updated the latest

		IInputDevice* last_device = nullptr;
		Mapping* last_mapping = nullptr;
		TimePoint last_active = {};
		for (auto& mapping : GetPlayer(input.Player)->Mappings[input.ActionID])
		{
			if (auto device = InputDevice(mapping.DeviceID); device && device->LastActiveTime() >= last_active)
			{
				last_mapping = &mapping;
				last_device = device;
				last_active = device->LastActiveTime();
			}
		}

		if (last_mapping)
		{
			if (auto props = last_device->PropertiesOf(last_mapping->Inputs[0]))
				return std::vformat(button_format, std::make_format_args(props->Name));
		}

		return {};
	}

}