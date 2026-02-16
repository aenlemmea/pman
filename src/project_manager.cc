#include "project_manager.hh"
#include "project.hh"
#include "project_repository.hh"
#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <stdexcept>

namespace pman {

// Validate the name.
void ProjectManager::validate(std::string_view name) const {

  if (name.empty())
    throw std::runtime_error("File name cannot be empty");

  static constexpr std::string_view invalid_char = R"(<>!:/\"\|?*)";
  if (std::find_first_of(name.begin(), name.end(), invalid_char.begin(),
                         invalid_char.end()) != name.end()) {
    throw std::runtime_error("File name has invalid char");
  }

  if (name == "." || name == "..") {
    throw std::runtime_error("File name has invalid char");
  }

  if (name.front() == ' ' || name.back() == ' ') {
    throw std::runtime_error("File name has space at start or end.");
  }

  if (std::all_of(name.begin(), name.end(),
                  [](unsigned char x) { return std::isspace(x); }))
    throw std::runtime_error("File name cannot be all space");

  if (name.size() > 128)
    throw std::runtime_error("File name is too long");
}

ProjectManager::ProjectManager(std::filesystem::path repo_root)
    : repo(repo_root) {}

ProjectManager::ProjectManager(std::filesystem::path repo_root,
                               const std::string_view repo_name)
    : repo(repo_root, repo_name) {}

Project ProjectManager::create(std::string_view name, Language lang,
                               std::string_view description) {
  validate(name);
  if (repo.exists(name))
    throw std::runtime_error("Duplicate project");

  auto fp = repo.get_repo_root_path() / name;
  if (std::filesystem::exists(fp))
    throw std::runtime_error("Project already exists @ " +
                             repo.get_repo_root_path().string());

  // TODO Advanced creation, instead of just a directory.
  std::filesystem::create_directories(fp);

  Project p{name,
            {},
            std::chrono::system_clock::now(),
            Status::PLANNED,
            lang,
            "",
            description,
            fp};

  // TODO Tie repo save to create directories to avoid sync issues.
  repo.save(p);
  return p;
}
Project ProjectManager::get(std::string_view name) const {
  if (!repo.exists(name))
    throw std::runtime_error("Project does not exists");

  return repo.load(name);
}
std::vector<Project> ProjectManager::list() const {
  return repo.list_projects();
}
void ProjectManager::update_status(std::string_view name, Status status) {
  auto p = repo.load(name);
  p.update_status(status);
  repo.save(p);
}
void ProjectManager::add_link(std::string_view name, std::string link) {
  // TODO validate_link
  auto p = repo.load(name);
  p.add_link(link);
  repo.save(p);
}
}; // namespace pman
