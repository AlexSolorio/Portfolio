/*
 * COMP3801 Winter 2018 Assignment 2
 *  
 * Scene object - define model placement in world coordinates
 */

/*
 * Constructor for Scene object. This object holds a list of models to render,
 * and a transform for each one.  The list is defined in a JSON file.  The
 * field named "models" in the JSON file defines the list.  This field is an
 * array of objects. Each object contains the "modelURL", a scale factor,
 * and the placement of the model frame in world frame as vec3 values for the
 * "location" point and "xBasis", "yBasis", and "zBasis" vectors for the frame.
 * The scale factor should be applied to the points before applying the frame
 * transform.
 * 
 * @param canvasID - string ID of canvas in which to render
 * @param sceneURL - URL of JSON file containing scene definition
 */

function Scene(canvasID, sceneURL) {
  // Set up WebGL context
  var t = this;
  this.canvasID = canvasID;
  var canvas = this.canvas = document.getElementById(canvasID);
  if (!canvas) {
      alert("Canvas ID '" + canvasID + "' not found.");
      return;
  }
  var gl = this.gl = WebGLUtils.setupWebGL(this.canvas);
  if (!gl) {
      alert("WebGL isn't available in this browser");
      return;
  }
  this.lookAtModelNum = 0;
  this.xChange = 0;
  this.yChange = 0;
  this.prevXRotation = 0;
  this.prevYRotation = 0;
  this.targetNumber = 1;
  
  // Add key press event handler
  canvas.addEventListener("keypress", function(event) { t.KeyInput(event); });
  
  // Load the scene definition
  var jScene = this.jScene = LoadJSON(sceneURL);
  if (jScene === null) return;  // scene load failed (LoadJSON alerts on error)

  // Set up WebGL rendering settings
  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.enable(gl.DEPTH_TEST);
  var bgColor = [ 0, 0, 0, 1 ];
  if ("bgColor" in jScene) {
    bgColor = jScene["bgColor"];
  }
  gl.clearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
  
  // Set up User Interface elements
  this.fovSliderID = canvasID + "-fov-slider";
  this.fovSlider = document.getElementById(this.fovSliderID);
  
  this.nearSliderID = canvasID + "-near-slider";
  this.nearSlider = document.getElementById(this.nearSliderID);
  
  this.farSliderID = canvasID + "-far-slider";
  this.farSlider = document.getElementById(this.farSliderID);
  
  this.zSliderID = canvasID + "-z-slider";
  this.zSlider = document.getElementById(this.zSliderID);
  
  this.perspectiveCheckBoxID = canvasID + "-projection";
  this.perspectiveCheckBox = document.getElementById(this.perspectiveCheckBoxID);
  
  // Get the initial camera parameters (copy values so we can change them
  // without modifying the jScene object, we might want the original values
  // to do a reset.
  this.ResetCamera();
  
  // Load each model in the scene
  var loadedModels = this.loadedModels = [];  // array of models
  for (var i = 0; i < jScene.models.length; ++i) {
    // Load model from JSON and add to loadedModels array
    var jModel = jScene.models[i];
    var model = new JSONModel(gl, jModel.modelURL);
    if (model === null) return;  // failed to load a model
    loadedModels.push(model);
  }
  
  //first model locations starts off as the average of the second and third model locations
  this.jScene.models[0].location = [  (this.jScene.models[1].location[0] + this.jScene.models[2].location[0]) / 2,
                                      (this.jScene.models[1].location[1] + this.jScene.models[2].location[1]) / 2,
                                      (this.jScene.models[1].location[2] + this.jScene.models[2].location[2]) / 2];
  
  //compute basis vectors for rocketship model
  this.ComputeBasisVectors();
  this.mouseDown = false;
  
  canvas.addEventListener("mousedown", function (e) {
    t.mouseDown = true;
    
    var currentX = e.pageX - e.target.offsetLeft;
    var currentY = e.pageY - e.target.offsetTop;
    
    var currentXClip = ((2.0 * currentX / (canvas.width - 1)) - 1.0).toFixed(3);
    var currentYClip = (1.0 - (2.0 * currentY / (canvas.height - 1))).toFixed(3);
    
    t.prevX = currentXClip;
    t.prevY = currentYClip;
  });
  
  canvas.addEventListener("mouseup", function (e) {
    t.mouseDown = false;
    
    t.prevXRotation = t.prevXRotation + t.tempRotateX;
    t.tempRotateX = 0;
    t.xChange = 0;
    
    t.prevYRotation = t.prevYRotation + t.tempRotateY;
    t.tempRotateY = 0;
    t.yChange = 0;
  });
  
  canvas.addEventListener("mousemove", function (e) {
    
    if (t.mouseDown) {
      var currentX = e.pageX - e.target.offsetLeft;
      var currentY = e.pageY - e.target.offsetTop;
    
      var currentXClip = ((2.0 * currentX / (canvas.width - 1)) - 1.0).toFixed(3);
      var currentYClip = (1.0 - (2.0 * currentY / (canvas.height - 1))).toFixed(3);
    
      t.xChange = currentXClip - t.prevX;
      t.yChange = currentYClip - t.prevY;
    }
    
  });
  
  // Start rendering
  requestAnimationFrame(function() { t.Render(); } );
};

