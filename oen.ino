#include <WiFi.h>
#include <HTTPClient.h> 




const int voltageSensorPin = 36;     // Analog pin for voltage sensor
const int currentSensorPin = 39;     // Analog pin for current sensor
const int temperatureSensorPin = 34; // Analog pin for LM35 temperature sensor

float voltage, current, temperature; // Variables to store sensor values

//initialising wifi
const char* ssid = "nihalrahoof";      
const char* password = "nihalrahoof123nn";  

void setup() {

  //connecing to wifi
   Serial.begin(9600);
  delay(1000);

  WiFi.begin((char*)ssid, password); // Cast ssid to char* type

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  
  
  // Read voltage from voltage sensor
  int voltageRaw = analogRead(voltageSensorPin);
  voltage = voltageRaw * (5.0 / 1023.0); // Assuming 10-bit ADC and 5V reference voltage

 
  
  // Read current from current sensor
  int currentRaw = analogRead(currentSensorPin);
  current = (currentRaw * 5.0 / 1023.0) * 5.0; // Assuming 10-bit ADC, 5V reference voltage, and 5A full-scale current range

 
  
  // Read temperature from LM35 sensor
  int lm35Raw = analogRead(temperatureSensorPin);
  temperature = (lm35Raw * 5.0 / 1023.0) * 100.0; // Assuming 10-bit ADC, 5V reference voltage, and LM35 outputting 10mV/°C

 
  
  // Print the sensor values
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  

  delay(1000); // Delay before taking the next reading
  
if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

   //sending sensor data to flask page
   HTTPClient http;   

   http.begin("https://innohack.pythonanywhere.com/mysite/sensordata");
   http.addHeader("Content-Type", "application/json");  
   String httpRequestData = "{\"value1\":\"" + String(temperature) + "\",\"value2\":\"" + String(current) + "\",\"value3\":\"" + String(voltage) + "\"}";
   int httpResponseCode = http.POST(httpRequestData);   

   if(httpResponseCode>0){

    Serial.println(httpResponseCode); 

   }else{

    Serial.println("Error on sending POST");

   }

   http.end();  //Free resources

 }else{

    Serial.println("Error in WiFi connection");   

 }

  delay(2000);  //Send a request every 10 seconds

}
