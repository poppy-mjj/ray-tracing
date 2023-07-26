//#define GLUT_DISABLE_ATEXIT_HACK
#undef UNICODE
#include <omp.h>
//#include "ObjLoader.h"
//#include "lineTriangle3DIntersection.h"
//#include "ray_trace.h"
//#include

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include<stdio.h>
//#include "GLUT.H"
#include "sphere.h"
#include"hittable_list.h"
#include"rtweekend.h"
#include "ray.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "bvh.h"
#include "texture.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
//#include "mytriangle.h"
#include "my_mesh_object.h"

int nearplane_width = 400; //视景体宽度
int nearplane_height =300; //视景体高度
auto aspect_ratio = 1.0;
int image_width = 400;
int samples_per_pixel = 2;
const int max_depth = 50;

// World

hittable_list world;

point3 lookfrom;
point3 lookat;
auto vfov = 40.0;
auto aperture = 0.0;
color background(0, 0, 0);
//int nearplane_distance = 500; //视景体近平面与视点距离
//int farplane_distance = nearplane_distance + 2000; //视景体远平面与视点距离
//my_3D_point_coord eye_position = { 0,0,800 };
//my_3D_point_coord target_position = { 0,0,0 };
float theta = 0.1;
//光源位置
//my_3D_point_coord light_position(1000.0, 1000.0, 1000.0);
//float light_rgb_ambient[] = { 0.8, 0.8, 0.8 };
//float light_rgb_diffuse_specular[] = { 1.0, 1.0, 1.0 };
//bool open_light = false;
//bool rendered = false;
//std::vector< my_triangle_3DModel> all_models; //场景中所有模型
//std::map<my_3D_point_coord*, my_draw_color*> render_vertices;//最终需要绘制的点以及采样点
//unsigned image_w, image_h;
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
hittable_list final_scene2() {

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));
   
    //auto light = make_shared<diffuse_light>(color(7, 7, 7));
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));
    auto emat = make_shared<lambertian>(make_shared<image_texture>("external/earthmap.jpg"));
    hittable_list objects;
    hittable_list boxes1;
    objects.add(make_shared<xz_rect>(-100, 100, -500, -300, 120,light));
    //objects.add(make_shared<bvh_node>(boxes1, 0, 1));
    //hittable_list boxes3;
    hittable_list s;
    s.add(make_shared<sphere>(point3(110, -20, -480), 20, make_shared<dielectric>(1.5)));
    s.add(make_shared<sphere>(point3(-110, -20, -480), 20, emat));
    objects.add(make_shared<bvh_node>(s, 0, 1));
    auto boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
   // objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));


    



    
   // objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    // objects.add(make_shared<xz_rect>(-400, 400, -400, 400, 400, emat));
    auto pertext = make_shared<noise_texture>(0.1);
   // objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));
      hittable_list tri, tri2, tri3, tri4, tri5;
    //auto white = make_shared<lambertian>(color(.73, .73, .73));
    //int ns = 1000;
   // tri.add(make_shared<my_mesh_object>(1,point3(0,0,0), "场景集合.obj", ground));
    //tri2.add(make_shared<my_mesh_object>(1, point3(150, -150, -10), "床.obj", emat));
    tri3.add(make_shared<my_mesh_object>(1, point3(0, -100, -2), "地板.obj", ground));
    tri.add(make_shared<my_mesh_object>(1, point3(0, -100, -400), "台灯.obj", white));
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    //tri3.add(make_shared<xz_rect>(-100, 100, -600, -400, -200, ground));
    tri4.add(make_shared<my_mesh_object>(1, point3(0, -100, -400), "桌子.obj", material2));
    tri5.add(make_shared<my_mesh_object>(1, point3(0, -100, -450), "椅子.obj",red));
    objects.add(make_shared<bvh_node>(tri, 0, 1));
    //objects.add(make_shared<bvh_node>(tri2, 0, 1));
    objects.add(make_shared<bvh_node>(tri3, 0, 1));
    objects.add(make_shared<bvh_node>(tri4, 0, 1));
    objects.add(make_shared<bvh_node>(tri5, 0, 1));   
    
    //objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    //objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    //objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    //objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    //objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    //objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    
 //   hittable_list tryy;
//    tryy.add(make_shared<mytriangle>(point3(400, 200, 400), point3(220, 280, 300), point3(100, 150, 350), white));
   // objects.add(make_shared<bvh_node>(tryy, 0, 1));

    //hittable_list boxes2;
        
        //boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));

    //objects.add(make_shared<translate>(
    //    make_shared<rotate_y>(
    //        make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
    //    vec3(-100, 270, 395)
    //    )
    //);


    return objects;
}

