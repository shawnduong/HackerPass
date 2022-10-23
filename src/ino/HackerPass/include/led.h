#ifndef LED_H
#define LED_H

#define LED_ESP 2

#define BLINK_INTERVAL 500

void led_init()
{
	pinMode(LED_ESP, OUTPUT);
}

void led_esp_on()
{
	digitalWrite(LED_ESP, LOW);
}

void led_esp_off()
{
	digitalWrite(LED_ESP, HIGH);
}

#endif
