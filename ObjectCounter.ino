#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

File myFile;
DS3231 rtc(SDA, SCL);
const int chipSelect = 10;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int ir_pin = 2;
int counter = 0;
int hitObject = false;
int ledPin = 7;
int reset_button = 3;
int reset_state = 0;

void setup() {
Serial.begin(9600);
pinMode(ir_pin,INPUT);
pinMode(ledPin, OUTPUT);
pinMode(reset_button,INPUT);

lcd.init();
lcd.backlight();
lcd.clear();

rtc.begin();

  while (!Serial)
  {
    ;
  }
  Serial.print("Initializing SD Card...");
  
  if (!SD.begin(chipSelect)) 
  {
    lcd.clear();
    lcd.print("SD failed");
    Serial.println("SD failed");
    while(1);
  }
  
  lcd.setCursor(0,0);

  lcd.print("SD done");
  Serial.println("SD done.");
  delay(5000);

  Serial.println("    DATE    |   TIME    | COUNTER");

//  rtc.setTime(19, 33, 0); // h.m.s
//   rtc.setDate(8, 01, 2023); // d.m.y

  myFile=SD.open("data.csv", FILE_WRITE);

  if (myFile) {
    myFile.println("Date, Time, Counter"); 
    }
  myFile.close();
}

void loop() {
int val = digitalRead(ir_pin);
lcd.setCursor(0,0);
lcd.print("Total Count:" + String(counter));
    
  if ((val == 0) && (hitObject == false)) {
    hitObject = true;
    counter++;
    digitalWrite(ledPin, HIGH);
    writeFile();    
   } 
   else if (( (val ==1) && (hitObject == true))) {
      hitObject = false; 
      digitalWrite(ledPin,LOW);
   }
  // reset_counter();
}

void writeFile() {

  myFile = SD.open("data.csv", FILE_WRITE);
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  Serial.print(rtc.getTimeStr());
  Serial.print(" -- ");
  Serial.println(counter);
  
    if (myFile) {
      myFile.print(rtc.getDateStr());
      myFile.print(',');
      myFile.print(rtc.getTimeStr());
      myFile.print(',');
      myFile.println(counter);
      myFile.close();
    }
    else {
    Serial.println("error opening data.txt");
    }
}
