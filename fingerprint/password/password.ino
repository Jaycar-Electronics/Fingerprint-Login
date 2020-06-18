#include <Adafruit_Fingerprint.h>
#include <Keyboard.h>

//choose the id for your finger from the enrol example
#define FINGER_LOGIN_ID 5
//your computer password
#define PASSWORD "YOUR COMPUTER PASSWORD HERE"

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial1);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // For Yun/Leo/Micro/Zero/...

  // set the data rate for the sensor serial port
  finger.begin(57600);
  Keyboard.begin();
  delay(5);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }

  finger.getTemplateCount();

  if (finger.templateCount == 0)
  {
    Serial.println("==========================================");
    Serial.println("There's no recorded fingerprints");
    Serial.println("This will never possibly work");
    Serial.println("Try running the 'enrol.ino' example first");
    Serial.println("which should be in the same project folder");
    Serial.println("==========================================");
    Serial.println("System HALT");
    while (1)
    {
      delay(1);
    }
  }
  else
  {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }
}

void loop() // run over and over again
{
  int id = getFingerprintIDez();
  if (id == -1)
    return;

  if (id == FINGER_LOGIN_ID)
  {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('l');
    Keyboard.releaseAll();
    delay(1000);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_DELETE);
    Keyboard.releaseAll();
    delay(1000);
    Keyboard.println(PASSWORD);
  }
  else if (id == 100)
  {
    Serial.print("Special finger ID: 100 ! ");
  }
  else
  {
    Serial.print("Not sure what to do with id:");
    Serial.println(id);
  }

  delay(50); //don't ned to run this at full speed.
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
    return -1;

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}
