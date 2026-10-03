#pragma once

#include "ofMain.h"

// A simple Weather App: type a place, press Enter, see the current weather.
class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		// openFrameworks calls this when the reply from the weather website arrives.
		void urlResponse(ofHttpResponse & response);

	private:
		void searchWeather();

		ofRectangle searchBox;
		ofRectangle searchButton;
		ofTrueTypeFont font;
		ofTrueTypeFont bigFont;

		std::string searchText;     // what the user has typed
		std::string message;        // text shown under the search box
		bool messageIsError = false;

		bool hasResult = false;     // true once we have weather to show
		std::string location;
		std::string condition;
		float tempC = 0;
		float feelsLikeC = 0;
		float windKph = 0;
		int humidity = 0;
};
