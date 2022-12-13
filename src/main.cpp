#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>
#include <reporting.h>

using namespace std;

// Create sensor interfaces
Adafruit_BMP280 bmp;
BH1750 bh;

void setup() {
    // Start serial monitor
    Serial.begin(9600);

    // Connect to Wi-Fi
    WiFi.begin("espWiFi", "password");

    // Wait for connection
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        // Print a period and wait half a second if there is no connection
        Serial.print(".");
        delay(500);
    }

    // Start taking input from BMP sensor with default settings
    bmp.begin(0x76);
    bmp.setSampling(
            Adafruit_BMP280::MODE_FORCED,
            Adafruit_BMP280::SAMPLING_X2,
            Adafruit_BMP280::SAMPLING_X16,
            Adafruit_BMP280::FILTER_X16,
            Adafruit_BMP280::STANDBY_MS_500
    );

    // Start taking input the BH sensor with default settings
    bh.begin();
    bh.configure(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

void loop() {
    // Read values from sensors
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0;
    float lightLevel = bh.readLightLevel();

    // Create reporter
    Reporter reporter = Reporter(temperature, pressure, lightLevel);
    // Print data to console
    reporter.printReport();
    // Send data to custom API
    reporter.sendToServer();

    // Wait one minute until next measurement
    delay(60000);
}
