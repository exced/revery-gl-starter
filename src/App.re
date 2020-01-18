open Revery;

let init = app => {
  let width = 1200;
  let height = 1000;

  let win =
    App.createWindow(
      ~createOptions=WindowCreateOptions.create(~width, ~height, ()),
      app,
      "ReveryStarter",
    );

  let _ = UI.start(win, <Main screenWidth=width screenHeight=height />);
  ();
};

App.start(init);