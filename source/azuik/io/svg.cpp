#include <azuik/io/svg.hpp>
#include <array>
#include <vector>
#include <utility>
#include <iostream>
namespace azuik
{
    namespace io
    {
        template <class T, class... Args>
        auto make(Args&&... args)
        {
            return std::unique_ptr<T>(new T{static_cast<Args&&>(args)...});
        }

        struct join_fn {
            template <class FwdIter, class Sep>
            std::ostream& operator()(std::ostream& os, FwdIter first, FwdIter last,
                                     Sep const& sep) const
            {
                if (first != last)
                {
                    os << *first;

                    for (++first; first != last; ++first)
                    {
                        os << sep << *first;
                    }
                }
                return os;
            }
            template <class FwdIter, class Sep, class Encloser>
            std::ostream& operator()(std::ostream& os, FwdIter first, FwdIter last, Sep const& sep,
                                     Encloser start, Encloser end) const
            {
                os << start;
                (*this)(os, first, last, sep);
                os << end;
                return os;
            }
        };
        inline static constexpr join_fn join = {};

        template <class T>
        std::ostream& operator<<(std::ostream& os, std::vector<T> const& v)
        {
            return join(os, std::begin(v), std::end(v), ',');
        }
        template <class T, std::size_t N>
        std::ostream& operator<<(std::ostream& os, std::array<T, N> const& v)
        {
            return join(os, std::begin(v), std::end(v), ' ');
        }
        //space
        struct space {
            explicit constexpr space(int count) noexcept
                : count{count}
            {}
            friend std::ostream& operator<<(std::ostream& os, space v)
            {
                for (int c = v.count; c-- > 0;)
                {
                    os << ' ';
                }
                return os;
            }
            int count;
        };
        //newline

        std::ostream& nl(std::ostream& os)
        {
            return os << "\n";
        }

        int indent_index()
        {
            static int index = std::ios_base::xalloc();
            return index;
        }
        struct set_indent {
            explicit set_indent(int offset)
                : offset{offset}
            {}
            friend std::ostream& operator<<(std::ostream& os, set_indent i)
            {
                os.iword(indent_index()) += i.offset;
                return os;
            }
            int offset;
        };

        std::ostream& indent(std::ostream& os)
        {
            return os << nl << space(os.iword(indent_index()));
        }

        template <class E>
        struct model;
        template <class E>
        struct name;

        template <class E>
        struct element_guard {
            explicit element_guard(model<E> const& e, std::ostream& os)
                : e{e}
                , m_os{os}

            {
                m_os << '<' << name<std::remove_reference_t<E>>::value;
                e.do_write_attributes(os);
                if (!e.empty())
                {
                    m_os << '>' << set_indent(2);
                }
                else
                {
                    m_os << "/>";
                }
            }
            ~element_guard()
            {
                if (!e.empty())
                {
                    m_os << set_indent(-2) << indent << '<' << '/'
                         << name<std::remove_reference_t<E>>::value << '>';
                    ;
                }
            }
            element_guard(element_guard const&) = delete;
            void operator=(element_guard const&) = delete;
            element_guard(element_guard&&) = delete;
            void operator=(element_guard&&) = delete;

        private:
            model<E> const& e;
            std::ostream& m_os;
        };

        template <class T>
        struct attribute {
            explicit constexpr attribute(char const* name, T value) noexcept
                : m_name{name}
                , m_value{std::move(value)}
            {}
            friend inline std::ostream& operator<<(std::ostream& os, attribute const& a)
            {
                return os << ' ' << a.m_name << "=\"" << a.m_value << "\"";
            }

        private:
            char const* m_name;
            T m_value;
        };

        std::ostream& operator<<(std::ostream& os, svg::aspect_ratio x)
        {
            switch (x)
            {
                case svg::aspect_ratio::none: return os << "None";
                case svg::aspect_ratio::xmin_ymin: return os << "xMinYMin";
                case svg::aspect_ratio::xmin_ymid: return os << "xMinYMid";
                case svg::aspect_ratio::xmin_ymax: return os << "xMinYMax";
                case svg::aspect_ratio::xmid_ymin: return os << "xMidYMin";
                case svg::aspect_ratio::xmid_ymid: return os << "xMidYMid";
                case svg::aspect_ratio::xmid_ymax: return os << "xMidYMax";
                case svg::aspect_ratio::xmax_ymin: return os << "xMaxYMin";
                case svg::aspect_ratio::xmax_ymid: return os << "xMaxYMid";
                case svg::aspect_ratio::xmax_ymax: return os << "XMaxYMax";
            }
            return os;
        }
        std::ostream& operator<<(std::ostream& os, svg::linejoin x)
        {
            switch (x)
            {
                case svg::linejoin::bevel: return os << "bevel";
                case svg::linejoin::miter: return os << "miter";
                case svg::linejoin::round: return os << "round";
            }
        }
        std::ostream& operator<<(std::ostream& os, svg::linecap x)
        {
            switch (x)
            {
                case svg::linecap::round: return os << "round";
                case svg::linecap::square: return os << "square";
                case svg::linecap::butt: return os << "butt";
            }
        }
        std::ostream& operator<<(std::ostream& os, svg::anchor x)
        {
            switch (x)
            {
                case svg::anchor::start: return os << "start";
                case svg::anchor::middle: return os << "middle";
                case svg::anchor::end: return os << "end";
            }
        }
        template <class T>
        void write_attributes(T const& self, std::ostream& os);

