#pragma once
#pragma once


#include "rtweekend.h"
#include "my_objloader.h"
#include "mytriangle.h"
#include "aarect.h"
#include "hittable_list.h"

class my_mesh_object : public hittable {
public:
    my_mesh_object() {}
    my_mesh_object(float scale, point3 pos, string filename, shared_ptr<material> ptr);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        output_box = aabb(box_min, box_max);
        return true;
    }

public:
    point3 box_min{1000, 1000, 1000
};
    point3 box_max;
    hittable_list sides;
};

my_mesh_object::my_mesh_object(float scale, point3 pos,string filename, shared_ptr<material> ptr) {
    my_objloader mesh_obj(filename);

    box_min += pos;
    box_max += pos;
    for (unsigned int i = 0; i < mesh_obj.my_3DModel.faceSets.size(); i++)
    {
        int firstPointIndex = mesh_obj.my_3DModel.faceSets[i].first_point_index;//取出顶点索引
        int secondPointIndex = mesh_obj.my_3DModel.faceSets[i].second_point_index;
        int thirdPointIndex = mesh_obj.my_3DModel.faceSets[i].third_point_index;

        int firstTextureIndex = mesh_obj.my_3DModel.faceSets[i].first_point_texture_index; //取出纹理索引
        int secondTextureIndex = mesh_obj.my_3DModel.faceSets[i].second_point_texture_index;
        int thirdTextureIndex = mesh_obj.my_3DModel.faceSets[i].third_point_texture_index;

        glm::mat3 rotz{ 0,1,0,  -1,0,0,  0,0,1 };
        glm::mat3 roty{ 0,0,-1, 0,1,0,   1,0,0 };
        glm::mat3 rotx{ 1,0,0,  0,0,-1,  0,1,0 };

        point3 mesha = mesh_obj.my_3DModel.pointSets[firstPointIndex];
        point3 meshb = mesh_obj.my_3DModel.pointSets[secondPointIndex];
        point3 meshc = mesh_obj.my_3DModel.pointSets[thirdPointIndex];

        glm::vec3 ma = { mesha.x(),mesha.y(),mesha.z() };
        glm::vec3 mb = { meshb.x(),meshb.y(),meshb.z() };
        glm::vec3 mc = { meshc.x(),meshc.y(),meshc.z() };

//            ma = (rotx * ma);
//            mb = (rotx * mb);
//            mc = (rotx * mc);
        glm::vec3  po = { pos.x(), pos.y(), pos.z() };
        ma = (ma)* scale + po;
        mb = (mb)* scale + po;
        mc = (mc)* scale + po;

            mesha = { ma.x,ma.y,ma.z };
            meshb = { mb.x,mb.y,mb.z };
            meshc = { mc.x,mc.y,mc.z };
            //cerr << ma.x<<"    " << ma.y <<"    "<< ma.z <<"  min  " << box_min << "    max " << box_max << endl;
            glm::vec2 p1Texture =
                mesh_obj.my_3DModel.pointTextureSets[firstTextureIndex];//第一个顶点纹理
            glm::vec2 p2Texture =
                mesh_obj.my_3DModel.pointTextureSets[secondTextureIndex];//第二个顶点纹理
            glm::vec2 p3Texture =
                mesh_obj.my_3DModel.pointTextureSets[thirdTextureIndex];//第三个顶点纹理

            mytriangle mesh(mesha, meshb, meshc, p1Texture, p2Texture, p3Texture, ptr);//, p1Normal, p2Normal, p3Normal);
            //sides.add(make_shared<mytriangle>(mesh));
            //objects.add(make_shared<bvh_node>(boxes1, 0, 1));
           // hittable_list side;
            //side.add(make_shared<mytriangle>(mesh));
            sides.add(make_shared<bvh_node>((hittable_list)(make_shared<mytriangle>(mesh)), 0, 1));


            if (box_min.e[0] > mesh.x0) box_min.e[0] = mesh.x0;
            if (box_min.e[1] > mesh.y0) box_min.e[1] = mesh.y0;
            if (box_min.e[2] > mesh.z0) box_min.e[2] = mesh.z0;
            if (box_max.e[0] < mesh.x1) box_max.e[0] = mesh.x1;
            if (box_max.e[1] < mesh.y1) box_max.e[1] = mesh.y1;
            if (box_max.e[2] < mesh.z1) box_max.e[2] = mesh.z1;
           // box_max = p1;
    /*        cerr << box_min[0] <<"  " << box_min[1] <<"  " << box_min[2] << endl;
            cerr << box_max[0] <<"  " << box_max[1] <<"  " << box_max[2] << endl;*/
         
        }
    
 

}

bool my_mesh_object::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    return sides.hit(r, t_min, t_max, rec);
}


