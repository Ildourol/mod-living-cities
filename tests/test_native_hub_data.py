"""Validate the source configuration and seed data consumed by local city admission."""

import csv
from pathlib import Path
import re
import unittest


MODULE = Path(__file__).resolve().parents[1]


class NativeHubDataTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        template = (MODULE / "conf/mod_living_cities.conf.dist").read_text(encoding="utf8")
        cls.config = dict(re.findall(r"^(CityLife\.[\w.]+)\s*=\s*(.+)$", template, re.MULTILINE))
        manual = (MODULE / "data/sql/manual/world_city_life.sql").read_text(encoding="utf8")
        values = manual.split("INSERT IGNORE INTO `city_life_hub`", 1)[1].split("VALUES", 1)[1].split(";", 1)[0]
        rows = re.findall(r"^\(([^\n]+)\)", values, re.MULTILINE)
        cls.seed_hubs = {
            row[0]: tuple(map(int, row[1:]))
            for row in csv.reader(rows, quotechar="'", skipinitialspace=True)
        }
        bootstrap = (
            MODULE / "data/sql/world/updates/2026_10_11_01_city_life_bootstrap_repair.sql"
        ).read_text(encoding="utf8")
        cls.zones = {
            name: int(zone)
            for zone, name in re.findall(r"SET `zone_id` = (\d+) WHERE `name` = '(\w+)'", bootstrap)
        }
        for zone, names in re.findall(r"SET `zone_id` = (\d+) WHERE `name` IN \(([^)]+)\)", bootstrap):
            cls.zones.update({name: int(zone) for name in re.findall(r"'(\w+)'", names)})

    def test_native_rpg_is_the_default_mode(self):
        self.assertEqual(self.config["CityLife.Strategies.NativeRpg"], "1")

    def test_enabled_seed_hubs_have_zone_and_valid_level_ranges(self):
        for name, (enabled, team, minimum, maximum, _map, _population, _priority) in self.seed_hubs.items():
            if not enabled:
                continue
            with self.subTest(hub=name):
                self.assertGreater(self.zones[name], 0)
                self.assertIn(team, (0, 1, 2))
                self.assertTrue(1 <= minimum <= maximum <= 80)

    def test_faction_towns_sharing_hellfire_keep_separate_anchors(self):
        self.assertEqual(self.zones["HonorHold"], self.zones["Thrallmar"])
        self.assertEqual(self.seed_hubs["HonorHold"][1], 0)
        self.assertEqual(self.seed_hubs["Thrallmar"][1], 1)
        manual = (MODULE / "data/sql/manual/world_city_life.sql").read_text(encoding="utf8")
        anchors = {}
        for hub in ("HonorHold", "Thrallmar"):
            coordinates = re.search(rf"SELECT '{hub}','\w+','square',([^,]+),([^,]+),([^,]+)", manual)
            anchors[hub] = tuple(map(float, coordinates.groups()))
        self.assertGreater(abs(anchors["HonorHold"][0] - anchors["Thrallmar"][0]), 200.0)


if __name__ == "__main__":
    unittest.main()
