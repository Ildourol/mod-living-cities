/*
 * Living Cities
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef CITY_LIFE_NATIVE_POLICY_H
#define CITY_LIFE_NATIVE_POLICY_H

#include <algorithm>
#include <string>
#include <vector>

namespace CityLife
{
    constexpr float NativeServiceRadius = 30.0f;

    // Callers supply only live, reachable-by-context NPCs. Prefer a needed
    // repair service, then distance. A vendor remains useful when none repairs.
    inline bool PreferNativeService(bool repairs, bool vendor, float distance, bool bestRepairs, float bestDistance)
    {
        if ((!repairs && !vendor) || !(distance < NativeServiceRadius))
            return false;
        return repairs != bestRepairs ? repairs : distance < bestDistance;
    }

    inline void RecordStrategyDelta(std::vector<std::string> const& before, std::vector<std::string> const& after,
        std::vector<std::string>& added, std::vector<std::string>& removed)
    {
        auto record = [](auto const& from, auto const& other, auto& changes)
        {
            for (auto const& strategy : from)
                if (std::find(other.begin(), other.end(), strategy) == other.end() &&
                    std::find(changes.begin(), changes.end(), strategy) == changes.end())
                    changes.push_back(strategy);
        };
        record(after, before, added);
        record(before, after, removed);
    }
}

#endif
