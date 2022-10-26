#ifndef RC522_H
#define RC522_H

#include <MFRC522.h>   // MFRC522 library by GithubCommunity.

/* RST is unused (255) to free up a pin. */
MFRC522 mfrc522(4, 255);

void rc522_init()
{
	SPI.begin();
	mfrc522.PCD_Init();
}

bool read_uid(byte attempts, byte *buffer, byte *len)
{
	for (byte i = 0; i < attempts; i++)
	{
		if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
		{
			for (byte j = 0; j < mfrc522.uid.size; j++)
				buffer[j] = mfrc522.uid.uidByte[j];
			*len = mfrc522.uid.size;
			return true;
		}

		delay(100);
	}

	return false;
}

#endif
