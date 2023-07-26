#pragma once



#include"rtweekend.h"
#include"vec3.h"
#include <vector>
#include <map>
#include <string> 
#include <iostream>
#include <fstream>
using namespace std;

//三维空间中构成一个三角面的三个点序列，逆时针排列
struct my_triangle_indices
{
	int first_point_index; //第一个点序号
	int first_point_texture_index; //第一个纹理坐标序号
	int first_point_normal_index; //第一个点法向序号
	int second_point_index; //第二个点序号
	int second_point_texture_index; //第二个纹理坐标序号
	int second_point_normal_index; //第二个法向序号
	int third_point_index; //第三个点序号
	int third_point_texture_index; //第三个纹理坐标序号
	int third_point_normal_index; //第三个法向序号
};
//三维空间中的三角网格模型
struct my_triangle_3DModel
{
	float transparency = 0;
	float reflection = 0;
	//以下用于计算直接光照能量
	float material_ambient_rgb_reflection[3] = { 0,0,0 }; //环境光反射系数，初始不反射
	float material_diffuse_rgb_reflection[3] = { 0,0,0 };//漫反射系数，初始不反射
	float material_specular_rgb_reflection[3] = { 0,0,0 };//镜面光反射系数，初始不反射
	float ns = 0;//聚光指数，初始不聚光
	vector<point3> pointSets; //存放模型所有顶点
	vector<vec3> pointNormalSets; //存放模型所有顶点的法向
	vector<glm::vec2>pointTextureSets;
	vector<my_triangle_indices> faceSets; //存放模型所有三角网格面
	//模型尺寸
	float max_x = -1e8, min_x = 1e8;
	float max_y = -1e8, min_y = 1e8;
	float max_z = -1e8, min_z = 1e8;
	void modify_color_configuration(float transparency, float reflection, float
		ambient_reflection[], float diffuse_reflection[], float specular_reflection[], float ns)
	{
		this->transparency = transparency;
		this->reflection = reflection;
		//以下用于计算直接光照能量 memcpy用于把资源内存拷到目标内存中
		memcpy(this->material_ambient_rgb_reflection, ambient_reflection, 3 * sizeof(float));
		memcpy(this->material_diffuse_rgb_reflection, diffuse_reflection, 3 * sizeof(float));
		memcpy(this->material_specular_rgb_reflection, specular_reflection, 3 * sizeof(float));
		this->ns = ns;
	}
};
//模型加载器
class my_objloader
{
public:
	my_triangle_3DModel my_3DModel;
public:
	my_objloader() {}
	my_objloader(string filename) //构造函数
	{
		string line;
		fstream f;
		f.open(filename, ios::in);
		if (!f.is_open()) {
			cout << "Something Went Wrong When Opening Objfiles" << endl;
		}
		while (!f.eof())
		{
			getline(f, line);//拿到obj文件中一行，作为一个字符串
			if (line.find("#") != -1)
				continue;
			line.append(" ");
			vector<string> parameters;
			string ans = "";
			for (unsigned int i = 0; i < line.length(); i++)
			{
				char ch = line[i];
				if (ch != ' ')
				{
					ans += ch;
				}
				else if (ans != "")
				{
					parameters.push_back(ans); //取出字符串中的元素，以空格切分
					ans = "";
				}
			}
			if (parameters.size() == 4 || parameters.size() == 3)
			{
				if (parameters[0] == "v") //顶点,从1开始，将顶点的xyz三个坐标放入顶点vector
				{
					point3 curPoint;
					curPoint.e[0] = atof(parameters[1].c_str());
					my_3DModel.max_x = my_3DModel.max_x < curPoint.e[0] ? curPoint.e[0] :my_3DModel.max_x;
					my_3DModel.min_x = my_3DModel.min_x > curPoint.e[0] ? curPoint.e[0] :my_3DModel.min_x;

					curPoint.e[1] = atof(parameters[2].c_str());
					my_3DModel.max_y = my_3DModel.max_y < curPoint.e[1] ? curPoint.e[1] :my_3DModel.max_y;
					my_3DModel.min_y = my_3DModel.min_y > curPoint.e[1] ? curPoint.e[1] :my_3DModel.min_y;

					curPoint.e[2] = atof(parameters[3].c_str());
					my_3DModel.max_z = my_3DModel.max_z < curPoint.e[2] ? curPoint.e[2] :my_3DModel.max_z;
					my_3DModel.min_z = my_3DModel.min_z > curPoint.e[2] ? curPoint.e[2] :my_3DModel.min_z;
					my_3DModel.pointSets.push_back(curPoint);
				}
				else if (parameters[0] == "vn") //顶点的法向量
				{
					vec3 curPointNormal;
					curPointNormal.e[0] = atof(parameters[1].c_str());
					curPointNormal.e[1] = atof(parameters[2].c_str());
					curPointNormal.e[2] = atof(parameters[3].c_str());
					my_3DModel.pointNormalSets.push_back(curPointNormal);
				}
				else if (parameters[0] == "vt")
				{
					glm::vec2 curTextureCoord;
					curTextureCoord[0] = atof(parameters[1].c_str());
					curTextureCoord[1] = atof(parameters[2].c_str());
					my_3DModel.pointTextureSets.push_back(curTextureCoord);
				}
				else if (parameters[0] == "f") //面， 顶点索引/纹理uv点索引/法向索引
				{
					//因为顶点索引在obj文件中是从1开始的，而我们存放的顶点vector是从0开始的，因此要减1
					my_triangle_indices curTri;
					curTri.first_point_index = atoi(parameters[1].substr(0, parameters[1].find_first_of('/')).c_str()) - 1;
					parameters[1] = parameters[1].substr(parameters[1].find_first_of('/') + 1);
					curTri.first_point_texture_index = atoi(parameters[1].substr(0,parameters[1].find_first_of('/')).c_str()) - 1;
					parameters[1] = parameters[1].substr(parameters[1].find_first_of('/') + 1);
					curTri.first_point_normal_index = atoi(parameters[1].substr(0,parameters[1].find_first_of('/')).c_str()) - 1;

					curTri.second_point_index = atoi(parameters[2].substr(0,parameters[2].find_first_of('/')).c_str()) - 1;
					parameters[2] = parameters[2].substr(parameters[2].find_first_of('/') + 1);
					curTri.second_point_texture_index = atoi(parameters[2].substr(0,parameters[2].find_first_of('/')).c_str()) - 1;
					parameters[2] = parameters[2].substr(parameters[2].find_first_of('/') + 1);
					curTri.second_point_normal_index = atoi(parameters[2].substr(0,parameters[2].find_first_of('/')).c_str()) - 1;

					curTri.third_point_index = atoi(parameters[3].substr(0,parameters[3].find_first_of('/')).c_str()) - 1;
					parameters[3] = parameters[3].substr(parameters[3].find_first_of('/') + 1);
					curTri.third_point_texture_index = atoi(parameters[3].substr(0,parameters[3].find_first_of('/')).c_str()) - 1;
					parameters[3] = parameters[3].substr(parameters[3].find_first_of('/') + 1);
					curTri.third_point_normal_index = atoi(parameters[3].substr(0,parameters[3].find_first_of('/')).c_str()) - 1;
					my_3DModel.faceSets.push_back(curTri);
				}
			}
		}
		f.close();
	}
};
