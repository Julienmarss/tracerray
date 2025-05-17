/*
** EPITECH PROJECT, 2025
** B-OOP-400-LIL-4-1-raytracer-yanis.asselman
** File description:
** plugin_chair
*/

#ifndef PLUGIN_CHAIR_HPP_
#define PLUGIN_CHAIR_HPP_

#include "Interfaces/IPrimitive.hpp"
#include "Core/Vector3D.hpp"
#include "Core/Material.hpp"
#include "Core/Ray.hpp"
#include "Core/Color.hpp"
#include <optional>
#include <memory>
#include <vector>

namespace RayTracer {

class Box : public IPrimitive {
    public:
        Box(const Vector3D& min, const Vector3D& max, const Material& mat);

        std::optional<HitInfo> intersect(const Ray& ray, double minDist, double maxDist) const override;
        const Material& getMaterial() const override;
        void setMaterial(const Material& material) override;
        std::unique_ptr<IPrimitive> clone() const override;

    private:
        Vector3D _min, _max;
        Material _material;
};

class Chair : public IPrimitive {
    public:
        Chair();
        Chair(const Chair& other);

        std::optional<HitInfo> intersect(const Ray& ray, double minDist, double maxDist) const override;
        const Material& getMaterial() const override;
        void setMaterial(const Material& material) override;
        std::unique_ptr<IPrimitive> clone() const override;

    private:
        std::vector<std::unique_ptr<IPrimitive>> parts;
        void addBox(const Vector3D& min, const Vector3D& max, const Material& mat);
    };

}

extern "C" RayTracer::IPrimitive* create();

#endif /* !PLUGIN_CHAIR_HPP_ */
