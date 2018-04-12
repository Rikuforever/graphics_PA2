#include "VECTOR.h"
#include "Face.h"
#include "Mesh.h"
#include "Init.h"


using namespace std;

bool DepthTest = true;

enum Mode { None, Translation, Rotation };
Mode currentMode;

#pragma region Camera

float camera_distance = 30;
int camera_degree = 0;

#pragma endregion

#pragma region Translation & Rotation

Mesh teapot_mesh;
Mesh torus_mesh;
VECTOR3D teapot_position(-3.0, 0.0, 0.0);
VECTOR3D torus_position(3.0, 0.0, 0.0);
VECTOR3D teapot_rotation(0.0, 0.0, 0.0);
VECTOR3D torus_rotation(0.0, 0.0, 0.0);

enum Focus { Teapot, Torus };
Focus currentFocus;
enum Axis { X, Y, Z };
Axis currentAxis;

int mouse_state; int mouse_event;
int anchorX; int anchorY;

#pragma endregion

void MeshLoad()
{
	//To do
	// Load Meshes
	teapot_mesh.LoadMesh("teapot.obj");
	torus_mesh.LoadMesh("torus.obj");

}

void ComputeNormal()
{
	//To do
	// Compute Normals
	teapot_mesh.FindNeighborFaceArray();
	teapot_mesh.ComputeFaceNormal();
	teapot_mesh.ComputeVertexNormal();
	torus_mesh.FindNeighborFaceArray();
	torus_mesh.ComputeVertexNormal();
	torus_mesh.ComputeFaceNormal();

}

void Mouse(int event, int state, int x, int y)
{
	// Pass variables to "Motion"
	mouse_event = event;
	mouse_state = state;

	// When dragging starts, put anchor
	if (event == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		anchorX = x;
		anchorY = y;
	}

	glutPostRedisplay();
}

void Motion(int x, int y)
{
	if (mouse_event == GLUT_LEFT_BUTTON && mouse_state == GLUT_DOWN) {
		// Compare with previous frame position to track movement
		int motionX = x - anchorX;
		int motionY = y - anchorY;

		// Change position or rotation values
		switch (currentMode) {
			case Translation:
				VECTOR3D * p;
				if (currentFocus == Teapot) p = &teapot_position;
				else p = &torus_position;

				switch (currentAxis) {
					case X:
						*p += VECTOR3D(motionX * 0.01, 0.0, 0.0);
						break;
					case Y:
						*p += VECTOR3D(0.0, -motionY * 0.01, 0.0);
						break;
					case Z:
						*p += VECTOR3D(0.0, 0.0, motionY * 0.01);
						break;
					default:
						break;
				}
				break;
			case Rotation:
				VECTOR3D * r;
				if (currentFocus == Teapot) r = &teapot_rotation;
				else r = &torus_rotation;

				switch (currentAxis) {
				case X:
					*r += VECTOR3D(motionY * 0.5, 0.0, 0.0);
					break;
				case Y:
					*r += VECTOR3D(0.0, -motionX * 0.5, 0.0);
					break;
				case Z:
					*r += VECTOR3D(0.0, 0.0, -motionX * 0.5);
					break;
				default:
					break;
				}
				r->x = fmodf(r->x, 360.0f);
				r->y = fmodf(r->y, 360.0f);
				r->z = fmodf(r->z, 360.0f);
				break;
			default:
				break;
		}

		// Save position for next frame
		anchorX = x;
		anchorY = y;
	}

	glutPostRedisplay();
}

// Custom LoadIdentity
void LoadIdentity() {
	glLoadIdentity();
	gluLookAt(
		camera_distance * sin(camera_degree * 3.14159f / 180.0f),
		camera_distance * (2.0f/3.0f), 
		camera_distance * cos(camera_degree * 3.14159f / 180.0f),
		0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f
	);
}

void RenderPlane()
{
	LoadIdentity();
	
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.8, 0.8);
	glNormal3f(0, 1, 0);
	glVertex3f(-10.0f, -1.0f, 10.0f);
	glVertex3f(-10.0f, -1.0f, -10.0f);
	glVertex3f(10.0f, -1.0f, -10.0f);
	glVertex3f(10.0f, -1.0f, 10.0f);
	glEnd();	
}

