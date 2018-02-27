#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::dvec3& kd, const glm::dvec3& ks, double shininess);
  virtual ~PhongMaterial();

public:
  const glm::dvec3 m_kd;
  const glm::dvec3 m_ks;

  const double m_shininess;
};
