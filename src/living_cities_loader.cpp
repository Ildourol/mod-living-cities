/*
 * Living Cities
 * Copyright (C) 2026 iCore
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

void AddCityLifeScripts();

void Addmod_living_citiesScripts()
{
    AddCityLifeScripts();
}

// Backward compatibility alias for legacy loader invocations
void Addmod_playerbots_city_lifeScripts()
{
    Addmod_living_citiesScripts();
}
