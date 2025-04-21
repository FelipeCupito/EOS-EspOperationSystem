#include "gtest/gtest.h"
#include "eos_shared_services.hpp"

// Un peripheral de ejemplo para testear SharedBase
struct Dummy {
    int x = 42;
    template<enum Var> static decltype(auto) access_shared(Dummy& d);
    enum class Var : uint8_t { x };
};

template<>
decltype(auto) Dummy::access_shared<Dummy::Var::x>(Dummy& d) {
    return d.x;
}

TEST(SharedBaseTest, ReadWriteAccess) {
    Dummy d;
    SharedBase<Dummy, Dummy::Var> sb(d);
    // get<Var::x>() devuelve referencia int&
    auto& ref = sb.get<Dummy::Var::x>();
    EXPECT_EQ(ref, 42);
    ref = 7;
    EXPECT_EQ(d.x, 7);
}

