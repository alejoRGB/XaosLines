#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSpout2Sender.h"
#include "ofxXmlSettings.h"
class XaosLine {
public:
	ofVec2f pos;
	ofVec2f vel;
	
	XaosLine();
	~XaosLine();


	void update(float tamañ, float veli);
	void updateVelocity(float velo);
	void draw(ofColor c, int z_);

	float velj;
	float maxVelocity;

};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		
		//todo para hacer la animación
		// array de puntos		
		vector <XaosLine> lines;
		//cantidad de puntos
		int cantidad;
		int cantidadDibujar;
		//camara y pixeles
		ofVideoGrabber cam;
		ofPixels camPixels;
		ofFbo fbo;
		//imagen
		ofImage imagen;
		ofPixels pixelesImagen;
		//variable del tamaño inicial de los puntos
		int size;


		//cosas de la interfaz grafica

		ofxToggle camPic;
		ofxFloatSlider tamaño;
		ofxIntSlider cantidad_;
		ofxIntSlider velocidad;
		ofxToggle fondo;
		ofxButton elegirImagen;

		ofxGuiGroup savePresets;
		ofxButton savepresetA_;
		ofxButton savepresetB_;
		ofxButton savepresetC_;

		ofxGuiGroup setPresets;
		ofxButton presetA_;
		ofxButton presetB_;
		ofxButton presetC_;

		ofxToggle spout_;
		

		ofxPanel panel;

		//selección de imagen por el usuario
		ofImage imagenDelUsuario;
		ofPixels pixelesImagenDelUsuario;
		string camino;
		int witchImageDisplay;
		//Hasta 3 presets cargando archivos xml
		ofxXmlSettings presetA;
		
		
		
		//setear el sender de Spout
		ofxSpout2::Sender spout;

	
};
