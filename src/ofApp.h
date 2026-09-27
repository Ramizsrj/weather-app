#pragma once

#include "ofMain.h"

// A simple Weather App.
//
// Everything lives in one class to keep the code easy to follow:
//  - type a location and press Enter
//  - it asks weatherapi.com for the current weather (this happens in the
//    background so the window never freezes while waiting)
//  - the result (or an error message) is drawn on screen
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// Called automatically by openFrameworks whenever a background
		// network request (started with ofLoadURLAsync) finishes.
		void urlResponse(ofHttpResponse & response);

	private:
		void searchWeather();          // starts the API request
		ofRectangle getSearchBoxBounds() const;
		ofRectangle getSearchButtonBounds() const;

		std::string searchText;        // what the user has typed so far
		std::string statusMessage;     // shown under the search box
		bool statusIsError = false;

		bool hasResult = false;
		std::string resultLocation;    // e.g. "London, United Kingdom"
		std::string resultCondition;   // e.g. "Partly cloudy"
		float resultTempC = 0;
		float resultFeelsLikeC = 0;
		int resultHumidity = 0;
		float resultWindKph = 0;

		int pendingRequestId = -1;     // id of the search currently in flight (-1 = none)

		ofTrueTypeFont bodyFont;
		ofTrueTypeFont bigFont;
};
