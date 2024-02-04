#include "gtest/gtest.h"
#include "CircularBuffer.h"

namespace BufferConstructor_TESTS {
    TEST(BufferConstructor, WithoutParameters) {
        CircularBuffer<int> buffer;
        ASSERT_EQ(buffer.capacity(), 0);
        ASSERT_EQ(buffer.size(), 0);
    }

    TEST(BufferConstructor, UsingCircularBuffer) {
        CircularBuffer<int> buffer1(3);
        buffer1.push_front(5);
        buffer1.push_back(2);
        buffer1.push_back(7);
        buffer1.push_back(1);
        buffer1.push_front(10);
        CircularBuffer<int> buffer2(buffer1);
        ASSERT_EQ(buffer1, buffer2);
    }

    TEST(BufferConstructor, UsingCapacity) {
        CircularBuffer<int> buffer(3);
        ASSERT_EQ(buffer.size(), 0);
        ASSERT_EQ(buffer.capacity(), 3);
    }

    TEST(BufferConstructor, UsingCapacityAndFilling) {
        CircularBuffer<int> buffer(3, 9);
        ASSERT_EQ(buffer.size(), 3);
        ASSERT_EQ(buffer.capacity(), 3);
        for (int i = 0; i < buffer.size(); ++i)
            ASSERT_EQ(buffer[i], 9);
    }
}

namespace BufferDestructor_TESTS {
    TEST(BufferDestructor, OneTime) {
        auto buffer = new CircularBuffer<int>(5, 9);
        ASSERT_TRUE(buffer->empty());
        delete buffer;
        ASSERT_FALSE(buffer->empty());
    }
}

namespace AccessByIndex_TESTS {
    namespace WithoutCheck {
        namespace Const {
            TEST(AccessByIndex_WithoutCheck_Const, RightIndex) {
                CircularBuffer<int> buffer(3, 9);
                for (int i = 0; i < buffer.size(); ++i)
                    ASSERT_EQ(buffer[i], 9);
            }
        }
        namespace NotConst {
            TEST(AccessByIndex_WithoutCheck_NotConst, RightIndex) {
                CircularBuffer<int> buffer(3, 9);
                for (int i = 0; i < buffer.size(); ++i)
                    ASSERT_EQ(buffer[i], 9);
            }
        }
    }
    namespace WithCheck {
        namespace Const {
            TEST(AccessByIndex_WithCheck_Const, RightIndex) {
                CircularBuffer<int> buffer(3, 9);
                for (int i = 0; i < buffer.size(); ++i)
                    ASSERT_EQ(buffer.at(i), 9);
            }

            TEST(AccessByIndex_WithCheck_Const, WrongIndex) {
                CircularBuffer<int> buffer(3, 9);
                for (int i = -buffer.size(); i < 0; ++i)
                    ASSERT_THROW(buffer.at(i), std::out_of_range);
                for (int i = buffer.size(); i < 2 * buffer.size(); ++i)
                    ASSERT_THROW(buffer.at(i), std::out_of_range);
            }
        }
        namespace NotConst {
            TEST(AccessByIndex_WithCheck_NotConst, RightIndex) {
                CircularBuffer<int> buffer(3, 9);
                for (int i = 0; i < buffer.size(); ++i)
                    ASSERT_EQ(buffer.at(i), 9);
            }

            TEST(AccessByIndex_WithCheck_NotConst, WrongIndex) {
                CircularBuffer<int> buffer(3, 9);
                for (int i = -buffer.size(); i < 0; ++i)
                    ASSERT_THROW(buffer.at(i), std::out_of_range);
                for (int i = buffer.size(); i < 2 * buffer.size(); ++i)
                    ASSERT_THROW(buffer.at(i), std::out_of_range);
            }
        }
    }
}

