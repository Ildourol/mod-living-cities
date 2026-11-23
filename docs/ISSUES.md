# Living Cities

## LC-001 — Native city movement and services

**Status** — Implemented in source. In-game acceptance remains pending.

- `AdmitResident` used a random POI and a forced landing. Its strategy expression included `+stay,-rpg`. In this Playerbots checkout, `CanMoveAroundValue` refuses movement under `stay`, so native RPG cannot choose service targets. `MaintainResidents` could reapply strategies and relocate again, while `ExecuteVisitNpc` supplied ambient behavior for POIs without guaranteeing a real trainer or vendor interaction.
- Native RPG is now the default. Local, eligible, ungrouped bots are admitted in the actual map and zone. Towns sharing an outdoor zone also require proximity to a town POI. Admission applies one noncombat strategy transition, and the module leaves travel within the city to Playerbots. The scripted mode remains available through `CityLife.Strategies.NativeRpg = 0`.
- `TryNativeService` can offer a nearby vendor or repairer only when native RPG has no goal, relevant native need and budget checks pass, and the NPC is alive, friendly, visible and in phase. It respects native travel and the RPG ignore list. The native scheduler handles approach, buying and repair. In range, it can run the registered `rpg sell` or `rpg train` action if core's NPC interaction check and native action eligibility pass. It sends no simulated NPC interaction. It leaves an existing RPG target in place.
- Release drops the lease or restores the recorded changes as ownership allows. Native residents leave on combat, grouping, human takeover, hub exit or other invalid context. They do not enter the scripted inn cycle or return-teleport path. Release clears a module-offered RPG target only if it still matches.

### Verification

- `python tests/run_all_tests.py --module mod-living-cities --verbose` passed one Python suite and ten SQL text audits. Running `python mod-living-cities/tests/test_native_hub_data.py -v` directly passed three tests.
- `tests/test_native_policy.cpp` exercises production service prioritization and strategy journaling. It was added as a standalone executable test and left unbuilt.
- Runtime acceptance should follow a needy bot to a vendor or repairer, confirm a transaction through actual inventory, money or durability changes, verify class and budget restrictions on training, and observe recovery from an unavailable NPC, grouping, combat, hub exit and human takeover.
