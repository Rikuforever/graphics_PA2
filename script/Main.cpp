#include "VECTOR.h"
#include "Face.h"
#include "Mesh.h"
#include "Init.h"


using namespace std;

bool DepthTest = true;

Mesh M_teapot;
Mesh M_torus;
void MeshLoad()
{
	//To do
	// Load Meshes
	M_teapot.LoadMesh("teapot2.obj");
	M_torus.LoadMesh("torus.obj");

}

void ComputeNormal()
{
	//To do
	// Compute Normals
	M_teapot.FindNeighborFaceArray();
	M_teapot.ComputeVertexNormal();
	M_teapot.ComputeFaceNormal();
	M_torus.FindNeighborFaceArray();
	M_torus.ComputeFaceNormal();
	M_torus.ComputeVertexNormal();

}

void Mouse(int mouse_event, int state, int x, int y)
{
	//To Do




	glutPostRedisplay();
}

void Motion(int x, int y)
{
	//To Do




	glutPostRedisplay();
}

void RenderPlane()
{
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.8, 0.8);
	glNormal3f(0, 1, 0);
	glVertex3f(-10.0f, -1.0f, 10.0f);
	glVertex3f(-10.0f, -1.0f, -10.0f);
	glVertex3f(10.0f, -1.0f, -10.0f);
	glVertex3f(10.0f, -1.0f, 10.0f);
	glEnd();	
}

void RenderMesh(Mesh m) {
	for (int i = 0; i < m.faceArray.size(); i++) {
		Face f = m.faceArray[i];
		VECTOR3D n = f.normal;
		VECTOR3D v0 = m.vertexArray[f.vertex0].position;
		VECTOR3D v1 = m.vertexArray[f.vertex1].position;
		VECTOR3D v2 = m.vertexArray[f.vertex2].position;

		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
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
	glLoadIdentity();
	gluLookAt(0.0f, 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//To Do
	RenderMesh(M_teapot);
	RenderMesh(M_torus);




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
	//To Do

	switch(key){
		case '3':
			if(DepthTest)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
			DepthTest = !DepthTest;
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