        template <class T>
        struct model : svg::concept {
            template <class... Args>
            constexpr model(std::in_place_t, Args&&... args) noexcept
                : m_value{static_cast<Args&&>(args)...}
            {}
            void do_write(std::ostream& os) const override
            {
                element_guard<T> scope{*this, os};
                for (auto const& e : elements)
                {
                    os << indent;
                    e.write(os);
                }
            }
            void do_write_attributes(std::ostream& os) const override
            {
                write_attributes(m_value, os);
            }

            void push_back(svg&& e)
            {
                elements.push_back(std::move(e));
            }
            bool empty() const noexcept
            {
                return elements.empty();
            }

        private:
            T m_value;
            std::vector<svg> elements;
        };

        struct self;
        template <>
        struct name<self> {
            inline static constexpr char const* value = "svg";
        };

        struct self {
            double x;
            double y;
            double height;
            double width;
            std::array<double, 4> view_box;

            friend void write_attributes(self const& self, std::ostream& os)
            {
                os << attribute("x", self.x) << attribute("y", self.y)
                   << attribute("height", self.height) << attribute("width", self.width);
            }
        };

        svg::svg(double x, double y, double height, double width)
            : m_concept{make<model<self>>(std::in_place, x, y, width, height)}
        {}
        template <class T>
        svg::svg(bool, T&& x)
            : m_concept{make<model<T>>(std::in_place, static_cast<T&&>(x))}
        {}
        void svg::write(std::ostream& os) const
        {
            static_cast<model<self> const&>(*m_concept).do_write(os);
        }
        //circle
        template <>
        struct name<svg::circle> {
            inline static constexpr char const* value = "circle";
        };
        void write_attributes(svg::circle const& v, std::ostream& os)
        {
            os << attribute("cx", v.cx) << attribute("cy", v.cy) << attribute("r", v.r);
        }

        void svg::push_back(circle&& v)
        {
            static_cast<model<self>&>(*m_concept).push_back(svg{true, v});
        }

        //ellipse
        template <>
        struct name<svg::ellipse> {
            inline static constexpr char const* value = "ellipse";
        };
        void write_attributes(svg::ellipse const& v, std::ostream& os)
        {
            os << attribute("cx", v.cx) << attribute("cy", v.cy) << attribute("rx", v.rx)
               << attribute("ry", v.ry);
        }

        void svg::push_back(ellipse&& v)
        {
            static_cast<model<self>&>(*m_concept).push_back(svg{true, v});
        }
        //line
        template <>
        struct name<svg::line> {
            inline static constexpr char const* value = "line";
        };

        void write_attributes(svg::line const& v, std::ostream& os)
        {
            os << attribute("x1", v.x1) << attribute("y1", v.y1) << attribute("x2", v.x2)
               << attribute("y2", v.y2);
        }
        void svg::push_back(svg::line&& v)
        {
            static_cast<model<self>&>(*m_concept).push_back(svg{true, v});
        }
        //rect
        template <>
        struct name<svg::rect> {
            inline static constexpr char const* value = "rect";
        };
        void write_attributes(svg::rect const& v, std::ostream& os)
        {
            os << attribute("x", v.x) << attribute("y", v.y) << attribute("width", v.width)
               << attribute("height", v.height);
        }
        void svg::push_back(svg::rect&& v)
        {
            static_cast<model<self>&>(*m_concept).push_back(svg{true, v});
        }
        //polygon
        template <>
        struct name<svg::polygon> {
            inline static constexpr char const* value = "polygon";
        };
        void write_attributes(svg::polygon const& v, std::ostream& os)
        {
            os << attribute("points", v.points);
        }
        void svg::push_back(svg::polygon&& v)
        {
            static_cast<model<self>&>(*m_concept).push_back(svg{true, v});
        }
        //polyline
        template <>
        struct name<svg::polyline> {
            inline static constexpr char const* value = "polyline";
        };
        void write_attributes(svg::polyline const& v, std::ostream& os)
        {
            os << attribute("points", v.points);
        }
        void svg::push_back(svg::polyline&& v)
        {
            static_cast<model<self>&>(*m_concept).push_back(svg{true, v});
        }

    } // namespace io

} // namespace azuik