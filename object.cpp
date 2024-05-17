#define _CRT_SECURE_NO_WARNINGS

#include "object.h"
#include <fstream>
#include <glut.h>

vec3 vert(std::ifstream& file);
vec3 vert_n(std::ifstream&);
vec3 vert_t(std::ifstream& file);
face ext_face(std::ifstream& file);

void object::loadmtl(char* mtl_filename)
{
	log("File loading -> ");
	log(mtl_filename);
	log("\n");

	if (strlen(mtl_filename) <= 3) {
		log("\t");
		log(mtl_filename);
		log(" -> File name too short !\n");
		log("Load failed! -> ");
		log(mtl_filename);
		log("\n");
		return;
	}
	if (mtl_filename[strlen(mtl_filename) - 0] == 'l'
		|| mtl_filename[strlen(mtl_filename) - 1] == 't'
		|| mtl_filename[strlen(mtl_filename) - 2] == 'm') {
		log("\t");
		log(mtl_filename);
		log(" -> material not supported !\n");
		log("Load failed! -> ");
		log(mtl_filename);
		log("\n");
		return;
	}

	std::ifstream file(mtl_filename);
	if (!file.is_open()) {
		log("\t");
		log(mtl_filename);
		log(" -> File not found !\n");
		log("Load failed! -> ");
		log(mtl_filename);
		log("\n");
		return;
	}

	char* nbuf = new char[1000];

	for (std::string ch; !file.eof(); file >> ch) {

		if (ch == "newmtl") {
			file.getline(nbuf, 100);
			obj[obj_count].m << material();
			obj[obj_count].m[obj[obj_count].m.size() - 1].mat_name = new char[strlen(nbuf) + 1];
			strcpy(obj[obj_count].m[obj[obj_count].m.size() - 1].mat_name, nbuf);
			nbuf = new char[1000];
			continue;
		}

		switch (ch[0]) {
		case 'N':
			switch (ch[1])
			{
			case 's':
				file >> obj[obj_count].m[obj[obj_count].m.size() - 1].Ns;
				break;
			case 'i':
				file >> obj[obj_count].m[obj[obj_count].m.size() - 1].Ni;
				break;
			default:
				file.getline(nbuf, 1024);
				break;
			}
			break;
		case 'K':
			switch (ch[1])
			{
			case 'a':
				file >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ka.r >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ka.g >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ka.b;
				break;
			case 'd':
				file >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Kd.r >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Kd.g >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Kd.b;
				break;
			case 's':
				file >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ks.r >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ks.g >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ks.b;
				break;
			case 'e':
				file >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ke.r >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ke.g >>
					obj[obj_count].m[obj[obj_count].m.size() - 1].Ke.b;
				break;
			default:
				file.getline(nbuf, 1024);
				break;
			}
			break;
		case 'd':
			file >> obj[obj_count].m[obj[obj_count].m.size() - 1].d;
			break;
		default:
			file.getline(nbuf, 1024);
			break;
		}
	}

	file.close();

	obj[obj_count].mat_found = true;

	log("\t -> materials : ");
	if (obj[obj_count].m.size() != 0)log(obj[obj_count].m.size());
	log("\n");

	log("Load success -> ");
	log(mtl_filename);
	log("\n");
}

void object::addObject(const char* filename, float scale)
{
	log("File loading -> ");
	log(filename);
	log("\n");

	if (strlen(filename) <= 3) {
		log("\t");
		log(filename);
		log(" -> File name too short !\n");
		log("Load failed! -> ");
		log(filename);
		log("\n");
		return;
	}
	if (filename[strlen(filename) - 0] == 'j'
		|| filename[strlen(filename) - 1] == 'b'
		|| filename[strlen(filename) - 2] == 'o') {
		log("\t");
		log(filename);
		log(" -> File not supported !\n");
		log("Load failed! -> ");
		log(filename);
		log("\n");
		return;
	}

	std::ifstream file(filename);
	if (!file.is_open()) {
		log("\t");
		log(filename);
		log(" -> File not found !\n");
		log("Load failed! -> ");
		log(filename);
		log("\n");
		return;
	}

	// To normalize their positions in faces !
	obj += 1;
	obj[obj_count].v += 1;
	obj[obj_count].vt += 1;
	obj[obj_count].vn += 1;

	char* nbuf = new char[1000];

	int mat_cur = 0;

	for (std::string ch; !file.eof(); file >> ch) {
		if (ch == "usemtl") {
			file.getline(nbuf, 100);
			for (unsigned long i = 0; i < obj[obj_count].m.size(); i++) {
				if (!strcmp(obj[obj_count].m[i].mat_name, nbuf))mat_cur = i;
			}
			nbuf = new char[1000];
			continue;
		}
		if (ch == "mtllib") {
			file.seekg(-1, std::ios::cur);
			file.getline(nbuf, 100);
			size_t l = strlen(filename);
			char ch = ' ';
			for (; l != 0; l--) {
				ch = filename[l];
				if (ch == '/')break;
			}
			mtllib = new char[100];

			if (l != 0) {
				for (int i = 0; i < l; i++)mtllib[i] = filename[i];
				mtllib[l] = '/';
				mtllib[l + 1] = '\0';
				strcat(mtllib, nbuf);
			}
			else strcpy(mtllib, nbuf);

			loadmtl(mtllib);
			continue;
		}
		switch (ch[0]) {
		case 'f':
			obj[obj_count].f << ext_face(file);
			if (mat_cur != 0) {
				obj[obj_count].f[obj[obj_count].f.size() - 1].mat_no = mat_cur;
			}
			break;
		case 'v':
			switch (ch.length())
			{
			case 1:
				obj[obj_count].v << vert(file);
				break;
			case 2:
				switch (ch[1])
				{
				case 't':
					obj[obj_count].vt << vert_t(file);
					break;
				case 'n':
					obj[obj_count].vn << vert_n(file);
					break;
				default:
					break;
				}
			default:
				break;
			}
		default:
			file.getline(nbuf, 1024);
			break;
		}
	}

	file.close();

	obj[obj_count].scaleTo(scale);

	// obj[obj_count].centralize();

	log("\t -> vertices : ");
	if (obj[obj_count].v.size() != 1)log(obj[obj_count].v.size() - 1);
	log("\n");

	log("\t -> textures : ");
	if (obj[obj_count].vt.size() != 1)log(obj[obj_count].vt.size() - 1);
	log("\n");

	log("\t -> normals : ");
	if (obj[obj_count].vn.size() != 1)log(obj[obj_count].vn.size() - 1);
	log("\n");

	log("\t -> faces : ");
	if (obj[obj_count].f.size() != 0)log(obj[obj_count].f.size());
	log("\n");

	log("Load success -> ");
	log(filename);
	log("\n");

	obj_count++;
}

