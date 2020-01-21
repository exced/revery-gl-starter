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

  let rotate = _ => dispatch(Rotate);

  let%hook () = Hooks.tick(~tickRate=Time.milliseconds(5.), rotate);

  <OpenGL
    style=Style.[height(screenHeight), width(screenWidth)]
    render={(_transform, _pctx) => {
      glClearColor(0., 0., 0., 1.);
      glClearDepth(1.);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);

      let fov = 45. *. pi /. 180.;
      let aspect = float_of_int(screenWidth) /. float_of_int(screenHeight);

      let projection = Mat4.create();
      Mat4.perspective(projection, fov, aspect, 0.1, 100.);

      let translation = Mat4.create();
      Mat4.fromTranslation(translation, Vec3.create(0., 0., -10.));

      let rotationZ = Mat4.create();
      Mat4.fromRotation(
        rotationZ,
        Angle.from_degrees(angle),
        Vec3.create(0., 0., 1.),
      );

      let rotationY = Mat4.create();
      Mat4.fromRotation(
        rotationY,
        Angle.from_degrees(angle *. 0.7),
        Vec3.create(0., 1., 0.),
      );

      let modelView = Mat4.create();
      Mat4.multiply(modelView, modelView, translation);
      Mat4.multiply(modelView, modelView, rotationZ);
      Mat4.multiply(modelView, modelView, rotationY);

      let geometry = BoxGeometry.create();

      let shaderProgram = ShaderProgram.create();
      CompiledShader.use(shaderProgram.compiledShader);

      CompiledShader.setUniformMatrix4fv(
        shaderProgram.uProjection,
        projection,
      );
      CompiledShader.setUniformMatrix4fv(shaderProgram.uModelView, modelView);
      Revery.Geometry.draw(geometry, shaderProgram.compiledShader);
      ();
    }}
  />;
};