#ifndef AZUIK_IO_SVG_HPP
#define AZUIK_IO_SVG_HPP
#include <iosfwd> //iostream
#include <memory> //unique_ptr
#include <vector> //vector
namespace azuik
{
    namespace io
    {
        struct svg {
            enum class linecap { round, square, butt };
            friend std::ostream& operator<<(std::ostream& os, linecap self);
            enum class linejoin { bevel, miter, round };
            friend std::ostream& operator<<(std::ostream& os, linejoin);
            enum class anchor { start, middle, end };
            friend std::ostream& operator<<(std::ostream& os, anchor);
            enum class aspect_ratio {
                none,
                xmin_ymin,
                xmin_ymid,
                xmin_ymax,
                xmid_ymin,
                xmid_ymid,
                xmid_ymax,
                xmax_ymin,
                xmax_ymid,
                xmax_ymax
            };
            friend std::ostream& operator<<(std::ostream& os, aspect_ratio v);

            struct circle {
                double cx;
                double cy;
                double r;
            };
            struct ellipse {
                double cx;
                double cy;
                double rx;
                double ry;
            };
            struct line {
                double x1;
                double y1;
                double x2;
                double y2;
            };
            struct rect {
                double x;
                double y;
                double width;
                double height;
            };
            struct polygon {
                std::vector<double> points;
            };
            struct polyline {
                std::vector<double> points;
            };

        public:
            svg(double x, double y, double height, double width);
            template <class T>
            svg(bool, T&& value);
            void write(std::ostream& os) const;
            void push_back(circle&& v);
            void push_back(ellipse&& v);
            void push_back(line&& v);
            void push_back(rect&& v);
            void push_back(polygon&& v);
            void push_back(polyline&& v);

        public:
            struct concept
            {
                virtual void do_write_attributes(std::ostream & os) const = 0;
                virtual void do_write(std::ostream & os) const = 0;
                virtual ~concept() = default;
            };
            std::unique_ptr<concept> m_concept;
        };
    } // namespace io
} // namespace azuik
#endif