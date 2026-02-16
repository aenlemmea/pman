#ifndef PMAN_PROJECT_MANAGER_HH
#define PMAN_PROJECT_MANAGER_HH
#include "project.hh"
#include "project_repository.hh"
#include <filesystem>
#include <string_view>
#include <vector>
namespace pman {

class ProjectManager {
  ProjectRepository repo;

  void validate(std::string_view name) const;

public:
  explicit ProjectManager(std::filesystem::path repo_root);
  explicit ProjectManager(std::filesystem::path repo_root,
                          const std::string_view repo_name);

  Project create(std::string_view name, Language lang,
                 std::string_view description);
  Project get(std::string_view name) const;
  std::vector<Project> list() const;
  void update_status(std::string_view name, Status status);
  void add_link(std::string_view name, std::string link);
};
}; // namespace pman
#endif
