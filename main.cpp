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



// ユニフォーム
GLuint uniTimer, uniresolution;
float	gTimer=1.0f;



// タイマ
#define REFRESH_DELAY     10 //ms
void timerEvent(int value);
float	prevTime;

// ウインドウサイズ
unsigned int windowW = 512, windowH = 512;

/*
** シェーダ
*/
#include "glsl.h"
static GLuint vertShader;
static GLuint fragShader;
static GLuint gl2Program;


// ** 初期化
static void init(void)
{
  /* シェーダプログラムのコンパイル／リンク結果を得る変数 */
  GLint compiled, linked;

  /* 初期設定 */
  glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // 光源の初期設定
//  glEnable(GL_LIGHTING);
//  glEnable(GL_LIGHT0);
//  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
//  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
//  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
//  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);



  /* GLSL の初期化 */
  if (glslInit()) exit(1);

  /* シェーダオブジェクトの作成 */
  vertShader = glCreateShader(GL_VERTEX_SHADER);
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  /* シェーダのソースプログラムの読み込み */
  if (readShaderSource(vertShader, "phong.vert")) exit(1);
//  if (readShaderSource(fragShader, "tlcBWH.frag")) exit(1);
//  if (readShaderSource(fragShader, "ttdfz2.frag")) exit(1);
//  if (readShaderSource(fragShader, "llK3Dy.frag")) exit(1);
  if (readShaderSource(fragShader, "wl2SRK.frag")) exit(1);
 // if (readShaderSource(fragShader, "dot001.frag")) exit(1);


  /* バーテックスシェーダのソースプログラムのコンパイル */
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(vertShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in vertex shader.\n");
    exit(1);
  }

  /* フラグメントシェーダのソースプログラムのコンパイル */
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
  printShaderInfoLog(fragShader);
  if (compiled == GL_FALSE) {
    fprintf(stderr, "Compile error in fragment shader.\n");
    exit(1);
  }

  /* プログラムオブジェクトの作成 */
  gl2Program = glCreateProgram();

  /* シェーダオブジェクトのシェーダプログラムへの登録 */
  glAttachShader(gl2Program, vertShader);
  glAttachShader(gl2Program, fragShader);

  /* シェーダオブジェクトの削除 */
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

glEnable(GL_LINE_SMOOTH);

  /* シェーダプログラムのリンク */
  glLinkProgram(gl2Program);
  glGetProgramiv(gl2Program, GL_LINK_STATUS, &linked);
  printProgramInfoLog(gl2Program);
  if (linked == GL_FALSE) {
    fprintf(stderr, "Link error.\n");
    exit(1);
  }


	// ユニフォーム
  uniTimer = glGetUniformLocation(gl2Program, "time");
  uniresolution = glGetUniformLocation(gl2Program, "resolution");

  // タイマ
  prevTime = 0.0f;
  glutTimerFunc(REFRESH_DELAY, timerEvent, 0);

  /* シェーダプログラムの適用 */
  glUseProgram(gl2Program);
}

/*
** シーンの描画
*/
static void scene(void)
{
  
  glUniform2f(uniresolution, (float)windowW, (float)windowH);
  glUniform1f(uniTimer, prevTime);



#if 1
  /* １枚の４角形を描く */
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
		glutPostRedisplay();	// 描画依頼
	}

//	prevTime = time;
	prevTime += 0.01f;
	if (prevTime > 3600.0f)	prevTime = 0.0f;

	glutTimerFunc(REFRESH_DELAY, timerEvent, 0);
}

/****************************
** GLUT のコールバック関数 **
****************************/

/* トラックボール処理用関数の宣言 */
#include "trackball.h"

static void display(void)
{
  /* モデルビュー変換行列の初期化 */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* 画面クリア */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* 光源の位置を設定 */
 // glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  /* 視点の移動（物体の方を奥に移動）*/
  glTranslated(0.0, 0.0, -1.0);

  /* トラックボール処理による回転 */
 // glMultMatrixd(trackballRotation());

  /* シーンの描画 */
  scene();

  /* ダブルバッファリング */
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  /* トラックボールする範囲 */
//  trackballRegion(w, h);

  // ウィンドウ全体をビューポートにする
  glViewport(0, 0, w, h);

  /* 透視変換行列の指定 */
//  glMatrixMode(GL_PROJECTION);

  /* 透視変換行列の初期化 */
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 1.0, 100.0);

  windowW = w;
  windowH = h;
}

static void idle(void)
{
  /* 画面の描き替え */
  glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      // トラックボール開始
//      trackballStart(x, y);
//      glutIdleFunc(idle);
      break;
    case GLUT_UP:
      // トラックボール停止
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
  /* トラックボール移動 */
  trackballMotion(x, y);
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    /* ESC か q か Q をタイプしたら終了 */
    exit(0);
  default:
    break;
  }
}

/*
** メインプログラム
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
