#include "OptionsManager.h"

typedef sf::Keyboard::Key Key;

OptionsManager::InputType OptionsManager::currentInputType = OptionsManager::InputType::KeyboardAZERTY;

bool OptionsManager::IsForwardsActive()
{
    Key forwards = currentInputType == InputType::KeyboardAZERTY ? Key::Z : Key::W;

    return sf::Keyboard::isKeyPressed(forwards) || sf::Keyboard::isKeyPressed(Key::Up);
}

bool OptionsManager::IsBackwardsActive()
{
    Key backwards = currentInputType == InputType::KeyboardAZERTY ? Key::S : Key::S;

    return sf::Keyboard::isKeyPressed(backwards) || sf::Keyboard::isKeyPressed(Key::Down);
}

bool OptionsManager::IsLeftActive()
{
    Key left = currentInputType == InputType::KeyboardAZERTY ? Key::Q : Key::A;

    return sf::Keyboard::isKeyPressed(left) || sf::Keyboard::isKeyPressed(Key::Left);
}

bool OptionsManager::IsRightActive()
{
    Key right = currentInputType == InputType::KeyboardAZERTY ? Key::D : Key::D;

    return sf::Keyboard::isKeyPressed(right) || sf::Keyboard::isKeyPressed(Key::Right);
}
bool OptionsManager::IsEnterActive()
{

    return sf::Keyboard::isKeyPressed(Key::Enter) || sf::Joystick::isButtonPressed(0, 0);
}

float OptionsManager::GetJoystickAxisX()
{
    return sf::Joystick::getAxisPosition(0, sf::Joystick::X);
}

float OptionsManager::GetJoystickAxisY()
{
    return sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
}


float OptionsManager::GetJoystickSecondaryAxisX()
{
    return sf::Joystick::getAxisPosition(0, sf::Joystick::U);
}

float OptionsManager::GetJoystickSecondaryAxisY()
{
    return sf::Joystick::getAxisPosition(0, sf::Joystick::V);
}
