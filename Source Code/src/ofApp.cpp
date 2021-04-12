#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//setear la camara, los pixeles y el fbo
	ofSetWindowTitle("Xaos Lines");
	ofSetEscapeQuitsApp(false);
	cam.listDevices();
	cam.setDeviceID(0);
	cam.initGrabber(ofGetWidth(), ofGetHeight());
	camPixels = cam.getPixels();
	ofEnableAlphaBlending();

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	fbo.begin();
	ofClear(0,0,0);
	fbo.end();

	//crear los puntos
	cantidad = 10000;
	cantidadDibujar = 1;
	for (int i = 0; i < cantidad;i++) {
		XaosLine newXaosLines;
		lines.push_back(newXaosLines);

	}
	

	//iniciar el tamaño de las bolas con un valor
	size =15;
	
	//el valor en 0 de witchImageDisplay indica 
	//que se carga la imagen por defecto
	witchImageDisplay = 0;

	
	panel.setup("XaosLines", "XaosLinessettings.xml");
	panel.add(camPic.setup("Camera or Picture",true));
	panel.add(tamaño.setup("Size", 15, 1, 50));
	panel.add(cantidad_.setup("Amount", 500, 0, 10000));
	panel.add(velocidad.setup("Velocity", 2, 0, 5));
	panel.add(fondo.setup("Trails", true));
	panel.add(elegirImagen.setup("Seleccionar imagen"));
	panel.add(spout_.setup("Send via Spout", false));
	//grupo guardar presets
	savePresets.setup("Save Presets");
	savePresets.add(savepresetA_.setup("Save as preset A"));
	savePresets.add(savepresetB_.setup("Save as preset B"));
	savePresets.add(savepresetC_.setup("Save as preset C"));
	panel.add(&savePresets);
	//grupo setear presets
	setPresets.setup("Select Presets");
	setPresets.add(presetA_.setup("Preset A"));
	setPresets.add(presetB_.setup("Preset B"));
	setPresets.add(presetC_.setup("Preset C"));
	panel.add(&setPresets);
	panel.loadFromFile("XaosLinessettings.xml");
	
	//para selecciónar imagen


	//cargar imagen por defecto
	imagen.load("osaka.jpg");
	imagen.resize(ofGetWidth(), ofGetHeight());
	pixelesImagen = imagen.getPixels();
	/*
	//presetsofxXmlSettings settings;
	presetA.addTag("XaosLines");
	presetA.pushTag("XaosLines");
	presetA.setValue("Size", 0);
	presetA.setValue("Velocity", 0);
	presetA.setValue("Trails", 1);
	presetA.setValue("Send_Via_Spout", 0);
	presetA.popTag();
	presetA.saveFile("presetA.xml");
	*/
}


//--------------------------------------------------------------
void ofApp::update() {

	imagen.update();
	camPixels = cam.getPixels();
	size = tamaño;
	for (int i = 0; i < cantidad_;i++) {
		ofColor c;

		float tamañ = tamaño;
		lines[i].update(tamañ, velocidad);
	}
	cam.update();
	//dibuja la animación adentro del fbo para despues enviarla
	fbo.begin();
	if (!fondo) {
		ofBackground(0, 20);
	}
	for (int i = 0; i < cantidad_;i++) {
		ofColor d;


		//cambio de elección de pixeles de la camara o la imagen

		switch (witchImageDisplay) {

		case 0:
			if (camPic) {
				d.set(camPixels.getColor(lines[i].pos.x, lines[i].pos.y));
			}
			else {
				d.set(pixelesImagen.getColor(lines[i].pos.x, lines[i].pos.y));
			}
			break;
		case 1:
			if (camPic) {
				d.set(camPixels.getColor(lines[i].pos.x, lines[i].pos.y));
			}
			else {
				d.set(imagenDelUsuario.getColor(lines[i].pos.x, lines[i].pos.y));
			}
			break;
		default:		
			if (camPic) {
			d.set(camPixels.getColor(lines[i].pos.x, lines[i].pos.y));
		}
			   else {
			d.set(pixelesImagen.getColor(lines[i].pos.x, lines[i].pos.y));
		}
			   break;

		}

		lines[i].draw(d, size);
	}
	fbo.end();

	if (elegirImagen == true) {
		ofFileDialogResult result = ofSystemLoadDialog("Load file");
		if (result.bSuccess) {
			string path = result.getPath();
			// load your file at `path`
			camino = path;
			witchImageDisplay = 1;
			imagenDelUsuario.load(camino);
			imagenDelUsuario.resize(ofGetWidth(), ofGetHeight());
		}
	}
	//guardado de seteos actuales como presets
	if (savepresetA_ == true) {
		panel.saveToFile("presetA.xml");
	}
	if (savepresetB_ == true) {
		panel.saveToFile("presetB.xml");

	}
	if (savepresetC_ == true) {
		panel.saveToFile("presetC.xml");

	}
	//Selección de presets
	
	if (presetA_ == true) {
		panel.loadFromFile("presetA.xml");
	}
	if (presetB_ == true) {
		panel.loadFromFile("presetB.xml");
	
	}
	if (presetC_ == true) {
		panel.loadFromFile("presetC.xml");
	
	}

	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	//primero dibuja de fondo el fbo
	fbo.draw(0,0);
	//despues el panel grafico
	panel.draw();
	

	

	//Sender de spout
	if (spout_) {
		spout.sendTexture(fbo.getTexture(), "XaosLines");
	}
	

	}
//--------------------------------------------------------------
void ofApp::exit() {
	panel.saveToFile("XaosLinessettings.xml");
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Funciones de la clase XaosLine
XaosLine::XaosLine() {
	//setear posición inicial y velocidad random 
	pos.set(ofGetWidth() / 2, ofGetHeight() / 2);
	vel.set(ofRandom(-2,2), ofRandom(-2, 2));
	
	velj = 2;
	maxVelocity = 5;
}

XaosLine::~XaosLine() {

}

void XaosLine::update(float tamañ,float veli) {
	//ofSetColor(c);
	
	if (veli != velj) {
		XaosLine::updateVelocity(veli);
	}
	velj = veli;
	
	pos = pos + vel;

	//checkear si el punto se choco con el borde, y hacer que rebote
	if (pos.x > (ofGetWidth() - (25))) {
		vel.x = -vel.x;
	}
	if (pos.x < (0 + (25))) {
		vel.x = -vel.x;
	}
	if (pos.y > (ofGetHeight() - (25))) {
		vel.y = -vel.y;
	}	
	if (pos.y < (0 + (25))) {
		vel.y = -vel.y;
	}
}

void XaosLine::updateVelocity(float velo) {
	vel.set(ofRandom(-velo, velo), ofRandom(-velo, velo));
}
void XaosLine::draw(ofColor c, int z_) {
	ofSetColor(c);
	ofDrawEllipse(pos.x, pos.y, z_, z_);
}