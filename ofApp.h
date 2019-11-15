/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "ofxOsc_BiDirection.h"

#include "sj_common.h"

/************************************************************
************************************************************/

/****************************************
****************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		TYPE_CALM,
		TYPE_EVIL,
		
		NUM_TYPES,
	};
	
	/****************************************
	****************************************/
	ofFbo fbo_mixed;
    ofFbo fbo[NUM_TYPES];
	ofVideoPlayer mov[NUM_TYPES];
	
	ofShader shader_Add;
	
	bool b_DispGui = false;
	
	OSC_TARGET Osc_XBE;
	
	/****************************************
	****************************************/
	void setup_Gui();
	void clear_fbo(ofFbo& fbo);
	void load_and_set_mov();
	void SetUp_Start_Mov(ofVideoPlayer* video);
	void update_fbo(ofFbo& _fbo, ofVideoPlayer& _mov);
	void ReceiveOsc_from_XBE();
	void draw_mixed();
	
public:
	/****************************************
	****************************************/
	ofApp();
	~ofApp();
	void exit();
	
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
	
};
