#ifndef LED_H
#define LED_H

#define LED_ESP 2
#define LED_RGB_R 5
#define LED_RGB_G 0
#define LED_RGB_B 15

#define BLINK_INTERVAL 500

void led_init()
{
	pinMode(LED_ESP, OUTPUT);
	pinMode(LED_RGB_R, OUTPUT);
	pinMode(LED_RGB_G, OUTPUT);
	pinMode(LED_RGB_B, OUTPUT);
}

void led_esp_on()
{
	digitalWrite(LED_ESP, LOW);
}

void led_esp_off()
{
	digitalWrite(LED_ESP, HIGH);
}

void led_rgb_red()
{
	analogWrite(LED_RGB_R, 255);
	analogWrite(LED_RGB_G, 0);
	analogWrite(LED_RGB_B, 0);
}

void led_rgb_green()
{
	analogWrite(LED_RGB_R, 0);
	analogWrite(LED_RGB_G, 255);
	analogWrite(LED_RGB_B, 0);
}

void led_rgb_blue()
{
	analogWrite(LED_RGB_R, 0);
	analogWrite(LED_RGB_G, 0);
	analogWrite(LED_RGB_B, 255);
}

void led_rgb_purple()
{
	analogWrite(LED_RGB_R, 128);
	analogWrite(LED_RGB_G, 0);
	analogWrite(LED_RGB_B, 255);
}

void led_rgb_white()
{
	analogWrite(LED_RGB_R, 128);
	analogWrite(LED_RGB_G, 192);
	analogWrite(LED_RGB_B, 255);
}

void led_rgb_off()
{
	analogWrite(LED_RGB_R, 0);
	analogWrite(LED_RGB_G, 0);
	analogWrite(LED_RGB_B, 0);
}

void led_rgb_test()
{
	led_rgb_red();
	delay(500);
	led_rgb_green();
	delay(500);
	led_rgb_blue();
	delay(500);
	led_rgb_purple();
	delay(500);
	led_rgb_white();
	delay(500);
}

#endif
