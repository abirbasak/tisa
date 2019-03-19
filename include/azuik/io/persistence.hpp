#ifndef AZUIK_IO_PERSISTENCE_HPP
#define AZUIK_IO_PERSISTENCE_HPP
#include <azuik/core/mpl.hpp>
#include <iostream>
namespace azuik
{
    namespace io
    {
        template <class T, class F>
        void visit_each(T&& x, F f)
        {
            using self_type = core::remove_cv_ref<T>;
            using visit_type = typename self_type::visit;
            visit_type{}(static_cast<T&&>(x), f);
        }
        struct write_xml_fn {
            template <class T>
            void operator()(T const& x, std::ostream& os) const
            {
                visit_each(x, [&os](char const* name, auto const& value, const auto&&...) {
                    os << '<' << name << '>' << value << "</" << name << '>';
                });
            }
        } const write_xml{};
    } // namespace io
} // namespace azuik
#endif
