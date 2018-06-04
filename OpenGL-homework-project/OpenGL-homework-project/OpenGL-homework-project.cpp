#include <GL/glut.h>
#include <iostream>
#include <time.h>
using namespace std;

#define max_bullet 5

struct Bullet {
	GLdouble x, y, z, x_velocity, y_velocity, z_velocity;
};
Bullet player_bullet[max_bullet];
Bullet teapothead_bullet[max_bullet];

struct Teapothead {
	double x, y, z, red, green, blue;
};
Teapothead teapothead[100];

int number_of_teapotheads = 0, number_of_player_bullets = 0, number_of_teapothead_bullets = 0;
int window_width = 1280, window_height = 720, room_length = 1000;
int center_x = window_width / 2, center_y = window_height / 2;
double player_x = room_length / 3, player_y = 0, player_z = room_length / 3, player_y_velocity = 0;
double rotation = 4, look_up_down = 0;
double mouse_last_x = window_width / 2, mouse_last_y = window_height / 2;
double t = 0.01, reloading = 0;
bool reset_Mouse = false;

void createBullet(Bullet & bullet) {
	bullet.x = player_x + cos(rotation) * 10;
	bullet.y = player_y + look_up_down * 10;
	bullet.z = player_z + sin(rotation) * 10;
	bullet.x_velocity = cos(rotation) * 100;
	bullet.y_velocity = look_up_down * 100;
	bullet.z_velocity = sin(rotation) * 100;
}

void createTeapothead(void) {
	for (int i = 0; i < number_of_teapotheads; i++) {
		teapothead[i].y = 0;
		if (i % 2) {
			teapothead[i].x = rand() % (room_length - 30) - (room_length - 30) / 2;
			teapothead[i].z = 30 - room_length / 2;
		}
		else {
			teapothead[i].x = 30 - room_length / 2;
			teapothead[i].z = rand() % (room_length - 30) - (room_length - 30) / 2;
		}
		teapothead[i].red = (double)rand() / RAND_MAX;
		teapothead[i].green = (double)rand() / RAND_MAX;
		teapothead[i].blue = (double)rand() / RAND_MAX;
	}
}

void playerShoot(Bullet & bullet) {
	bullet.x = player_x + cos(rotation) * 10;
	bullet.y = player_y + look_up_down * 10;
	bullet.z = player_z + sin(rotation) * 10;
	bullet.x_velocity = cos(rotation) * 1500;
	bullet.y_velocity = look_up_down * 1500;
	bullet.z_velocity = sin(rotation) * 1500;
}

void teapotheadShoot(Teapothead & teapothead, Bullet & bullet) {
	bullet.x = teapothead.x;
	bullet.y = teapothead.y;
	bullet.z = teapothead.z;
	bullet.x_velocity = player_x - teapothead.x;
	bullet.z_velocity = player_z -teapothead.z;
}

//void DrawBullet(Bullet bullet) {
//	glPushMatrix();
//	glTranslated(bullet.x, bullet.y, bullet.z);
//	glColor3f(0, 0, 1);
//	glutSolidSphere(ball.r, 10, 10);
//	glPopMatrix();
//}

bool edgeCollision(Bullet & bullet) {
	int boundary = room_length / 2;
	if (bullet.x < -boundary || bullet.x > boundary
		|| bullet.y < -boundary || bullet.y > boundary
		|| bullet.z < -boundary || bullet.z > boundary) {
		return true;
	}
	else {
		return false;
	}
}