void RenderMesh(Mesh m, VECTOR3D p, VECTOR3D r, bool isHighlight = false) {
	// Reset & Translate & Rotate
	LoadIdentity();
	glTranslatef(p.x, p.y, p.z);
	glRotatef(r.x, 1.0, 0.0, 0.0);
	glRotatef(r.y, 0.0, 1.0, 0.0);
	glRotatef(r.z, 0.0, 0.0, 1.0);

	// Color red when highlighted
	if (isHighlight)
		glColor3f(1.0, 0.0, 0.0);
	else
		glColor3f(1.0, 1.0, 1.0);

	// Then draw
	for (int i = 0; i < m.faceArray.size(); i++) {
		Face f = m.faceArray[i];
		VECTOR3D n = f.normal;
		VECTOR3D v0 = m.vertexArray[f.vertex0].position;
		VECTOR3D v1 = m.vertexArray[f.vertex1].position;
		VECTOR3D v2 = m.vertexArray[f.vertex2].position;

		glBegin(GL_TRIANGLES);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glEnd();
	}
}

void Rendering(void)
{
	// 화면 버퍼 클리어
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 화면을 제대로 바라보기 위해 카메라를 회전 후 이동
	glMatrixMode(GL_MODELVIEW);
	LoadIdentity();
	
	// Render Meshes
	RenderMesh(teapot_mesh, teapot_position, teapot_rotation, currentFocus == Teapot);
	RenderMesh(torus_mesh, torus_position, torus_rotation, currentFocus == Torus);
	RenderPlane();

	// back 버퍼에 랜더링한 후 swap
	glutSwapBuffers();

}

void Reshape(int w, int h)
{
	// 뷰포트 설정
	glViewport(0, 0, w, h);

	// 원근 투영 사용
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.1, 500);

	// 모델뷰 매트릭스 초기화
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch(key){
		// Object
		case '1':
			currentFocus = Teapot;
			break;
		case '2':
			currentFocus = Torus;
			break;
		// Depth Test
		case '3':
			if(DepthTest)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
			DepthTest = !DepthTest;
			break;
		// Mode
		case 't':
		case 'T':
			currentMode = Translation;
			break;
		case 'r':
		case 'R':
			currentMode = Rotation;
			break;
		// Axis
		case 'x':
		case 'X':
			currentAxis = X;
			break;
		case 'y':
		case 'Y':
			currentAxis = Y;
			break;
		case 'z':
		case 'Z':
			currentAxis = Z;
			break;
		// Camera
		case 'w':
		case 'W':
			camera_distance -= 0.8;
			if (camera_distance < 3) camera_distance = 3;
			break;
		case 's':
		case 'S':
			camera_distance += 0.8;
			break;
		case 'a':
		case 'A':
			camera_degree -= 3;
			camera_degree %= 360;
			break;
		case 'd':
		case 'D':
			camera_degree += 3;
			camera_degree %= 360;
			break;
	}
	
	glutPostRedisplay();
}

void EventHandlingAndLoop()
{
	glutKeyboardFunc(Keyboard);  // 키보드 입력 처리 Callback 함수 등록
	glutDisplayFunc(Rendering);  // 변환된 값에 따른 Rendering Callback 함수 등록
	glutReshapeFunc(Reshape);    // 윈도우 창 크기가 바뀌었을때 호출되는 Callback 함수 등록
	glutMouseFunc(Mouse);        // 마우스 클릭 처리 Callback 함수 등록
	glutMotionFunc(Motion);      // 마우스 움직임 처리 Callback 함수 등록

	glutMainLoop(); // 등록된 callback 함수를 반복하여 호출
}

int main(int argc, char** argv)
{
	Initialize(argc, argv);			  // 윈도우 생성, 배경색 설정

	MeshLoad();       //To Do
	ComputeNormal();  //To Do

	EventHandlingAndLoop();      // Event Handling 및 Loop

	// 에러 없이 끝났을 경우 0을 리턴함
	return 0;
}