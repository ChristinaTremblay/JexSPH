
#pragma region HEADERS

#include <VTK\vtkCylinderSource.h>
#include <VTK\vtkSphereSource.h>
#include <VTK\vtkPolyDataMapper.h>
#include <VTK\vtkActor.h>
#include <VTK\vtkRenderer.h>
#include <VTK\vtkRenderWindow.h>
#include <VTK\vtkRenderWindowInteractor.h>
#include <VTK\vtkProperty.h>
#include <VTK\vtkCamera.h>
#include <VTK\vtkSmartPointer.h>
#include <VTK\vtkAutoInit.h>
#include <VTK\vtkConeSource.h>
#include <VTK\vtkAnimationCue.h>
#include <VTK\vtkAnimationScene.h>
#include <VTK\vtkCommand.h>
#include <VTK\vtkProgrammableFilter.h>
#include <VTK\vtkCallbackCommand.h>
#include <VTK\vtkPolyLine.h>
#include <VTK\vtkCubeSource.h>
#include <VTK\vtkPoints.h>
#include <VTK\vtkCellArray.h>
#include <VTK\vtkPolyData.h>
#include <VTK\vtkNamedColors.h>
#include <VTK\vtkLine.h>

#include <iostream>
#include <cstdio>
#include <omp.h>

#include "JexSphInterface.h"
#include "JexFluidSystem.h"
#include "JexGridContainer.h"
#include "JexNeighorTable.h"
#include "JexParticleDataStruct.h"
#include "JexShaderCompiler.h"

#include <OpenGL\glew.h>
#include <OpenGL\freeglut.h>

#pragma endregion

#pragma region INCLUDE_VTK_LIB

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglutd.lib")
#pragma comment(lib, "vtkIOCore-7.1.lib")
#pragma comment(lib, "vtkFiltersCore-7.1.lib")
#pragma comment(lib, "vtkCommonCore-7.1.lib")
#pragma comment(lib, "vtkViewsCore-7.1.lib")
#pragma comment(lib, "vtkCommonDataModel-7.1.lib")
#pragma comment(lib, "vtkCommonExecutionModel-7.1.lib")
#pragma comment(lib, "vtkRenderingCore-7.1.lib")
#pragma comment(lib, "vtkCommonComputationalGeometry-7.1.lib")
#pragma comment(lib, "vtkexoIIc-7.1.lib")
#pragma comment(lib, "vtksys-7.1.lib")
#pragma comment(lib, "vtkexpat-7.1.lib")
#pragma comment(lib, "vtkChartsCore-7.1.lib")
#pragma comment(lib, "vtkCommonMisc-7.1.lib")
#pragma comment(lib, "vtkCommonSystem-7.1.lib")
#pragma comment(lib, "vtkCommonTransforms-7.1.lib")
#pragma comment(lib, "vtkFiltersExtraction-7.1.lib")
#pragma comment(lib, "vtkFiltersFlowPaths-7.1.lib")
#pragma comment(lib, "vtkFiltersGeneral-7.1.lib")
#pragma comment(lib, "vtkFiltersGeneric-7.1.lib")
#pragma comment(lib, "vtkFiltersHybrid-7.1.lib")
#pragma comment(lib, "vtkFiltersModeling-7.1.lib")
#pragma comment(lib, "vtkfreetype-7.1.lib")
#pragma comment(lib, "vtkGeovisCore-7.1.lib")
#pragma comment(lib, "vtkgl2ps-7.1.lib")
#pragma comment(lib, "vtkFiltersSources-7.1.lib")
#pragma comment(lib, "vtkglew-7.1.lib")
#pragma comment(lib, "vtkRenderingContextOpenGL2-7.1.lib")
#pragma comment(lib, "vtkRenderingOpenGL2-7.1.lib")
#pragma comment(lib, "vtkInteractionImage-7.1.lib")
#pragma comment(lib, "vtkInteractionStyle-7.1.lib")
#pragma comment(lib, "vtkInteractionWidgets-7.1.lib")
#pragma comment(lib, "vtkParallelCore-7.1.lib")
#pragma comment(lib, "vtkFiltersPoints-7.1.lib")
#pragma comment(lib, "vtkFiltersSources-7.1.lib")
#pragma comment(lib, "vtkFiltersGeometry-7.1.lib")
#pragma comment(lib, "vtkFiltersProgrammable-7.1.lib")
#pragma endregion

