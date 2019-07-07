/*
Weather Client

You are going to develop a program that gets the statistics about weather in the current city
using information from a certain server. The goal is to calculate statistics using the data from weather server.

To communicate with the weather server you have to implement interface IWeatherServer,
which provides the raw string from the real server for the requested day and time.

The real server (i.e. "weather.com") gets the requests in this format:
"<date>;<time>", for example:
"31.08.2018;03:00"

The server answers on requests with string like this:
"20;181;5.1"
This string contains the weather for the requested time and means next:
"<air_temperature_in_celsius>;<wind_direction_in_degrees>:<wind_speed>".
Wind direction value may be in range from 0 to 359 inclusively, temperature may be negative.

The task:
1. Implement fake server, because interacting with real network is inacceptable within the unit tests.
To do this, you need to use real server responses. Fortunately, you've collected some results for the several dates from the weather server.
Each line means "<request>" : "<response>":

"31.08.2018;03:00" : "20;181;5.1"
"31.08.2018;09:00" : "23;204;4.9"
"31.08.2018;15:00" : "33;193;4.3"
"31.08.2018;21:00" : "26;179;4.5"

"01.09.2018;03:00" : "19;176;4.2"
"01.09.2018;09:00" : "22;131;4.1"
"01.09.2018;15:00" : "31;109;4.0"
"01.09.2018;21:00" : "24;127;4.1"

"02.09.2018;03:00" : "21;158;3.8"
"02.09.2018;09:00" : "25;201;3.5"
"02.09.2018;15:00" : "34;258;3.7"
"02.09.2018;21:00" : "27;299;4.0"

IMPORTANT:
* Server returns empty string if request is invalid.
* Real server stores weather only for times 03:00, 09:00, 15:00 and 21:00 for every date. Do not use other hours in a day.

2. Implement IWeatherClient using fake server.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cmath>
#include <string>

class IWeatherServer
{
public:
    virtual ~IWeatherServer() { }
    // Returns raw response with weather for the given day and time in request
    virtual std::string GetWeather(const std::string& request) = 0;
};

// Implement this interface
class IWeatherClient
{
public:
    virtual ~IWeatherClient() { }
    virtual double GetAverageTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMinimumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetAverageWindDirection(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date) = 0;
};

struct Weather
{
    short temperature = 0;
    unsigned short windDirection = 0;
    double windSpeed = 0;
    bool operator==(const Weather& right)
    {
        return temperature == right.temperature &&
               windDirection == right.windDirection &&
               std::abs(windSpeed - right.windSpeed) < 0.01;
    }
};

class MockWeatherServer : public IWeatherServer
{
public:
    std::string GetWeather(const std::string& request) {
        return this->weatherMap[request];
    }

private:
    std::map<std::string, std::string> weatherMap = {{"31.08.2018;03:00", "20;181;5.1"},
     {"31.08.2018;09:00", "23;204;4.9"},
     {"31.08.2018;15:00", "33;193;4.3"},
     {"31.08.2018;21:00", "26;179;4.5"},

     {"01.09.2018;03:00", "19;176;4.2"},
     {"01.09.2018;09:00", "22;131;4.1"},
     {"01.09.2018;15:00", "31;109;4.0"},
     {"01.09.2018;21:00", "24;127;4.1"},

     {"02.09.2018;03:00", "21;158;3.8"},
     {"02.09.2018;09:00", "25;201;3.5"},
     {"02.09.2018;15:00", "34;258;3.7"},
     {"02.09.2018;21:00", "27;299;4.0"}};
};


class WeatherClient : IWeatherClient
{
public:
    double GetAverageTemperature(IWeatherServer& server, const std::string& date)
    {
        std::vector<Weather> serverResults = this->GetWeatherObjects(server, date);
        double totalTemp = 0.0;
        for (int i = 0; i < int(serverResults.size()); i++) {
            totalTemp = totalTemp + serverResults[i].temperature;
        }

        return totalTemp / serverResults.size();
    }
    double GetMinimumTemperature(IWeatherServer& __unused server, const __unused std::string& date)
    {return 0.0;}
    double GetMaximumTemperature(IWeatherServer& __unused server, const __unused std::string& date)
    {return 0.0;}
    double GetAverageWindDirection(IWeatherServer& __unused server, const __unused std::string& date)
    {return 0.0;}
    double GetMaximumWindSpeed(IWeatherServer& __unused server, const __unused std::string& date)
    {return 0.0;}

private:
    const std::vector<std::string> availableHours = { "03:00", "09:00", "15:00", "21:00" };

    std::vector<Weather> GetWeatherObjects(IWeatherServer& server,std::string date) {
        std::vector<Weather> serverResults;
        for(auto time : this->availableHours)
        {
            Weather weather;
            std::string response = server.GetWeather(date + ';' + time);
            std::replace(response.begin(), response.end(), ';', ' ');
            std::istringstream iss(response);
            iss >> weather.temperature >> weather.windDirection >> weather.windSpeed;
            serverResults.push_back(weather);
        }
        return serverResults;
    }

};

// GetWeather with proper request
TEST(getWeather, getWeatherCorrectRequest) {
    MockWeatherServer server = MockWeatherServer();
    ASSERT_EQ(server.GetWeather("31.08.2018;09:00"), "23;204;4.9");
}

// GetWeather with incorrect request
//* Server returns empty string if request is invalid.
TEST(getWeather, getWeatherIncorrectRequest) {
    MockWeatherServer server = MockWeatherServer();
    ASSERT_EQ(server.GetWeather("fff"), "");
}


// Get Avarage Temperature

TEST(weatherClient, getAvarageTemperatureForProperDate) {
    MockWeatherServer server = MockWeatherServer();
    WeatherClient client = WeatherClient();
    ASSERT_EQ(client.GetAverageTemperature(server, "02.09.2018"), 26.75);
}

