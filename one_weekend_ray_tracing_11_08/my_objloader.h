#pragma once



#include"rtweekend.h"
#include"vec3.h"
#include <vector>
#include <map>
#include <string> 
#include <iostream>
#include <fstream>
using namespace std;

//��ά�ռ��й���һ������������������У���ʱ������
struct my_triangle_indices
{
	int first_point_index; //��һ�������
	int first_point_texture_index; //��һ�������������
	int first_point_normal_index; //��һ���㷨�����
	int second_point_index; //�ڶ��������
	int second_point_texture_index; //�ڶ��������������
	int second_point_normal_index; //�ڶ����������
	int third_point_index; //�����������
	int third_point_texture_index; //�����������������
	int third_point_normal_index; //�������������
};
//��ά�ռ��е���������ģ��
struct my_triangle_3DModel
{
	float transparency = 0;
	float reflection = 0;
	//�������ڼ���ֱ�ӹ�������
	float material_ambient_rgb_reflection[3] = { 0,0,0 }; //�����ⷴ��ϵ������ʼ������
	float material_diffuse_rgb_reflection[3] = { 0,0,0 };//������ϵ������ʼ������
	float material_specular_rgb_reflection[3] = { 0,0,0 };//����ⷴ��ϵ������ʼ������
	float ns = 0;//�۹�ָ������ʼ���۹�
	vector<point3> pointSets; //���ģ�����ж���
	vector<vec3> pointNormalSets; //���ģ�����ж���ķ���
	vector<glm::vec2>pointTextureSets;
	vector<my_triangle_indices> faceSets; //���ģ����������������
	//ģ�ͳߴ�
	float max_x = -1e8, min_x = 1e8;
	float max_y = -1e8, min_y = 1e8;
	float max_z = -1e8, min_z = 1e8;
	void modify_color_configuration(float transparency, float reflection, float
		ambient_reflection[], float diffuse_reflection[], float specular_reflection[], float ns)
	{
		this->transparency = transparency;
		this->reflection = reflection;
		//�������ڼ���ֱ�ӹ������� memcpy���ڰ���Դ�ڴ濽��Ŀ���ڴ���
		memcpy(this->material_ambient_rgb_reflection, ambient_reflection, 3 * sizeof(float));
		memcpy(this->material_diffuse_rgb_reflection, diffuse_reflection, 3 * sizeof(float));
		memcpy(this->material_specular_rgb_reflection, specular_reflection, 3 * sizeof(float));
		this->ns = ns;
	}
};
//ģ�ͼ�����
class my_objloader
{
public:
	my_triangle_3DModel my_3DModel;
public:
	my_objloader() {}
	my_objloader(string filename) //���캯��
	{
		string line;
		fstream f;
		f.open(filename, ios::in);
		if (!f.is_open()) {
			cout << "Something Went Wrong When Opening Objfiles" << endl;
		}
		while (!f.eof())
		{
			getline(f, line);//�õ�obj�ļ���һ�У���Ϊһ���ַ���
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
					parameters.push_back(ans); //ȡ���ַ����е�Ԫ�أ��Կո��з�
					ans = "";
				}
			}
			if (parameters.size() == 4 || parameters.size() == 3)
			{
				if (parameters[0] == "v") //����,��1��ʼ���������xyz����������붥��vector
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
				else if (parameters[0] == "vn") //����ķ�����
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
				else if (parameters[0] == "f") //�棬 ��������/����uv������/��������
				{
					//��Ϊ����������obj�ļ����Ǵ�1��ʼ�ģ������Ǵ�ŵĶ���vector�Ǵ�0��ʼ�ģ����Ҫ��1
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
