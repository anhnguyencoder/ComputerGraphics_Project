//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/

// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

float horizontalAngle = 0.0f;
float verticalAngle = 0.0f;

typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints];




point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;
GLuint model_loc;
GLuint view_loc;
GLuint projection_loc;

mat4 instance;
//vec4 colorCode;
mat4 control2;
mat4 control3;
mat4 control;
GLfloat sf = 1.0;// sf = Scale Facter Thêm biến toàn cục cho tỉ lệ


// KHAI BÁO THÊM CÁC BIẾN TOÀN CỤC Ở DƯỚI NÀY 👇
bool batBongDen = true;

// KHAI BÁO THÊM CÁC BIẾN TOÀN CỤC Ở TRÊN NÀY 👆


///* Khởi tạo các tham số chiếu sáng - tô bóng*/
point4 light_position(0.0, 1, 1, 1);
color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);

color4 material_ambient(1.0, 1, 1.0, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1, 1, 1, 1.);
float material_shininess = 500;





color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;

//đèn tắt
color4 light_ambient_off(0.0, 0.0, 0.0, 1.0);
color4 light_diffuse_off(0.0, 0.0, 0.0, 1.0);
color4 light_specular_off(0.0, 0.0, 0.0, 1.0);

color4 ambient_product_off = light_ambient_off * material_ambient;
color4 diffuse_product_off = light_diffuse_off * material_diffuse;
color4 specular_product_off = light_specular_off * material_specular;
void updateLighting() {
	if (batBongDen) {
		ambient_product = light_ambient * material_ambient;
		diffuse_product = light_diffuse * material_diffuse;
		specular_product = light_specular * material_specular;
	}
	else {
		ambient_product = ambient_product_off;
		diffuse_product = diffuse_product_off;
		specular_product = specular_product_off;
	}

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
}


// các vị trí
vec4 eye = vec4(0.0, 0.0, 40, 1.0); //vị trí camera , nên để  y =12;
vec4 at = vec4(0.0, 0.0, 0.0, 1.0); //vị trí nhìn vào
vec4 up = vec4(0.0, 1.0, 0.0, 0.0);

void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}


void generateGeometry(void)
{
	initCube();
	makeColorCube();

}

void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);

}

void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));


	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	updateLighting();  // Cập nhật ánh sáng

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");




	glEnable(GL_DEPTH_TEST);


	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}

void projection_view() {

	mat4 v = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, v);
	mat4 p = Perspective(45.0, 1.0, 0.1, 10.0);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);
}

void updateCamera()
{
	mat4 rotation = RotateY(horizontalAngle) * RotateX(verticalAngle);
	vec4 forward = vec4(0.0, 0.0, -1.0, 0.0); // Initial forward vector

	forward = rotation * forward;
	at = eye + forward;
}

void rotateCameraHorizontally(float angle)
{
	horizontalAngle += angle;
	updateCamera();
}

void rotateCameraVertically(float angle)
{
	verticalAngle += angle;
	// Clamp the vertical angle to avoid flipping
	if (verticalAngle > 89.0f) verticalAngle = 89.0f;
	if (verticalAngle < -89.0f) verticalAngle = -89.0f;

	updateCamera();
}

void moveForward()
{
	vec4 forward = normalize(at - eye);
	eye += forward * 0.5;
	at += forward * 0.5;
}

void moveBackward()
{
	vec4 forward = normalize(at - eye);
	eye -= forward * 0.5;
	at -= forward * 0.5;
}

void moveLeft()
{
	vec4 forward = normalize(at - eye);
	vec4 left = normalize(cross(up, forward));
	eye += left * 0.5;
	at += left * 0.5;
}

void moveRight()
{
	vec4 forward = normalize(at - eye);
	vec4 left = normalize(cross(up, forward));
	eye -= left * 0.5;
	at -= left * 0.5;
}

void moveUp()
{
	eye += up * 0.5;
	at += up * 0.5;
}

void moveDown()
{
	eye -= up * 0.5;
	at -= up * 0.5;
}


//tạo hình
void Tao(mat4 ins) {
	mat4 instance = ins;
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, control3 * control2 * control * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void Tao2(mat4 ins) {
	mat4 instance = ins;
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, control3 * control2 * control * instance);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}




void Mau(color4 color) {
	material_diffuse = color;
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
}

// CODE TẠO HÌNH Ở DƯỚI NÀY 👇

//SetMau
vec4 mauThanquat = vec4(0.498, 1, 0.831, 1);
vec4 mauCanhquat = vec4(0, 0.749, 1, 1);
vec4 mauGhe = vec4(0.0, 1.0, 0.0, 1.0);
vec4 mauTuong = vec4(0.690, 0.769, 0.871, 1.0);
vec4 mauGhesofa = vec4(0.824, 0.412, 0.118, 1); // Màu gỗ bàn ghế sofa gỗ
vec4 mauCuaChinh = vec4(0.55, 0.27, 0.07, 1.0);//màu cửa chính
vec4 mauMaiNha = vec4(0.863, 0.078, 0.235, 1);
vec4 mauCuaso = vec4(0.118, 0.565, 1, 1);
vec4 tayCamcuaso = vec4(1, 0.549, 0, 1);
vec4 mauKhungcua = vec4(0.941, 0.973, 1, 1);
vec4 mauCauThang = vec4(0.502, 0, 0, 1);
vec4 maukhungtivi = vec4(0.184, 0.31, 0.31, 1);
vec4 maumanhinh = vec4(0, 0.749, 1, 1);
vec4 blackColor = vec4(0.0, 0.0, 0.0, 1.0);
vec4 whiteColor = vec4(1.0, 1.0, 1.0, 1.0);
vec4 mauGiaden = vec4(1, 0.871, 0.678, 1);//màu giá đèn
vec4 mauduiden = vec4(0.502, 0, 0, 1);//đui đèn
vec4 mauWifi = vec4(0.753, 0.753, 0.753, 1);
vec4 mauBongwifi = vec4(0.498, 1, 0, 1);
vec4 mauTuTV = vec4(1, 0.87, 0.67, 1);

//{Giáp
vec4 mauchu = vec4(1, 0, 0, 1);
//Giáp}

//{Bằng
vec4 mauthanmayloc = vec4(0.961, 0.961, 0.961, 1);
vec4 maudemayloc = vec4(0, 0, 0.502, 1);
vec4 mauden = vec4(0, 0, 0, 1);

//Bằng}

//{Cường
vec4 mautulanh = vec4(0.467, 0.533, 0.6, 1);
vec4 maukhungtulanh = vec4(0.412, 0.412, 0.412, 1);
vec4 maukhungtrongtulanh = vec4(0.69, 0.878, 0.902, 1);
vec4 mauthanhtrongtulanh = vec4(0.529, 0.808, 0.922, 1);
vec4 mauDieuhoa = vec4(0.863, 0.863, 0.863, 1);

//Cường}


//{Nam
vec4 mauSoGio = vec4(0, 0, 0, 1);
vec4 mauKimGiay = vec4(255, 255, 0, 1);
vec4 mauMatDongHo = vec4(245, 245, 220, 1);
//Nam}






void TaoHinhTru3(mat4 instance) {

	for (int i = 0; i <= 79; i++) {
		Tao(instance * RotateZ(i * 4.5) * Translate(0, 0.25, 0) * Scale(0.0395, 0.5, 1));
	}
}
//Hàm dành riêng cho Bằng, Bằng sửa code, Ae thông cảm
void TaoHinhTruB(mat4 instance) {

	for (int i = 0; i <= 79; i++) {
		Tao(instance * RotateY(i * 4.5) * Translate(0, 0.25, 0) * Scale(0.0395, 0.5, 1));
	}
}
void TaoHinhTru2(mat4 instance) {

	for (int i = 0; i <= 39; i++) {
		Tao(instance * RotateZ(i * 9) * Translate(0, 0.25, 0) * Scale(0.079, 0.5, 1));
	}
}
void TaoHinhTru(mat4 instance) {

	for (int i = 0; i <= 19; i++) {
		Tao(instance * RotateZ(i * 18) * Translate(0, 0.25, 0) * Scale(0.159, 0.5, 1));
	}
}

//nhà
void Nha() {
	control2 = 1; control3 = 1;
	control = RotateY(0);
	Mau(mauTuong);
	//Tao(Translate(0, 0, 15.05) * Scale(20, 9, 0.1));// mặt trước
	//mặt trước	
			///trước trái
	Tao(Translate(-7.1, 0, 14.85) * Scale(6.2, 9, 0.5));
	///trước phải
	Tao(Translate(7.1, 0, 14.85) * Scale(6.2, 9, 0.5));
	///trước trên
	Tao(Translate(0, 4, 14.85) * Scale(8, 1, 0.5));
	//các mặt còn lại
	Tao(Translate(0, 0.2, -15.05) * Scale(20, 9.4, 0.1));// mặt sau
	Tao(Translate(-9.95, 0, 0) * Scale(0.5, 9, 30.3));// bên trái
	//tường bên phải
		/// trái
	Tao(Translate(9.95, 0, 10.15) * Scale(0.5, 9, 10));
	///phải
	Tao(Translate(9.95, 0, -7.5) * Scale(0.5, 9, 15.3));
	///trên
	Tao(Translate(9.95, 3.5, 2.65) * Scale(0.5, 2, 5));
	//dưới
	Tao(Translate(9.95, -3, 2.65) * Scale(0.5, 3, 5));

	//các tường khác
//mặt trên
	Tao(Translate(0, 4.45, 2.15) * Scale(21.13, 0.1, 26));//trên trước
	//sau
	Tao(Translate(-5.565, 4.45, -13) * Scale(10, 0.1, 4.3));//trên trái
	Tao(Translate(9.065, 4.45, -13) * Scale(3, 0.1, 4.3));//trên trái
	Tao(Translate(3.5, 4.45, -14.95) * Scale(8.13, 0.1, 0.4));//trên giữa


	// mặt dưới
	Tao(Translate(0, -4.45, 0) * Scale(20, 0.1, 30));
}
void Canhcua() {
	Tao(Scale(4, 8, 0.1));



	Tao(Translate(0,2.5,0)*Scale(2, 1, 0.2));
	Tao(Translate(0,-2.5,0)*Scale(2, 1, 0.2));
	Tao(Translate(0, 1, 0) * Scale(2, 1, 0.2));
	Tao(Translate(0, -1, 0) * Scale(2, 1, 0.2));

	Tao(Translate(0,3.8,0)*Scale(4, 0.4, 0.2));
	Tao(Translate(0,-3.8,0)*Scale(4, 0.4, 0.2));

	Tao(Translate(1.8,0,0)*Scale(0.4, 8, 0.2));
	Tao(Translate(-1.8,0,0)*Scale(0.4, 8, 0.2));
}
//cửa chính
GLfloat   bCanhT = 0;
GLfloat   bCanhP = 0;
void Bocua() {
	control = 1, control2 = 1, control3 = 1;
	Mau(mauCuaChinh);
	//cánh trái
	control = Translate(-4, -0.5, 15.05) * RotateY(bCanhT) * Translate(2, 0, 0);
	Canhcua();
	////cánh phải
	control = Translate(4, -0.5, 15.05) * RotateY(bCanhP) * Translate(-2, 0, 0);
	Canhcua();
}

