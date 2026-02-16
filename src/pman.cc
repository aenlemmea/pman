#include "project_manager.hh"
#include "ui_components.hh"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

int main() {
  ScreenInteractive screen = ScreenInteractive::TerminalOutput();
  //  std::filesystem::remove_all(std::filesystem::temp_directory_path() /
  //                              "pmgr_ui_test1");
  //  std::filesystem::remove_all(std::filesystem::temp_directory_path() /
  //                              "sample");
  //  std::filesystem::remove_all(std::filesystem::temp_directory_path() /
  //                              "sample2");
  //  std::filesystem::remove_all(std::filesystem::temp_directory_path() /
  //                              "sample3");
  pman::ProjectManager pmgr(std::filesystem::temp_directory_path(),
                            "pmgr_ui_test1");
  pmgr.create("sample", pman::Language::UNKNOWN, "Sample Project");
  //  pmgr.create("sample2", pman::Language::UNKNOWN, "Sample Project");
  //  pmgr.create("sample3", pman::Language::UNKNOWN, "Sample Project");

  pman::LeftMenu left(pmgr);
  pman::RightPanel right(left);

  auto left_component = left.component();
  auto right_component = right.component();

  auto container = Container::Horizontal({
      left_component,
      right_component,
  });

  auto renderer = Renderer(container, [&] {
    return hbox({
        left_component->Render() | size(WIDTH, EQUAL, 30) | border,
        right_component->Render() | flex | border,
    });
  });

  screen.Loop(renderer);
  // std::filesystem::remove_all(std::filesystem::temp_directory_path() /
  //                             "pmgr_ui_test1");
  // std::filesystem::remove_all(std::filesystem::temp_directory_path() /
  //                             "sample");
  // std::filesystem::remove_all(std::filesystem::temp_directory_path() /
  //                             "sample2");
  // std::filesystem::remove_all(std::filesystem::temp_directory_path() /
  //                             "sample3");
}
