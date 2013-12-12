//#####################################################################
// Stanford CS148 Ray Tracer Example Code
// Copyright 2012, Ben Mildenhall
//#####################################################################

#include "Material.h"

STColor3f Material::shade(Intersection *inter, STVector3 view, std::vector<Light *>& lights, const STColor3f& mirrorCol, const STColor3f& refractCol) {
    STColor3f result = mirrorCol * mirror + refractCol * refract;
    for (int i = 0; i < (int)lights.size(); i++) result += lights[i]->color(inter, view, ambient, diffuse, specular, shininess);
    return result;
}

STColor3f Material::shade(Intersection* inter, STVector3 view, std::vector<Light *>& lights, std::vector<STColor3f>& attenuation, const STColor3f& mirrorCol /*= STColor3f()*/, const STColor3f& refractCol /*= STColor3f()*/)
{
	STColor3f result = mirrorCol * mirror + refractCol * refract;
	for (int i = 0; i < (int)lights.size(); i++){
		//std::cout<<"atten: "<<i<<": "<<attenuation[i].r<<", "<<attenuation[i].g<<", "<<attenuation[i].b<<std::endl;
		result += lights[i]->color(inter, view, ambient, diffuse, specular, shininess) * attenuation[i];
	}
	return result;
}

Ray *Material::refracted(STVector3 d, Intersection *inter, float bias) {
    d.Normalize();
    float cos_theta = STVector3::Dot(inter->normal, d);
    if (snell) {
        float sin_theta = (cos_theta > 0.f ? snell : 1.f / snell) * sqrt(1 - cos_theta * cos_theta);
        if (sin_theta >= 1) return NULL;
        STVector3 perp = d - STVector3::Dot(inter->normal, d) * inter->normal;
        perp.Normalize();
        return new Ray(inter->point, perp * sin_theta + (cos_theta > 0.f ? 1.f : -1.f) * inter->normal * sqrt(1.f - sin_theta * sin_theta), bias);
    }
    return NULL;
}

Ray *Material::refracted(Ray into, Intersection *inter, float bias) {
    return refracted(into.d, inter, bias);
}