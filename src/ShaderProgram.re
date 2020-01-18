open Sdl2.Gl;
open Revery_Shaders;

let attributes: list(ShaderAttribute.t) = [
  {dataType: ShaderDataType.Vector4, name: "aPosition", channel: Position},
  {dataType: ShaderDataType.Vector4, name: "aColor", channel: Color},
];

let uniforms: list(ShaderUniform.t) = [
  {dataType: ShaderDataType.Mat4, name: "uProjection", usage: VertexShader},
  {dataType: ShaderDataType.Mat4, name: "uModelView", usage: VertexShader},
];

let varying: list(ShaderVarying.t) = [
  {
    dataType: ShaderDataType.Vector4,
    name: "vColor",
    precision: ShaderPrecision.Low,
  },
];

let vertexShader = {|
  gl_Position = uProjection * uModelView * aPosition;
  vColor = aColor;
|};

let fragmentShader = {|
  gl_FragColor = vColor;
|};

type t = {
  compiledShader: CompiledShader.t,
  uProjection: uniformLocation,
  uModelView: uniformLocation,
};

let create = () => {
  let shader =
    Shader.create(
      ~attributes,
      ~uniforms,
      ~varying,
      ~vertexShader,
      ~fragmentShader,
    );
  let compiledShader = Shader.compile(shader);
  let uProjection =
    CompiledShader.getUniformLocation(compiledShader, "uProjection");
  let uModelView =
    CompiledShader.getUniformLocation(compiledShader, "uModelView");

  {compiledShader, uProjection, uModelView};
};