//ghế gỗ
void Matghe() {
	Tao(Translate(-0.04, -0.1747, 0) * Scale(1.0971, 0.08, 0.4));
	Tao(Translate(-0.04, -0.1297, 0) * Scale(1.0971, 0.01, 0.4));

	Tao(Translate(-0.0425, 0.32, 0.175) * Scale(0.9421, 0.015, 0.05));
	//dung hai ben
	//benphai
	Tao(Translate(0.425, 0.08, 0.175) * Scale(0.007, 0.48, 0.05));
	//bentrai
	Tao(Translate(-0.5088, 0.08, 0.175) * Scale(0.007, 0.48, 0.05));
}
void Tayghe() {

	Tao(Translate(0.46855, -0.1072, -0.1925) * Scale(0.08, 0.4, 0.015));
	Tao(Translate(0.46855, -0.1072, 0.1925) * Scale(0.08, 0.4, 0.015));
	Tao(Translate(0.46855, 0.0928, 0) * Scale(0.08, 0.015, 0.4));
	//dung
	Tao(Translate(0.46855, -0.13, -0.05) * Scale(0.04, 0.15, 0.015));
	Tao(Translate(0.46855, -0.01, 0.06) * Scale(0.04, 0.1, 0.015));
	Tao(Translate(0.46855, 0.005, -0.105) * Scale(0.04, 0.05, 0.015));
	Tao(Translate(0.46855, -0.005, 0) * Scale(0.04, 0.025, 0.015));
	//ngang
	Tao(Translate(0.46855, 0.03, -0.025) * Scale(0.04, 0.015, 0.18));
	Tao(Translate(0.46855, -0.02, -0.053) * Scale(0.04, 0.015, 0.12));
	Tao(Translate(0.46855, -0.02, 0.13) * Scale(0.04, 0.015, 0.13));
	Tao(Translate(0.46855, -0.06, -0.06) * Scale(0.04, 0.015, 0.25));
}

void Lungghe() {
	//benphai
	Tao(Translate(0.418, 0.08, 0.175) * Scale(0.007, 0.48, 0.05));
	//bentrai
	Tao(Translate(0.118, 0.08, 0.175) * Scale(0.007, 0.48, 0.05));
	//thanh duoi cung
	Tao(Translate(0.27, -0.09, 0.175) * Scale(0.3, 0.015, 0.05));
	//tren
	Tao(Translate(0.27, -0.05, 0.175) * Scale(0.3, 0.015, 0.05));
	//tren nua
	Tao(Translate(0.27, -0.01, 0.175) * Scale(0.3, 0.015, 0.05));
	//tren nua 2
	Tao(Translate(0.27, 0.22, 0.175) * Scale(0.3, 0.015, 0.05));
	//lung vuong
	Tao(Translate(0.21355 + 0.052, 0.105, 0.175) * Scale(0.22, 0.24, 0.05));
}

GLfloat xGhe = 1.6, yGhe = -0.55, zGhe = -0.75, aGhe = 0.0, bGhe = 0, cGhe = 0;
void Ghe() {
	Mau(mauGhesofa);
	control = Scale(5, 5, 5) * Translate(xGhe, yGhe, zGhe) * RotateX(aGhe) * RotateY(bGhe) * RotateZ(cGhe) * RotateY(90);
	Matghe();
	Tayghe();

	Lungghe();
	control = control * Translate(-0.31, 0, 0);
	Lungghe();
	control = control * Translate(-0.31, 0, 0);
	Lungghe();
	control = control * Translate(-0.4, 0, 0);
	Tayghe();
}


//GHẾ NGẮN
void Matghengan() {
	Tao(Translate(0.271, -0.1747, 0) * Scale(0.474, 0.08, 0.4));
	Tao(Translate(0.271, -0.1297, 0) * Scale(0.474, 0.01, 0.4));

	Tao(Translate(0.21355 + 0.058, 0.32, 0.175) * Scale(0.3 + 0.007 + 0.007, 0.015, 0.05));
	//dung hai ben
	//bentrai
	Tao(Translate(0.425, 0.08, 0.175) * Scale(0.007, 0.48, 0.05));
	//benphai
	Tao(Translate(0.125, 0.08, 0.175) * Scale(0.007, 0.48, 0.05));
}
void Tayghengan() {

	Tao(Translate(0.46855, -0.1072, -0.1925) * Scale(0.08, 0.4, 0.015));//trước
	Tao(Translate(0.46855, -0.1072, 0.1925) * Scale(0.08, 0.4, 0.015));//sau
	Tao(Translate(0.46855, 0.0928, 0) * Scale(0.08, 0.015, 0.4));//trên
	//dung
	Tao(Translate(0.46855, -0.13, -0.05) * Scale(0.04, 0.15, 0.015));
	Tao(Translate(0.46855, -0.01, 0.06) * Scale(0.04, 0.1, 0.015));
	Tao(Translate(0.46855, 0.005, -0.105) * Scale(0.04, 0.05, 0.015));
	Tao(Translate(0.46855, -0.005, 0) * Scale(0.04, 0.025, 0.015));
	//ngang
	Tao(Translate(0.46855, 0.0325, -0.0225) * Scale(0.04, 0.015, 0.18));//trên cùng
	Tao(Translate(0.46855, -0.02, -0.0525) * Scale(0.04, 0.015, 0.12));//dưới trái
	Tao(Translate(0.46855, -0.02, 0.13) * Scale(0.04, 0.015, 0.13));//dưới phải
	Tao(Translate(0.46855, -0.06, -0.059) * Scale(0.04, 0.015, 0.2525));//dưới cùng
}
void Lungghengan() {
	//bentrai
	Tao(Translate(0.418, 0.08, 0.175) * Scale(0.007, 0.48, 0.05));
	//benphai
	Tao(Translate(0.118, 0.08, 0.175) * Scale(0.007, 0.48, 0.05));
	//thanh duoi cung
	Tao(Translate(0.27, -0.09, 0.175) * Scale(0.3, 0.015, 0.05));
	//tren
	Tao(Translate(0.27, -0.05, 0.175) * Scale(0.3, 0.015, 0.05));
	//tren nua
	Tao(Translate(0.27, -0.01, 0.175) * Scale(0.3, 0.015, 0.05));
	//tren nua 2
	Tao(Translate(0.27, 0.22, 0.175) * Scale(0.3, 0.015, 0.05));
	//lung vuong
	Tao(Translate(0.21355 + 0.052, 0.105, 0.175) * Scale(0.22, 0.24, 0.05));
}
GLfloat xGheN = 0.1, yGheN = -0.55, zGheN = -0.59, aGheN = 0.0, bGheN = 0, cGheN = 0, zGheN2 = -1.31;
void Ghengan() {
	Mau(mauGhesofa);
	control = 1; control2 = 1; control3 = 1;
	control = Scale(5, 5, 5) * Translate(xGheN, yGheN, zGheN) * RotateX(aGheN) * RotateY(bGheN) * RotateZ(cGheN) * RotateY(90 + 180);
	Matghengan();
	Tayghengan();
	Lungghengan();
	control = control * Translate(-0.395, 0, 0);
	Tayghengan();
}
void Ghengan2() {
	Mau(mauGhesofa);

	control = 1; control2 = 1; control3 = 1;
	control = Scale(5, 5, 5) * Translate(xGheN, yGheN, zGheN2) * RotateX(aGheN) * RotateY(bGheN) * RotateZ(cGheN) * RotateY(90 + 180);
	Matghengan();
	Tayghengan();
	Lungghengan();
	control = control * Translate(-0.395, 0, 0);
	Tayghengan();
}


// cái quat đứng

	//bảo vệ cánh quạt
void Haicaique() {

	//que thẳng
	Tao(Translate(8, 0, 0.15) * Scale(12, 0.2, 0.3));
	//que xiên
	Tao(Translate(17.5, 2.7, 0.15) * RotateZ(35) * Scale(9, 0.2, 0.3));

}
void Quecao() {
	Tao(Translate(0.6, 5.5, 21) * RotateY(1.8) * Scale(4, 0.5, 0.5));
}
void Nhieuquecao() {

	control = 1; Quecao();
	for (int i = 1; i <= 35; i++) {
		control = control * RotateY(-10); Quecao();
	}




}
void Nhieuque() {
	control = RotateY(0); Haicaique();

	for (int i = 1; i <= 21; i++) {
		control = control * RotateY(-360 / 22); Haicaique();
	}
	control = control * RotateY(-360 / 28); Haicaique();

}
void Khungbaove() {
	Mau(mauThanquat);
	control2 = RotateZ(0);
	Nhieuquecao();
	Nhieuque();
	control2 = control2 * Translate(0, 11, 0) * RotateZ(180);
	Nhieuquecao();
	Nhieuque();
	control = 1; control2 = 1;
	TaoHinhTru3(Translate(0, 12.5, 0) * Scale(4, 14, 4) * RotateX(-90));
	Tao(RotateY(45) * Scale(4, 1, 4));
}
GLfloat cCanhQuat = 0;
void Canhquat() {
	Mau(mauCanhquat);
	control = Translate(0, 5.5, 0);
	Tao(RotateY(cCanhQuat) * Translate(0, 0, 9) * Scale(6, 0.5, 18));
	Tao(RotateY(120) * RotateY(cCanhQuat) * Translate(0, 0, 9) * Scale(6, 0.5, 18));
	Tao(RotateY(240) * RotateY(cCanhQuat) * Translate(0, 0, 9) * Scale(6, 0.5, 18));

}
void Thanquat() {
	control2 = 1;
	control = Translate(0, 5.5, -2);
	Mau(mauThanquat);
	TaoHinhTru3(Translate(0, 12, -29) * Scale(4, 4, 60));
	TaoHinhTru(Translate(0, 12, -58) * Scale(25, 25, 3));



}
GLfloat xQuat = 6, yQuat = -1.65, zQuat = 1, bKhungquat = 0;