hittable_list final_scene3() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));



    boxes1.add(make_shared<box>(point3(-1000.0, 0.0, -1000.0), point3(1000.0, 100, 1000.0), ground));
    //    }
    //}

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));



    auto emat = make_shared<lambertian>(make_shared<image_texture>("external/earthmap.jpg"));
    //objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));

    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));
    hittable_list tri;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    //int ns = 1000;
    tri.add(make_shared<my_mesh_object>(2,point3(0, 0, 0), "测试1.obj", emat)); 
    //tri.add(make_shared<my_mesh_object>(point3(200, 0, 1400), "台灯.obj", emat));
    //tri.add(make_shared<my_mesh_object>(point3(-50, 0, -700), "书架.obj", emat));
    
    objects.add(make_shared<bvh_node>(tri, 0, 1));


    hittable_list boxes2;

    boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
        vec3(-100, 270, 395)
        )
    );
    /* hittable_list boxes2
      auto white = make_shared<lambertian>(color(.73, .73, .73));
      int ns = 1000;
      for (int j = 0; j < ns; j++) {
          boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
      }

      objects.add(make_shared<translate>(
          make_shared<rotate_y>(
              make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
          vec3(-100, 270, 395)
          )
      );*/

    return objects;
}


hittable_list final_scene() {

    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
        ));

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("external/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
        vec3(-100, 270, 395)
        )
    );

    return objects;
}
hittable_list cornell_smoke() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    return objects;
}
hittable_list cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    /*objects.add(make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white));
    objects.add(make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white));*/

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);
    return objects;
}
hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}
hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("external/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}
color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return vec3(0, 0, 0);

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);

}
hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}
hittable_list two_perlin_spheres() {
    hittable_list objects;
    auto pertext = make_shared<noise_texture>(4);

    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}
hittable_list random_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    //return world;
    return static_cast<hittable_list>(make_shared<bvh_node>(world, 0, 1));
}
//void keyboard(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//
//    case 'w': //关闭Zbuffer深度测试
//    case 'W':
//    {
//       // double distance = sqrt((lookat.x - lookfrom.x) * (target_position.x - eye_position.x) + (target_position.y - eye_position.x) * (target_position.y - eye_position.y) + (target_position.z - eye_position.z) * (target_position.z - eye_position.z));
//
//        lookfrom += (lookat - lookfrom)/10.0 ;
//        /*eye_position.y += (target_position.y - eye_position.y) / distance;
//        eye_position.z += (target_position.z - eye_position.z) / distance;*/
//        //reshape(nearplane_width, nearplane_height);
//        glutPostRedisplay();
//        //camera参数
//        break;
//    }
//    case 'a': //关闭Zbuffer深度测试
//    case 'A':
//    {
//       // double distance = sqrt((target_position.x - eye_position.x) * (target_position.x - eye_position.x) + (target_position.y - eye_position.x) * (target_position.y - eye_position.y) + (target_position.z - eye_position.z) * (target_position.z - eye_position.z));
//        //cout << distance;
//
//        
//        vec3 lefting = -cross(lookat-lookfrom,vec3(0,1,0))/10.0;
//        //cout << lefting.length() << endl;
//        lookat += lefting;
//        lookfrom += lefting;
//
//        //reshape(nearplane_width, nearplane_height);
//        glutPostRedisplay();
//
//        //camera参数
//        break;
//    }
//    case 's': //关闭Zbuffer深度测试
//    case 'S':
//    {
//
//        lookfrom += -(lookat - lookfrom)/10.0;
//        glutPostRedisplay();
//        //camera参数
//        break;
//    }
//    case 'd': //关闭Zbuffer深度测试
//    case 'D':
//    {
//        vec3 lefting = cross(lookat - lookfrom, vec3(0, 1, 0))/10;
//        lookat += lefting;
//        lookfrom += lefting;
//
//    }
//    case 'z': //打开Zbuffer深度测试
//    case 'Z':
//    {
//        //glEnable(GL_DEPTH_TEST); //打开深度缓冲测试
//        //glDepthFunc(GL_LESS); //判断遮挡关系时，离视点近的物体遮挡离视点远的物体
//        //myZbuffer();
//        glutPostRedisplay();
//        break;
//    }
//    case 'c': //关闭Zbuffer深度测试
//    case 'C':
//    {
//        //glDisable(GL_DEPTH_TEST); //关闭深度缓冲测试
//        //glutPostRedisplay();
//        break;
//    }
//    case 'l'://打开灯光
//    case 'L':
//    {
//        //open_light = true;
//        //glutPostRedisplay();
//        break;
//    }
//    case 'r'://光线跟踪
//    case 'R':
//    {
//        //依据当前视景体设置对投影平面进行顶点采样
//        /*samplepoint_sonprojectionplan(-0.5 * nearplane_width, 0.5 * nearplane_width,
//            -0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
//            0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
//            nearplane_distance, eye_position.z, render_vertices, image_w, image_h);
//        for (std::map<my_3D_point_coord*, my_draw_color*>::iterator piter =
//            render_vertices.begin(); piter != render_vertices.end(); piter++)
//        {
//            my_3Dvector raydir(eye_position, *(piter->first));
//            raydir.normalized();
//            my_draw_color newColor2 = one_ray_trace_my(eye_position, raydir, all_models,
//                0, eye_position, light_position, light_rgb_ambient, light_rgb_diffuse_specular);
//            *(piter->second) = newColor2;
//        }
//        rendered = true;
//        glutPostRedisplay();
//        break;*/
//    }
//    case 27:
//        exit(0);
//        break;
//    }
//}
void display(void);
void init()
{
    switch (0) {
    case 1:
        world = random_scene();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break;


    case 2:
        world = two_spheres();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        break;

    case 3:
        world = two_perlin_spheres();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        break;


    case 4:
        world = earth();
        background = color(0.70, 0.80, 1.00);
        lookfrom = point3(13, 2, 3);
        lookat = point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 5:
        world = simple_light();
        samples_per_pixel = 400;
        background = color(0, 0, 0);
        lookfrom = point3(26, 3, 6);
        lookat = point3(0, 2, 0);
        vfov = 20.0;
        break;


    case 6:
        world = cornell_box();
        aspect_ratio = 1.0;
        image_width = 600;
        samples_per_pixel = 200;
        background = color(0, 0, 0);
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
        break;

    case 7:
        world = cornell_smoke();
        aspect_ratio = 1.0;
        image_width = 600;
        samples_per_pixel = 200;
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
        break;
    case 8:
        world = final_scene();
        aspect_ratio = 1.0;
        image_width = 800;
        samples_per_pixel = 10000;
        background = color(0, 0, 0);
        lookfrom = point3(478, 278, -600);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
        break;

    case 9:
        //world = cornell_smoke();
        //world = final_scene();
        world = final_scene2();
        aspect_ratio = 1.0;
        image_width = 10;
        samples_per_pixel = 2;
        background = color(0, 0, 0);
        lookfrom = point3(478, 278, -600);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
        break;
default:  
    case 10:

        world = final_scene2();
        aspect_ratio = 4.0/3;
        image_width = 400;
        samples_per_pixel = 1000;
        background = color(0, 0, 0);
        //lookfrom = point3(-700, 40, -600);//角度三（我自己还在探索，你们跑下面这俩）
        lookfrom = point3(400,40, -400);//角度一
        //lookfrom = point3(0, 0, 0);//角度二
        lookat = point3(0, 0, -400);
        vfov = 40.0;
        break;
        
        
    }
    display();
}
void display(void) {

    //glClearColor(0.f, 0.f, 0.f, 0.f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   

    

    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
   // glBegin(GL_POINTS);
    //omp_set_num_threads(32);
   // #pragma omp parallel 
    {
        //#pragma omp single 
        {
            for (int j = image_height - 1; j >= 0; --j)
            {

                for (int i = 0; i < image_width; ++i)
                {
                   // #pragma omp task
                    {
                        std::cerr << "\rScanlines remaining: " << j << ' ' << image_width - i << ' ' << std::flush;
                        vec3 pixel_color(0, 0, 0);

                        #pragma omp parallel for
                        for (int s = 0; s < samples_per_pixel; ++s) {
                            auto u = (i + random_double()) / image_width;
                            auto v = (j + random_double()) / image_height;
                            ray r = cam.get_ray(u, v);
                            pixel_color += ray_color(r, background, world, max_depth);
                        }

                        pixel_color.write_color(std::cout, samples_per_pixel);
                        // pixel_color.my_write_color(samples_per_pixel);

                        // glVertex3f(2.0*i/ image_width -1.0, 2.0*j/ image_height -1.0, 0);
                    }
                }
            }
        }
    }
    //std::cout << "over " <<endl;

   // glEnd();
    //glutSwapBuffers();
    

}

