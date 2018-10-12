#ifndef AZUIK_TEST_TOOLBOX_HPP
#define AZUIK_TEST_TOOLBOX_HPP
namespace azuik
{
    namespace test
    {
        class move_only {
        public:
            move_only(int data = 1)
                : data{data}
            {}
            move_only(move_only&& that) noexcept
                : data{that.data}
            {
                that.data = 0;
            }
            move_only& operator=(move_only&& that) noexcept
            {
                data = that.data;
                that.data = 0;
                return *this;
            }
            move_only(const move_only&) = delete;
            move_only& operator=(const move_only&) = delete;

            bool operator==(const move_only& that) const noexcept
            {
                return data == that.data;
            }
            bool operator<(const move_only& that) const noexcept
            {
                return data < that.data;
            }
            move_only operator+(const move_only& that) const
            {
                return move_only{data + that.data};
            }
            move_only operator*(const move_only& that) const
            {
                return move_only{data * that.data};
            }

            int data;
        };
    } // namespace test
} // namespace azuik
#endif
