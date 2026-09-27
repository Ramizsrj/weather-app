#include "ofApp.h"
#include "Secrets.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Weather App");
	ofBackground(30, 33, 40);
	ofSetVerticalSync(true);

	bodyFont.load(OF_TTF_SANS, 16);
	bigFont.load(OF_TTF_SANS, 36);

	// Get told whenever a background network request finishes.
	ofRegisterURLNotification(this);

	if(WEATHER_API_KEY == "YOUR_WEATHERAPI_KEY_HERE"){
		statusMessage = "Add your WeatherAPI key to src/Secrets.h before searching.";
		statusIsError = true;
	}else{
		statusMessage = "Type a location and press Enter to search.";
		statusIsError = false;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	// Nothing to do here - network replies arrive automatically through
	// urlResponse() below.
}

//--------------------------------------------------------------
void ofApp::draw(){
	// --- search box ---
	ofRectangle box = getSearchBoxBounds();
	ofSetColor(45, 48, 56);
	ofDrawRectangle(box);

	ofSetColor(searchText.empty() ? ofColor(120) : ofColor(230));
	std::string shown = searchText.empty() ? "Enter a city..." : searchText;
	bodyFont.drawString(shown, box.x + 12, box.y + box.height / 2 + 6);

	// --- search button ---
	ofRectangle button = getSearchButtonBounds();
	ofSetColor(70, 130, 220);
	ofDrawRectangle(button);
	ofSetColor(255);
	bodyFont.drawString("Search", button.x + 20, button.y + button.height / 2 + 6);

	// --- status / error message ---
	ofSetColor(statusIsError ? ofColor(220, 100, 100) : ofColor(170));
	bodyFont.drawString(statusMessage, 40, box.y + box.height + 40);

	// --- weather result ---
	if(hasResult){
		ofSetColor(255);
		bigFont.drawString(resultLocation, 40, 220);

		ofSetColor(255);
		std::string tempLine = ofToString(resultTempC, 1) + " C   (" + resultCondition + ")";
		bodyFont.drawString(tempLine, 40, 260);

		ofSetColor(180);
		std::string feelsLine = "Feels like " + ofToString(resultFeelsLikeC, 1) + " C";
		bodyFont.drawString(feelsLine, 40, 290);

		std::string humidityLine = "Humidity: " + ofToString(resultHumidity) + "%";
		bodyFont.drawString(humidityLine, 40, 315);

		std::string windLine = "Wind: " + ofToString(resultWindKph, 1) + " kph";
		bodyFont.drawString(windLine, 40, 340);
	}
}

//--------------------------------------------------------------
ofRectangle ofApp::getSearchBoxBounds() const{
	return ofRectangle(40, 40, 400, 44);
}

//--------------------------------------------------------------
ofRectangle ofApp::getSearchButtonBounds() const{
	ofRectangle box = getSearchBoxBounds();
	return ofRectangle(box.getRight() + 10, box.y, 110, 44);
}

//--------------------------------------------------------------
void ofApp::searchWeather(){
	if(WEATHER_API_KEY == "YOUR_WEATHERAPI_KEY_HERE"){
		statusMessage = "Add your WeatherAPI key to src/Secrets.h before searching.";
		statusIsError = true;
		return;
	}
	if(searchText.empty()){
		statusMessage = "Type a location first.";
		statusIsError = true;
		return;
	}

	statusMessage = "Searching for \"" + searchText + "\"...";
	statusIsError = false;
	hasResult = false;

	// Build the API request URL. "q" is the location the user typed.
	std::string url = "https://api.weatherapi.com/v1/current.json?key=" + WEATHER_API_KEY + "&q=" + searchText;

	// Cancel any search that's still in flight so an old, slow reply can't
	// overwrite a newer one.
	if(pendingRequestId != -1){
		ofRemoveURLRequest(pendingRequestId);
	}
	pendingRequestId = ofLoadURLAsync(url, "weather");
}

//--------------------------------------------------------------
void ofApp::urlResponse(ofHttpResponse & response){
	// Ignore replies to requests we've already moved on from.
	if(response.request.getId() != pendingRequestId){
		return;
	}
	pendingRequestId = -1;

	if(response.status != 200){
		hasResult = false;
		statusIsError = true;
		if(response.status == 400){
			statusMessage = "Location not found. Check the spelling and try again.";
		}else if(response.status == 401){
			statusMessage = "The WeatherAPI key is missing or invalid.";
		}else{
			statusMessage = "Something went wrong (status " + ofToString(response.status) + "). Try again.";
		}
		return;
	}

	// Parse the JSON reply. ofJson is openFrameworks' built-in JSON type.
	ofJson json;
	try{
		json = ofJson::parse(response.data.getText());
	}catch(std::exception &){
		hasResult = false;
		statusIsError = true;
		statusMessage = "Couldn't understand the response from the API.";
		return;
	}

	// Pull out the fields we care about. Using .value("key", fallback)
	// means a field that's missing just falls back safely instead of
	// crashing the app.
	std::string name = json["location"].value("name", std::string(""));
	std::string country = json["location"].value("country", std::string(""));
	resultLocation = name + (country.empty() ? "" : (", " + country));

	resultCondition = json["current"]["condition"].value("text", std::string("Unknown"));
	resultTempC = json["current"].value("temp_c", 0.0);
	resultFeelsLikeC = json["current"].value("feelslike_c", 0.0);
	resultHumidity = json["current"].value("humidity", 0);
	resultWindKph = json["current"].value("wind_kph", 0.0);

	hasResult = true;
	statusIsError = false;
	statusMessage = "Showing weather for \"" + searchText + "\".";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == OF_KEY_RETURN){
		searchWeather();
		return;
	}
	if(key == OF_KEY_BACKSPACE){
		if(!searchText.empty()){
			searchText.pop_back();
		}
		return;
	}
	// Only accept normal printable characters (letters, numbers, spaces,
	// commas, etc.) so control keys don't end up in the search text.
	if(key >= 32 && key <= 126 && searchText.size() < 60){
		searchText += static_cast<char>(key);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(getSearchButtonBounds().inside(x, y)){
		searchWeather();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
