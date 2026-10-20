#ifndef BASIC_TRANSFORM_H
#define BASIC_TRANSFORM_H

#include <cassert>

#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    // compose(f, g) is equivalent to first applying g, then applying f.
    static BasicTransform compose(const BasicTransform& f, const BasicTransform& g) {
        // f(p) = p * f.scale + f.translation
        // g(p) = p * g.scale + g.translation
        // f o g = f(g(p)) = g(p) * f.scale + f.translation
        // = (p * g.scale + g.translation) * f.scale + f.translation
        // = p * (g.scale * f.scale) + (g.translation * f.scale) + f.translation
        //
        // So, for f o g:
        // scale = g.scale * f.scale
        // translation = (g.translation * f.scale) + f.translation
        return { g.scale * f.scale, g.translation * f.scale + f.translation };
    };

    static BasicTransform inverse(const BasicTransform& f) {
        // f(p) = p * f.scale + f.translation
        // f(p) - f.translation = p * f.scale
        // p = [f(p) - f.translation] / f.scale
        // p = f(p) * 1 / f.scale + (- f.translation / f.scale)
        //
        // This mean inverting a transformation is possible
        // only if f.scale is not (0, 0).

        return BasicTransform(
            { 1 / f.scale.x, 1 / f.scale.y },
            { - f.translation.x / f.scale.x, - f.translation.y / f.scale.y });
    };

    constexpr BasicTransform(const sf::Vector2f& scale = {1.f, 1.f},
                             const sf::Vector2f& translation = {0.f, 0.f})
        : scale{scale}, translation{translation} {}

    constexpr sf::Vector2f applyPoint(const sf::Vector2f& p) const {
        // first scale, then translate -- if you do the reverse, the translation gets messed up.
        return {p.x * scale.x + translation.x, p.y * scale.y + translation.y};
    }

    constexpr sf::Vector2f applyVector(const sf::Vector2f& p) const {
        // vectors are not translated by transformations
        return {p.x * scale.x, p.y * scale.y};
    }

    constexpr bool operator==(const BasicTransform& other) const {
        return scale == other.scale && translation == other.translation;
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};

#endif