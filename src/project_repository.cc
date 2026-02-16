#include "include/project_repository.hh"
#include "include/project.hh"
#include <filesystem>
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>
#include <string_view>

namespace pman {

ProjectRepository::ProjectRepository(const std::filesystem::path &repo_root)
    : repository_path(repo_root) {
  std::filesystem::create_directories(get_repo_path());
}

ProjectRepository::ProjectRepository(const std::filesystem::path &repo_root,
                                     const std::string_view repo_name_ov)
    : repository_path(repo_root), repo_name(repo_name_ov) {
  std::filesystem::create_directories(get_repo_path());
}

void ProjectRepository::save(const Project &proj) {
  nlohmann::json j = proj;

  auto jfp = get_repo_path() / proj.get_name();
  jfp += ".json";

  std::ofstream os{jfp};
  if (!os)
    throw std::runtime_error("Failed to open project file");

  os << std::setw(4) << j << '\n';
}

Project ProjectRepository::load(const std::string_view name) const {
  auto jfp = get_repo_path() / name;
  jfp += ".json";

  std::ifstream is{jfp};
  if (!is) {
    throw std::runtime_error("Could not find: " + std::string(name) +
                             " to load.\n");
  }
  nlohmann::json j;
  is >> j;
  Project p{name, Language::UNKNOWN, get_repo_path() / name};
  from_json(j, p);
  return p;
}

std::vector<Project> ProjectRepository::list_projects() const {
  std::vector<Project> projects;
  auto repo_path = get_repo_path();

  if (!std::filesystem::exists(repo_path))
    return projects;

  for (const auto &entry : std::filesystem::directory_iterator(repo_path)) {
    if (!entry.is_regular_file())
      continue;

    if (entry.path().extension() != ".json")
      continue;

    auto name = entry.path().stem().string(); // filename without .json
    try {
      projects.push_back(load(name));
    } catch (...) {
      // TODO: Add exception handling.
    }
  }

  return projects;
}

// Checks if a registry entry exists for a project.
bool ProjectRepository::exists(const std::string_view name) const {
  auto jfp = get_repo_path() / name;
  jfp += ".json";
  return std::filesystem::exists(jfp);
}

}; // namespace pman
