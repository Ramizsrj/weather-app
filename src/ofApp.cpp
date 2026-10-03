#include "ofApp.h"
#include "Secrets.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Weather App");
	ofBackground(30, 33, 40);

	font.load(OF_TTF_SANS, 16);
	bigFont.load(OF_TTF_SANS, 36);

	searchBox.set(40, 40, 400, 44);
	searchButton.set(450, 40, 110, 44);

	// Tell openFrameworks to call urlResponse() when a web reply arrives.
	ofRegisterURLNotification(this);

	message = "Type a place and press Enter.";
}

//--------------------------------------------------------------
void ofApp::draw(){
	// Search box and button
	ofSetColor(45, 48, 56);
	ofDrawRectangle(searchBox);
	ofSetColor(searchText.empty() ? 120 : 230);
	font.drawString(searchText.empty() ? "Enter a city..." : searchText, 52, 68);

	ofSetColor(70, 130, 220);
	ofDrawRectangle(searchButton);
	ofSetColor(255);
	font.drawString("Search", 470, 68);

	// Message under the box (red if it is an error)
	if(messageIsError){
		ofSetColor(220, 100, 100);
	}else{
		ofSetColor(170);
	}
	font.drawString(message, 40, 125);

	// Weather result
	if(hasResult){
		ofSetColor(255);
		bigFont.drawString(location, 40, 220);
		font.drawString(ofToString(tempC, 1) + " C   (" + condition + ")", 40, 260);

		ofSetColor(180);
		font.drawString("Feels like " + ofToString(feelsLikeC, 1) + " C", 40, 290);
		font.drawString("Humidity: " + ofToString(humidity) + "%", 40, 315);
		font.drawString("Wind: " + ofToString(windKph, 1) + " kph", 40, 340);
	}
}

//--------------------------------------------------------------
void ofApp::searchWeather(){
	if(searchText.empty()){
		message = "Type a location first.";
		messageIsError = true;
		return;
	}

	message = "Searching...";
	messageIsError = false;
	hasResult = false;

	// Build the web address from our key and the place typed (spaces become %20).
	std::string place = searchText;
	ofStringReplace(place, " ", "%20");
	std::string url = "https://api.weatherapi.com/v1/current.json?key=" + WEATHER_API_KEY + "&q=" + place;

	// "Async" means it runs in the background, so the window does not freeze.
	ofLoadURLAsync(url, "weather");
}

//--------------------------------------------------------------
void ofApp::urlResponse(ofHttpResponse & response){
	messageIsError = true;
	hasResult = false;

	// The website tells us what happened with a status number.
	if(response.status == 400){
		message = "Location not found. Check the spelling.";
		return;
	}
	if(response.status == 401){
		message = "The API key is missing or wrong.";
		return;
	}
	if(response.status != 200){
		message = "Something went wrong. Try again.";
		return;
	}

	// 200 means success. The reply is JSON (text with labels), so read it.
	ofJson json;
	try{
		json = ofJson::parse(response.data.getText());
	}catch(std::exception &){
		message = "Could not read the weather data.";
		return;
	}

	// .value("label", backup) gives a safe backup if a label is missing.
	location = json["location"].value("name", std::string("")) + ", " + json["location"].value("country", std::string(""));
	condition = json["current"]["condition"].value("text", std::string("Unknown"));
	tempC = json["current"].value("temp_c", 0.0);
	feelsLikeC = json["current"].value("feelslike_c", 0.0);
	humidity = json["current"].value("humidity", 0);
	windKph = json["current"].value("wind_kph", 0.0);

	messageIsError = false;
	hasResult = true;
	message = "Showing weather for \"" + searchText + "\".";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == OF_KEY_RETURN){
		searchWeather();
	}else if(key == OF_KEY_BACKSPACE){
		if(!searchText.empty()){
			searchText.pop_back();
		}
	}else if(key >= 32 && key <= 126 && searchText.size() < 60){
		// A normal letter, number or space: add it to the text.
		searchText += static_cast<char>(key);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(searchButton.inside(x, y)){
		searchWeather();
	}
}
