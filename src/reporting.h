#include <Arduino.h>
#include <sstream>
#include <WiFi.h>
#include <HTTPClient.h>

using namespace std;

// Reporter class to handle measurements and forward data
class Reporter {
public:
    // Constructor
    Reporter(float temperature, float pressure, float lightLevel) {
        _temperature = temperature;
        _pressure = pressure;
        _lightLevel = lightLevel;
    }

    void printReport() {
        // Create string stream
        stringstream report;
        // Set decimal precision to 2
        report.precision(2);
        report << fixed;

        // Build string with data from sensors
        report << "\n-----------[ WEATHER REPORT ]-----------\n\n";
        report << "\tTemperature:\t\t" << _temperature << "\t*C" << "\n";
        report << "\tPressure:\t\t\t" << _pressure << "\tPa" << "\n";
        report << "\tLight level:\t\t" << _lightLevel << "\tLux" << "\n";
        report << "\n----------------------------------------\n";

        // Output generated string
        Serial.print(report.str().c_str());
    }

    void sendToServer() {
        // Client to use Wi-Fi
        WiFiClient wifiClient;
        // Client to make HTTP requests
        HTTPClient httpClient;

        // Start HTTP client
        httpClient.begin(
                wifiClient,
                "http://192.168.154.167:8080/data"
        );

        // Add header to specify data type
        httpClient.addHeader("Content-Type", "application/json");
        // Post data as JSON to custom API
        httpClient.POST(formattedData());
    }

private:
    float _temperature;
    float _pressure;
    float _lightLevel;

    // Formats data as JSON string
    String formattedData() {
        // Create string stream
        stringstream json;

        // Build JSON string
        json << "{\"temperature\":" << _temperature
            << ",\"pressure\":" << _pressure
            << ",\"lightLevel\":" << _lightLevel << "}";

        // Output generated string
        return json.str().c_str();
    }
};