#define PARTICLE_NUM 4096 * 2

Jex::JexFluidSystem * jex_system;
GLfloat ptc_pos[PARTICLE_NUM][3];

GLuint frame_count = 0;
GLuint vao;
GLuint vbo;
GLuint vertex_position_flag = 0;
GLuint shader_program;

#ifdef VTK_Render

VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2  
VTK_MODULE_INIT(vtkInteractionStyle);

class ParticlePositionModifier : public vtkCommand {

public:

	static ParticlePositionModifier * New() {
		return new ParticlePositionModifier;
	}

	virtual void Execute(vtkObject * caller, unsigned long eventId, void * vtkNotUsed(callData)) {
		jex_system->Tick();
		printf("frame count : %d\n", frame_count++);
		const Jex::Particle * particles = jex_system->GetParticles();
		unsigned int stride = jex_system->GetParticleStride();
		for (int i = 0; i < jex_system->GetParticleNum(); i++) {
			m_actors[i]->SetPosition(particles->position.X(), particles->position.Y(), particles->position.Z());
			particles = (const Jex::Particle *)(((const char*)particles) + stride);
		}
		vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
		iren->GetRenderWindow()->Render();
	}

	void set_actors(std::vector<vtkSmartPointer<vtkActor>> actors) {
		m_actors = actors;
	}

private:

	std::vector<vtkSmartPointer<vtkActor>> m_actors;

};



int main() {

	Jex::Float3 wall_min = { -25.0, 0.0, -25.0 };
	Jex::Float3 wall_max = { 25.0, 50.0, 25.0 };
	Jex::Float3 fluid_min = { -15.0, 5.0, -15.0 };
	Jex::Float3 fluid_max = { 15.0, 35.0, 15.0 };
	Jex::Float3 gravity = { 0.0, -9.8f, 0.0 };

	jex_system = new Jex::JexFluidSystem();
	jex_system->Init(PARTICLE_NUM, &wall_min, &wall_max, &fluid_min, &fluid_max, &gravity);

	float Particle_Radius = jex_system->GetSpacing() * 0.5;
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetThetaResolution(5);
	sphere->SetPhiResolution(6);
	sphere->SetRadius(Particle_Radius);
	sphere->Update();

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.6, 0.6, 0.6);

	const Jex::Particle * particles = jex_system->GetParticles();
	unsigned int stride = jex_system->GetParticleStride();
	printf("particle stride %d\n", stride);
	std::vector<vtkSmartPointer<vtkActor>> actors;
	printf("particle number %d\n", jex_system->GetParticleNum());
	for (int i = 0; i < jex_system->GetParticleNum(); i++) {
		vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
		//sphereActor->GetProperty()->EdgeVisibilityOn();
		sphereActor->SetMapper(sphereMapper);
		sphereActor->GetProperty()->SetColor(0.1, 0.4, 0.8);
		sphereActor->SetPosition(particles->position.X(), particles->position.Y(), particles->position.Z());
		/*printf("Test particles position\n");
		printf("%d : x = %f, y = %f, z = %f\n", i, particles->position.X(), particles->position.Y(), particles->position.Z());*/
		renderer->AddActor(sphereActor);
		actors.push_back(sphereActor);
		particles = (const Jex::Particle *)(((const char*)particles) + stride);
	}

	float scale_factor = 100.0;
	vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->GetActiveCamera()->SetPosition(Particle_Radius * scale_factor, Particle_Radius * scale_factor, Particle_Radius * scale_factor);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	//renderer->ResetCamera();
	//renderer->GetActiveCamera()->ParallelProjectionOn();

	//renderer->GetActiveCamera()->SetParallelScale(1.5);

	vtkSmartPointer<ParticlePositionModifier> ppm = vtkSmartPointer<ParticlePositionModifier>::New();
	ppm->set_actors(actors);

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(800, 500);
	//renderWindow->SetFullScreen(1);
	renderWindow->AddRenderer(renderer);
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, ppm);
	cout << renderWindowInteractor->CreateRepeatingTimer(100) << endl;
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;

}

