#include "main_control.cc"
#include "project_manager.hh"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

int main() {
  pman::ProjectManager pmgr(std::filesystem::temp_directory_path(),
                            "pmgr_ui_test1");

  pman::MainControl ui(pmgr);
  ui.run();

  return 0;
}
