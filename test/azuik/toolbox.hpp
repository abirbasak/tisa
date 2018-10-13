#ifndef AZUIK_TEST_TOOLBOX_HPP
#define AZUIK_TEST_TOOLBOX_HPP
namespace azuik
{
    namespace test
    {
        struct default_constructed {
            int value;

            default_constructed(const default_constructed&);
            default_constructed& operator=(const default_constructed&);

        public:
            default_constructed()
                : value{-1}
            {}
            ~default_constructed()
            {
                value = 0;
            }

            bool operator==(const default_constructed& that) const noexcept
            {
                return value == that.value;
            }
            bool operator<(const default_constructed& that) const noexcept
            {
                return value < that.value;
            }
        };

        struct move_only {
            move_only(int value = 1)
                : value{value}
            {}
            move_only(move_only&& that) noexcept
                : value{that.value}
            {
                that.value = 0;
            }
            move_only& operator=(move_only&& that) noexcept
            {
                value = that.value;
                that.value = 0;
                return *this;
            }
            move_only(const move_only&) = delete;
            move_only& operator=(const move_only&) = delete;

            bool operator==(const move_only& that) const noexcept
            {
                return value == that.value;
            }
            bool operator<(const move_only& that) const noexcept
            {
                return value < that.value;
            }
            move_only operator+(const move_only& that) const
            {
                return move_only{value + that.value};
            }
            move_only operator*(const move_only& that) const
            {
                return move_only{value * that.value};
            }

            int value;
        };
    } // namespace test
} // namespace azuik
#endif