unsigned long object::count()
{
	return obj_count;
}

void object::setCurrent(unsigned long cur) {
	if (cur < obj_count)obj_current = cur;
}

unsigned long object::getCurrent()
{
	return obj_current;
}

object::object()
{
	time_t now = time(0);
	char* date = ctime(&now);
	log("\n");
	log(date);
	log("--------------------------------------------------\n");
}

object::~object()
{
	log("--------------------------------------------------\n");
}

template <typename T>
void object::log(T _log) {
	std::ofstream file("object.log", std::ios::app);

	if (_log == 0) {
		file.close();
		file.open("object.log", std::ios::trunc);
	}
	else file << _log;

	file.close();
}

vec3 vert(std::ifstream& file) {
	vec3 v = vec3();
	file >> v.x >> v.y >> v.z;
	return v;
}

vec3 vert_n(std::ifstream& file) {
	vec3 vn = vec3();
	file >> vn.x >> vn.y >> vn.z;
	return vn;
}

vec3 vert_t(std::ifstream& file) {
	vec3 vt = vec3();
	file >> vt.x >> vt.y;
	return vt;
}

face ext_face(std::ifstream& file) {
	face f;

	char line[1024];
	file.getline(line, 1024);
	std::string str = line;

	for (unsigned int i = 0; i < str.length(); i++) {
		switch (str[i])
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			f.n[f.n.size() - 1][f.n[f.n.size() - 1].size() - 1] *= 10;
			f.n[f.n.size() - 1][f.n[f.n.size() - 1].size() - 1] += ((int)str[i]) - 48;
			break;
		case '/':
			f.n[f.n.size() - 1] << int();
			break;
		case ' ':
			f.n << shl::vector<int>();
			f.n[f.n.size() - 1] << int();
			break;
		}
	}

	return f;
}

void wavefront::centralize() {
	vec3 min = vec3(), max = vec3();

	for (unsigned int i = 0; i < v.size(); i++) {
		if (v[i].x < min.x)min.x = v[i].x;
		if (v[i].x > max.x)max.x = v[i].x;
		if (v[i].z < min.x)min.z = v[i].z;
		if (v[i].z > max.x)max.z = v[i].z;
	}

	vec3 diff = vec3();
	diff.x = (max.x + min.x) / 2.0f;
	diff.z = (max.z + min.z) / 2.0f;

	for (unsigned int i = 0; i < v.size(); i++) {
		v[i].x -= diff.x;
		v[i].z += diff.z;
	}
}

void wavefront::scaleTo(float scale) {
	float min = 0, max = 0;

	for (unsigned int i = 0; i < v.size(); i++) {
		if (v[i].x < min)min = v[i].x;
		if (v[i].x > max)max = v[i].x;
		if (v[i].y < min)min = v[i].y;
		if (v[i].y > max)max = v[i].y;
		if (v[i].z < min)min = v[i].z;
		if (v[i].z > max)max = v[i].z;
	}

	float multiplier = ((max - min) / 2.0f) / scale;

	for (unsigned int i = 0; i < v.size(); i++) {
		v[i].x /= multiplier;
		v[i].y /= multiplier;
		v[i].z /= multiplier;
	}
}



void object::renderObject() {

	for (unsigned long j = 0; j < obj[obj_current].f.size(); j++) {

		if (!obj[obj_current].wireframe && obj[obj_current].mat_found) {
			glColor4f(
				obj[obj_current].m[obj[obj_current].f[j].mat_no].Kd.r,
				obj[obj_current].m[obj[obj_current].f[j].mat_no].Kd.g,
				obj[obj_current].m[obj[obj_current].f[j].mat_no].Kd.b,
				obj[obj_current].m[obj[obj_current].f[j].mat_no].d
			);
			glBegin(GL_POLYGON);
		}
		else {
			glBegin(GL_LINE_LOOP);
			glColor3f(0, 0, 1);
		}
		{
			for (unsigned long k = 0; k < obj[obj_current].f[j].n.size(); k++) {
				glVertex3f(
					obj[obj_current].v[obj[obj_current].f[j].n[k][0]].x,
					obj[obj_current].v[obj[obj_current].f[j].n[k][0]].y,
					obj[obj_current].v[obj[obj_current].f[j].n[k][0]].z
				);
			}
		}glEnd();
	}
}