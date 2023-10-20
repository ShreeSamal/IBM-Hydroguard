#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "DHT.h"

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define ONE_WIRE_BUS 2                          //D4 pin of nodemcu
#define WATER_SENSOR 14                      //D5

#define WIFI_SSID "Shree"
#define WIFI_PASSWORD "#Cehhack123"


#define API_KEY "AIzaSyAaEGeEJPhCjW_bMpi3UHltzV4TkGqdDEc"


#define USER_EMAIL "aaman.bhowmick21@gmail.com"
#define USER_PASSWORD "AamanB@21"


#define DATABASE_URL "https://ibm-hackathon-f5e60-default-rtdb.asia-southeast1.firebasedatabase.app/"


#define VREF 3.3              // analog reference voltage(Volt) of the ADC
#define SCOUNT  30            // sum of sample point
#define K_VALUE 0.5           // Your sensor's calibration constant

#define S0 16                             
#define S1 5                             
#define S2 4                      
#define S3 0                             
#define SIG A0



#define DHTPIN 12 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;

String tempPath = "/temperature";
String flowPath = "/flowrate";
String tdsPath = "/tdsvalue";
String ecPath = "/ecvalue";
String phPath = "/phvalue";
String turbidityPath = "/turbidityvalue";
String surroundingTempPath = "/surroundingTemp";
String surroundingHumPath = "/surroundingHum";
String timePath = "/timestamp";

String parentPath;

FirebaseJson json;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variable to save current epoch time
int timestamp;


// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 5000;


// Water flow
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;

float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;


int analogBuffer[SCOUNT];     // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float ecValue = 0;  // Added variable for electrical conductivity
float temperature = 0;       // current temperature for compensation






float calibration_value = 21.34 - 0.7;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

float ph_act;




float Turbidity_Sensor_Voltage;
int samples = 600;
float ntu;




void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

OneWire oneWire(ONE_WIRE_BUS);
 
DallasTemperature sensors(&oneWire);            // Pass the oneWire reference to Dallas Temperature.



int getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}




float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}





void setup(void)
{
  Serial.begin(9600); 
  sensors.begin();
  initWiFi();
  timeClient.begin();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";


  pinMode(WATER_SENSOR, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
  attachInterrupt(digitalPinToInterrupt(WATER_SENSOR), pulseCounter, FALLING);

  pinMode(S0,OUTPUT);                         
  pinMode(S1,OUTPUT);                       
  pinMode(S2,OUTPUT);                 
  pinMode(S3,OUTPUT);                     
  pinMode(SIG, INPUT);   

  dht.begin();
}

void loop(void)
{ 
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();



    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      pulse1Sec = pulseCount;
      pulseCount = 0;
      flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
      previousMillis = millis();
      // flowMilliLitres = (flowRate / 60) * 1000;
      // totalMilliLitres += flowMilliLitres;

      // Print the flow rate for this second in litres / minute
      Serial.print("Flow rate: ");
      Serial.print(int(flowRate));  // Print the integer part of the variable
      Serial.print("L/min");
      Serial.print("\t");       // Print tab space

      // Print the cumulative total of litres flowed since starting
      // Serial.print("Output Liquid Quantity: ");
      // Serial.print(totalMilliLitres);
      // Serial.print("mL / ");
      // Serial.print(totalMilliLitres / 1000);
      // Serial.println("L");
    }


    float h1 = dht.readHumidity();

    float t1 = dht.readTemperature();

    float f1 = dht.readTemperature(true);


    if (isnan(h1) || isnan(t1) || isnan(f1)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }


    Serial.print("Humidity Dht1: ");
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("Temperature Dht1: ");
    Serial.print(t1);
    Serial.println(" *C ");


    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);
    sensors.requestTemperatures();  

    Serial.println("Temperature is: ");
    Serial.println(sensors.getTempCByIndex(0)); 

    temperature = sensors.getTempCByIndex(0);


    digitalWrite(S0,LOW); digitalWrite(S1,LOW); digitalWrite(S2,LOW); digitalWrite(S3,LOW);

    for (int i = 0; i < SCOUNT; i++) {
      analogBuffer[i] = analogRead(SIG);
    }

    // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    averageVoltage = getMedianNum(analogBuffer, SCOUNT) * (float) VREF / 1024.0;

    // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0)); 
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    // temperature compensation
    float compensationVoltage = averageVoltage / compensationCoefficient;

    // convert voltage value to tds value
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;

    // Calculate electrical conductivity (EC)
    ecValue = averageVoltage * K_VALUE;

    // Display both TDS and EC values
    Serial.print("TDS Value: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");

    Serial.print("EC Value: ");
    Serial.print(ecValue, 2);  // You can adjust the decimal places as needed
    Serial.println(" µS/cm");







    for (int i = 0; i < 10; i++) {
      digitalWrite(S0, HIGH); digitalWrite(S1, LOW); digitalWrite(S2, LOW); digitalWrite(S3, LOW);
      buffer_arr[i] = analogRead(SIG);
      delay(30);
    }

    for (int i = 0; i < 9; i++) {
      for (int j = i + 1; j < 10; j++) {
        if (buffer_arr[i] > buffer_arr[j]) {
          temp = buffer_arr[i];
          buffer_arr[i] = buffer_arr[j];
          buffer_arr[j] = temp;
        }
      }
    }

    avgval = 0;
    for (int i = 2; i < 8; i++)
      avgval += buffer_arr[i];

    float volt = (float)avgval * 5.0 / 1024 / 6;
    ph_act = -(-5.70 * volt + calibration_value);

    Serial.println("pH Val: ");
    Serial.print(ph_act);




    Turbidity_Sensor_Voltage = 0;
    for(int i=0; i<samples; i++)
    {   
        digitalWrite(S0,LOW); digitalWrite(S1,HIGH); digitalWrite(S2,LOW); digitalWrite(S3,LOW);
        Turbidity_Sensor_Voltage += ((float)analogRead(SIG)/1023)*5;
    }
    
    Turbidity_Sensor_Voltage = Turbidity_Sensor_Voltage/samples;
     
    Turbidity_Sensor_Voltage = round_to_dp(Turbidity_Sensor_Voltage,2);
    if(Turbidity_Sensor_Voltage < 2.5){
      ntu = 3;
    }else{
      ntu = (-1120.4*pow(Turbidity_Sensor_Voltage, 2)+ 5742.3*Turbidity_Sensor_Voltage - 4352.9)/1000; 
    }

    Serial.print(Turbidity_Sensor_Voltage);
    Serial.print(" V\t");
    Serial.print(ntu);
    Serial.println(" NTU");





    parentPath = databasePath + "/" + String(timestamp);

    json.set(tempPath.c_str(), String(sensors.getTempCByIndex(0)));
    json.set(flowPath.c_str(), String(flowRate));
    json.set(tdsPath.c_str(), String(tdsValue));
    json.set(ecPath.c_str(), String(ecValue));
    json.set(phPath.c_str(), String(ph_act));
    json.set(turbidityPath.c_str(), String(ntu));
    json.set(surroundingTempPath.c_str(), String(t1));
    json.set(surroundingHumPath.c_str(), String(h1));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }
}