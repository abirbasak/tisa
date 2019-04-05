#ifndef AZUIK_IO_PLOT_HPP
#define AZUIK_IO_PLOT_HPP
namespace azuik
{
    namespace io
    {
        namespace svg
        {
            enum class axis_orientation {
                top,
                right,
                bottom,
                left,
            };

            struct scale {};
            struct axis {
                static axis top(scale scale)
                {
                    return axis{axis_orientation::top, scale};
                }

                static axis right(scale scale)
                {
                    return axis{axis_orientation::right, scale};
                }

                static axis bottom(double scale)
                {
                    return axis{axis_orientation::bottom, scale};
                }

                static axis left(double scale)
                {
                    return axis{axis_orientation::left, scale};
                }

                static auto constexpr translate_x(double x) noexcept
                {
                    return "translate(" + (x + 0.5) + ",0)";
                }

                static auto constexpr translate_y(double y) noexcept
                {
                    return "translate(0," + (y + 0.5) + ")";
                }

            private:
                axis_orientation m_orientation;
            };

            class line_plot {};
        } // namespace svg
    }     // namespace io
} // namespace azuik
#endif
