#ifndef MATERIAL_H
#define MATERIAL_H

typedef struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

#endif