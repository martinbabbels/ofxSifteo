#pragma once

#include "ofMain.h";
#include "json.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"

#define BUFFER_SIZE 8192


struct SifteoEvent {
	SifteoEvent(Json::Value &mes) {
		message = mes;
	};
	Json::Value message;
};

class SifteoEventListener {
	
	public:
		virtual void onSifteoEvent(SifteoEvent &message){
		};
		void onSifteoEvent(const void * sender, SifteoEvent &response) {
			onSifteoEvent(response);
		}
	
};

enum SifteoCubeSide {
	TOP		= 0
	,LEFT	= 1
	,BOTTOM	= 2
	,RIGHT	= 3
	,NONE	= 4 
};

class SifteoCube  {
	public:
};

class ofxSifteo  : public ofThread {
	
	public:
		
		ofxSifteo();
		void setup(string pathToSifteoExe);
		void setup();
		void update();
		void stop();
		void exit();
		void send(Json::Value json);
		void addListener(SifteoEventListener *listener);
		void removeListener(SifteoEventListener *listener);
	
	private:
	
		void start();
		void threadedFunction();
		void dispatchEvent(SifteoEvent message);
		void strip(char *s);
	
		Json::Reader					reader;
		ofxSifteo*						instance;
		FILE*							pipe;
		string							exePath;
		bool							hasMessageReady;
		Json::Value						message;
		Poco::BasicEvent<SifteoEvent>	messageEvent;
};