void Quatdung() {
	control3 = Translate(xQuat, yQuat, zQuat) * Scale(0.0424, 0.0424, 0.0424) * RotateX(-90);
	Thanquat();
	control3 = Translate(xQuat, yQuat, zQuat) * Scale(0.0424, 0.0424, 0.0424) * Translate(0, 0, -17.5) * RotateY(bKhungquat) * Translate(0, 0, -17.5) * RotateY(180) * RotateX(-90);
	Khungbaove();
	control3 = Translate(xQuat, yQuat, zQuat) * Scale(0.0424, 0.0424, 0.0424) * Translate(0, 0, -17.5) * RotateY(bKhungquat) * Translate(0, 0, -17.5) * RotateY(180) * RotateX(-90);
	Canhquat();


	if (bKhungquat > 90) bKhungquat = 90;// nếu để quạt chạy tự động thì ko cần 2 dòng này nhưng nếu ta tác động thủ công vào nó bằng keyboard thì phải cần nếu ko sẽ lỗi
	if (bKhungquat < -90) bKhungquat = -90;
}
//mái nhá
void Mainha() {

	//tam giác trước
	Tao2(Translate(0, 4.45, 14) * RotateZ(-135) * Scale(15, 15, 1));
	//tam giác sau
	Tao2(Translate(0, 4.45, -15.5) * RotateZ(-135) * Scale(15, 15, 1));
	Mau(mauMaiNha);
	//mái trái
	Tao(Translate(-5.621, 9.38, 0) * RotateZ(45) * Scale(16, 0.1, 33));
	//mái phải
	Tao(Translate(5.621, 9.38, 0) * RotateZ(-45) * Scale(16, 0.1, 33));
}
//cầu thang
void Bac1() {
	Tao(Translate(-4.2, -4.2, -8) * Scale(1, 0.4, 4));//bậc
	Tao(Translate(-3.75, -3.4, -6.05) * Scale(0.1, 2, 0.1));//cột trái
	Tao(Translate(-3.75, -3.4, -9.95) * Scale(0.1, 2, 0.1));//cột phải
}
void Goibac1() {
	Mau(mauCauThang);
	Bac1();
	for (int i = 1; i <= 3; i++) {
		control = control * Translate(-1, 0.4, 0);
		Bac1();
	}
	control = 1; control2 = 1; control3 = 1;
	//cote tiếp các phần còn lại của bậc dưới
	Tao(Translate(-8.7, -2.6, -10.5) * Scale(2, 0.4, 9));//bậc to
	Tao(Translate(-7.75, -1.8, -6.05) * Scale(0.1, 2, 0.1));//cột trái 1
	Tao(Translate(-8.7, -1.8, -6.05) * Scale(0.1, 2, 0.1));//cột trái 2
	Tao(Translate(-7.75, -1.8, -9.95) * Scale(0.1, 2, 0.1));//cột phải
	Tao(Translate(-7.75, -1.8, -11.05) * Scale(0.1, 2, 0.1));//cột phải phải
	//thanh ngang
	Tao(Translate(-8.7, -0.777, -6.05) * Scale(1.89, 0.1, 0.13));
	//thanh doc
	Tao(Translate(-7.75, -0.777, -10.5) * Scale(0.13, 0.1, 1.2));

	//thanh xien trai
	Tao(Translate(-5.72, -1.59, -6.05) * RotateZ(-21.6) * Scale(4.43, 0.11, 0.13));
	//thanh xien phai
	Tao(Translate(-5.72, -1.59, -9.95) * RotateZ(-21.6) * Scale(4.43, 0.11, 0.13));


}
void Bac2() {
	Tao(Translate(-7.2, -2.2, -13) * Scale(1, 0.4, 4));//bậc
	Tao(Translate(-6.75, -1.4, -11.05) * Scale(0.1, 2, 0.1));//cột trái
}
void Goibac2() {
	Mau(mauCauThang);
	control = 1; control2 = 1; control3 = 1;
	Bac2();
	for (int i = 1; i <= 12; i++) {
		control = control * Translate(1, 0.4, 0);
		Bac2();
	}
	control = 1; control2 = 1; control3 = 1;
	//code tiếp các phần còn lại của bậc trên
	Tao(Translate(5.8, 3, -13) * Scale(1, 0.4, 4));//bậc
	Tao(Translate(6.25, 3.8, -11.05) * Scale(0.1, 1.22, 0.1));//cột trái
	//trên 1 bậc
	Tao(Translate(6.8, 3.4, -13) * Scale(1, 0.4, 4));//bậc
	Tao(Translate(7.25, 4.01, -11.05) * Scale(0.1, 0.82, 0.1));//cột trái
	//thanh trên cùng
	Tao(Translate(-1.295, 1.8, -11.05) * RotateZ(21.6) * Scale(14.071, 0.1, 0.13));//cột trái

}
void Cauthang() {

	control = 1; control2 = 1; control3 = 1;
	Goibac1();
	Goibac2();

}
//cửa sổ
void KhungCua() {
	//khung cửa

	Mau(mauKhungcua);
	//thanh đứng
	Tao(Translate(9.785, 0.5, 0.864) * Scale(0.1, 4, 0.1));

	for (int i = 1; i <= 5; i++) {
		control = control * Translate(0, 0, 0.71428);
		Tao(Translate(9.785, 0.5, 0.864) * Scale(0.1, 4, 0.1));//thanh đứng
	}
	//thanh ngang
	control = 1;
	Tao(Translate(9.785, -0.5, 0.507) * Scale(0.1, 0.1, 0.714));//thanh ngang dưới
	Tao(Translate(9.785, 1.5, 0.507) * Scale(0.1, 0.1, 0.714));//thanh ngang tren
	for (int i = 1; i <= 3; i++) {
		control = control * Translate(0, 0, 0.714 * 2);
		Tao(Translate(9.785, -0.5, 0.507) * Scale(0.1, 0.1, 0.714));//thanh ngang dưới
		Tao(Translate(9.785, 1.5, 0.507) * Scale(0.1, 0.1, 0.714));//thanh ngang tren
	}

	//thanh ngang giữa
	control = 1;
	Tao(Translate(9.785, 0.5, 1.221) * Scale(0.1, 0.1, 0.714));//thanh ngang tren
	Tao(Translate(9.785, 0.5, 4.078) * Scale(0.1, 0.1, 0.714));//thanh ngang tren





}
GLfloat yCuaso = 0.5;
GLfloat gtYcuaso = 4;
GLfloat ytaycam = -1.3;
void Cuaso() {
	//cửa sổ kéo

	Mau(mauCuaso);
	Tao(Translate(9.95, yCuaso, 2.65) * Scale(0.1, gtYcuaso, 5));
	Mau(tayCamcuaso);
	Tao(Translate(9.85, ytaycam, 2.65) * Scale(0.1, 0.08, 0.5));//tay cầm
}
void BoCuaso() {
	control = 1; control2 = 1; control3 = 1;
	KhungCua();
	control = 1;  control3 = 1;
	Cuaso();
}
//quạt trần
void Thanquattran() {
	//giá quạt
	TaoHinhTru2(Translate(0, 4.25, 0) * Scale(2, 0.3, 2) * RotateX(90));
	//trục đứng
	TaoHinhTru2(Translate(0, 3.1, 0) * Scale(0.3, 2, 0.3) * RotateX(90));
}
void Canhquattran() {
	Tao(Translate(0, 2.05, -2.5) * Scale(1.5, 0.1, 5));
	Tao(RotateY(120) * Translate(0, 2.05, -2.5) * Scale(1.5, 0.1, 5));
	Tao(RotateY(240) * Translate(0, 2.05, -2.5) * Scale(1.5, 0.1, 5));
}
GLfloat yQuattran = 5.8;
GLfloat bCanhquattran = 0;
void Quattran() {

	control = 1; control2 = 1; control3 = 1;
	control = Scale(0.43, 0.43, 0.43) * Translate(0, yQuattran, 0);
	Thanquattran();
	control2 = RotateY(bCanhquattran);
	Canhquattran();
}

bool tvOn = false;
void tivi() {
	if (tvOn == true) {
		maumanhinh = whiteColor;
	}
	else {
		maumanhinh = blackColor;
	}
	control = Scale(0.8, 0.8, 0.8) * Translate(5.5, 0, -13);
	control2 = 1; control3 = 1;
	//màn hình
	Mau(maumanhinh);
	Tao(Translate(0, 0, 0.001) * Scale(5, 3.3, 0.2));
	//khung
	Mau(maukhungtivi);
	Tao(Translate(0, 0, 0) * Scale(5.1, 3.4, 0.2));
	//chân phải
	Tao(Translate(1.6, 0, 0) * Translate(0, -1.7, 0) * RotateX(-45) * Translate(0, -0.4, 0) * Scale(0.1, 0.8, 0.05));
	Tao(Translate(1.6, 0, 0) * Translate(0, -1.7, 0) * RotateX(45) * Translate(0, -0.4, 0) * Scale(0.1, 0.8, 0.05));
	//chân trái
	Tao(Translate(-1.6, 0, 0) * Translate(0, -1.7, 0) * RotateX(-45) * Translate(0, -0.4, 0) * Scale(0.1, 0.8, 0.05));
	Tao(Translate(-1.6, 0, 0) * Translate(0, -1.7, 0) * RotateX(45) * Translate(0, -0.4, 0) * Scale(0.1, 0.8, 0.05));
}

void Bongden() {
	/*if (batBongDen == false) {
		color4 light_ambient(1, 1, 1, 1.0);
	}*/

	if (batBongDen) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	control = 1; control2 = 1; control3 = 1;
	Mau(mauGiaden);
	Tao(Translate(0, 4, 14.5) * Scale(8, 0.4, 0.2));
	Mau(mauduiden);
	Tao(Translate(3.89, 4, 14.275) * Scale(0.22, 0.3, 0.25));
	Tao(Translate(-3.89, 4, 14.275) * Scale(0.22, 0.3, 0.25));

	//bóng đèn
	Mau(vec4(1, 1, 1, 1));
	TaoHinhTru2(Translate(0.005, 4, 14.275) * RotateY(90) * Scale(0.2, 0.2, 7.652));
}

