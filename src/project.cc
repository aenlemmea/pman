#include "include/project.hh"
#include <chrono>
#include <ctime>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <ostream>

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

std::ostream &operator<<(std::ostream &os, const Project &proj) {
  os << "Name: " << proj.name << "\nDescription: " << proj.description
     << "\nStatus: " <<
      [&]() {
        switch (proj.status) {
        case Status::PLANNED:
          return "PLANNED";
        case Status::START:
          return "START";
        case Status::ABANDONED:
          return "ABANDONED";
        case Status::DONE_ARCHIVED:
          return "DONE_ARCHIVED";
        case Status::POLISH:
          return "POLISH";
        case Status::DONE:
          return "DONE";
        }
        return "Unknown";
      }()
     << "\nLanguage: " <<
      [&]() {
        switch (proj.language) {
        case Language::C:
          return "C";
        case Language::CPP:
          return "C++";
        case Language::RUST:
          return "Rust";
        case Language::GO:
          return "Go";
        case Language::PYTHON:
          return "Python";
        case Language::SCRIPT:
          return "Scripting";
        case Language::UNKNOWN:
          return "Unknown";
        }
        return "Unknown";
      }()
     << "\nCreated At: " << std::format("{:%Y-%m-%d %H:%M:%S}", proj.created_at)
     << "\nProject Path: " << proj.proj_path
     << "\nGit Remote: " << proj.git_remote << "\nLinks: " << "[\n";
  for (const auto &item : proj.links)
    os << item << "\n";
  os << "\n]";
  return os;
}
}; // namespace pman
