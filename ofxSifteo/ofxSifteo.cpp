#include "ofxSifteo.h";

ofxSifteo::ofxSifteo() {
	instance = this;
}

void ofxSifteo::setup() {
	exePath = ofToDataPath("SifteoConnector/exe/SifteoConnector.exe", true);
	hasMessageReady = false;
	start();
}

void ofxSifteo::setup(string pathToSifteoExe) {
	exePath = pathToSifteoExe;
	start();
}

void ofxSifteo::start() {
	if(!isThreadRunning()) {
        startThread(true, false);
    }
}

void ofxSifteo::stop() {
    stopThread();
}

void ofxSifteo::exit() {
	stop();
}

void ofxSifteo::update() {
}

void ofxSifteo::threadedFunction() {
	
	string cmd = "mono ";
	cmd.append(exePath);
	
    if(isThreadRunning()) {
			
		pipe = popen(cmd.c_str(), "r");
		
		if (!pipe) {
			cout << "ofxSifteo: Failed to create pipe" << endl;
			stop();
		}
		
		char buffer[BUFFER_SIZE];

		while(!feof(pipe)) {
			if(fgets(buffer, BUFFER_SIZE, pipe) != NULL) {
				
				cout << buffer << endl;
				hasMessageReady
				if(hasMessageReady) {
					
					Json::FastWriter writer;
					
					string ser_json = writer.write(message);
					int size = ser_json.size();
					
					char char_ser_json[size];
					strcpy(char_ser_json, ser_json.c_str());
					cout << char_ser_json << endl;;
					fputs(char_ser_json, pipe);
					
					hasMessageReady = false;
				}
				
				
				//Json::Value json_message;
				//bool is_parsed = reader.parse(buffer, json_message);
				
				
				
				//if(is_parsed) {
					//SifteoEvent e = SifteoEvent(json_message);	
				//	dispatchEvent(e);
				//}
				 
			}
		}
	}
}

void ofxSifteo::send(Json::Value json) {
	message = json;
	hasMessageReady = true;
}

void ofxSifteo::strip(char* s) {
    s[strcspn(s, "\n" )] = '\0';
}

void ofxSifteo::addListener(SifteoEventListener * listener) {
	messageEvent += Poco::Delegate<SifteoEventListener, SifteoEvent>(listener,&SifteoEventListener::onSifteoEvent);
}

void ofxSifteo::removeListener(SifteoEventListener * listener) {
	messageEvent += Poco::Delegate<SifteoEventListener, SifteoEvent>(listener,&SifteoEventListener::onSifteoEvent);
}

void ofxSifteo::dispatchEvent(SifteoEvent message) {
	messageEvent.notify(instance, message);
}