GLfloat cRouterT = 15, cRouterP = -15;
void Wifi() {
	if (cRouterT > 90) cRouterT = 90;
	if (cRouterT < -60) cRouterT = -60;
	if (cRouterP < -90) cRouterP = -90;
	if (cRouterP > 60) cRouterP = 60;
	control = Scale(0.7, 0.7, 0.7) * Translate(3, -2.3, -15); control2 = 1; control3 = 1;
	Mau(mauWifi);
	Tao(Scale(2, 0.3, 1));//hộp wifi
	Tao(Translate(-0.4, 0, 0) * RotateZ(cRouterT) * Translate(0, 0.5, -0.525) * Scale(0.1, 1, 0.05));//rou trái
	Tao(Translate(0.4, 0, 0) * RotateZ(cRouterP) * Translate(0, 0.5, -0.525) * Scale(0.1, 1, 0.05));//rou phải
	Mau(mauBongwifi);
	Tao(Translate(-0.627, 0, 0.505) * Scale(0.05, 0.05, 0.01));
	Tao(Translate(-0.478, 0, 0.505) * Scale(0.05, 0.05, 0.01));
	Tao(Translate(-0.334, 0, 0.505) * Scale(0.05, 0.05, 0.01));



}
void Loa() {
	control = Translate(0.6, -4.2, -11) * Scale(2, 2, 2);

	Mau(vec4(1, 0.647, 0, 1));
	Tao(Translate(0, 0.75, 0.227) * Scale(0.4, 1.5, 0.4));
	//Loa trên
	Mau(vec4(0, 0, 0, 1));
	TaoHinhTru2(Translate(0, 1.179, 0.435) * Scale(0.075 * 2, 0.075 * 2, 0.01));
	Mau(vec4(0.973, 0.973, 1, 1));
	TaoHinhTru2(Translate(0, 1.179, 0.432) * Scale(0.095 * 2, 0.095 * 2, 0.01));
	Mau(vec4(0, 0, 0, 1));
	TaoHinhTru2(Translate(0, 1.179, 0.43) * Scale(0.1 * 2, 0.1 * 2, 0.01));
	//Loa giữa
	Mau(vec4(0, 0, 0, 1));
	TaoHinhTru2(Translate(0, 0.844, 0.435) * Scale(0.105 * 2, 0.105 * 2, 0.01));
	Mau(vec4(0.973, 0.973, 1, 1));

	TaoHinhTru2(Translate(0, 0.844, 0.432) * Scale(0.125 * 2, 0.125 * 2, 0.01));
	Mau(vec4(0, 0, 0, 1));

	TaoHinhTru2(Translate(0, 0.844, 0.430) * Scale(0.13 * 2, 0.13 * 2, 0.01));
	//Loa dưới
	Mau(vec4(0, 0, 0, 1));

	TaoHinhTru2(Translate(0, 0.44, 0.435) * Scale(0.125 * 2, 0.125 * 2, 0.01));
	Mau(vec4(0.973, 0.973, 1, 1));

	TaoHinhTru2(Translate(0, 0.44, 0.432) * Scale(0.155 * 2, 0.155 * 2, 0.01));
	Mau(vec4(0, 0, 0, 1));

	TaoHinhTru2(Translate(0, 0.44, 0.430) * Scale(0.16 * 2, 0.16 * 2, 0.01));
}
void BoLoa() {
	control = 1;
	control3 = 1;
	Loa();
	//loa phải
	control2 = Translate(7.5, 0, 0);
	Loa();


}


void HoaVan() {
	Mau(vec4(1, 0.843, 0, 1));

	control = 1;

	TaoHinhTru2(Translate(0, 0.2, 0) * Scale(0.1, 0.4, 0.01));
	control = RotateZ(90) * Translate(0, 0.2, 0);
	TaoHinhTru2(Scale(0.1, 0.4, 0.01));
	control = RotateZ(180) * Translate(0, 0.2, 0);
	TaoHinhTru2(Scale(0.1, 0.4, 0.01));
	control = RotateZ(270) * Translate(0, 0.2, 0);
	TaoHinhTru2(Scale(0.1, 0.4, 0.01));
}
void HoaVan2() {
	Mau(vec4(1, 0.412, 0.706, 1));

	control = 1;


	for (int i = 0; i <= 10; i++) {
		control = RotateZ(i * 20);
		TaoHinhTru2(Scale(0.4, 2, 0.03));
	}

}

void ThamTraiNha() {
	control = 1;
	control2 = 1;
	control3 = Translate(-3.4, -4.4, 6) * Scale(2, 2, 2) * RotateX(-90);
	control2 = Translate(0, 0, 0.002);
	HoaVan();
	control2 = Translate(1, 1, 0.002);
	HoaVan();
	control2 = Translate(1, -1, 0.002);
	HoaVan();
	control2 = Translate(2, 0, 0.002);
	HoaVan();
	control2 = Translate(3, 1, 0.002);
	HoaVan();
	control2 = Translate(3, -1, 0.002);
	HoaVan();
	control2 = Translate(4, 0, 0.002);
	HoaVan();






	//thảm to
	Mau(vec4(0.847, 0.749, 0.847, 1));
	control2 = Translate(1.8, 0, -0.001);
	Tao(Scale(4, 6.25, 0.01));
	//thảm nhỏ
	Mau(vec4(0.294, 0, 0.51, 1));
	control2 = Translate(1.8, 0, 0);
	Tao(Scale(3, 5.125, 0.01));
	//viền
	Mau(vec4(0.294, 0, 0.51, 1));

	//trên
	control2 = Translate(1.8, 2, 0.001);
	Tao(Scale(0.09, 6.25, 0.01));
	//dưới
	control2 = Translate(1.8, -2, 0.001);
	Tao(Scale(0.09, 6.25, 0.01));

	//trái
	control2 = Translate(1.8 - 3.125, 0, 0.001);
	Tao(Scale(4.09, 0.09, 0.01));
	//phải
	control2 = Translate(1.8 + 3.125, 0, 0.001);
	Tao(Scale(4.09, 0.09, 0.01));
}

void Hoa() {
	HoaVan2();
	//nhị hoa
	control = 1;
	Mau(mauCauThang);
	TaoHinhTru(Translate(0, 0, 0) * Scale(1, 1, 0.04));
	Mau(vec4(0.42, 0.557, 0.137, 1));
	Tao(Translate(0, -3, 0) * Scale(0.1, 5, 0.029));
}
void binh() {
	control2 = Scale(1.5, 1.5, 1.5);
	Mau(vec4(0.251, 0.878, 0.816, 1));
	for (int i = 1; i <= 30; i++) {
		Tao(RotateY(i * 20) * RotateZ(20) * Translate(0, 0.75, 0) * Scale(0.38, 4, 0.38));
	}
}
void binhHoa() {
	control = 1;
	control2 = 1;
	control3 = Translate(-8.2, -0.65, 3.8) * RotateY(90) * Scale(0.25, 0.25, 0.25);
	binh();
	control2 = Translate(0, 8, 0);
	Hoa();
	control2 = Translate(0, 7, 0) * Translate(-0.1, 0, 0.3) * Translate(0, -5, 0) * RotateZ(15) * Translate(0, 5, 0);
	Hoa();
	control2 = Translate(0, 7, 0) * Translate(-0.1, 0, 0.3) * RotateY(60) * Translate(0, -5, 0) * RotateZ(15) * Translate(0, 5, 0);
	Hoa();
	control2 = Translate(0, 7, 0) * Translate(0.1, 0, -0.3) * Translate(0, -5, 0) * RotateZ(-15) * Translate(0, 5, 0);
	Hoa();
	control2 = Translate(0, 7, 0) * Translate(0.1, 0, 0.5) * RotateY(150) * Translate(0, -5, 0) * RotateZ(15) * Translate(0, 5, 0);
	Hoa();

}
void KeBinhHoa() {
	control = Scale(0.8, 0.8, 0.8) * Translate(-10.5, -3.4, 4.5);
	control2 = 1;
	control3 = 1;
	Mau(vec4(0.196, 0.804, 0.196, 1));
	for (int i = 1; i <= 5; i++) {
		TaoHinhTru2(RotateY(i * 72) * Translate(0, 0, 1) * Scale(0.1, 4, 0.1) * RotateX(90));
	}
	TaoHinhTru2(Translate(0, 2, 0) * Scale(2.5, 0.1, 2.5) * RotateX(90));

}

//Ghesofa
GLfloat cGoi1 = 0, xGoi1 = 0, yGoi1 = 0, zGoi1 = 0;
GLfloat cGoi2 = 0, xGoi2 = 0, yGoi2 = 0, zGoi2 = 0;

void GheSoFaDem() {
	Mau(vec4(0.529, 0.808, 0.922, 1));
	control = 1;
	control2 = 1;
	control3 = 1;
	Tao(Translate(-7.45, -4, 12.8) * Scale(4.5, 1, 2.0));
	Tao(Translate(-7.45, -3, 14.2) * Scale(4.5, 3, 0.8));

	Tao(Translate(-7.9, -4, 9.8) * Scale(2, 1, 8));
	Tao(Translate(-9.3, -3, 9.8) * Scale(0.8, 3, 8));
	//đệm
	Mau(vec4(0.439, 0.502, 0.565, 1));//màu xám
	Tao(Translate(-7.45, -3.25, 12.8) * Scale(4.5, 0.5, 2));
	Tao(Translate(-7.9, -3.25, 9.8) * Scale(2, 0.5, 8));
	Tao(Translate(-7.9, -2.75, 5.95) * Scale(2, 0.5, 0.3));
	 
	//Gối
	Mau(vec4(1, 0.753, 0.796, 1));
	Tao(Translate(-8.75, -2.5, 12.3) *Translate(xGoi1, yGoi1, zGoi1) *RotateZ(cGoi1)* Scale(0.3, 1, 1));
	Tao( Translate(-8.75, -2.5, 8.3) * Translate(xGoi2, yGoi2, zGoi2) * RotateZ(cGoi2) * Scale(0.3, 1, 1));

}

//Qanh}

//{Giáp

