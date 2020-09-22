#include "SckBase.h"

void SckBase::buttonEvent()
{
	// Don't bother user with charge blinks while he is operating the kit
	led.chargeStatus = led.CHARGE_NULL;

	if (!butState){
		// Button Down
		sckOut("Button Down", PRIO_LOW);

	} else {
		// Button Up
		sckOut("Button Up", PRIO_LOW);

		if (sckOFF) {
			
			goToSleep();

		} else if (st.sleeping) {

			if (st.mode == MODE_NOT_CONFIGURED) enterSetup();
			st.sleeping = false;

		} else if (st.onSetup) {

			st.onSetup = false;
			if (st.mode == MODE_NOT_CONFIGURED) st.mode = MODE_NET;
			led.update(st.error, st.mode, st.onSetup);

			if (st.mode == MODE_NET) ESPcontrol(ESP_REBOOT);

		} else enterSetup();
	}
}
void SckBase::buttonStillDown()
{
	uint32_t pressedTime = millis() - lastUserEvent;

	if (pressedTime >= buttonVeryLong) {

		sprintf(outBuff, "Button pressed for %lu milliseconds: Very long press", pressedTime);
		sckOut(PRIO_LOW);

		sckOFF = false;

		// Factory defaults
		saveConfig(true);
		sck_reset();

	} else if (pressedTime >= buttonLong && !sckOFF) {

		sprintf(outBuff, "Button pressed for %lu milliseconds: Long press", pressedTime);
		sckOut(PRIO_LOW);

		sckOFF = true;
		led.off();

		ESPcontrol(ESP_OFF);
	}

}
void SckBase::butFeedback()
{
	lastUserEvent = millis();
	if (!butState){
		if (sckOFF) sck_reset();
		if (!st.onSetup) {
			if (st.error != ERROR_NONE && st.error != ERROR_BATT) led.setColor(led.YELLOW_LIGHT);
			else if (st.mode == MODE_NET) led.setColor(led.BLUE_LIGHT);
			else if (st.mode == MODE_SD) led.setColor(led.PINK_LIGHT);
		} else if (st.onSetup) led.setColor(led.RED_LIGHT);
	}
}
