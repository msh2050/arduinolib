// keyboard.h
/*
modified from phi_interfaces library by Dr. John Liu
https://liudr.wordpress.com/libraries/phi_interfaces/

date:- 2-1-2017
author:Mustafa Salaman
*/

#ifndef _KEYBOARD_h
#define _KEYBOARD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif




// Button status
#define buttons_up 0        ///< Non-transitional button status
#define buttons_pressed 1   ///< Transitional button status
#define buttons_down 2      ///< Non-transitional button status
#define buttons_held 3      ///< Non-transitional button status
#define buttons_released 4  ///< Transitional button status
#define buttons_debounce 5  ///< One needs to wait till debounce status is over to become pressed status to confirm a press.

//Internal and external "NO KEY" return values
#define NO_KEYs 255         ///< This is no key in scan code, internal to the library.
#define NO_KEY 0            ///< This is no key that the library outputs to a caller, to be compatible with keypad.h

//Operating parameters
#define buttons_hold_time  250      ///< Default key down time needed to be considered the key is held down
#define buttons_debounce_time  20    ///< Default key down time needed to be considered the key is not bouncing anymore
#define buttons_dash_threshold  10   ///< Default key down time needed to be considered the key is held down long enough to repeat in a dash speed
#define buttons_repeat_time  20     ///< Default delay between repeating of a held key
#define buttons_dash_time  10        ///< Default delay between dash repeating of a held key


class analogkeyboad
{

private:
	unsigned long t_last_action = 0;

	int values[5] = { 0, 128, 302, 474, 718 };// analog values


	byte button_sensed = 0; // This indicates which button is sensed or 255 if no button is sensed.
	byte button_status = buttons_up; // This indicates the status of the button if button_sensed is not 255.
	unsigned long button_status_t; // This is the time stamp of the sensed button first in the status stored in button_status.

								   /**
								   * \details This is the most physical layer of the phi_keypads. Senses all input pins for a valid status.
								   * This function is not intended to be call by arduino code but called within the library instead.
								   * If all you want is a key press, call getKey.
								   * \return It returns the button scan code (0-max_button-1) that is pressed down or NO_KEYs if no button is pressed down. The return is 0-based so the value is 0-15 if the array has 16 buttons.
								   */
	byte getButton()
	{

		int temp = analogRead(0);
		for (byte i = 0; i < 5; i++)
		{
			int diff = abs(values[i] - temp); // Find the difference between analog read and stored values.
			if (diff < 20)
			{
				return (i + 1); // returns the button pressed
			}
		}

		return 0;
	}

public:
	byte scanKeypad()
	{
		byte button_pressed = getButton();

		switch (button_status)
		{
		case buttons_up:
			if (button_pressed != NO_KEYs)
			{
				button_sensed == button_pressed;
				button_status_t = millis();
				button_status = buttons_debounce;
			}
			else button_sensed = NO_KEYs;
			break;

		case buttons_debounce:
			if (button_pressed != NO_KEYs)
			{
				if (button_sensed == button_pressed)
				{
					if (millis() - button_status_t > buttons_debounce_time)
					{
						button_status = buttons_pressed;
						button_status_t = millis();
						t_last_action = button_status_t;
						return button_sensed;
					}
				}
				else
				{
					button_status_t = millis();
					button_sensed = button_pressed;
				}
			}
			else
			{
				button_status = buttons_up;
				button_sensed = NO_KEYs;
			}
			break;

		case buttons_pressed:
			if (button_pressed != NO_KEYs)
			{
				if (button_sensed == button_pressed) button_status = buttons_down;
				else button_status = buttons_debounce;
				button_status = buttons_down;
			}
			else button_status = buttons_released;
			button_status_t = millis();
			break;

		case buttons_down:
			if (button_sensed == button_pressed)
			{
				if (millis() - button_status_t > buttons_hold_time)
				{
					button_status = buttons_held;
					button_status_t = millis();
				}
			}
			else
			{
				button_status = buttons_released;
				button_status_t = millis();
			}
			break;

		case buttons_released:
			if (button_pressed == NO_KEYs) button_status = buttons_up;
			else
			{
				button_status = buttons_debounce;
				button_sensed = button_pressed;
				button_status_t = millis();
			}
			break;

		case buttons_held:
			if (button_sensed != button_pressed)
			{
				button_status = buttons_released;
				button_status_t = millis();
				return button_sensed;
			}
			else if (millis() - button_status_t > buttons_repeat_time)
			{
				button_status_t = millis();
				return button_sensed;
			}
			break;
		}
		return NO_KEYs;
	}

};

	



#endif