namespace Reference_TESTS {
    namespace Const {
        TEST(Reference_Const, TheFirstElement) {
            CircularBuffer<int> buffer(3, 9);
            for (int i = 0; i < buffer.size(); ++i)
                ASSERT_EQ(buffer.front(), 9);
        }
        TEST(Reference_Const, TheLastElement) {
            CircularBuffer<int> buffer(3, 9);
            for (int i = 0; i < buffer.size(); ++i)
                ASSERT_EQ(buffer.back(), 9);
        }
    }
    namespace NotConst {
        TEST(Reference_NotConst, TheFirstElement) {
            CircularBuffer<int> buffer(3, 9);
            for (int i = 0; i < buffer.size(); ++i)
                ASSERT_EQ(buffer.front(), 9);
        }
        TEST(Reference_NotConst, TheLastElement) {
            CircularBuffer<int> buffer(3, 9);
            for (int i = 0; i < buffer.size(); ++i)
                ASSERT_EQ(buffer.back(), 9);
        }
    }
}

namespace Linearize_TESTS {
    TEST(Linearize, MoveBuffer) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        buffer.push_back(7);
        buffer.push_back(1);
        buffer.push_front(10);
        buffer.linearize();
        ASSERT_EQ(buffer.front(), 10);
        ASSERT_EQ(buffer[1], 1);
        ASSERT_EQ(buffer.back(), 2);
    }

    TEST(Linearize, CheckBuffer) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        buffer.push_back(7);
        buffer.push_back(1);
        buffer.push_front(10);
        buffer.linearize();
        ASSERT_TRUE(buffer.is_linearized());
    }
}

namespace Rotate_TESTS {
    TEST(Rotate, RightIndex) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        buffer.push_back(7);
        buffer.push_back(1);
        buffer.push_front(10);
        buffer.rotate(0);
        ASSERT_EQ(buffer.front(), 10);
        ASSERT_EQ(buffer[1], 1);
        ASSERT_EQ(buffer.back(), 2);
        buffer.rotate(1);
        ASSERT_EQ(buffer.front(), 2);
        ASSERT_EQ(buffer[1], 10);
        ASSERT_EQ(buffer.back(), 1);
        buffer.rotate(2);
        ASSERT_EQ(buffer.front(), 1);
        ASSERT_EQ(buffer[1], 2);
        ASSERT_EQ(buffer.back(), 10);
    }
}

namespace Size_TESTS {
    TEST(Size, FullBuffer) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        buffer.push_back(7);
        buffer.push_back(1);
        buffer.push_front(10);
        ASSERT_EQ(buffer.size(), 3);
    }

    TEST(Size, EmptyBuffer) {
        CircularBuffer<int> buffer(3);
        ASSERT_EQ(buffer.size(), 0);
    }

    TEST(Size, BufferNotCompletelyFilled) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        ASSERT_EQ(buffer.size(), 2);
    }
}

namespace Empty_TESTS {
    TEST(Empty, FullBuffer) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        buffer.push_back(7);
        buffer.push_back(1);
        buffer.push_front(10);
        ASSERT_FALSE(buffer.empty());
    }

    TEST(Empty, EmptyBuffer) {
        CircularBuffer<int> buffer(3);
        ASSERT_TRUE(buffer.empty());
    }

    TEST(Empty, BufferNotCompletelyFilled) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        ASSERT_FALSE(buffer.empty());
    }
}

namespace Full_TESTS {
    TEST(Full, FullBuffer) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        buffer.push_back(7);
        buffer.push_back(1);
        buffer.push_front(10);
        ASSERT_TRUE(buffer.full());
    }

    TEST(Full, EmptyBuffer) {
        CircularBuffer<int> buffer(3);
        ASSERT_FALSE(buffer.full());
    }

    TEST(Full, BufferNotCompletelyFilled) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        ASSERT_FALSE(buffer.full());
    }
}

