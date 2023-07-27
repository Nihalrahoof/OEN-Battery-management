#include <WiFi.h>
#include <HTTPClient.h> 




const int voltageSensorPin = 35;     // Analog pin for voltage sensor on ESP-WROOM-32 (GPIO35 / A3)
const int currentSensorPin = 36;     // Analog pin for current sensor on ESP-WROOM-32 (GPIO36 / A4)
const int tempSensorPin1 = 34;       // Analog pin for thermistor 1 on ESP-WROOM-32 (GPIO34 / A2)
const int tempSensorPin2 = 39;       // Analog pin for thermistor 2 on ESP-WROOM-32 (GPIO39 / A5)
const int tempSensorPin3 = 32;       // Analog pin for thermistor 3 on ESP-WROOM-32 (GPIO32 / A0)
const int tempSensorPin4 = 33;       // Analog pin for thermistor 4 on ESP-WROOM-32 (GPIO33 / A1)

const int relayPin1 = 26;            // Digital pin for relay 1 control on ESP-WROOM-32
const int relayPin2 = 27;            // Digital pin for relay 2 control on ESP-WROOM-32
const int ledPin = 13;               // Digital pin for LED output on ESP-WROOM-32
const int buzzerPin = 15;            // Digital pin for buzzer output on ESP-WROOM-32


const float maxVoltage = 8.0;        // Maximum allowable voltage (in volts)
const float maxCurrent = 10.0;       // Maximum allowable current (in Amperes)
const int maxTemperature = 600;       // Maximum allowable temperature (in degrees Celsius)

float voltage, current;
int temp1, temp2, temp3, temp4;

//initialising wifi
const char* ssid = "Galaxy M5142E0";      
const char* password = "tomo0694";  

void setup() {

  //connecing to wifi
   Serial.begin(115200);
  delay(50);
  Serial.println("Connecting to WiFi...");

  WiFi.begin((char*)ssid, password); // Cast ssid to char* type

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  
  
  int voltageRaw = analogRead(voltageSensorPin);
  voltage = voltageRaw * (5.0 / 1023.0); // Assuming 10-bit ADC and 5V reference voltage

  // Read current from current sensor
  int currentRaw = analogRead(currentSensorPin);
  current = (currentRaw * 5.0 / 1023.0) * 5.0; // Assuming 10-bit ADC, 5V reference voltage, and 5A full-scale current range

  // Read temperature from thermistor 1
  temp1 = analogRead(tempSensorPin1);

  // Read temperature from thermistor 2
  temp2 = analogRead(tempSensorPin2);

  // Read temperature from thermistor 3
  temp3 = analogRead(tempSensorPin3);

  // Read temperature from thermistor 4
  temp4 = analogRead(tempSensorPin4);



  


 // Check for tolerance conditions
  if (voltage > maxVoltage || current > maxCurrent || temp1 > maxTemperature || temp2 > maxTemperature ) {
    // If any of the tolerance conditions are exceeded, trip 1st relay, turn on the LED, and sound the buzzer
    digitalWrite(relayPin1, HIGH);
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  }
  else if (voltage > maxVoltage || current > maxCurrent || temp3 > maxTemperature || temp4 > maxTemperature) {
    // If any of the tolerance conditions are exceeded, trip 2nd relay, turn on the LED, and sound the buzzer
    digitalWrite(relayPin2, HIGH);
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  }else {
    // If all sensor values are within tolerance, turn off the relay, LED, and buzzer
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }



  

  // Print the sensor values
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Temperature 1: ");
  Serial.print(temp1);
  Serial.println(" (raw)");

  Serial.print("Temperature 2: ");
  Serial.print(temp2);
  Serial.println(" (raw)");

  Serial.print("Temperature 3: ");
  Serial.print(temp3);
  Serial.println(" (raw)");

  Serial.print("Temperature 4: ");
  Serial.print(temp4);
  Serial.println(" (raw)");

  

  

  delay(10); // Delay before taking the next reading
  
if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

   //sending sensor data to flask page
   HTTPClient http;   
   Serial.println("starting to send ");

   http.begin("https://oenlbtms.pythonanywhere.com/mysite/sensordata");
   http.addHeader("Content-Type", "application/json");  
   String httpRequestData = "{\"value1\":\"" + String(temp1) + "\",\"value2\":\"" + String(temp2) +"\",\"value3\":\"" + String(temp3) +"\",\"value4\":\"" + String(temp4) +"\",\"value5\":\"" + String(current) + "\",\"value6\":\"" + String(voltage) + "\"}";
   int httpResponseCode = http.POST(httpRequestData);   

   if(httpResponseCode>0){

    Serial.println(httpResponseCode); 

   }else{

    Serial.println("Error on sending POST");

   }

   http.end();  //Free resources


   if (voltage > maxVoltage || current > maxCurrent || temp1 > maxTemperature || temp2 > maxTemperature || temp3 > maxTemperature || temp4 > maxTemperature) {
    // If any of the tolerance conditions are exceeded, send that values
    HTTPClient http;   
   Serial.println("starting to send ");

   http.begin("https://oenlbtms.pythonanywhere.com/mysite/sensordataerror");
   http.addHeader("Content-Type", "application/json");  
   String httpRequestData = "{\"value1\":\"" + String(temp1) + "\",\"value2\":\"" + String(temp2) +"\",\"value3\":\"" + String(temp3) +"\",\"value4\":\"" + String(temp4) +"\",\"value5\":\"" + String(current) + "\",\"value6\":\"" + String(voltage) + "\"}";
   int httpResponseCode = http.POST(httpRequestData);   

   if(httpResponseCode>0){

    Serial.println(httpResponseCode); 

   }else{

    Serial.println("Error on sending POST");

   }

   http.end();  //Free resources

  }

 }else{

    Serial.println("Error in WiFi connection");   

 }

  delay(30);  //Send a request every 10 seconds

}