#endif

#ifdef OpenGL_Render

GLvoid InitRenderSystem();
GLvoid InitDataSystem();
GLvoid DisplayFunc();
GLvoid ReshapeFunc(int w, int h);
GLvoid IdleFunc();
GLvoid MouseFunc(int button, int state, int x, int y);
GLvoid KeyboardFunc(uchr key, int x, int y);

GLvoid InitRenderSystem() {
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitContextVersion(4, 5);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitWindowSize(1024, 768);
	glutCreateWindow("Jex Fluid System");

	glewInit();

	glClearColor(0.6, 0.6, 0.6, 1.0);

}

GLvoid InitDataSystem() {

	Jex::Float3 wall_min = { -25.0, 0.0, -25.0 };
	Jex::Float3 wall_max = { 25.0, 50.0, 25.0 };
	Jex::Float3 fluid_min = { -15.0, 5.0, -15.0 };
	Jex::Float3 fluid_max = { 15.0, 35.0, 15.0 };
	Jex::Float3 gravity = { 0.0, -9.8f, 0.0 };

	jex_system = new Jex::JexFluidSystem();
	jex_system->Init(PARTICLE_NUM, &wall_min, &wall_max, &fluid_min, &fluid_max, &gravity);

	const Jex::Particle * particles = jex_system->GetParticles();
	uint stride = jex_system->GetParticleStride();
#pragma omp parallel for
	for (int i = 0; i < jex_system->GetParticleNum(); i++) {
		ptc_pos[i][0] = particles->position.X();
		ptc_pos[i][1] = particles->position.Y();
		ptc_pos[i][2] = particles->position.Z();
		particles = (const Jex::Particle *)(((const char*)particles) + stride);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glCreateBuffers(1, &vbo);

	glEnableVertexAttribArray(vertex_position_flag);

	Jex::ShaderCompiler shader_compiler;
	shader_program = shader_compiler.Compile();
	glUseProgram(shader_program);

	glPointSize(3);
}

GLvoid DisplayFunc() {

	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ptc_pos), ptc_pos, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(vertex_position_flag, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_POINTS, 0, PARTICLE_NUM);
	glutSwapBuffers();
}

GLvoid ReshapeFunc(int w, int h) {

}

GLvoid IdleFunc() {

	printf("Frame %d : x = %f, y = %f, z = %f\n", frame_count++, ptc_pos[0][0], ptc_pos[0][1], ptc_pos[0][2]);

	jex_system->Tick();

	const Jex::Particle * particles = jex_system->GetParticles();
	uint stride = jex_system->GetParticleStride();
#pragma omp parallel for
	for (int i = 0; i < jex_system->GetParticleNum(); i++) {
		ptc_pos[i][0] = particles->position.X();
		ptc_pos[i][1] = particles->position.Y();
		ptc_pos[i][2] = particles->position.Z();
		particles = (const Jex::Particle *)(((const char*)particles) + stride);
	}

	glutPostRedisplay();
}

GLvoid MouseFunc(int button, int state, int x, int y) {

}

GLvoid KeyboardFunc(uchr key, int x, int y) {

}

int main(int argc, char ** argv) {

	glutInit(&argc, argv);
	InitRenderSystem();
	InitDataSystem();

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutIdleFunc(IdleFunc);
	glutMouseFunc(MouseFunc);
	glutKeyboardFunc(KeyboardFunc);

	glutMainLoop();
}

#endif