//void Collision(int i) {
//	for (int j = 0; j < n; j++) {
//		if (i != j) {
//			double dx = ball[i].x - ball[j].x;
//			double dy = ball[i].y - ball[j].y;
//			double dz = ball[i].z - ball[j].z;
//			double d = sqrt(dx * dx + dy * dy + dz * dz);
//
//			if (d <= ball[i].r + ball[j].r) {
//				double v1n = (ball[i].vx * dx + ball[i].vy * dy + ball[i].vz * dz) / d;//法線方向的速度
//				double v1ix = v1n * dx / d;//法線方向的速度的x分量
//				double v1iy = v1n * dy / d;//法線方向的速度的y分量
//				double v1iz = v1n * dz / d;//法線方向的速度的z分量
//
//				double v2n = (ball[j].vx * dx + ball[j].vy * dy + ball[j].vz * dz) / d;
//				double v2ix = v2n * dx / d;
//				double v2iy = v2n * dy / d;
//				double v2iz = v2n * dz / d;
//
//				double m1 = pow(ball[i].r, 3);
//				double m2 = pow(ball[j].r, 3);
//
//				double v1fx = (m1 * v1ix + m2 * v2ix + CrBB * m2 * (v2ix - v1ix)) / (m1 + m2);
//				double v1fy = (m1 * v1iy + m2 * v2iy + CrBB * m2 * (v2iy - v1iy)) / (m1 + m2);
//				double v1fz = (m1 * v1iz + m2 * v2iz + CrBB * m2 * (v2iz - v1iz)) / (m1 + m2);
//
//				double v2fx = (m1 * v1ix + m2 * v2ix + CrBB * m1 * (v1ix - v2ix)) / (m1 + m2);
//				double v2fy = (m1 * v1iy + m2 * v2iy + CrBB * m1 * (v1iy - v2iy)) / (m1 + m2);
//				double v2fz = (m1 * v1iz + m2 * v2iz + CrBB * m1 * (v1iz - v2iz)) / (m1 + m2);
//
//				ball[i].vx = v1fx + ball[i].vx - v1ix;
//				ball[i].vy = v1fy + ball[i].vy - v1iy;
//				ball[i].vz = v1fz + ball[i].vz - v1iz;
//
//				ball[j].vx = v2fx + ball[j].vx - v2ix;
//				ball[j].vy = v2fy + ball[j].vy - v2iy;
//				ball[j].vz = v2fz + ball[j].vz - v2iz;
//			}
//		}
//	}
//}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(player_x, player_y, player_z, player_x + cos(rotation), player_y + look_up_down, player_z + sin(rotation), 0, 1, 0);
	//gluLookAt(0, 6000, 0, 0, 0, 0, 1, 0, 0);

	/*glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslated(player_x, player_y, player_z);
	glutWireSphere(100, 30, 30);
	glPopMatrix();*/

	glPushMatrix();
	glColor3f(0.5, 0.5, 0);
	glTranslated(room_length, 0, 0);
	glutSolidCube(room_length);
	glTranslated(-room_length * 2, 0, 0);
	glutSolidCube(room_length);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -(room_length / 2 + 50), 0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidCube(room_length);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0.5);
	glTranslated(0, 0, room_length);
	glutSolidCube(room_length);
	glTranslated(-0 * 2, 0, -room_length * 2);
	glutSolidCube(1000);
	glPopMatrix();

	for (int i = 0; i < number_of_teapotheads; i++) {
		//double line_cos = cos(teapothead[i].degree / 57.29577951);
		//double line_sin = sin(teapothead[i].degree / 57.29577951);
		glPushMatrix();
		glTranslated(teapothead[i].x, teapothead[i].y, teapothead[i].z);
		glColor3f(teapothead[i].red, teapothead[i].green, teapothead[i].blue);
		if (i % 2) {
			glRotatef(270, 0, 1, 0);
		}
		glutSolidTeapot(10);
		glutWireCube(20);
		glTranslated(0, -50, 0);
		glRotatef(270, 1, 0, 0);
		glutSolidCone(10, 50, 3, 1);
		glPopMatrix();

		printf("i %d ", i);
		if (number_of_teapothead_bullets < max_bullet){
			printf("FFF %d\n", number_of_teapothead_bullets);
			teapotheadShoot(teapothead[i], teapothead_bullet[number_of_teapothead_bullets]);
			number_of_teapothead_bullets++;
		}

		for (int j = 0; j < number_of_player_bullets; j++) {
			if (player_bullet[j].x < teapothead[i].x + 10 && player_bullet[j].x > teapothead[i].x - 10
				&& player_bullet[j].y < teapothead[i].y + 10 && player_bullet[j].y > teapothead[i].y - 10
				&& player_bullet[j].z < teapothead[i].z + 10 && player_bullet[j].z > teapothead[i].z - 10) {
				number_of_player_bullets--;
				player_bullet[j] = player_bullet[number_of_player_bullets];
				number_of_teapotheads--;
				teapothead[i] = teapothead[number_of_teapotheads];
			}
		}
	}

	for (int i = 0; i < number_of_player_bullets; i++) {
		glPushMatrix();
		glTranslated(player_bullet[i].x, player_bullet[i].y, player_bullet[i].z);
		glColor3f(1, 1, 1);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();

		player_bullet[i].x += player_bullet[i].x_velocity * t;
		player_bullet[i].y += player_bullet[i].y_velocity * t;
		player_bullet[i].z += player_bullet[i].z_velocity * t;

		if (edgeCollision(player_bullet[i])) {
			number_of_player_bullets--;
			player_bullet[i] = player_bullet[number_of_player_bullets];
		}
	}

	for (int i = 0; i < number_of_teapothead_bullets; i++) {
		glPushMatrix();
		glTranslated(teapothead_bullet[i].x, teapothead_bullet[i].y, teapothead_bullet[i].z);
		glColor3f(0, 0, 0);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();

		teapothead_bullet[i].x += teapothead_bullet[i].x_velocity * t;
		teapothead_bullet[i].y += teapothead_bullet[i].y_velocity * t;
		teapothead_bullet[i].z += teapothead_bullet[i].z_velocity * t;

		if (edgeCollision(teapothead_bullet[i])) {
			printf("RRR %d\n", i);
			printf("number_of_player_bullets %d\n", number_of_teapothead_bullets);
			number_of_teapothead_bullets--;
			teapothead_bullet[i] = teapothead_bullet[number_of_teapothead_bullets];
			printf("number_of_player_bullets %d\n", number_of_teapothead_bullets);
			printf("%lf %lf\n", teapothead_bullet[0].x, teapothead_bullet[0].z);
			printf("%lf %lf\n", teapothead_bullet[1].x, teapothead_bullet[1].z);
			printf("%lf %lf\n", teapothead_bullet[2].x, teapothead_bullet[2].z);
			printf("%lf %lf\n", teapothead_bullet[3].x, teapothead_bullet[3].z);
			printf("%lf %lf\n", teapothead_bullet[4].x, teapothead_bullet[4].z);
		}
	}

	player_y += player_y_velocity * t;
	if (player_y > 0) {
		player_y_velocity -= 98 * t;
	}
	else {
		player_y = 0;
	}

	reloading -= t;
	if (reloading > 0) {
		glutSetCursor(GLUT_CURSOR_WAIT);
	}
	else {
		reloading = 0;
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	}
	printf("%d", number_of_teapothead_bullets);
	glFlush();
}

