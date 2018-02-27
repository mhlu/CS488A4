#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
    const glm::dvec3& kd, const glm::dvec3& ks, double shininess )
    : m_kd(kd)
    , m_ks(ks)
    , m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}
