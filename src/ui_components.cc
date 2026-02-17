#include "ui_components.hh"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
namespace pman {

// TODO Make pmgr directly give project names.
LeftMenu::LeftMenu(ProjectManager &pmgr) : pmgr(pmgr) { refresh(); }

void LeftMenu::refresh() {
  projects = pmgr.list();
  project_names.clear();
  for (const auto &e : projects) {
    project_names.emplace_back(e.get_name());
  }
}

ftxui::Component LeftMenu::component() {
  return ftxui::Menu(&project_names, &selected);
}

const std::string_view LeftMenu::selected_project() const {
  if (projects.empty())
    return "";
  return projects[selected].get_name();
}

RightPanel::RightPanel(LeftMenu &left) : left_menu(left) {}

ftxui::Component RightPanel::component() {
  using namespace ftxui;

  return Renderer([&] {
    auto name = left_menu.selected_project();
    if (name.empty())
      return text("No projects found.") | center;

    std::ostringstream oss;
    oss << left_menu.pmgr.get(name);
    return paragraphAlignLeft(oss.str());
  });
}

} // namespace pman
