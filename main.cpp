#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#  include "glext.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif



// ���j�t�H�[��
GLuint uniTimer, uniresolution;
float	gTimer=1.0f;



// �^�C�}
#define REFRESH_DELAY     10 //ms
void timerEvent(int value);
float	prevTime;

// �E�C���h�E�T�C�Y
unsigned int windowW = 512, windowH = 512;

/*
** �V�F�[�_
*/
#include "glsl.h"
static GLuint vertShader;
static GLuint fragShader;
static GLuint gl2Program;


// ** ������
static void init(void)
{
  /* �V�F�[�_�v���O�����̃R���p�C���^�����N���ʂ𓾂�ϐ� */
  GLint compiled, linked;

  /* �����ݒ� */
  glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // �����̏����ݒ�
//  glEnable(GL_LIGHTING);
//  glEnable(GL_LIGHT0);
//  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
//  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
//  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
//  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);



  /* GLSL �̏����� */
  if (glslInit()) exit(1);

  /* �V�F�[�_�I�u�W�F�N�g�̍쐬 */
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  /* �V�F�[�_�̃\�[�X�v���O�����̓ǂݍ��� */
  if (readShaderSource(vertShader, "phong.vert")) exit(1);
//  if (readShaderSource(fragShader, "tlcBWH.frag")) exit(1);
//  if (readShaderSource(fragShader, "ttdfz2.frag")) exit(1);
//  if (readShaderSource(fragShader, "llK3Dy.frag")) exit(1);
  if (readShaderSource(fragShader, "wl2SRK.frag")) exit(1);
 // if (readShaderSource(fragShader, "dot001.frag")) exit(1);


  /* �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̃R���p�C�� */
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(vertShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in vertex shader.\n");
    exit(1);
  }

  /* �t���O�����g�V�F�[�_�̃\�[�X�v���O�����̃R���p�C�� */
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(fragShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in fragment shader.\n");
    exit(1);
  }

  /* �v���O�����I�u�W�F�N�g�̍쐬 */
  gl2Program = glCreateProgram();

  /* �V�F�[�_�I�u�W�F�N�g�̃V�F�[�_�v���O�����ւ̓o�^ */
  glAttachShader(gl2Program, vertShader);
  glAttachShader(gl2Program, fragShader);

  /* �V�F�[�_�I�u�W�F�N�g�̍폜 */
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

glEnable(GL_LINE_SMOOTH);

  /* �V�F�[�_�v���O�����̃����N */
  glLinkProgram(gl2Program);
  glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
  printProgramInfoLog(gl2Program);
  if (linked == GL_FALSE) {
    fprintf(stderr, "Link error.\n");
    exit(1);
  }


	// ���j�t�H�[��
  uniTimer = glGetUniformLocation(gl2Program, "time");
  uniresolution = glGetUniformLocation(gl2Program, "resolution");

  // �^�C�}
  prevTime = 0.0f;
  glutTimerFunc(REFRESH_DELAY, timerEvent, 0);

  /* �V�F�[�_�v���O�����̓K�p */
  glUseProgram(gl2Program);
}

/*
** �V�[���̕`��
*/
static void scene(void)
{
  
  glUniform2f(uniresolution, (float)windowW, (float)windowH);
  glUniform1f(uniTimer, prevTime);



#if 1
  /* �P���̂S�p�`��`�� */
  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
	  glVertex2f(-2.0, -2.0);
	  glVertex2f( 2.0, -2.0);
	  glVertex2f( 2.0,  2.0);
	  glVertex2f(-2.0,  2.0);
  glEnd();


#else
  glutSolidTeapot(1.0);
#endif
}

void timerEvent(int value)
{

//	if (gGLThread.m_PramTrig == 1)
	{
		glutPostRedisplay();	// �`��˗�
	}

//	prevTime = time;
	prevTime += 0.01f;
	if (prevTime > 3600.0f)	prevTime = 0.0f;

	glutTimerFunc(REFRESH_DELAY, timerEvent, 0);
}

/****************************
** GLUT �̃R�[���o�b�N�֐� **
****************************/

/* �g���b�N�{�[�������p�֐��̐錾 */
#include "trackball.h"

static void display(void)
{
  /* ���f���r���[�ϊ��s��̏����� */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* ��ʃN���A */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* �����̈ʒu��ݒ� */
 // glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  /* ���_�̈ړ��i���̂̕������Ɉړ��j*/
  glTranslated(0.0, 0.0, -1.0);

  /* �g���b�N�{�[�������ɂ���] */
 // glMultMatrixd(trackballRotation());

  /* �V�[���̕`�� */
  scene();

  /* �_�u���o�b�t�@�����O */
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  /* �g���b�N�{�[������͈� */
//  trackballRegion(w, h);

  // �E�B���h�E�S�̂��r���[�|�[�g�ɂ���
  glViewport(0, 0, w, h);

  /* �����ϊ��s��̎w�� */
//  glMatrixMode(GL_PROJECTION);

  /* �����ϊ��s��̏����� */
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 1.0, 100.0);

  windowW = w;
  windowH = h;
}

static void idle(void)
{
  /* ��ʂ̕`���ւ� */
  glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      // �g���b�N�{�[���J�n
//      trackballStart(x, y);
//      glutIdleFunc(idle);
      break;
    case GLUT_UP:
      // �g���b�N�{�[����~
//      trackballStop(x, y);
//      glutIdleFunc(0);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

static void motion(int x, int y)
{
  /* �g���b�N�{�[���ړ� */
  trackballMotion(x, y);
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    /* ESC �� q �� Q ���^�C�v������I�� */
    exit(0);
  default:
    break;
  }
}

/*
** ���C���v���O����
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(windowW, windowH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();

  getchar();

  return 0;
}