Scene.prototype.Render = function() {
  var gl = this.gl;
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  var camera = this.camera;
  
  if (this.lookAtModelNum === 0) {
      this.camera.lookAt = this.jScene.camera.lookAt.slice();
  } else if (this.lookAtModelNum > 0 && this.lookAtModelNum <= this.jScene.models.length) {
      this.camera.lookAt = this.jScene.models[this.lookAtModelNum-1].location;
  }
  
  // Compute aspect ratio of canvas
  var aspect = this.canvas.width / this.canvas.height;
  
  // Build projection matrix
  var projection = [];
  camera.FOVdeg = parseFloat(this.fovSlider.value);
  camera.near = parseFloat(this.nearSlider.value);
  camera.far = parseFloat(this.farSlider.value);
  camera.location[2] = parseFloat(this.zSlider.value);
  camera.perspective = this.perspectiveCheckBox.checked;
  
  if (camera.perspective) {
    projection = perspective(camera.FOVdeg, aspect, 
                             camera.near, camera.far);
  } else {
    // Scale width of ortho view depending on tangent of viewing angle
    var orthoFOV = Math.tan(camera.FOVdeg * 0.5 * Math.PI / 180.0);
    projection = ortho(-aspect * orthoFOV, aspect * orthoFOV, -orthoFOV, orthoFOV, 
                       camera.near, camera.far);
  }
  
  // Build view transform and initialize matrix stack
  var matrixStack = new MatrixStack;
  var modelViewMatrix = lookAt(camera.location, camera.lookAt, camera.approxUp);
  var look_At = vec4(camera.lookAt,1);
  var lookAtInViewCoords = mult(modelViewMatrix, look_At);
  var translateToOrigin = translate(vec3(-lookAtInViewCoords[0], -lookAtInViewCoords[1], -lookAtInViewCoords[2]));
  
  this.tempRotateX = this.yChange * 180;
  var rotate_X = rotateX(this.tempRotateX + this.prevXRotation);
  this.tempRotateY = this.xChange * 180;
  var rotate_Y = rotateY(this.tempRotateY + this.prevYRotation);
  
  var translateBack = translate(vec3(lookAtInViewCoords[0], lookAtInViewCoords[1], lookAtInViewCoords[2]));
  var firstTransform = mult(translateBack, mult(rotate_Y, mult(rotate_X, translateToOrigin)));
  matrixStack.LoadMatrix(firstTransform);
  matrixStack.MultMatrix(modelViewMatrix);
          //lookAt(camera.location, camera.lookAt, camera.approxUp));
    
  // Render each loaded object with its transform
  for (var i = 0; i < this.loadedModels.length; ++i) {
    // Build transform from JSON and add to transforms array
    var jModel = this.jScene.models[i];
    var ms = scalem(jModel.scale);
    var mt = translate(jModel.location);
    
    var mf = mat4(jModel.xBasis[0], jModel.yBasis[0], jModel.zBasis[0], 0.0,
                  jModel.xBasis[1], jModel.yBasis[1], jModel.zBasis[1], 0.0,
                  jModel.xBasis[2], jModel.yBasis[2], jModel.zBasis[2], 0.0,
                  0.0,              0.0,              0.0,              1.0);
    var transform = mult(mt, mult(mf, ms));
    
    matrixStack.PushMatrix();
    matrixStack.MultMatrix(transform);
    this.loadedModels[i].Render(projection, matrixStack);
    matrixStack.PopMatrix();
  }
  
  //move the rocket toward the target
  var loc = this.jScene.models[0].location;
  var toLoc = this.jScene.models[this.targetNumber].location;
  var step = this.jScene.animationStep;
  
  this.jScene.models[0].location = [  loc[0] + ((toLoc[0] - loc[0]) / step),
                                      loc[1] + ((toLoc[1] - loc[1]) / step),
                                      loc[2] + ((toLoc[2] - loc[2]) / step)];
  var distanceToTarget = Math.sqrt(   Math.pow(toLoc[0] - loc[0], 2), 
                                      Math.pow(toLoc[1] - loc[1], 2),
                                      Math.pow(toLoc[2] - loc[2], 2));
                                      
  //if close enough to target, switch target to other cube model and recompute basis vectors
  if (distanceToTarget <= this.jScene.collisionDistance){
    if (this.targetNumber === 1)
      this.targetNumber = 2;
    else
      this.targetNumber = 1;
    this.ComputeBasisVectors();
  }
  var t = this;
  requestAnimationFrame(function() { t.Render(); } );
};

