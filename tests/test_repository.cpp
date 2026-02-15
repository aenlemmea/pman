#include "project.hh"
#include <filesystem>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "project_repository.hh"

using namespace pman;

TEST_CASE("Project save and load") {

  // $HOME/Project } /tmp/
  auto fpr = std::filesystem::temp_directory_path() / "pname_test";
  std::filesystem::remove_all(fpr);

  ProjectRepository pr{fpr};
  Project p{"NYANCAT", Language::GO, fpr / "nyancat"};
  pr.save(p);

  auto loaded = pr.load("NYANCAT");

  CHECK(p == loaded);
  std::filesystem::remove_all(fpr);
}

TEST_CASE("Project exists") {
  auto fpr = std::filesystem::temp_directory_path() / "pname_test2";
  std::filesystem::remove_all(fpr);

  ProjectRepository pr{fpr};
  Project p{"GOGOGO", Language::GO, fpr / "gogogo"};
  pr.save(p);

  CHECK(pr.exists("GOGOGO"));
  CHECK_FALSE(pr.exists("NYANCAT"));
  std::filesystem::remove_all(fpr);
}

TEST_CASE("List returns all projects") {
  auto root = std::filesystem::temp_directory_path() / "pman_test3";
  std::filesystem::remove_all(root);

  ProjectRepository repo(root);

  repo.save(Project{"a", Language::CPP, root / "a"});
  repo.save(Project{"b", Language::CPP, root / "b"});

  auto list = repo.list_projects();
  CHECK(list.size() == 2);

  std::filesystem::remove_all(root);
}

// TODO

// Project name with spaces
// Project name with unusual characters
// Empty links vector
// Multiple saves overwriting same project
// Non existing project load.