//Tao tu tivi
void tu() {
	//matsautu
	Tao(Translate(0, 0, 0) * Scale(9, 4, 0.2));
	//bên phải
	Tao(Translate(4.4, 0.0, 2.1) * Scale(0.2, 4, 4));
	//bên trái
	Tao(Translate(-4.4, 0, 2.1) * Scale(0.2, 4, 4));
	//ở giữa
	Tao(Translate(0, 0, 2.1) * Scale(0.2, 4, 4));
	//bên trên
	Tao(Translate(0, 2, 2) * Scale(9, 0.2, 4.2));
	//bên dưới
	Tao(Translate(0, -2, 2) * Scale(9, 0.2, 4.2));
	//chantu
	Tao(Translate(4.25, -2.5, 3.85) * Scale(0.5, 1, 0.5));
	Tao(Translate(4.25, -2.5, 0.15) * Scale(0.5, 1, 0.5));
	Tao(Translate(-4.25, -2.5, 0.15) * Scale(0.5, 1, 0.5));
	Tao(Translate(-4.25, -2.5, 3.85) * Scale(0.5, 1, 0.5));
}
//cuatu
void cuatu1() {
	//Cánh Trái
	Mau(mauTuTV);
	Tao(Scale(4.6, 4.2, 0.2));
	Mau(vec4(1, 0.973, 0.863, 1));

	//tay cầm
	Tao(Translate(-1.3, 0, 0.2) * Scale(0.2, 0.4, 0.2));

}
void cuatu2() {
	//Cánh Trái
	Mau(mauTuTV);
	Tao(Scale(4.6, 4.2, 0.2));
	//tay cầm
	Mau(vec4(1, 0.973, 0.863, 1));
	Tao(Translate(1.3, 0, 0.2) * Scale(0.2, 0.4, 0.2));
	Mau(mauTuTV);

}

GLfloat yTutv = -5.6;
GLfloat zTutv = -20;
GLfloat xTutv = 6.4;
GLfloat acanhtu1 = 0;
GLfloat bcanhtu2 = 0;
void Tutv() {


	tu();
	//canhphai
	control = Translate(4.4, 0, 4.2) * RotateY(acanhtu1) * Translate(-2.2, 0, 0);
	cuatu1();
	//canhtrai
	control = Translate(-4.4, 0, 4.2) * RotateY(bcanhtu2) * Translate(2.2, 0, 0);
	cuatu2();
}

void tutivi() {
	control = 1; control2 = 1; control3 = 1;
	Mau(mauTuTV);
	control2 = Scale(0.68, 0.5, 0.6) * Translate(xTutv, yTutv, zTutv);
	Tutv();
}
////Chữ 2024--đang hoàn thiện 
void c2024() {
	//so2
	Tao(Translate(-10.38, 2.34, 0) * Scale(1.32, 1.28, 0.2));
	Tao(Translate(-10.05, 3.13, 0) * RotateZ(-40) * Scale(1.32, 1.5, 0.2));
	Tao(Translate(-8.75, 3.47, 0) * Scale(2.65, 1.32, 0.2));
	Tao(Translate(-7.4, 3.17, 0) * RotateZ(-45) * Scale(1.4, 1.32, 0.2));
	Tao(Translate(-7.1, 1.37, 0) * Scale(1.32, 3.53, 0.2));
	Tao(Translate(-7.54, -0.56, 0) * RotateZ(-45) * Scale(1.32, 1.8, 0.2));
	Tao(Translate(-8.94, -1, 0) * Scale(2.46, 1.32, 0.2));
	Tao(Translate(-10.11, -1.34, 0) * RotateZ(-45) * Scale(1.5, 1.32, 0.2));
	Tao(Translate(-10.45, -3.29, 0) * Scale(1.32, 4.04, 0.2));
	Tao(Translate(-8.84, -4.65, 0) * Scale(4.53, 1.32, 0.2));
	//So0
	Tao(Translate(-2.99, 3.47, 0) * Scale(2.83, 1.32, 0.2));
	Tao(Translate(-4.41, 3.2, 0) * RotateZ(45) * Scale(1.32, 1.32, 0.2));
	Tao(Translate(-4.66, -0.58, 0) * Scale(1.36, 7.56, 0.2));
	Tao(Translate(-4.41, -4.37, 0) * RotateZ(-45) * Scale(1.32, 1.32, 0.2));
	Tao(Translate(-2.99, -4.65, 0) * Scale(2.83, 1.32, 0.2));
	Tao(Translate(-1.57, -4.37, 0) * RotateZ(-45) * Scale(1.32, 1.32, 0.2));
	Tao(Translate(-1.32, -0.58, 0) * Scale(1.32, 7.56, 0.2));
	Tao(Translate(-1.57, 3.2, 0) * RotateZ(-45) * Scale(1.32, 1.32, 0.2));
	//so2
	Tao(Translate(1.09, 2.34, 0) * Scale(1.32, 1.28, 0.2));
	Tao(Translate(1.42, 3.13, 0) * RotateZ(-40) * Scale(1.32, 1.5, 0.2));
	Tao(Translate(2.72, 3.47, 0) * Scale(2.65, 1.32, 0.2));
	Tao(Translate(4.08, 3.17, 0) * RotateZ(-45) * Scale(1.4, 1.32, 0.2));
	Tao(Translate(4.38, 1.37, 0) * Scale(1.32, 3.53, 0.2));
	Tao(Translate(3.94, -0.56, 0) * RotateZ(-45) * Scale(1.32, 1.8, 0.2));
	Tao(Translate(2.53, -1, 0) * Scale(2.46, 1.32, 0.2));
	Tao(Translate(1.36, -1.34, 0) * RotateZ(-45) * Scale(1.5, 1.32, 0.2));
	Tao(Translate(1.03, -3.29, 0) * Scale(1.32, 4.04, 0.2));
	Tao(Translate(2.63, -4.65, 0) * Scale(4.53, 1.32, 0.2));
	//so4
	Tao(Translate(7.73, 0.66, 0) * RotateZ(-25) * Scale(1.32, 6.21, 0.2));
	Tao(Translate(8.74, -2.53, 0) * Scale(5.85, 1.32, 0.2));
	Tao(Translate(10.26, -2.1, 0) * Scale(1.32, 6.12, 0.2));
}
void vitrichu() {
	control = 1; control2 = 1; control3 = 1;
	Mau(mauchu);
	control = Scale(0.175, 0.175, 0.43) * Translate(0, 50, 35);
	c2024();
}
//Giáp}

//{Bằng

//máy lọc
void Thanmayloc() {
	Mau(mauthanmayloc);
	//thanmay
	Tao(Translate(-15, -16, 0) * Scale(4, 10, 4));
	//đế máy
}
void matmayloc()
{
	Mau(maudemayloc);
	Tao(Translate(-15, -11.5, 0) * Scale(4.05, 0.8, 4.05));
}
void nenmayloc()
{
	Mau(mauden);
	Tao(Translate(-13, -15.8, 0) * Scale(0.1, 7.8, 2));
}
void Demayloc()
{
	Mau(maudemayloc);
	Tao(Translate(-15, -20, 0) * Scale(4.1, 0.8, 4.1));
}

void devoi()
{
	Tao(Translate(-16, -11, 0) * Scale(1, 1.5, 1));

}

GLfloat bvannuoc = 0;
GLfloat aVoiLoc = 0;

void vannuoc() {
	Mau(mauCanhquat);
	control = Translate(-7, -0.1, -0.25) * RotateZ(bvannuoc);
	Tao(Translate(0, 0, 0) * Scale(0.1, 0.4, 0.1));
	Tao(RotateZ(120) * Translate(0, 0, 0) * Scale(0.1, 0.4, 0.1));
	Tao(RotateZ(240) * Translate(0, 0, 0) * Scale(0.1, 0.4, 0.1));
}
void voinuoc()
{
	TaoHinhTruB(Translate(-16, -10.5, 0) * Scale(0.35, 3, 0.35));
	TaoHinhTruB(Translate(-16, -9.1, 0) * RotateZ(-40) * Scale(0.35, 2, 0.35));
	TaoHinhTruB(Translate(-15.5, -8.42, 0) * RotateZ(-90) * Scale(0.35, 2, 0.35));
	TaoHinhTruB(Translate(-14.275, -9.56, 0) * RotateZ(15) * Scale(0.35, 2.5, 0.35));
}



void thanden()
{//de den
	Tao(Translate(0, -7, 0) * Scale(2, 0.3, 2));
	//thanden
	Tao(Translate(0, -5.5, 0) * Scale(0.5, 3, 0.5));

}


GLfloat ythanmayloc = 10;


void Mayloc() {
	control = 1; control2 = 1; control3 = 1;
	control3 = Scale(0.8, 0.8, 0.8) * Translate(4.3, -0.6, 7.5) * RotateY(180);
	control = Scale(0.43, 0.43, 0.43) * Translate(0, ythanmayloc, 0);
	Thanmayloc();
	Demayloc();
	matmayloc();
	voinuoc();
	nenmayloc();
	devoi();
	control = RotateY(bvannuoc);
	vannuoc();

}
//bongden

void khungden()
{
	Mau(mauThanquat);
	Tao(Translate(2, -3.5, 0) * RotateZ(20) * Scale(0.2, 3, 2.5));
	Tao(Translate(2, -3.5, 0) * RotateZ(20) * Scale(0.2, 3, 1.5) * RotateZ(90));
	Tao(Translate(2, -3.5, 0) * RotateZ(20) * Scale(0.2, 3, 1.5) * RotateZ(180));

}
void Den()
{
	thanden();

	khungden();
}


//Bằng}

//{Cường