//computing the basis vectors for the rocket (models[0])
Scene.prototype.ComputeBasisVectors = function() {
  var eyeArray = this.jScene.models[0].location;
  var eye = vec3(eyeArray[0], eyeArray[1], eyeArray[2]);
  var atArray = this.jScene.models[this.targetNumber].location;
  var at = vec3(atArray[0], atArray[1], atArray[2]);
  var vup = vec3(0, 0, 1);
  var atToEye = subtract(eye, at);
  var n = normalize(atToEye);
  var uDir = cross(vup, n);
  var u = normalize(uDir);
  var v = cross(n, u);
  this.jScene.models[0].xBasis = u;
  this.jScene.models[0].yBasis = negate(n);
  this.jScene.models[0].zBasis = v;
};

Scene.prototype.ResetCamera = function() {
  // Copy the camera parameters from the jScene object.  The copy's values
  // are independent of the originals, so changes won't affect the originals.
  this.camera = {};
  this.camera.location = this.jScene.camera.location.slice();
  this.camera.lookAt = this.jScene.camera.lookAt.slice();
  this.camera.approxUp = this.jScene.camera.approxUp.slice();
  this.camera.FOVdeg = this.jScene.camera.FOVdeg;
  this.camera.near = this.jScene.camera.near;
  this.camera.far = this.jScene.camera.far;
  this.camera.perspective = this.jScene.camera.perspective;

  // Set UI elements to the values defined in the scene files
  this.fovSlider.value = this.camera.FOVdeg;
  SliderUpdate(this.fovSliderID + "-output", this.camera.FOVdeg);

  this.nearSlider.value = this.camera.near;
  SliderUpdate(this.nearSliderID + "-output", this.camera.near);

  this.farSlider.value = this.camera.far;
  SliderUpdate(this.farSliderID + "-output", this.camera.far);
  
  this.zSlider.value = this.camera.location[2];
  SliderUpdate(this.zSliderID + "-output", this.camera.location[2]);

  this.perspectiveCheckBox.checked = this.camera.perspective;
};

Scene.prototype.KeyInput = function(event) {
  // Get character from event
  var c = String.fromCharCode(event.which);
  console.log("keyboard input character = " + c);  // this line may be removed
  
  // If numeric, switch view to selected object
  var atModel = parseInt(c);
  if (!isNaN(atModel)) {
      this.lookAtModelNum = atModel;
  }
};