//void mouse(int button, int state, int x, int y)
//{
//    switch (button)
//    {
//    case GLUT_LEFT_BUTTON:
//        if (state == GLUT_DOWN)
//        {
//            lookfrom[0] = lookfrom[0] * cosf(-theta) + lookfrom[2] * sinf(-theta);
//            lookfrom[2] = lookfrom[2] * cosf(-theta) - lookfrom[0] * sinf(-theta);
//            //lookfrom[1] = 1;
//            //reshape(nearplane_width, nearplane_height);
//            glutPostRedisplay();
//        }
//        break;
//    case GLUT_RIGHT_BUTTON:
//        if (state == GLUT_DOWN)
//        {
//            lookfrom[0] = lookfrom[0] * cosf(theta) + lookfrom[2] * sinf(theta);
//            lookfrom[2] = lookfrom[2] * cosf(theta) - lookfrom[0] * sinf(theta);
//            //lookfrom[1] = 1;
//            //reshape(nearplane_width, nearplane_height);
//            glutPostRedisplay();
//        }
//        break;
//    default:
//        break;
//    }
//}
//int main(int argc, char** argv) {
int main() {

    FILE* stream;
    freopen_s(&stream, "data.ppm", "w", stdout);
    //glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    //glutInitWindowSize(nearplane_width, nearplane_height);
    //glutInitWindowPosition(0, 0);

    //glutCreateWindow("全局光照之光线跟踪算法");
    init();
    //glutReshapeFunc(reshape);
    //glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouse);
    //glutMainLoop();
    return 0;
}