bool hienthikinhban = false
;
void Kinhban() {
	
		Mau(mauDieuhoa);
		Tao(Translate(0, 0.4, 0) * Scale(18, 0.2, 8));
	

	
	
}
void Matban() {
	//Vien mat ban
	Tao(Translate(0, 0.2, 3.5) * Scale(18, 0.4, 1));
	Tao(Translate(0, 0.2, -3.5) * Scale(18, 0.4, 1));
	Tao(Translate(8.5, 0.2, 0) * Scale(1, 0.4, 6));
	Tao(Translate(-8.5, 0.2, 0) * Scale(1, 0.4, 6));
	//Thanh trong mat ban
	Tao(Translate(0, 0.2, 1.5) * Scale(16, 0.4, 0.5));
	Tao(Translate(0, 0.2, -1.5) * Scale(16, 0.4, 0.5));
	Tao(Translate(0, 0.2, 0) * Scale(1, 0.4, 2.5));
	Tao(Translate(4, 0.2, 0) * Scale(1, 0.4, 2.5));
	Tao(Translate(-4, 0.2, 0) * Scale(1, 0.4, 2.5));

	Tao(Translate(-6.25, 0.2, 0) * Scale(3.5, 0.4, 0.5));
	Tao(Translate(-6.25, 0.2, 0.75) * Scale(3.5, 0.4, 0.5));
	Tao(Translate(-6.25, 0.2, -0.75) * Scale(3.5, 0.4, 0.5));

	Tao(Translate(6.25, 0.2, 0) * Scale(3.5, 0.4, 0.5));
	Tao(Translate(6.25, 0.2, -0.75) * Scale(3.5, 0.4, 0.5));
	Tao(Translate(6.25, 0.2, 0.75) * Scale(3.5, 0.4, 0.5));

	Tao(Translate(-2, 0.2, 0) * Scale(3, 0.4, 1));
	Tao(Translate(2, 0.2, 0) * Scale(3, 0.4, 1));
}
void Chanban() {
	//4 chan
	Tao(Translate(8.5, -2.6, 3.5) * Scale(1, 5, 1));
	Tao(Translate(8.5, -2.6, -3.5) * Scale(1, 5, 1));
	Tao(Translate(-8.5, -2.6, 3.5) * Scale(1, 5, 1));
	Tao(Translate(-8.5, -2.6, -3.5) * Scale(1, 5, 1));
	// Cai gi do trong chan
	Tao(Translate(0, -0.21, 3.5) * Scale(18, 0.4, 1));
	Tao(Translate(0, -0.21, -3.5) * Scale(18, 0.4, 1));
	Tao(Translate(8.5, -0.21, 0) * Scale(1, 0.4, 6));
	Tao(Translate(-8.5, -0.21, 0) * Scale(1, 0.4, 6));

	Tao(Translate(-8.5, -4.2, 0) * Scale(0.6, 0.5, 6));
	Tao(Translate(8.5, -4.2, 0) * Scale(0.6, 0.5, 6));
	Tao(Translate(0, -4.2, 3.5) * Scale(16, 0.5, 0.6));
	Tao(Translate(0, -4.2, -3.5) * Scale(16, 0.5, 0.6));

	Tao(Translate(0, -4.2, 0) * Scale(0.6, 0.5, 4));
	Tao(Translate(2, -4.2, 0) * Scale(0.6, 0.5, 4));
	Tao(Translate(4, -4.2, 0) * Scale(0.6, 0.5, 4));
	Tao(Translate(6.5, -4.2, 0) * Scale(0.6, 0.5, 6.4));
	Tao(Translate(0, -4.2, 2.2) * Scale(12.5, 0.5, 0.5));
	Tao(Translate(0, -4.2, -2.2) * Scale(12.5, 0.5, 0.5));
	Tao(Translate(-2, -4.2, 0) * Scale(0.6, 0.5, 4));
	Tao(Translate(-4, -4.2, 0) * Scale(0.6, 0.5, 4));
	Tao(Translate(-6.5, -4.2, 0) * Scale(0.6, 0.5, 6.4));
}

void Ban() {
	control = 1; control2 = 1; control3 = 1;
	Mau(mauGhesofa);
	control = Scale(0.35, 0.35, 0.35) * Translate(12, -7, -10) * RotateY(90);
	Matban();
	Chanban();

	if (hienthikinhban) {
		Kinhban();

	}
	else
	{

	}
	
}



GLfloat mdieuhoa = 0;
void Thanhdieuhoa() {
	Tao(Scale(1.4, 0.1, 9.8));
}
void Mdieuhoa() {
	control = Translate(1.8, -1.65, 0) * RotateZ(10) * RotateZ(mdieuhoa) * Translate(0.3, -0.05, 0);
	Thanhdieuhoa();
}
void Thandieuhoa() {
	Tao(Translate(0, 0, 0) * Scale(0.9, 4, 10));
	Tao(Translate(1.7, 1.62, 0) * RotateZ(-10) * Scale(2.6, 0.3, 10));

	Tao(Translate(0.9, -1.76, 0) * RotateZ(10) * Scale(1, 0.3, 10));
	//Tao(Translate(2.1, -1.65, 0) * RotateZ(10) * Scale(1.4, 0.1, 10));
	Tao(Translate(2.9, 0, 0) * Scale(0.2, 3.1, 10));

	Tao(Translate(0, 0, 5) * Scale(0.9, 4, 0.2));
	Tao(Translate(1.7, 1.62, 5) * RotateZ(-10) * Scale(2.6, 0.3, 0.2));
	Tao(Translate(1.7, -1.62, 5) * RotateZ(10) * Scale(2.6, 0.3, 0.2));
	Tao(Translate(2.9, 0, 5) * Scale(0.2, 3.1, 0.2));
	Tao(Translate(1.2, 0, 5) * Scale(1.6, 3.4, 0.2));
	Tao(Translate(2.4, 0, 5) * Scale(0.8, 3, 0.2));

	Tao(Translate(0, 0, -5) * Scale(0.9, 4, 0.2));
	Tao(Translate(1.7, 1.62, -5) * RotateZ(-10) * Scale(2.6, 0.3, 0.2));
	Tao(Translate(1.7, -1.62, -5) * RotateZ(10) * Scale(2.6, 0.3, 0.2));
	Tao(Translate(2.9, 0, -5) * Scale(0.2, 3.1, 0.2));
	Tao(Translate(1.2, 0, -5) * Scale(1.6, 3.4, 0.2));
	Tao(Translate(2.4, 0, -5) * Scale(0.8, 3, 0.2));

	/*Tao(Translate(1.5, 0.5, 0) * Scale(1,3,10));
	Tao2(Translate(1.5, -1.5,4.5) * Scale(1,1,1)*RotateZ(-90));
	Tao2(Translate(1.5, -1.5, -4.5) * Scale(1, 1, 1)*RotateX(180));*/
}
void Dieuhoa() {
	control = 1; control2 = 1; control3 = 1;
	control3 = Scale(0.4, 0.4, 0.4) * Translate(-23.5, 8, 10);
	Mau(mauDieuhoa);
	Thandieuhoa();
	control = Translate(1.8, -1.65, 0) * RotateZ(10) * RotateZ(mdieuhoa) * Translate(0.3, -0.05, 0);
	Thanhdieuhoa();
}
GLfloat cuanganda = 0;
void Cuanganda() {
	Mau(mautulanh);
	Tao(Translate(0, 0, 0) * Scale(5.2, 5, 0.2));
	Mau(maukhungtulanh);
	Tao(Translate(0, 2.75, 0) * Scale(5.2, 0.5, 0.2));
	Tao(Translate(0, -2.6, 0) * Scale(5.2, 0.25, 0.2));
	Mau(maukhungtrongtulanh);
	Tao(Translate(2.3, -1.85, -0.5) * RotateY(-90) * Scale(1, 1, 0.05));
	Tao(Translate(-2.3, -1.85, -0.5) * RotateY(-90) * Scale(1, 1, 0.05));
	Tao(Translate(0, -2.35, -0.5) * Scale(4.65, 0.05, 1));
	Tao(Translate(0, -1.85, -1) * Scale(4.65, 1, 0.05));
}
void Cuanganmat() {
	Mau(mautulanh);
	Tao(Translate(0, -7.4, 0) * Scale(5.2, 9, 0.2));
	Mau(maukhungtulanh);
	Tao(Translate(0, -12.15, 0) * Scale(5.2, 0.5, 0.2));
	Tao(Translate(0, -2.8, 0) * Scale(5.2, 0.25, 0.2));
	Mau(maukhungtrongtulanh);
	Tao(Translate(2.3, -1.85 - 9.4, -0.5) * RotateY(-90) * Scale(1, 1, 0.05));
	Tao(Translate(-2.3, -1.85 - 9.4, -0.5) * RotateY(-90) * Scale(1, 1, 0.05));
	Tao(Translate(0, -2.35 - 9.4, -0.5) * Scale(4.65, 0.05, 1));
	Tao(Translate(0, -1.85 - 9.4, -1) * Scale(4.65, 1, 0.05));

	Tao(Translate(2.3, -1.85 - 5, -0.5) * RotateY(-90) * Scale(1, 1, 0.05));
	Tao(Translate(-2.3, -1.85 - 5, -0.5) * RotateY(-90) * Scale(1, 1, 0.05));
	Tao(Translate(0, -2.35 - 5, -0.5) * Scale(4.65, 0.05, 1));
	Tao(Translate(0, -1.85 - 5, -1) * Scale(4.65, 1, 0.05));
}
GLfloat cuanganmat = 0;
void Thantulanh() {
	control = 1;
	//khung ngoai
	Mau(mautulanh);
	Tao(Translate(0, 0, -2.5) * Scale(5.2, 5, 0.2));
	Tao(Translate(2.5, 0, 0) * RotateY(-90) * Scale(5, 5, 0.2));
	Tao(Translate(-2.5, 0, 0) * RotateY(90) * Scale(5, 5, 0.2));
	Tao(Translate(0, -2.6, -0.05) * Scale(5.2, 0.25, 5.1));
	//Tao(Translate(0, 0, -0.05) * Scale(5.2, 0.1, 5.1));
	Mau(maukhungtulanh);
	Tao(Translate(0, 2.75, -0.05) * Scale(5.2, 0.5, 5.1));
	// khung trong
	control = 1;
	Mau(maukhungtrongtulanh);
	Tao(Translate(0, 0, -2.4) * Scale(5.1, 5, 0.01));
	Tao(Translate(2.4, 0, 0) * RotateY(-90) * Scale(5, 5, 0.01));
	Tao(Translate(-2.4, 0, 0) * RotateY(90) * Scale(5, 5, 0.01));
	Tao(Translate(0, -2.45, -0.05) * Scale(4.9, 0.01, 5.1));
	Tao(Translate(0, 2.5, -0.05) * Scale(4.9, 0.01, 5.1));
	Tao(Translate(1.75, 2, -2.15) * Scale(1.3, 1, 0.5));
	Tao(Translate(1.75, 2, -1.85) * RotateZ(35) * Scale(0.1, 0.6, 0.02));

	// thanh
	Mau(mauthanhtrongtulanh);
	Tao(Translate(0, -0.5, -0.05) * Scale(5.1, 0.1, 4.8));
	TaoHinhTru2(Translate(1.75, 2, -1.89) * Scale(0.7, 0.7, 0.05));
	/*Tao(Translate(-2.5, 0, 0) * RotateY(90) * Scale(5, 5, 0.2));
	Tao(Translate(0, -2.65, -0.05) * Scale(5.2, 0.5, 5.1));*/
	control = 1;
	//khung ngoai
	Mau(mautulanh);
	Tao(Translate(0, -7.4, -2.5) * Scale(5.2, 9, 0.2));
	Tao(Translate(2.5, -7.4, 0) * RotateY(-90) * Scale(5, 9, 0.2));
	Tao(Translate(-2.5, -7.4, 0) * RotateY(90) * Scale(5, 9, 0.2));
	Tao(Translate(0, -2.8, -0.05) * Scale(5.2, 0.25, 5.1));
	//
	Mau(maukhungtulanh);
	Tao(Translate(0, -12.15, -0.05) * Scale(5.2, 0.5, 5.1));
	Tao(Translate(0, -12.65, 0.05) * Scale(5.2, 0.5, 5.3));
	//
	Mau(maukhungtrongtulanh);
	Tao(Translate(0, -7.4, -2.4) * Scale(5.1, 9, 0.01));
	Tao(Translate(2.4, -7.4, 0) * RotateY(-90) * Scale(5, 9, 0.01));
	Tao(Translate(-2.4, -7.4, 0) * RotateY(90) * Scale(5, 9, 0.01));
	Tao(Translate(0, -11.9, -0.05) * Scale(4.9, 0.01, 5.1));
	Tao(Translate(0, -2.93, -0.05) * Scale(4.9, 0.01, 5.1));
	//

	Tao(Translate(1.75, -3.5, -2.15) * Scale(1.3, 1, 0.5));
	Tao(Translate(1.75, -3.5, -1.85) * RotateZ(35) * Scale(0.1, 0.6, 0.02));

	//
	Mau(mauthanhtrongtulanh);
	Tao(Translate(0, -9, -0.05) * Scale(5.1, 0.1, 4.8));
	Tao(Translate(0, -6, -0.05) * Scale(5.1, 0.1, 4.8));
	TaoHinhTru2(Translate(1.75, -3.5, -1.89) * Scale(0.7, 0.7, 0.05));

}
void Tulanh() {
	control = 1; control2 = 1; control3 = 1;
	control3 = Translate(-8.5, -0.5, 0) * RotateY(90) * Scale(0.3, 0.3, 0.3);
	Thantulanh();
	control = 1; control2 = 1;
	control = Translate(2.5, 0, 2.6) * RotateY(cuanganmat) * Translate(-2.5, 0, 0);
	Cuanganmat();
	control = 1; control2 = 1;
	control2 = Translate(2.5, 0, 2.6) * RotateY(cuanganda) * Translate(-2.5, 0, 0);
	Cuanganda();

}
//Cường}

