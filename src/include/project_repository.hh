#ifndef PMAN_PROJECT_REPOSITORY_HH
#define PMAN_PROJECT_REPOSITORY_HH

#include "project.hh"
#include <filesystem>
#include <string_view>

namespace pman {

class ProjectRepository {

  std::string_view repo_name{".pman"};
  std::filesystem::path repository_path;

public:
  explicit ProjectRepository(const std::filesystem::path &repo_root);
  explicit ProjectRepository(const std::filesystem::path &repo_root,
                             const std::string_view repo_name_ov);

  const std::filesystem::path get_repo_path() const noexcept {
    return repository_path / repo_name;
  }

  const std::filesystem::path get_repo_root_path() const noexcept {
    return repository_path;
  }

  void save(const Project &proj);
  Project load(const std::string_view name) const;
  std::vector<Project> list_projects() const;
  bool exists(const std::string_view name) const;
  void delete_project(const std::string_view name); // TODO
};

}; // namespace pman

#endif
