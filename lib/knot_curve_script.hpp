#ifndef KNOT_CURVE_SCRIPT_HPP
#define KNOT_CURVE_SCRIPT_HPP
#include "knot_curve_style.hpp"


class knot_curve_script : public knot_curve_style
{
    public:
        void draw_joint ( path_builder& path,
                            const TraversalInfo& ti,
                            styleinfo style );
};
#endif // KNOT_CURVE_SCRIPT_HPP