//{Nam
void ThanTuGiay() {

	Mau(vec4(0.439, 0.502, 0.565, 1));//màu xám
	Tao(Translate(0, 4.2, 0) * Scale(10, 0.5, 3.4));//đệm trên
	Mau(vec4(0.804, 0.522, 0.247, 1));//màu gỗ
	Tao(Translate(0, 1.85, -1.6) * Scale(9.6, 4.2, 0.2));// mặt gỗ sau

	Mau(vec4(0.933, 0.91, 0.667, 1));//màu cạnh
	Tao(Translate(-1.4, 3.35, 1.55) * Scale(6.8, 0.8, 0.2));// thanh chắn trên
	Tao(Translate(-4.9, 1.85, 0.1) * Scale(0.2, 3.8, 3.2));// mặt trái
	Tao(Translate(4.9, 1.85, 0.1) * Scale(0.2, 3.8, 3.2));// mặt phải
	//bổ sung 2 mặt trái phải
	Tao(Translate(4.9, 1.85, -1.6) * Scale(0.2, 4.2, 0.2));
	Tao(Translate(-4.9, 1.85, -1.6) * Scale(0.2, 4.2, 0.2));

	Tao(Translate(0, 3.85, 0.1) * Scale(10, 0.2, 3.2));// mặt gỗ trên
	Tao(Translate(0, -0.15, 0.1) * Scale(10, 0.2, 3.2));// mặt gỗ dưới
	Tao(Translate(2.1, 1.85, 0.1) * Scale(0.2, 3.8, 3.2));// thanh ngăn giữa
	Tao(Translate(3.514, 1.85, 0.1) * Scale(2.8, 0.2, 3.2));// ngăn tủ




}
GLfloat aTuGiay = -90;
void NganTuGiay() {

	Mau(vec4(0.804, 0.522, 0.247, 1));//màu gỗ
	Tao(Translate(-1.4, 1.45, 1.55) * Translate(0, -1.5, 0) * RotateX(aTuGiay + 0) * Translate(0, 1.5, 0) * Scale(6.8, 3, 0.2));// thanh ngăn 30 độ
	Tao(Translate(-1.4, 1.45, 1.55) * Translate(0, -1.5, 0) * RotateX(aTuGiay + 30) * Translate(0, 1.5, 0) * Scale(6.8, 3, 0.2));// thanh ngăn 30 độ
	Tao(Translate(-1.4, 1.45, 1.55) * Translate(0, -1.5, 0) * RotateX(aTuGiay + 60) * Translate(0, 1.5, 0) * Scale(6.8, 3, 0.2));// thanh ngăn 60 độ
	Tao(Translate(-1.4, 1.45, 1.55) * Translate(0, -1.5, 0) * RotateX(aTuGiay + 90) * Translate(0, 1.5, 0) * Scale(6.8, 3, 0.2));// thanh ngăn 90 độ
	Mau(vec4(0.933, 0.91, 0.667, 1));//màu cạnh
	Tao(Translate(-1.4, 1.45, 1.55) * Translate(0, -1.5, 0) * RotateX(aTuGiay + 95) * Translate(0, 1.5, 0) * Scale(0.8, 0.2, 0.2));// tay cầm
	Mau(vec4(0.804, 0.522, 0.247, 1));//màu gỗ
	//các thanh 2 bên
	///bên trái
	Tao(Translate(-4.75, 2.58, 2.254) * Translate(0, -2.63, -0.704) * RotateX(aTuGiay + 0) * Translate(0, 2.63, 0.704) * RotateX(15.5) * Scale(0.1, 0.4, 1.37));
	Tao(Translate(-4.75, 1.879, 3.47) * Translate(0, -1.929, -1.92) * RotateX(aTuGiay + 0) * Translate(0, 1.929, 1.92) * RotateX(45.5) * Scale(0.1, 0.4, 1.37));
	Tao(Translate(-4.75, 0.656, 4.179) * Translate(0, -0.706, -2.629) * RotateX(aTuGiay + 0) * Translate(0, 0.706, 2.629) * RotateX(75.5) * Scale(0.1, 0.4, 1.37));
	///bên phải


	Tao(Translate(1.95, 2.58, 2.254) * Translate(0, -2.63, -0.704) * RotateX(aTuGiay + 0) * Translate(0, 2.63, 0.704) * RotateX(15.5) * Scale(0.1, 0.4, 1.37));
	Tao(Translate(1.95, 1.879, 3.47) * Translate(0, -1.929, -1.92) * RotateX(aTuGiay + 0) * Translate(0, 1.929, 1.92) * RotateX(45.5) * Scale(0.1, 0.4, 1.37));
	Tao(Translate(1.95, 0.656, 4.179) * Translate(0, -0.706, -2.629) * RotateX(aTuGiay + 0) * Translate(0, 0.706, 2.629) * RotateX(75.5) * Scale(0.1, 0.4, 1.37));


}
GLfloat bTuGiay = -90, xTugiay = 17, yTugiay = -8.5, zTugiay = 20;
void TuGiay() {
	control = Scale(0.5, 0.5, 0.5) * Translate(xTugiay, yTugiay, zTugiay) * RotateY(bTuGiay); control3 = 1; control2 = 1;
	if (aTuGiay >= -20) aTuGiay = -20;
	if (aTuGiay <= -90) aTuGiay = -90;
	ThanTuGiay();



	NganTuGiay();
}


//Đồng hồ
void Top_12h() {
	Tao(Translate(-0.03, 0.2, -0.04) * Scale(0.01, 0.05, 0.01));
	Tao(Translate(-0.03, 0.2, -0.02) * Scale(0.01, 0.05, 0.01));
	Tao(Translate(-0.03, 0.2, +0.02) * RotateX(45) * Scale(0.01, 0.06, 0.01));
	Tao(Translate(-0.03, 0.2, +0.02) * RotateX(-45) * Scale(0.01, 0.06, 0.01));
}
void Bot_6h() {
	Tao(Translate(-0.03, -0.2, -0.03) * Scale(0.01, 0.05, 0.01));
	Tao(Translate(-0.03, -0.2, 0) * RotateX(-30) * Scale(0.01, 0.05, 0.01));
	Tao(Translate(-0.03, -0.2, 0.02) * RotateX(30) * Scale(0.01, 0.05, 0.01));
}
void Left_9h() {
	Tao(Translate(-0.03, 0, 0.22) * Scale(0.01, 0.05, 0.01));
	Tao(Translate(-0.03, 0, 0.18) * RotateX(45) * Scale(0.01, 0.06, 0.01));
	Tao(Translate(-0.03, 0, 0.18) * RotateX(-45) * Scale(0.01, 0.06, 0.01));
}
void Right_3h() {
	Tao(Translate(-0.03, 0, -0.22) * Scale(0.01, 0.05, 0.01));
	Tao(Translate(-0.03, 0, -0.2) * Scale(0.01, 0.05, 0.01));
	Tao(Translate(-0.03, 0, -0.18) * Scale(0.01, 0.05, 0.01));
}
void KimGio() {
	Mau(mauSoGio);
	Tao(Translate(-0.03, 0.06, 0) * Scale(0.01, 0.12, 0.01));
}
void KimPhut() {
	Mau(mauSoGio);
	Tao(Translate(-0.03, 0.085, 0) * Scale(0.01, 0.155, 0.01));
}
void KimGiay() {
	Mau(mauKimGiay);
	Tao(Translate(-0.03, 0.075, 0) * Scale(0.01, 0.16, 0.01));
}

void MatDongHo() {
	Mau(mauSoGio);
	Top_12h();
	Bot_6h();
	Left_9h();
	Right_3h();

	Mau(mauMatDongHo);
	TaoHinhTru2(RotateY(90) * Translate(0, 0, -0.06) * Scale(0.5, 0.5, 0.05));
}
GLfloat kimGiayQuay = 0;
GLfloat kimPhutQuay = 0;
GLfloat kimGioQuay = 0;

