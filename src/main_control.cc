#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "project_manager.hh"
#include "ui_components.hh"
namespace pman {
struct MainControl {
  explicit MainControl(pman::ProjectManager &pmgr)
      : pmgr(pmgr), left(pmgr), right(left), npp(pmgr, left) {

    using namespace ftxui;

    left_component = left.component();
    right_component = right.component();
    npp_component = npp.component();

    body_container = Container::Horizontal({
        left_component,
        right_component,
    });

    body_renderer = Renderer(body_container, [&] {
      return hbox({
          left_component->Render() | size(WIDTH, EQUAL, 30) | border,
          right_component->Render() | flex | border,
      });
    });

    root_container = Container::Stacked({
        body_renderer,
        npp_component,
    });

    root = Renderer(root_container, [&] {
      if (npp.is_visible()) {
        root_container->SetActiveChild(npp_component);
        return npp_component->Render();
      } else {
        root_container->SetActiveChild(body_renderer);
        return body_renderer->Render();
      }
    });

    root |= CatchEvent([&](ftxui::Event event) {
      if (event == ftxui::Event::Character('n') && !npp.is_visible()) {
        npp.set_visible(true);
        return true;
      }

      if (event == ftxui::Event::Escape && !npp.is_visible()) {
        screen.Exit();
        return true;
      }

      return false;
    });
  }

  void run() { screen.Loop(root); }

private:
  pman::ProjectManager &pmgr;

  // UI Parts
  pman::LeftMenu left;
  pman::RightPanel right;
  pman::NewProjectPanel npp;

  // Components
  ftxui::Component left_component;
  ftxui::Component right_component;
  ftxui::Component npp_component;

  ftxui::Component body_container;
  ftxui::Component body_renderer;
  ftxui::Component root_container;
  ftxui::Component root;

  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::TerminalOutput();
};
} // namespace pman