namespace Reserve_TESTS {
    TEST(Reserve, FullBuffer) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        buffer.push_back(7);
        buffer.push_back(1);
        buffer.push_front(10);
        ASSERT_EQ(buffer.reserve(), 0);
    }

    TEST(Reserve, EmptyBuffer) {
        CircularBuffer<int> buffer(3);
        ASSERT_EQ(buffer.reserve(), 3);
    }

    TEST(Reserve, BufferNotCompletelyFilled) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        ASSERT_EQ(buffer.reserve(), 1);
    }
}

namespace Capacity_TESTS {
    TEST(Capacity, AfterRewriting) {
        CircularBuffer<int> buffer(3);
        buffer.push_front(5);
        buffer.push_back(2);
        buffer.push_back(7);
        buffer.push_back(1);
        buffer.push_front(10);
        ASSERT_EQ(buffer.capacity(), 3);
    }

    TEST(Capacity, AfterInitialization) {
        CircularBuffer<int> buffer(3);
        ASSERT_EQ(buffer.capacity(), 3);
    }

    TEST(Capacity, AfterSettingCapacity) {
        CircularBuffer<int> buffer(3);
        buffer.set_capacity(10);
        ASSERT_EQ(buffer.capacity(), 10);
    }
}

namespace SetCapacity_TESTS {
    TEST(SetCapacity, WrongCapacity) {
        CircularBuffer<int> buffer(3);
        ASSERT_THROW(buffer.set_capacity(-10), std::bad_alloc);
    }

    TEST(SetCapacity, SmallerCapacity) {
        CircularBuffer<int> buffer(10);
        buffer.set_capacity(3);
        ASSERT_EQ(buffer.capacity(), 3);
    }

    TEST(SetCapacity, BiggerCapacity) {
        CircularBuffer<int> buffer(3);
        buffer.set_capacity(10);
        ASSERT_EQ(buffer.capacity(), 10);
    }

    TEST(SetCapacity, TheSameCapacity) {
        CircularBuffer<int> buffer(3);
        buffer.set_capacity(3);
        ASSERT_EQ(buffer.capacity(), 3);
    }
}

namespace Resize_TESTS {
    TEST(Resize, WrongCapacity) {
        CircularBuffer<int> buffer(3);
        ASSERT_THROW(buffer.resize(-10, 5), std::bad_alloc);
    }

    TEST(Resize, SmallerCapacity) {
        CircularBuffer<int> buffer(10);
        buffer.resize(3, 5);
        ASSERT_EQ(buffer.capacity(), 3);
    }

    TEST(Resize, BiggerCapacity) {
        CircularBuffer<int> buffer(3);
        buffer.resize(10, 5);
        ASSERT_EQ(buffer.capacity(), 10);
    }

    TEST(Resize, TheSameCapacity) {
        CircularBuffer<int> buffer(3);
        buffer.resize(3, 5);
        ASSERT_EQ(buffer.capacity(), 3);
    }
}

namespace Assignment_TESTS {
    TEST(Assignment, OneTime) {
        CircularBuffer<int> buffer1(3);
        CircularBuffer<int> buffer2(10);
        buffer2 = buffer1;
        ASSERT_EQ(buffer1, buffer2);
    }

    TEST(Assignment, MultipleTimes) {
        CircularBuffer<int> buffer1(3);
        CircularBuffer<int> buffer2(10);
        buffer2 = buffer1;
        buffer1 = buffer2;
        ASSERT_EQ(buffer1, buffer2);
    }
}

namespace Swap_TESTS {
    TEST(Swap, DifferentSizes) {
        CircularBuffer<int> buffer1(3, 3);
        CircularBuffer<int> buffer2(10, 4);
        buffer1.swap(buffer2);
        for (int i = 0; i < buffer1.size(); ++i)
            ASSERT_EQ(buffer1[i], 4);
        for (int i = 0; i < buffer2.size(); ++i)
            ASSERT_EQ(buffer2[i], 3);
    }
}