void reshape(int width, int height) {
	window_width = width;
	window_height = height;
	mouse_last_x = center_x = window_width / 2;
	mouse_last_y = center_y = window_height / 2;
	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (GLfloat)window_width / window_height, 1, 10000);
}

void specialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		glRotatef(5.0, 1, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		glRotatef(5.0, 0, 1, 0);
		break;
	case GLUT_KEY_DOWN:
		glRotatef(-5.0, 1, 0, 0);
		break;
	case GLUT_KEY_RIGHT:
		glRotatef(-5.0, 0, 1, 0);
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	double cos_rotation = cos(rotation) * 5;
	double sin_rotation = sin(rotation) * 5;
	switch (key) {
	case 'w':
		player_x += cos_rotation;
		player_z += sin_rotation;
		break;
	case 'a':
		player_x += sin_rotation;
		player_z -= cos_rotation;
		break;
	case 's':
		player_x -= cos_rotation;
		player_z -= sin_rotation;
		break;
	case 'd':
		player_x -= sin_rotation;
		player_z += cos_rotation;
		break;
	case ' ':
		if (player_y == 0) {
			player_y_velocity = 50;
		}
		break;
	}
	glutPostRedisplay();
}

void Timer(int c) {
	glutPostRedisplay();
	glutTimerFunc(100, Timer, 0);
}

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && reloading == 0) {
		playerShoot(player_bullet[number_of_player_bullets]);
		number_of_player_bullets++;
		if (number_of_player_bullets >= max_bullet) {
			reloading = 2;
		}
	}
}

void mouseMove(int x, int y) {
	if (reset_Mouse == false) {
		double distance_x = x - mouse_last_x;
		double distance_y = y - mouse_last_y;
		mouse_last_x = x;
		mouse_last_y = y;
		rotation += distance_x * 0.005;
		look_up_down -= distance_y * 0.005;

		if (look_up_down > 1) {
			look_up_down = 1;
		}
		else if (look_up_down < -1) {
			look_up_down = -1;
		}
		if (mouse_last_x != center_x || mouse_last_y != center_y) {
			reset_Mouse = true;
		}
	}
	else if (reset_Mouse == true) {
		glutWarpPointer(center_x, center_y);
		mouse_last_x = center_x;
		mouse_last_y = center_y;
		reset_Mouse = false;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	cout << "number of teapotheads: ";
	cin >> number_of_teapotheads;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Project #4");
	glutWarpPointer(center_x, center_y);
	srand((unsigned)time(NULL));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	createTeapothead();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseButton);
	glutPassiveMotionFunc(mouseMove);
	//glutTimerFunc(10, Timer, 0);

	glutMainLoop();
	return 0;
}