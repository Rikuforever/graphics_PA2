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
	// ȭ�� ���� Ŭ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ȭ���� ����� �ٶ󺸱� ���� ī�޶� ȸ�� �� �̵�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 20.0f, 30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//To Do
	RenderMesh(M_teapot);
	RenderMesh(M_torus);




	RenderPlane();

	// back ���ۿ� �������� �� swap
	glutSwapBuffers();

}

void Reshape(int w, int h)
{
	// ����Ʈ ����
	glViewport(0, 0, w, h);

	// ���� ���� ���
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.1, 500);

	// �𵨺� ��Ʈ���� �ʱ�ȭ
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
	glutKeyboardFunc(Keyboard);  // Ű���� �Է� ó�� Callback �Լ� ���
	glutDisplayFunc(Rendering);  // ��ȯ�� ���� ���� Rendering Callback �Լ� ���
	glutReshapeFunc(Reshape);    // ������ â ũ�Ⱑ �ٲ������ ȣ��Ǵ� Callback �Լ� ���
	glutMouseFunc(Mouse);        // ���콺 Ŭ�� ó�� Callback �Լ� ���
	glutMotionFunc(Motion);      // ���콺 ������ ó�� Callback �Լ� ���

	glutMainLoop(); // ��ϵ� callback �Լ��� �ݺ��Ͽ� ȣ��
}

int main(int argc, char** argv)
{
	Initialize(argc, argv);			  // ������ ����, ���� ����

	MeshLoad();       //To Do
	ComputeNormal();  //To Do

	EventHandlingAndLoop();      // Event Handling �� Loop

	// ���� ���� ������ ��� 0�� ������
	return 0;
}