/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp()
: Osc_XBE("127.0.0.1", 12347, 12348)

{
	/********************
	********************/
	fp_Log = fopen("../../../Log.csv", "w");
	if(fp_Log == nullptr){
		printf("Log file open NG\n");
		fflush(stdout);
		std::exit(1);
	}	
}

/******************************
******************************/
ofApp::~ofApp()
{
	if(fp_Log) fclose(fp_Log);
	if(Gui_Global) delete Gui_Global;	
}

/******************************
******************************/
void ofApp::exit()
{
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("XBVideo");
	
	ofSetWindowShape( WINDOW_WIDTH, WINDOW_HEIGHT );
	/*
	ofSetVerticalSync(false);
	ofSetFrameRate(0);
	/*/
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	//*/
	
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	setup_Gui();
	
	/* */
	fbo_mixed.allocate(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA);
	clear_fbo(fbo_mixed);
	
	for(int i = 0; i < NUM_TYPES; i++){
		fbo[i].allocate(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA);
		clear_fbo(fbo[i]);
	}
	
	/* */
	load_and_set_mov();

	shader_Add.load( "sj_shader/BlendAdd.vert", "sj_shader/BlendAdd.frag");
}

/******************************
******************************/
void ofApp::load_and_set_mov()
{
	string FileName[NUM_TYPES] = {"mov/Blue.mp4", "mov/Red.mp4"};
	
	for(int i = 0; i < NUM_TYPES; i++){
		mov[i].load(FileName[i]);
		if(!mov[i].isLoaded()) { ERROR_MSG(); std::exit(1); }
		
		SetUp_Start_Mov(&mov[i]);
	}
}

/******************************
******************************/
void ofApp::SetUp_Start_Mov(ofVideoPlayer* video)
{
	if(video->isLoaded()){
		video->setLoopState(OF_LOOP_NORMAL);
		// video->setLoopState(OF_LOOP_PALINDROME);
		video->setSpeed(1);
		video->setVolume(0);
		video->setPaused(false);
		video->setPosition(0); // *.movは、"OF_LOOP_NONE"の時、明示的にsetPosition(0) しないと、戻らないようだ.
		video->play();
	}
}

/******************************
******************************/
void ofApp::clear_fbo(ofFbo& fbo)
{
	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();
}

/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("XBVideo", "gui.xml", 1000, 10);
}

/******************************
******************************/
void ofApp::update_fbo(ofFbo& _fbo, ofVideoPlayer& _mov){
	_fbo.begin();
		ofBackground(0);
		ofSetColor(255);
		
		_mov.draw(0, 0, _fbo.getWidth(), _fbo.getHeight());
	_fbo.end();
}

/******************************
******************************/
void ofApp::ReceiveOsc_from_XBE(){
	while(Osc_XBE.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc_XBE.OscReceive.getNextMessage(m_receive); // 引数がpointerから参照に変わった.
		
		if(m_receive.getAddress() == "/XBE"){
			Gui_Global->a_Calm = m_receive.getArgAsFloat(0);
			Gui_Global->a_Evil = m_receive.getArgAsFloat(1);
		}
	}	
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	for(int i = 0; i < NUM_TYPES; i++){
		if(mov[i].isLoaded()){
			mov[i].update();
			if(mov[i].isFrameNew()){
				update_fbo(fbo[i], mov[i]);
			}
		}
	}
	
	/********************
	********************/
	ReceiveOsc_from_XBE();
}

/******************************
******************************/
void ofApp::draw_mixed(){
	/********************
	********************/
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	// ofDisableAlphaBlending();
	
	/********************
	********************/
	fbo_mixed.begin();
		ofBackground(0);
		ofSetColor(255);
		
		shader_Add.begin();
			shader_Add.setUniform1f( "a_Calm", Gui_Global->a_Calm );
			shader_Add.setUniform1f( "a_Evil", Gui_Global->a_Evil );
			
			shader_Add.setUniformTexture( "Calm", fbo[TYPE_CALM].getTexture(), 1 );
			
			fbo[TYPE_EVIL].draw(0, 0, fbo[TYPE_EVIL].getWidth(), fbo[TYPE_EVIL].getHeight());
		shader_Add.end();
	fbo_mixed.end();	
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	draw_mixed();
	
	/********************
	********************/
	/* */
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	// ofDisableAlphaBlending();
	
	/* */
	ofBackground(0);
	ofSetColor(255);
	
	/* */
	fbo_mixed.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	/********************
	********************/
	if(b_DispGui) Gui_Global->gui.draw();
	
	printf("%5.2f\r", ofGetFrameRate());
	fflush(stdout);
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case 'd':
			b_DispGui = !b_DispGui;
			break;
	}
}

/******************************
******************************/
void ofApp::keyReleased(int key){

}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
