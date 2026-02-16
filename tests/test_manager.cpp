#include "doctest/doctest.h"
#include "project.hh"
#include "project_manager.hh"
#include <filesystem>

using namespace pman;
TEST_CASE("Manager creates project correctly") {
  auto root = std::filesystem::temp_directory_path() / "pman_mgr_test";
  std::filesystem::remove_all(root);

  ProjectManager pmgr{std::filesystem::temp_directory_path(), "pman_mgr_test"};
  auto p = pmgr.create("alpha", Language::CPP, "test_proj");
  auto proj_path = p.get_path();
  auto json_path = root / "alpha.json";
  auto repo_path = root;

  CHECK(p.get_name() == "alpha");
  CHECK(std::filesystem::exists(proj_path));
  CHECK(std::filesystem::exists(json_path));
  CHECK(std::filesystem::exists(repo_path));
  CHECK(std::filesystem::is_directory(proj_path));
  CHECK(std::filesystem::is_directory(repo_path));
  CHECK(std::filesystem::is_regular_file(json_path));
  CHECK_FALSE(std::filesystem::exists(repo_path / "alpha"));
  std::filesystem::remove_all(root);
  std::filesystem::remove_all(proj_path);
}

TEST_CASE("Duplicate project create throws") {
  auto root = std::filesystem::temp_directory_path() / "pman_mgr_dup_test";
  std::filesystem::remove_all(root);

  ProjectManager pmgr{std::filesystem::temp_directory_path(),
                      "pman_mgr_dup_test"};

  auto p = pmgr.create("beta", Language::RUST, "test proj");

  CHECK_THROWS(pmgr.create("beta", Language::CPP, "test proj"));

  std::filesystem::remove_all(root);
  std::filesystem::remove_all(root / p.get_path());
}

TEST_CASE("Update status persists") {
  auto root = std::filesystem::temp_directory_path() / "pman_mgr_status";
  std::filesystem::remove_all(root);

  ProjectManager pmgr(root, "pman_mgr_status");

  pmgr.create("alpha", pman::Language::CPP, "x");

  pmgr.update_status("alpha", pman::Status::DONE);

  auto p = pmgr.get("alpha");
  CHECK(p.get_status() == Status::DONE);
  std::filesystem::remove_all(root);
  std::filesystem::remove_all(p.get_path());
}

TEST_CASE("Add link persists") {
  auto root = std::filesystem::temp_directory_path() / "pman_mgr_links";
  std::filesystem::remove_all(root);

  pman::ProjectManager pmgr(root, "pman_mgr_links");

  pmgr.create("alpha", pman::Language::CPP, "x");

  pmgr.add_link("alpha", "https://example.com");

  auto p = pmgr.get("alpha");

  CHECK(p.get_links().size() == 1);
  CHECK(p.get_links()[0] == "https://example.com");

  std::filesystem::remove_all(root);
  std::filesystem::remove_all(p.get_path());
}
