#include "../src/CityLifeNativePolicy.h"

#include <stdexcept>

namespace
{
    void Expect(bool condition, char const* message)
    {
        if (!condition)
            throw std::runtime_error(message);
    }
}

int main()
{
    using CityLife::NativeServiceRadius;
    using CityLife::PreferNativeService;
    Expect(!PreferNativeService(false, false, 2.0f, false, NativeServiceRadius),
        "an NPC without a needed service is ineligible");
    Expect(!PreferNativeService(true, true, 31.0f, false, NativeServiceRadius),
        "services outside the local scan are ineligible");
    Expect(PreferNativeService(false, true, 8.0f, false, NativeServiceRadius),
        "a seller can visit a plain vendor");
    Expect(PreferNativeService(true, false, 20.0f, false, 8.0f),
        "needed repairs prefer a viable repair service");
    Expect(!PreferNativeService(false, true, 2.0f, true, 20.0f),
        "vendor distance alone cannot displace needed repairs");
    Expect(PreferNativeService(true, true, 10.0f, true, 20.0f),
        "among needed repair services the nearer one wins");

    std::vector<std::string> before = {"duel", "grind", "new rpg"};
    std::vector<std::string> after = {"duel", "rpg"};
    std::vector<std::string> added, removed;
    CityLife::RecordStrategyDelta(before, after, added, removed);
    CityLife::RecordStrategyDelta(before, after, added, removed);
    Expect(added == std::vector<std::string>{"rpg"}, "the journal records only actual additions");
    Expect(removed == std::vector<std::string>{"grind", "new rpg"}, "removals and sibling changes are recorded once");
    CityLife::RecordStrategyDelta(after, after, added, removed);
    Expect(added.size() == 1 && removed.size() == 2, "a strategy no-op leaves the journal intact");
}
