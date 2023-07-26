#pragma once
//#include<iostream>
#include "rtweekend.h"
#include "hittable_list.h"
#include "hittable.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "my_objloader.h"

class mytriangle : public hittable
{
public:
	mytriangle() {};
	mytriangle(const point3& _p1, const point3& _p2, const point3& _p3, const glm::vec2& _p1t, const glm::vec2& _p2t, const glm::vec2& _p3t, shared_ptr<material> ptr)
	{
		p1 = _p1;
		p2 = _p2;
		p3 = _p3;

		p1t = _p1t;
		p2t = _p2t;
		p3t = _p3t;

		x0 = fmin(fmin(_p1.e[0], _p2.e[0]), _p3.e[0]);
		y0 = fmin(fmin(_p1.e[1], _p2.e[1]), _p3.e[1]);
		z0 = fmin(fmin(_p1.e[2], _p2.e[2]), _p3.e[2]);

		x1 = fmax(fmax(_p1.e[0], _p2.e[0]), _p3.e[0]);
		y1 = fmax(fmax(_p1.e[1], _p2.e[1]), _p3.e[1]);
		z1=  fmax(fmax(_p1.e[2], _p2.e[2]), _p3.e[2]);

		mp = ptr;
		
	
	};
	
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	//mytriangle(double _x0, double _x1, double _y0, double _y1, double _k,
	//	shared_ptr<material> mat)
		//: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};


	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {

		output_box = aabb(point3(x0, y0, z0), point3(x1, y1, z1));
		return true;
		
	}


public:
	point3 p1;
	point3 p2;
	point3 p3;
	shared_ptr<material> mp;
	double x0, x1, y0, y1,z0,z1, k;
	glm::vec2 p1t;
	glm::vec2 p2t;
	glm::vec2 p3t;


};

bool mytriangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

	
	//cerr << "p1:  " << p1[0] << "        " << p1[1] << "         " << p1[2] << "        " << endl;
	//cerr << "p2:  " << p2[0] << "        " << p2[1] << "         " << p2[2] << "        " << endl;
	//cerr << "p3:  " << p3[0] << "        " << p3[1] << "         " << p3[2] << "        " << endl;
	vec3 S = r.origin();            // 射线起点
	vec3 d = unit_vector(r.direction());         // 射线方向
	vec3 N = unit_vector(cross(p2 - p1, p3 - p1));    // 法向量
	if (dot(N, d) > 0.0f) N = -N;                     // 获取正确的法向量
	// 如果视线和三角形平行
	if (fabs(dot(N, d)) < 0.00001f) return false;
	// 距离
	double t1 = (dot(N, p1) - dot(S, N)) / dot(d, N);	
	if (t1 < t_min || t1 > t_max)
		return false;
	if (t1 < 0.0005f) return false;    // 如果三角形在光线背面

	// 交点计算
	vec3 P = S + d * t1;
	if ((p1.x() == p2.x() && p1.y() == p2.y() && p1.z() == p2.z()) || (p1.x() == p3.x() && p1.y() == p3.y() && p1.z() == p3.z()) || (p3.x() == p2.x() && p3.y() == p2.y() && p3.z() == p2.z()))
	{
		rec.u = p1t[0];
		rec.v = p1t[1];
		rec.t = t1;
		rec.mat_ptr = mp;
		rec.p = P;
		return true;
	}
	// 判断交点是否在三角形中
	vec3 c1 = cross(p2 - p1, P - p1);
	vec3 c2 = cross(p3 - p2, P - p2);
	vec3 c3 = cross(p1 - p3, P - p3);
	//if (dot(c1, N) > 0 && dot(c2, N) > 0 && dot(c3, N) > 0) return true;
	//if (dot(c1, N) < 0 && dot(c2, N) < 0 && dot(c3, N) < 0) return true;


	if (dot(c1, N) * dot(c2, N) < 0 || dot(c3, N) * dot(c2, N) < 0) return false;

	
	rec.set_face_normal(r, N);

	//三维插值参考资料https://zhuanlan.zhihu.com/p/337296743
	glm::mat3 matt //((glm::vec3)(p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]) , (glm::vec3)(p3[0]-p1[0], p3[1]-p1[1], p3[2]-p2[1]) , (glm::vec3)(-d[0], -d[1], -d[2]));
	{ p2.e[0] - p1.e[0]   , p2.e[1] - p1.e[1],   p2.e[2] - p1.e[2] ,
	  p3.e[0] - p1.e[0]   , p3.e[1] - p1.e[1],   p3.e[2] - p1.e[2],
	  -d.e[0]   ,-d.e[1] ,   -d.e[2]};

	//matt = glm::inverse(matt);
	matt = glm::transpose(matt);
	double hang = glm::determinant(matt);
	if(hang != 0)
	{
		

		matt = glm::inverse(matt);

		matt = glm::transpose(matt);
		glm::vec3 cha{ (S.e[0] - p1.e[0]), (S.e[1] - p1.e[1]), (S.e[2] - p1.e[2]) };
		glm::vec3 ve =  matt*cha;
	//	cerr << ve.z << "         " << t1 << endl;
		auto a = 1 - ve.x - ve.y;
		auto b = ve.x;
		auto y = ve.y;
		rec.u= a * p1t[0] + b * p2t[0] + y * p3t[0];
		rec.v== a * p1t[1] + b * p2t[1] + y * p3t[1];
		rec.t = t1;
		rec.mat_ptr = mp;
		rec.p = P;
	}
	//else
	//{
	//	cerr << "p1:  " << p1[0] << "        " << p1[1] << "         " << p1[2] << "        " << endl;
	//	cerr << "p2:  " << p2[0] << "        " << p2[1] << "         " << p2[2] << "        " << endl;
	//	cerr << "p3:  " << p3[0] << "        " << p3[1] << "         " << p3[2] << "        " << endl;

	//	rec.u = p1t[0];
	//	rec.v = p1t[1];
	//	rec.t = t1;
	//	rec.mat_ptr = mp;
	//	rec.p = P;
	//
	//	//return false;
	//}
		return true;
	


}

