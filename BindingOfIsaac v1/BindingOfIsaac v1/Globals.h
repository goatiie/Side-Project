extern void winReshapeFunc(GLint w, GLint h);
extern void renderScene();
extern void update();
extern void processMouse(int button, int state, int x, int y);
extern void processKeyboard(unsigned char key, int x, int y);
extern void processKeyUp(unsigned char key, int x, int y);

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;



struct RGB
{
	float r, g, b;
};


const RGB TRANSCOLOUR = {1.0, 0.0, 1.0};