#include<time.h>
void DongHo() {
	control = Translate(19, 0, -5) * Scale(3, 3, 3) * RotateY(180); control2 = 1; control3 = 1;
	control2 = Translate(-9.6, 2, 0);
	MatDongHo();

	time_t now = time(0);
	tm* timeInfor = localtime(&now);
	int hour = timeInfor->tm_hour;
	int min = timeInfor->tm_min;
	int sec = timeInfor->tm_sec;

	mat4 tmp = control;
	kimGiayQuay = 360.0 * sec / 60;
	kimPhutQuay = 360.0 * min / 60 + kimGiayQuay / 60;
	kimGioQuay = 360.0 * hour / 12 + kimPhutQuay / 60;
	control = tmp * RotateX(-kimGiayQuay);
	KimGiay();
	control = tmp * RotateX(-kimPhutQuay);
	KimPhut();
	control = tmp * RotateX(-kimGioQuay);
	KimGio();
}

//Nam}
// CODE TẠO HÌNH Ở TRÊN NÀY 👆	
// Hiển thị
bool hienthitham = true;
bool hienthibinhhoa = true;


void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection_view();


	control = 1; control2 = 1; control3 = 1;
	Ghe();
	Ghengan();
	Ghengan2();
	Nha();
	Mainha();
	Quatdung();
	Bocua();
	Cauthang();
	BoCuaso();
	Quattran();
	tivi();
	Bongden();
	Wifi();
	BoLoa();
	
	KeBinhHoa();
	GheSoFaDem();
	vitrichu();
	tutivi();
	Mayloc();
	Ban();
	Dieuhoa();
	Tulanh();
	TuGiay();
	DongHo();
	if (hienthitham) {
		
		ThamTraiNha();
	}
	else {
	}

	if (hienthibinhhoa) {
	binhHoa();

	}
	else {
	}

	glutSwapBuffers();
}



GLfloat targetSpQt = 0; // Tốc độ mục tiêu quạt trần
GLfloat currentSpQt = 0;   // Tốc độ hiện tại quạt trần

void spinQuattran() {
	// Điều chỉnh tốc độ hiện tại y dần dần tiến tới tốc độ mục tiêu spd
	if (currentSpQt < targetSpQt) {
		currentSpQt += 0.1; // Tăng tốc độ
	}
	else if (currentSpQt > targetSpQt) {
		currentSpQt -= 0.01; // Giảm tốc độ
		if (currentSpQt < 0) { // không cho y không giảm xuống dưới 0, cho tắt hẳn
			currentSpQt = 0;
		}
	}

	// Cập nhật góc quay của cánh quạt theo tốc độ hiện tại
	bCanhquattran += currentSpQt;

	glutPostRedisplay();
}

GLfloat direction = 1;//biến tạm dùng để điều hướng
bool batDaochieu = false;
void spinKhungQuatdung() {
	if (batDaochieu) {
		bKhungquat += direction * 0.5;

		// nếu quá 90 độ hoặc -90 độ thì nhân -1 để đảo chiều
		if (bKhungquat >= 90 || bKhungquat <= -90) {
			direction *= -1;
		}


	}

	glutPostRedisplay();
}
GLfloat targetSpQd = 0; // Tốc độ mục tiêu quạt đứng
GLfloat currentSpQd = 0;   // Tốc độ hiện tại quạt đứng
void spinCanhquatdung() {
	//code số 1 2 3

	if (currentSpQd < targetSpQd) {
		currentSpQd += 1;
	}
	else if (currentSpQd > targetSpQd) {
		currentSpQd -= 0.1;
		if (currentSpQd < 0) {
			currentSpQd = 0;
		}
	}
	cCanhQuat += currentSpQd;
	glutPostRedisplay();
}

GLfloat spd1 = 0.0;
void spin3() {
	if (spd1 > 0) {
		mdieuhoa -= 0.2;
		if (mdieuhoa <= -70) mdieuhoa = -70;
	}
	if (spd1 <= 0) {
		mdieuhoa += 0.2;
		if (mdieuhoa >= 0) mdieuhoa = 0;
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033:
		exit(1);
		break;
		// CÁC ĐIỀU KHIỂN QUAN TRỌNG
	case '[':
		moveForward();//phong to
		break;
	case ']':
		moveBackward();//thu nhỏ
		break;
	case 'a':
		moveLeft(); // tịnh tiến camera sang trái
		break;
	case 'd':
		moveRight();// tịnh tiến camera sang phải
		break;
	case 'w':
		moveUp();// tịnh tiến camera lên trên
		break;
	case 's':
		moveDown();// tịnh tiến camera xuống dưới
		break;

		// CODE THÊM BÀN PHÍM Ở DƯỚI NÀY 👇
	case't':bCanhT -= 10;
		if (bCanhT <= -175) bCanhT = -175;
		break;
	case'T':bCanhT += 10;
		if (bCanhT >= 0) bCanhT = 0;
		break;
	case 'p':bCanhP += 10;
		if (bCanhP >= 175) bCanhP = 175;
		break;
	case 'P': bCanhP -= 10;
		if (bCanhP <= 0) bCanhP = 0;

	case ',':
		sf *= 0.9;

		break;
	case '.':
		sf *= 1.1;
	case 'c':
		if (gtYcuaso > 0.4) {
			yCuaso += 0.1;
			//yCuaso += 0.2;
			gtYcuaso -= 0.2;
			//yCuaso -= 0.1;
			ytaycam += 0.2;
		}

		if (gtYcuaso <= 0.3) gtYcuaso = 0.4;
		break;
	case 'C':
		if (gtYcuaso < 4) {
			yCuaso -= 0.1;
			//yCuaso -= 0.2;
			gtYcuaso += 0.2;
			//yCuaso += 0.1;
			ytaycam -= 0.2;
		}

		if (gtYcuaso >= 4) gtYcuaso = 4;
		break;
		// bật quạt trần
	case '0': // Tắt quạt
		targetSpQt = 0;
		break;
	case '1': // số 1
		targetSpQt = 10;
		break;
	case '2': // số 2
		targetSpQt = 20;
		break;
	case '3': // số 3
		targetSpQt = 30;
		break;

		// bật quạt đứng    
	case 'b':batDaochieu = !batDaochieu; break;//bật auto đảo chiều
	case ')': // Tắt quạt
		targetSpQd = 0;
		break;
	case '!': // số 1
		targetSpQd = 15; direction = 1;
		break;
	case '@': // số 2
		targetSpQd = 25; direction = 1.25f;
		break;
	case '#': // số 3
		targetSpQd = 35; direction = 1.5;
		break;
	case 'q':bKhungquat -= 10; break;//đảo sang phải
	case 'Q':bKhungquat += 10; break;//đảo sang trái

		//bật tivi
	case 'B': tvOn = !tvOn; break;

		//Wifi
	case'5':cRouterT += 10; break;
	case'%':cRouterT -= 10; break;
	case'6':cRouterP += 10; break;
	case'^':cRouterP -= 10; break;
		//bật tắt bóng đèn
	case'f':
		batBongDen = !batBongDen;
		updateLighting();
		break;

		//bong den



	case'k':aTuGiay += 2;break;
	case'K':aTuGiay -= 2; break;

	case'l':acanhtu1 += 10;
		if (acanhtu1 >= 60) acanhtu1 = 60;
		break;
	case'L':acanhtu1 -= 10;
		if (acanhtu1 <= 0) acanhtu1 = 0;
		break;
	case'o':bcanhtu2 -= 10;
		if (bcanhtu2 <= -60) bcanhtu2 = -60;
		break;
	case'O':bcanhtu2 += 10;
		if (bcanhtu2 >= 0) bcanhtu2 = 0;
		break;
		
	case 'h':
		glutIdleFunc(spin3);
		spd1 = 4;
		break;
	case 'H':
		glutIdleFunc(spin3);
		spd1 = 0;
		break;
	case 'n':cuanganda += 5;
		if (cuanganda >= 145) cuanganda = 145;
		break;
	case 'N':cuanganda -= 5;
		if (cuanganda <= 0) cuanganda = 0;
		break;
	case 'm':cuanganmat += 5;
		if (cuanganmat >= 145) cuanganmat = 145;
		break;
	case 'M':cuanganmat -= 5;
		if (cuanganmat <= 0) cuanganmat = 0;
		break;
	case 'v': 
		bvannuoc += 10;
		if (bvannuoc > 60)
			bvannuoc = 60;
		break;
	case 'V':
		bvannuoc -= 10;
		if (bvannuoc <= 0)
			bvannuoc = 0;
		break;

	case'y':xGoi1 += 0.1; break;
	case'u':yGoi1 += 0.1; break;
	case'i':zGoi1 += 0.1; break;
	case'j':cGoi1 += 10; break;
	case'J':cGoi1 -= 10; break;
	case'Y':xGoi1 -= 0.1; break;
	case'U':yGoi1 -= 0.1; break;
	case'I':zGoi1 -= 0.1; break;

	case'g':xGoi2 += 0.1; break;
	case'r':yGoi2 += 0.1; break;
	case'e':zGoi2 += 0.1; break;
	case'z':cGoi2 += 10; break;
	case'Z':cGoi2 -= 10; break;
	case'G':xGoi2 -= 0.1; break;
	case'R':yGoi2 -= 0.1; break;
	case'E':zGoi2 -= 0.1; break;
	case'9':hienthitham = !hienthitham; break;
	case'(':hienthibinhhoa = !hienthibinhhoa; break;
	case'8':hienthikinhban = !hienthikinhban; break;

	


		// CODE THÊM BÀN PHÍM Ở TRÊN NÀY 👆
	}
	glutPostRedisplay();
}
void CallSpin() {
	spinCanhquatdung();
	spinKhungQuatdung();
	spinQuattran();
	spin3();
}

void specialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT: // Xoay camera sang trái (camera chỉ xoay chứ không tịnh tiến)
		rotateCameraHorizontally(5.0f);
		break;
	case GLUT_KEY_RIGHT: // Xoay camera sang phải
		rotateCameraHorizontally(-5.0f);
		break;
	case GLUT_KEY_UP: // Xoay camera lên trên
		rotateCameraVertically(5.0f);
		break;
	case GLUT_KEY_DOWN: // Xoay camera xuống dưới
		rotateCameraVertically(-5.0f);
		break;
	}
	glutPostRedisplay();
}



int main(int argc, char** argv)
{
	// main function: program starts here

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Drawing a Cube");


	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(CallSpin);

	glutSpecialFunc(specialKeys);



	glutMainLoop();
	return 0;
}
