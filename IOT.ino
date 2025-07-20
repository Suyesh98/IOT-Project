#define BLYNK_TEMPLATE_ID "TMPL6aKTYw9BW"
#define BLYNK_TEMPLATE_NAME "IOT"
#define BLYNK_AUTH_TOKEN "xEnqCns-PE6uF0tJ0ngpH6ApdmGBdZOr"


#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
 // ESP32 pin GPIO23 connected to DHT11 sensor
#define DHT_SENSOR_PIN  23 
#define DHT_SENSOR_TYPE DHT11
#define Buzzer 19
#define relay 27
#define TRIG_PIN   5 // ESP32 pin GPIO5 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN   18// ESP32 pin GPIO18 connected to Ultrasonic Sensor's ECHO pin
 // ESP32 pin GPIO19 connected to Piezo Buzzer's pin
float duration, distance;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27 (from DIYables LCD), 16 column and 2 rows
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
int _moisture,sensor_analog;
const int sensor_pin = 4;
char authentication[] ="xEnqCns-PE6uF0tJ0ngpH6ApdmGBdZOr";
char user[] = "Sambridhi";
char passoword[] = "sanas123";


void setup() {
  WiFi.begin(user,passoword);
  Blynk.begin(authentication, user, passoword);
  
  // initializing the DHT sensor
  dht_sensor.begin(); 
  // initializing the lcd
  lcd.init();         
  lcd.backlight();    
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);   // setting ESP32 pin to output mode
  pinMode(ECHO_PIN, INPUT);    // setting ESP32 pin to input mode
  pinMode(Buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(sensor_pin, INPUT);
  
}


void loop() {
  float humi  = dht_sensor.readHumidity();    // read humidity
  float tempC = dht_sensor.readTemperature();
  sensor_analog = analogRead(sensor_pin);
  _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
  Serial.print("Moisture = ");
  Serial.print(_moisture);  /* Print Temperature on the serial window */
  Serial.println("");
  Serial.println(humi);
 
String readmoisture;
 if (_moisture>=0)
 {
  //turning on the water pump
   digitalWrite(relay,LOW);
   readmoisture = "soil is dry";
 }
  else if(_moisture<=100)
 {
  //turning off the water pump
  digitalWrite(relay,HIGH);
  readmoisture = "Soil is wet";
 }

  lcd.clear();
    // check whether the reading is successful or not
  if (isnan(tempC) || isnan(humi)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 0);  // display position
    lcd.print("T: ");
    lcd.print(tempC);     // display the temperature
    lcd.print("C");
    lcd.print("D:");
    lcd.print(distance);
    lcd.print("cm");

    lcd.setCursor(0, 1);  // display position
    lcd.print("H: ");
    lcd.print(humi);      // display the humidity
    lcd.print("%");
    lcd.print("M:");
    lcd.print(_moisture);

  }
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance = 0.017 * duration;
  if (distance < 5){
    digitalWrite (Buzzer,HIGH);
  }
  else{
   digitalWrite (Buzzer,LOW);
  }
  // print the value to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(humi);
  Serial.println("%");

   Blynk.virtualWrite(V0,tempC);
   Blynk.virtualWrite(V1,humi);
   Blynk.virtualWrite(V2,_moisture);
   



  delay(2000);
}
