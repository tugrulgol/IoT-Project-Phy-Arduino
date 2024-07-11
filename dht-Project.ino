#include "DHT.h"

#define DHTPIN1 3    
#define DHTTYPE1 DHT22   

#define DHTPIN2 4 
#define DHTTYPE2 DHT11 

#define BUZZER 5

#define MQ135PIN A0

DHT dht1(DHTPIN1, DHTTYPE1);
DHT dht2(DHTPIN2, DHTTYPE2);

void setup() {
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();

  pinMode(BUZZER, OUTPUT);
  analogWrite(BUZZER, 0);

  pinMode(MQ135PIN, INPUT);
}

void loop() {
  // Read temperature and humidity values ​​from DHT22 sensor
  float humidity_dht22 = dht1.readHumidity();
  float temperature_dht22 = dht1.readTemperature();

  // Read temperature and humidity values ​​from DHT11 sensor
  float humidity_dht11 = dht2.readHumidity();
  float temperature_dht11 = dht2.readTemperature();

  int mq135Value = analogRead(MQ135PIN);
  int mq135Threshold = 300;

  bool sensorError = false;
  bool airQualityDanger = false;

  if (isnan(humidity_dht22) || isnan(temperature_dht22)) {
    Serial.println("Cannot read data from DHT22 sensor!");
    sensorError = true;
  } else {
    Serial.print("DHT22 - Temperature: ");
    Serial.print(temperature_dht22);
    Serial.print("°C ");
    Serial.print("Humidity: ");
    Serial.print(humidity_dht22);
    Serial.println("%");
  }

  if (isnan(humidity_dht11) || isnan(temperature_dht11)) {
    Serial.println("Cannot read data from DHT11 sensor!");
    sensorError = true;
  } else {
    Serial.print("DHT11 - Temperature: ");
    Serial.print(temperature_dht11);
    Serial.print("°C ");
    Serial.print("Humidity: ");
    Serial.print(humidity_dht11);
    Serial.println("%");
  }

  if (mq135Value < 20 || mq135Value > 1020) { 
    Serial.println("Cannot read data from MQ-135 sensor!");
    sensorError = true;
  } else {
    if (mq135Value > mq135Threshold) {
      Serial.print("MQ-135 - Air Quality Hazardous! Value: ");
      Serial.println(mq135Value);
      airQualityDanger = true;
    } else {
      Serial.print("MQ-135 - Air Quality Safe. Value: ");
      Serial.println(mq135Value);
    }
  }

   if (sensorError || airQualityDanger) {
      Serial.println("BUZZER_ON");
      for (int i = 0; i < 5; i++) {
        analogWrite(BUZZER, 128);
        delay(200);
        analogWrite(BUZZER, 0);
        delay(200);
      }
  } else {
    Serial.println("System Work");
    analogWrite(BUZZER, 0);
  }

  delay(10000);
}
