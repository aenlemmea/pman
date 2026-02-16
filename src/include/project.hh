// A Project Record
#ifndef PMAN_PROJECT_HH
#define PMAN_PROJECT_HH

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <string_view>
#include <vector>
namespace pman {

enum class Status : uint8_t {
  PLANNED,
  START,
  POLISH,
  DONE,
  DONE_ARCHIVED,
  ABANDONED,
};

enum class Language {
  CPP,
  GO,
  C,
  RUST,
  PYTHON,
  SCRIPT,
  UNKNOWN,
};

NLOHMANN_JSON_SERIALIZE_ENUM(Status,
                             {
                                 {Status::PLANNED, "PLANNED"},
                                 {Status::START, "START"},
                                 {Status::POLISH, "POLISH"},
                                 {Status::DONE, "DONE"},
                                 {Status::DONE_ARCHIVED, "DONE_ARCHIVED"},
                                 {Status::ABANDONED, "ABANDONED"},

                             })

NLOHMANN_JSON_SERIALIZE_ENUM(Language, {
                                           {Language::CPP, "CPP"},
                                           {Language::GO, "GO"},
                                           {Language::C, "C"},
                                           {Language::RUST, "RUST"},
                                           {Language::PYTHON, "PYTHON"},
                                           {Language::SCRIPT, "SCRIPT"},
                                           {Language::UNKNOWN, "UNKNOWN"},

                                       })

class Project {
  std::string name;
  std::vector<std::string> links;
  std::chrono::system_clock::time_point created_at;
  Status status;
  Language language;
  std::string git_remote;
  std::string description;
  std::filesystem::path proj_path;

  Project() = delete;

public:
  explicit Project(std::string_view nm, std::vector<std::string> &&links,
                   std::chrono::system_clock::time_point cat, Status st,
                   Language lang, std::string_view gitr,
                   std::string_view description, const std::filesystem::path &p)
      : name{nm}, links(std::move(links)), created_at(cat), status(st),
        language(lang), git_remote(gitr), description(description),
        proj_path(p) {}

  explicit Project(std::string_view nm, Language lang,
                   const std::filesystem::path &p)
      : name(nm), language(lang), proj_path(p),
        created_at(std::chrono::system_clock::now()), status(Status::PLANNED) {}

  std::string_view get_name() const noexcept { return name; }
  const auto &get_path() const { return proj_path; }
  const Status &get_status() const noexcept { return status; }
  const std::vector<std::string> &get_links() const noexcept { return links; }

  void update_name(std::string_view new_name) { name = new_name; }
  void update_status(Status st) { status = st; }
  void add_link(const std::string &link) { links.push_back(link); }
  void update_description(std::string_view desc) { description = desc; }
  void update_path(const std::filesystem::path &path) { proj_path = path; }

  friend void to_json(nlohmann::json &j, const Project &p);
  friend void from_json(const nlohmann::json &j, Project &p);

  bool operator==(const Project &p) const {
    return (name == p.name && p.proj_path == proj_path);
  }
};
}; // namespace pman

#endif
