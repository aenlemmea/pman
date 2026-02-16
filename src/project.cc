#include "include/project.hh"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>

namespace pman {
void to_json(nlohmann::json &j, const Project &p) {
  j = {
      {"name", p.name},
      {"created_at", std::chrono::system_clock::to_time_t(p.created_at)},
      {"status", p.status},
      {"links", p.links},
      {"description", p.description},
      {"language", p.language},
      {"git_remote", p.git_remote},
      {"proj_path", p.proj_path.string()},
  };
}
void from_json(const nlohmann::json &j, Project &p) {
  j.at("name").get_to(p.name);
  std::time_t tm = j.at("created_at").get<std::time_t>();
  p.created_at = std::chrono::system_clock::from_time_t(tm);
  j.at("status").get_to(p.status);
  j.at("links").get_to(p.links);
  j.at("language").get_to(p.language);
  j.at("git_remote").get_to(p.git_remote);
  auto pz = j.at("proj_path").get<std::string>();
  p.proj_path = std::filesystem::path{pz};
  j.at("description").get_to(p.description);
}
}; // namespace pman
