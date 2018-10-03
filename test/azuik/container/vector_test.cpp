#include <azuik/container/vector.hpp>
#include <azuik/tool/unit_test.hpp>
AZUIK_TEST_SUIT(vector)
{
    using namespace azuik;
    AZUIK_TEST_CASE(vec_test_1)
    {
        core::vector<int> v;
        auto const& cv = v;

        core::vector<int>::iterator it = v.begin();
        core::vector<int>::const_iterator cit = cv.begin();

        AZUIK_TEST(v.empty() == true);
        AZUIK_TEST(v.size() == 0);

        v.push_back(42);

        AZUIK_TEST(v.size() == 1);

        AZUIK_TEST(v[0] == 42);
    }
    AZUIK_TEST_CASE(vec_test_2)
    {
        core::vector<core::vector<int>> vect(10);
        core::vector<core::vector<int>>::iterator it(vect.begin()), end(vect.end());
        for (; it != end; ++it)
        {
            AZUIK_TEST((*it).empty());
            AZUIK_TEST((*it).size() == 0);
            AZUIK_TEST((*it).capacity() == 0);
            AZUIK_TEST((*it).begin() == (*it).end());
        }
    }

    AZUIK_TEST_CASE(vec_test_3)
    {
        core::vector<double> v1;
        v1.push_back(32.1);
        v1.push_back(40.5);

        core::vector<double> v2;
        v2.push_back(3.56);

        AZUIK_TEST(v1.size() == 2);
        AZUIK_TEST(v1[0] == 32.1);
        AZUIK_TEST(v1[1] == 40.5);

        AZUIK_TEST(v2.size() == 1);
        AZUIK_TEST(v2[0] == 3.56);
        size_t v1Cap = v1.capacity();
        size_t v2Cap = v2.capacity();

        v1.swap(v2); // Swap the vector's contents.

        AZUIK_TEST(v1.size() == 1);
        AZUIK_TEST(v1.capacity() == v2Cap);
        AZUIK_TEST(v1[0] == 3.56);

        AZUIK_TEST(v2.size() == 2);
        AZUIK_TEST(v2.capacity() == v1Cap);
        AZUIK_TEST(v2[0] == 32.1);
        AZUIK_TEST(v2[1] == 40.5);

        v2 = v1; // Assign one vector to another.

        AZUIK_TEST(v2.size() == 1);
        AZUIK_TEST(v2[0] == 3.56);
    }

    AZUIK_TEST_CASE(vec_test_4)
    {
        using V = core::vector<char>;

        V v1;
        v1.push_back('h');
        v1.push_back('i');

        AZUIK_TEST(v1.size() == 2);
        AZUIK_TEST(v1[0] == 'h');
        AZUIK_TEST(v1[1] == 'i');

        V v2(v1.begin(), v1.end());
        v2[1] = 'o';

        AZUIK_TEST(v2.size() == 2);
        AZUIK_TEST(v2[0] == 'h');
        AZUIK_TEST(v2[1] == 'o');

        AZUIK_TEST((v1 == v2) == false);

        AZUIK_TEST((v1 < v2) == true);
    }

    AZUIK_TEST_CASE(vec_test_5)
    {
        core::vector<int> v(4);

        v[0] = 1;
        v[1] = 4;
        v[2] = 9;
        v[3] = 16;

        AZUIK_TEST(core::front(v) == 1);
        AZUIK_TEST(core::back(v) == 16);

        v.push_back(25);

        AZUIK_TEST(core::back(v) == 25);
        AZUIK_TEST(v.size() == 5);

        v.pop_back();

        AZUIK_TEST(core::back(v) == 16);
        AZUIK_TEST(v.size() == 4);
    }

    AZUIK_TEST_CASE(vec_test_6)
    {
        int array[] = {1, 4, 9, 16};

        core::vector<int> v(array, array + 4);

        AZUIK_TEST(v.size() == 4);

        AZUIK_TEST(v[0] == 1);
        AZUIK_TEST(v[1] == 4);
        AZUIK_TEST(v[2] == 9);
        AZUIK_TEST(v[3] == 16);
    }

    AZUIK_TEST_CASE(erase_test1)
    {
        int array[] = {1, 4, 9, 16, 25, 36};

        core::vector<int> v(array, array + 6);
        core::vector<int>::iterator vit;

        AZUIK_TEST(v.size() == 6);
        AZUIK_TEST(v[0] == 1);
        AZUIK_TEST(v[1] == 4);
        AZUIK_TEST(v[2] == 9);
        AZUIK_TEST(v[3] == 16);
        AZUIK_TEST(v[4] == 25);
        AZUIK_TEST(v[5] == 36);

        vit = v.erase(v.begin());
        AZUIK_TEST(*vit == 4);

        AZUIK_TEST(v.size() == 5);
        AZUIK_TEST(v[0] == 4);
        AZUIK_TEST(v[1] == 9);
        AZUIK_TEST(v[2] == 16);
        AZUIK_TEST(v[3] == 25);
        AZUIK_TEST(v[4] == 36);

        vit = v.erase(v.end() - 1);
        AZUIK_TEST(vit == v.end());

        AZUIK_TEST(v.size() == 4);
        AZUIK_TEST(v[0] == 4);
        AZUIK_TEST(v[1] == 9);
        AZUIK_TEST(v[2] == 16);
        AZUIK_TEST(v[3] == 25);

        v.erase(v.begin() + 1, v.end() - 1);

        AZUIK_TEST(v.size() == 2);
        AZUIK_TEST(v[0] == 4);
        AZUIK_TEST(v[1] == 25);
    }

    AZUIK_TEST_CASE(insert_test1)
    {
        int array1[] = {1, 4, 25};
        int array2[] = {9, 16};

        core::vector<int> v(array1, array1 + 3);
        core::vector<int>::iterator vit;
        vit = v.insert(v.begin(), 0);
        AZUIK_TEST(*vit == 0);

        vit = v.insert(v.end(), 36);
        AZUIK_TEST(*vit == 36);

        AZUIK_TEST(v.size() == 5);
        AZUIK_TEST(v[0] == 0);
        AZUIK_TEST(v[1] == 1);
        AZUIK_TEST(v[2] == 4);
        AZUIK_TEST(v[3] == 25);
        AZUIK_TEST(v[4] == 36);

        v.insert(v.begin() + 3, array2, array2 + 2);

        AZUIK_TEST(v.size() == 7);

        AZUIK_TEST(v[0] == 0);
        AZUIK_TEST(v[1] == 1);
        AZUIK_TEST(v[2] == 4);
        AZUIK_TEST(v[3] == 9);
        AZUIK_TEST(v[4] == 16);
        AZUIK_TEST(v[5] == 25);
        AZUIK_TEST(v[6] == 36);

        v.clear();
        AZUIK_TEST(v.empty());

        v.insert(v.begin(), 5, 10);
        AZUIK_TEST(v.size() == 5);
        AZUIK_TEST(v[0] == 10);
        AZUIK_TEST(v[1] == 10);
        AZUIK_TEST(v[2] == 10);
        AZUIK_TEST(v[3] == 10);
        AZUIK_TEST(v[4] == 10);
    }

    struct TestStruct {
        unsigned int a[3];
    };

    AZUIK_TEST_CASE(capacity_test1)
    {
        {
            core::vector<int> v;

            AZUIK_TEST(v.capacity() == 0);
            v.push_back(42);
            AZUIK_TEST(v.capacity() >= 1);
            v.reserve(5000);
            AZUIK_TEST(v.capacity() >= 5000);
        }

        {
            core::vector<TestStruct> va;
            va.reserve(1);
            va.reserve(2);
        }
    }

    AZUIK_TEST_CASE(pointer_elem)
    {
        core::vector<int*> v1;
        core::vector<int*> v2 = v1;
        core::vector<int*> v3;

        v3.insert(v3.end(), v1.begin(), v1.end());
    }

    AZUIK_TEST_CASE(auto_ref)
    {
        core::vector<int> ref;
        for (int i = 0; i < 5; ++i)
        {
            ref.push_back(i);
        }

        core::vector<core::vector<int>> v_v_int(1, ref);
        v_v_int.push_back(v_v_int[0]);
        v_v_int.push_back(ref);
        v_v_int.push_back(v_v_int[0]);
        v_v_int.push_back(v_v_int[0]);
        v_v_int.push_back(ref);

        core::vector<core::vector<int>>::iterator vvit(v_v_int.begin()), vvitEnd(v_v_int.end());
        for (; vvit != vvitEnd; ++vvit)
        {
            AZUIK_TEST(*vvit == ref);
        }
    }

    struct stack_allocator {
        stack_allocator(char* beg, char* end)
            : m_beg(beg)
            , m_end(end)
            , m_cur(m_beg)
            , m_isOk(true)
            , m_swaped(false)
            , m_nbAlloc(0)
            , m_sharedCur(&m_cur)
            , m_sharedOk(&m_isOk)
            , m_sharedNbAlloc(&m_nbAlloc)
        {}
        template <class T>
        T* allocate(std::size_t n)
        {
            if (n == 0)
            {
                return 0;
            }

            ++(*m_sharedNbAlloc);

            if (*m_sharedCur + (n * sizeof(T)) < m_end)
            {
                char* ret = *m_sharedCur;
                *m_sharedCur += n * sizeof(T);
                return reinterpret_cast<T*>(ret);
            }
        }
        template <class T>
        void deallocate(T* p, std::size_t n)
        {
            if (p == 0)
            {
                return;
            }

            --(*m_sharedNbAlloc);

            if ((char*)p == (*m_sharedCur - n * sizeof(T)))
            {
                *m_sharedCur -= n * sizeof(T);
            }

            if ((char*)p < m_beg || (char*)p >= m_end)
            {
                *m_sharedOk = false;
            }
        }

        bool ok() const
        {
            return m_isOk && (m_nbAlloc == 0);
        }
        void reset()
        {
            m_cur = m_beg;
            m_isOk = true;
            m_swaped = false;
        }

        bool operator==(stack_allocator const& other) const
        {
            return m_beg == other.m_beg;
        }

        bool operator!=(stack_allocator const& other) const
        {
            return !(*this == other);
        }
        bool swaped() const
        {
            return m_swaped;
        }

    private:
        char* m_beg;
        char* m_end;
        char* m_cur;
        bool m_isOk, m_swaped;
        int m_nbAlloc;

        //The following members are shared among all StackAllocator instance created from
        //a reference StackAllocator instance:
        char** m_sharedCur;
        bool* m_sharedOk;
        int* m_sharedNbAlloc;
    };

    AZUIK_TEST_CASE(allocator_with_state)
    {
        char buf1[1024];
        stack_allocator stack1(buf1, buf1 + sizeof(buf1));

        char buf2[1024];
        stack_allocator stack2(buf2, buf2 + sizeof(buf2));

        {
            using V = core::vector<int, stack_allocator>;
            V vint1(10, 0, stack1);
            V vint1Cpy(vint1);

            V vint2(10, 1, stack2);
            V vint2Cpy(vint2);

            vint1.swap(vint2);

            AZUIK_TEST(vint1.get_allocator().swaped());
            AZUIK_TEST(vint2.get_allocator().swaped());

            AZUIK_TEST(vint1 == vint2Cpy);
            AZUIK_TEST(vint2 == vint1Cpy);
            AZUIK_TEST(vint1.get_allocator() == stack2);
            AZUIK_TEST(vint2.get_allocator() == stack1);
        }
        AZUIK_TEST(stack1.ok());
        AZUIK_TEST(stack2.ok());
    }

    struct point {
        int x, y;
    };

    struct point2 : public point {
        point2()
            : copied(false)
        {}
        point2(const point&)
            : copied(true)
        {}

        bool copied;
    };

    AZUIK_TEST_CASE(optimizations_check)
    {
        core::vector<point> v1(1);
        AZUIK_TEST(v1.size() == 1);

        core::vector<point2> v2(v1.begin(), v1.end());
        AZUIK_TEST(v2.size() == 1);
        AZUIK_TEST(v2[0].copied == true);
    }

    AZUIK_TEST_CASE(assign_check)
    {
        core::vector<int> v(3, 1);
        int array[] = {1, 2, 3, 4, 5};

        v.assign(array, array + 5);
        AZUIK_TEST(v[4] == 5);
        AZUIK_TEST(v[0] == 1);
        AZUIK_TEST(v[1] == 2);
    }

    AZUIK_TEST_CASE(iterators)
    {
        core::vector<int> vint(10, 0);
        core::vector<int> const& crvint = vint;

        AZUIK_TEST(vint.begin() == vint.begin());
        AZUIK_TEST(crvint.begin() == vint.begin());
        AZUIK_TEST(vint.begin() == crvint.begin());
        AZUIK_TEST(crvint.begin() == crvint.begin());

        AZUIK_TEST(vint.begin() != vint.end());
        AZUIK_TEST(crvint.begin() != vint.end());
        AZUIK_TEST(vint.begin() != crvint.end());
        AZUIK_TEST(crvint.begin() != crvint.end());
    }

    class IncompleteClass {
        core::vector<IncompleteClass> instances;
        typedef core::vector<IncompleteClass>::iterator it;
    };
}
