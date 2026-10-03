# Weather App

Repository: https://github.com/Ramizsrj/weather-app

A Data Driven App built with openFrameworks (C++) that shows the current
weather for any location using [WeatherAPI](https://www.weatherapi.com/).

## Setup

1. Get a free API key: https://www.weatherapi.com/signup.aspx (no credit
   card needed).
2. Open `src/Secrets.h` and replace `YOUR_WEATHERAPI_KEY_HERE` with your
   key. This file is gitignored so your key is never pushed to GitHub - a
   template is kept in `src/Secrets.example.h`.
3. Open `weather.sln` in Visual Studio and build/run (Debug|x64).

## Features

- Type a location, press Enter (or click Search)
- Shows current temperature, condition, "feels like" temperature,
  humidity and wind speed
- Handles a location that isn't found, a missing/invalid API key, and
  other network errors with a clear on-screen message

## Structure

Everything lives in `src/ofApp.cpp` to keep the code easy to follow:
- `searchWeather()` builds the request URL and starts it in the
  background (`ofLoadURLAsync`), so the window never freezes
- `urlResponse()` is called automatically once the reply arrives; it
  reads the JSON and either fills in the result fields or sets an error
  message
- `draw()` just displays whatever is currently stored in those fields

## Documents

The `docs` folder has the development document and the video script.
