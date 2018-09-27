#ifndef AZUIK_CONTAINER_DETAIL_PARTS_HPP
#define AZUIK_CONTAINER_DETAIL_PARTS_HPP
#include <azuik/core/mpl.hpp>
#include <azuik/core/allocator.hpp>
namespace azuik
{
    namespace core
    {
        namespace container_
        {
            template <class T>
            struct is_typed_allocator : false_c {};
            template <class T, template <class> class A>
            struct is_typed_allocator<A<T>> : true_c {};

            template <class A>
            struct allocator_storage : private A {
                using allocator_type = A;

                explicit constexpr allocator_storage(A&& a) noexcept
                    : allocator_type{std::move(a)}
                {}
                template <class T, class... Args>
                allocator_pointer<T, A> make(Args&&... args)
                {
                    return allocate_and_construct<T>(static_cast<allocator_type&>(*this),
                                                     static_cast<Args&&>(args)...);
                }
            };
        } // namespace container_
    }     // namespace core
} // namespace azuik
#endif
