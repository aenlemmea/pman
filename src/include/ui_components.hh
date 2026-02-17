#ifndef PMAN_UI_COMPONENTS_HH
#define PMAN_UI_COMPONENTS_HH

#include "project.hh"
#include "project_manager.hh"
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <string_view>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

namespace pman {
enum class UIMode { Normal, UpdateProject, NewProject, OpenProject };

class LeftMenu {
public:
  explicit LeftMenu(ProjectManager &pmgr);

  ftxui::Component component();
  const std::string_view selected_project() const;
  void refresh();

private:
  ProjectManager &pmgr;
  std::vector<Project> projects;
  std::vector<std::string> project_names;
  int32_t selected = 0;

  friend class RightPanel;
};

class RightPanel {
public:
  explicit RightPanel(LeftMenu &left);

  ftxui::Component component();

private:
  LeftMenu &left_menu;
};

class NewProjectPanel {
public:
  NewProjectPanel(pman::ProjectManager &pmgr, pman::LeftMenu &left)
      : pmgr(pmgr), left_menu(left) {

    using namespace ftxui;

    InputOption opt;
    opt.multiline = false;

    name_input = Input(&name, "", opt);
    desc_input = Input(&description, "", opt);

    languages = {"C++", "Python", "C", "Go", "Rust", "Scripting", "Unknown"};
    lang_menu = Menu(&languages, &lang_selected);

    create_button = Button("Create", [&] {
      try {
        pmgr.create(name,
                    Project::language_from_string(languages[lang_selected]),
                    description);

        left_menu.refresh();
        clear();
        visible = false;
      } catch (...) {
        // TODO: add error message display
      }
    });

    form = Container::Vertical({
        name_input,
        desc_input,
        lang_menu,
        create_button,
    });

    component_ = Renderer(form, [&] {
      if (!visible)
        return ftxui::emptyElement();

      return ftxui::window(ftxui::text(" New Project "),
                           ftxui::vbox({
                               ftxui::text("Name:"),
                               name_input->Render(),
                               ftxui::separator(),
                               ftxui::text("Description:"),
                               desc_input->Render(),
                               ftxui::separator(),
                               ftxui::text("Language:"),
                               lang_menu->Render(),
                               ftxui::separator(),
                               create_button->Render(),
                           })) |
             ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 50) | ftxui::center;
    });

    component_ |= ftxui::CatchEvent([&](ftxui::Event event) {
      if (event == ftxui::Event::Escape && visible) {
        clear();
        visible = false;
        return true;
      }
      return false;
    });
  }

  ftxui::Component component() { return component_; }

  void set_visible(bool v) {
    visible = v;
    if (visible) {
      clear();
      form->SetActiveChild(name_input);
    }
  }

  bool is_visible() const { return visible; }

private:
  void clear() {
    name.clear();
    description.clear();
    lang_selected = 0;
  }

  pman::ProjectManager &pmgr;
  pman::LeftMenu &left_menu;

  bool visible = false;

  std::string name;
  std::string description;

  std::vector<std::string> languages;
  int lang_selected = 0;

  ftxui::Component name_input;
  ftxui::Component desc_input;
  ftxui::Component lang_menu;
  ftxui::Component create_button;
  ftxui::Component form;
  ftxui::Component component_;
};

} // namespace pman
#endif
