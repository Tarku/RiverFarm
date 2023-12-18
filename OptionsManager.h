#include "Shared.h"

class OptionsManager
{
public:
	enum class InputType { KeyboardAZERTY, KeyboardQWERTY, Controller };

	static InputType currentInputType;
	static bool fullscreenMode;

	static bool IsForwardsActive();
	static bool IsBackwardsActive();
	static bool IsLeftActive();
	static bool IsRightActive();

	static bool IsEnterActive();

	static float GetJoystickAxisX();
	static float GetJoystickAxisY();

	static float GetJoystickSecondaryAxisX();
	static float GetJoystickSecondaryAxisY();
};