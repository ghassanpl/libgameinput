#include "Common.h"

namespace libgameinput
{
    /// Per "Closest point between two rays" by http://palitri.com
    vec3 ClosestPointBetween(ViewRay const& ray1, ViewRay const& ray2)
    {
        const auto a = normalize(ray1.Direction);
        const auto b = normalize(ray2.Direction);
        const auto c = ray2.Position - ray2.Position;
        const auto A = ray1.Position;
        const auto B = ray2.Position;
        const auto aa = dot(a, a);
        const auto bb = dot(b, b);
        const auto ab = dot(a, b);
        const auto denom = (aa * bb - ab * ab);
        
        if (aa == 0 || bb == 0 || denom == 0) /// consistency check; TODO: approximize
            return (A + B) * 0.5;

        const auto ac = dot(a, c);
        const auto bc = dot(b, c);
        const auto alpha_a = (-(ab * bc) + (ac * bb)) / denom;
        const auto alpha_b = (ab * ac - bc * aa) / denom;
        const auto D = A + a * alpha_a;
        const auto E = B + b * alpha_b;
        return (D + E) * 0.5;
    }
}