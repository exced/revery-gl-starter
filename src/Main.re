open Reglm;
open Sdl2;
open Sdl2.Gl;
open Revery;
open Revery.Key;
open Revery.UI;
open Revery.Draw;
open Revery.Shaders;
open Revery.Geometry;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  let container =
    Style.[
      position(`Absolute),
      top(0),
      bottom(0),
      left(0),
      right(0),
      alignItems(`Center),
      justifyContent(`Center),
      flexDirection(`Column),
    ];
};

type action =
  | Rotate;

type state = {angle: float};

let reducer = (action, state) => {
  switch (action) {
  | Rotate => {angle: state.angle +. 1.}
  };
};

let%component make = (~screenWidth, ~screenHeight, ()) => {
  let%hook ({angle}, dispatch) =
    Hooks.reducer(~initialState={angle: 0.}, reducer);

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let clear =
          Tick.interval(_ => dispatch(Rotate), Time.milliseconds(5.));
        Some(clear);
      },
    );

  <View style=Styles.container>
    <OpenGL
      style=Style.[height(screenHeight), width(screenWidth)]
      render={(_transform, _pctx) => {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClearDepth(1.0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        let fov = 45. *. pi /. 180.;
        let aspect = float_of_int(screenWidth) /. float_of_int(screenHeight);
        let projection = Mat4.create();

        Mat4.perspective(projection, fov, aspect, 0.1, 100.);

        let modelView = Mat4.create();

        let translation = Mat4.create();
        Mat4.fromTranslation(translation, Vec3.create(-0.0, 0.0, -6.0));
        Mat4.multiply(modelView, modelView, translation);

        let rotationZ = Mat4.create();
        Mat4.fromRotation(
          rotationZ,
          Angle.from_degrees(angle),
          Vec3.create(0., 0., 1.),
        );
        Mat4.multiply(modelView, modelView, rotationZ);

        let rotationY = Mat4.create();
        Mat4.fromRotation(
          rotationY,
          Angle.from_degrees(angle *. 0.7),
          Vec3.create(0., 1., 0.),
        );
        Mat4.multiply(modelView, modelView, rotationY);

        let geometry = BoxGeometry.create();

        let shaderProgram = ShaderProgram.create();
        CompiledShader.use(shaderProgram.compiledShader);

        CompiledShader.setUniformMatrix4fv(
          shaderProgram.uProjection,
          projection,
        );
        CompiledShader.setUniformMatrix4fv(
          shaderProgram.uModelView,
          modelView,
        );
        Revery.Geometry.draw(geometry, shaderProgram.compiledShader);
        ();
      }}
    />
  </View>;
};