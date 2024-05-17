#include <glut.h> // Thư viện GLUT cho đồ họa OpenGL
#include <cmath> // Thư viện toán học cho các hàm toán học
#include <fstream> // Thư viện đọc và ghi file
#include "object.h" // Thư viện chứa định nghĩa lớp object

// Tham số của camera
float angle = 0.0f, lx = 0.0f, lz = -1.0f, x = 0.0f, z = 5.0f, y = 1.0f, ly = 0.0f;
// angle: góc xoay của camera
// lx, lz: hướng nhìn theo trục x và z
// x, z, y: vị trí của camera
// ly: hướng nhìn theo trục y

float deltaAngle = 0.0f, deltaMove = 0.0f; // deltaAngle: thay đổi góc, deltaMove: thay đổi di chuyển
int xOrigin = -1, yOrigin = -1; // Tọa độ gốc ban đầu của chuột
const float moveSpeed = 0.1f; // Tốc độ di chuyển
const float rotateSpeed = 0.01f; // Tốc độ xoay
const float verticalMoveSpeed = 0.5f; // Tốc độ di chuyển lên xuống

// Biến lưu trữ model
object objs; // Đối tượng objects để quản lý các mô hình

// Hàm thay đổi kích thước cửa sổ
void changeSize(int w, int h) {
	if (h == 0) h = 1; // Tránh chia cho 0
	float ratio = 1.0f * w / h; // Tính tỷ lệ khung hình
	glMatrixMode(GL_PROJECTION); // Chuyển sang ma trận chiếu
	glLoadIdentity(); // Đặt lại ma trận đơn vị
	glViewport(0, 0, w, h); // Đặt lại viewport
	gluPerspective(45, ratio, 0.1, 100); // Đặt lại góc nhìn
	glMatrixMode(GL_MODELVIEW); // Trở lại ma trận mô hình
}

// Hàm vẽ lưới
void drawGrid() {
	glColor3f(0.0f, 1.0f, 0.0f); // Màu xanh cho lưới
	glBegin(GL_LINES); // Bắt đầu vẽ các đường
	for (int i = -100; i <= 100; i++) { // Vẽ các đường lưới từ -100 đến 100
		glVertex3f((float)i, 0, (float)-100); // Điểm bắt đầu của đường lưới
		glVertex3f((float)i, 0, (float)100); // Điểm kết thúc của đường lưới
		glVertex3f((float)-100, 0, (float)i); // Điểm bắt đầu của đường lưới
		glVertex3f((float)100, 0, (float)i); // Điểm kết thúc của đường lưới
	}
	glEnd(); // Kết thúc vẽ các đường
}

// Hàm tính toán vị trí
void computePos(float deltaMove) {
	x += deltaMove * lx * moveSpeed; // Tính toán vị trí mới theo trục x
	z += deltaMove * lz * moveSpeed; // Tính toán vị trí mới theo trục z
}

// Hàm tính toán hướng
void computeDir(float deltaAngle) {
	angle += deltaAngle * rotateSpeed; // Tính toán góc xoay mới
	lx = sin(angle); // Tính toán hướng nhìn mới theo trục x
	lz = -cos(angle); // Tính toán hướng nhìn mới theo trục z
}

// Hàm xử lý phím thường
void processNormalKeys(unsigned char key, int xx, int yy) {
	if (key == 27) // Nếu phím ESC được nhấn
		exit(0); // Thoát chương trình
	switch (key) {
	case 'w':
		y += verticalMoveSpeed; // Tăng vị trí theo trục y (di chuyển lên)
		break;
	case 's':
		y -= verticalMoveSpeed; // Giảm vị trí theo trục y (di chuyển xuống)
		break;
	}
}

// Hàm xử lý phím đặc biệt
void processSpecialKeys(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP:
		deltaMove = 1.0f; // Di chuyển tới
		break;
	case GLUT_KEY_DOWN:
		deltaMove = -1.0f; // Di chuyển lùi
		break;
	case GLUT_KEY_LEFT:
		deltaAngle = -1.0f; // Xoay trái
		break;
	case GLUT_KEY_RIGHT:
		deltaAngle = 1.0f; // Xoay phải
		break;
	}
}

// Hàm giải phóng phím đặc biệt
void releaseSpecialKeys(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		deltaMove = 0.0f; // Dừng di chuyển
		break;
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		deltaAngle = 0.0f; // Dừng xoay
		break;
	}
}

// Hàm khởi tạo (hiện tại trống)
void init() {
}

// Hàm vẽ cảnh
void renderScene(void) {
	if (deltaMove)
		computePos(deltaMove); // Tính toán vị trí nếu có thay đổi di chuyển
	if (deltaAngle)
		computeDir(deltaAngle); // Tính toán hướng nếu có thay đổi góc xoay

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Xóa bộ đệm màu và độ sâu
	glLoadIdentity(); // Đặt lại ma trận đơn vị
	gluLookAt(x, y, z, // Vị trí camera
		x + lx, y + ly, z + lz, // Hướng nhìn của camera
		0.0f, 1.0f, 0.0f); // Vector hướng lên

	objs.renderObject(); // Vẽ đối tượng
	glutSwapBuffers(); // Hoán đổi bộ đệm kép
}

// Hàm chính
int main(int argc, char** argv) {
	objs.addObject("data/coconuts.obj", 10); // Thêm đối tượng từ file "data/car.obj"

	glutInit(&argc, argv); // Khởi tạo GLUT
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Thiết lập chế độ hiển thị
	glutInitWindowPosition(100, 100); // Thiết lập vị trí cửa sổ
	glutInitWindowSize(800, 600); // Thiết lập kích thước cửa sổ
	glutCreateWindow("3D Grid with FPS Controls"); // Tạo cửa sổ

	init(); // Gọi hàm khởi tạo

	glutDisplayFunc(renderScene); // Thiết lập hàm vẽ cảnh
	glutReshapeFunc(changeSize); // Thiết lập hàm thay đổi kích thước cửa sổ
	glutIdleFunc(renderScene); // Thiết lập hàm vẽ lại cảnh khi rảnh
	glutKeyboardFunc(processNormalKeys); // Thiết lập hàm xử lý phím thường
	glutSpecialFunc(processSpecialKeys); // Thiết lập hàm xử lý phím đặc biệt
	glutSpecialUpFunc(releaseSpecialKeys); // Thiết lập hàm giải phóng phím đặc biệt

	glEnable(GL_DEPTH_TEST); // Kích hoạt kiểm tra độ sâu
	glutMainLoop(); // Bắt đầu vòng lặp chính của GLUT

	return 0; // Trả về 0 (thành công)
}
