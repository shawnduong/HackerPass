#ifndef LED_H
#define LED_H

#define LED_ESP 2
#define LED_RGB_R 5
#define LED_RGB_G 0
#define LED_RGB_B 15

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

/* This is meant to be used once. Interval must be >100 (ms). */
void led_rgb_blink_to_red(uint32_t interval)
{
	led_rgb_off();
	delay(100);
	led_rgb_red();
	delay(interval-100);
}

/* This is meant to be used in a loop. */
void led_rgb_blink_red(uint32_t interval)
{
	led_rgb_off();
	delay(interval/2);
	led_rgb_red();
	delay(interval/2);
}

/* This is meant to be used once. Interval must be >100 (ms). */
void led_rgb_blink_to_green(uint32_t interval)
{
	led_rgb_off();
	delay(100);
	led_rgb_green();
	delay(interval-100);
}

/* This is meant to be used in a loop. */
void led_rgb_blink_green(uint32_t interval)
{
	led_rgb_off();
	delay(interval/2);
	led_rgb_green();
	delay(interval/2);
}

/* This is meant to be used once. Interval must be >100 (ms). */
void led_rgb_blink_to_blue(uint32_t interval)
{
	led_rgb_off();
	delay(100);
	led_rgb_blue();
	delay(interval-100);
}

/* This is meant to be used in a loop. */
void led_rgb_blink_blue(uint32_t interval)
{
	led_rgb_off();
	delay(interval/2);
	led_rgb_blue();
	delay(interval/2);
}

/* This is meant to be used once. Interval must be >100 (ms). */
void led_rgb_blink_to_purple(uint32_t interval)
{
	led_rgb_off();
	delay(100);
	led_rgb_purple();
	delay(interval-100);
}

/* This is meant to be used in a loop. */
void led_rgb_blink_purple(uint32_t interval)
{
	led_rgb_off();
	delay(interval/2);
	led_rgb_purple();
	delay(interval/2);
}

/* This is meant to be used once. Interval must be >100 (ms). */
void led_rgb_blink_to_white(uint32_t interval)
{
	led_rgb_off();
	delay(100);
	led_rgb_white();
	delay(interval-100);
}

/* This is meant to be used in a loop. */
void led_rgb_blink_white(uint32_t interval)
{
	led_rgb_off();
	delay(interval/2);
	led_rgb_white();
	delay(interval/2);
}

void led_rgb_test()
{
	led_rgb_blink_to_red(1000);
	led_rgb_blink_to_green(1000);
	led_rgb_blink_to_blue(1000);
	led_rgb_blink_to_purple(1000);
	led_rgb_blink_to_white(1000);
}

#endif
