/**
 * Lab 2 - COMP3801 Winter 2018
 *   Basic WebGL shaders, mouse events and coordinates
 * 
 */

"use strict";

// Constructor
//
// @param canvasID - string containing name of canvas to render.
//          Buttons and sliders should be prefixed with this string.
//
function Assignment1(canvasID /* name of canvas to render */) {
    this.canvasID = canvasID;
    this.canvas = document.getElementById(canvasID);
    if (!this.canvas) {
        alert("Canvas ID '" + canvasID + "' not found.");
        return;
    }
    this.gl = WebGLUtils.setupWebGL(this.canvas);
    if (!this.gl) {
        alert("WebGL isn't available in this browser");
        return;
    }

    this.init();
}

// Define prototype values common to all Lab2 objects
Assignment1.prototype.gl = null;

//initialize 4 vertices in currentRect
Assignment1.prototype.currentRect = [new vec3(), new vec3(), new vec3(), new vec3()];
Assignment1.prototype.midRect = false;
Assignment1.prototype.lastColor = new vec3();

Assignment1.prototype.toString = function () {
    return JSON.stringify(this);
};

Assignment1.prototype.init = function () {
    var canvas = this.canvas;
    var gl = this.gl;
    var t = this;  // make available to event handlers
    this.midRect = false;
    
    //enable transparency in WebGL (not enabled by default)
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

    // WebGL setup

    gl.viewport(0, 0, canvas.width, canvas.height);

    // Compile and link shaders
    this.shaderProgram = initShaders(gl, "vShader.glsl", "fShader.glsl");
    if (this.shaderProgram === null)
        return;
    gl.useProgram(this.shaderProgram);
    
    //save location of "alphaValue" uniform variable
    this.alphaLoc = gl.getUniformLocation(this.shaderProgram, "alphaValue");
    // Array of initial rectangle vertex coordinates
    this.rectVertexCoords = [];

    // Load vertex coordinates into WebGL buffer
    this.rectVertexCoordBuffer = gl.createBuffer();  // get unique buffer ID number
    gl.bindBuffer(gl.ARRAY_BUFFER, this.rectVertexCoordBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(this.rectVertexCoords), gl.STATIC_DRAW);

    // Associate buffer with shader variable vPosition
    this.vPosition = gl.getAttribLocation(this.shaderProgram, "vPosition");
    gl.vertexAttribPointer(this.vPosition, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(this.vPosition);

    // Array of rectangle vertex colors corresponding to vertex coordinates
    this.rectVertexColors = [];

    // Load vertex colors into WebGL buffer
    this.rectVertexColorBuffer = gl.createBuffer();  // get unique buffer ID number
    gl.bindBuffer(gl.ARRAY_BUFFER, this.rectVertexColorBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(this.rectVertexColors), gl.STATIC_DRAW);

    // Associate buffer with shader variable vColor
    this.vColor = gl.getAttribLocation(this.shaderProgram, "vColor");
    gl.vertexAttribPointer(this.vColor, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(this.vColor);
    
    //buffer for "rubber band" points/lines
    this.rbCoordBuffer = gl.createBuffer();
    this.rbColorBuffer = gl.createBuffer();
    this.rbCoords = [];
    this.rbColors = [];

    // Define callback for change of slider value
    var sliderCallback = function (e) {
        // Update text display for slider
        var color = e.target.value;
        e.target.valueDisplay.textContent = color;

        // Re-render canvas
        requestAnimationFrame(render);
    };

    // Set up HTML user interface
    this.colors = ["r", "g", "b"];
    var rgbSliders = [];         // array of slider HTML elements
    var rgbSliderValues = [];    // array of slider value HTML elements

    // Set up an object with sliders for the three colors. The sliders are
    // accessed using "indices" of "r", "g", and "b".
    for (var i in this.colors) {
        var color = this.colors[i];
        var sliderID = this.canvasID + "-" + color + "-slider";
        rgbSliders[color] = document.getElementById(sliderID);
        if (rgbSliders[color] === null) {
            alert("Slider ID not found: " + sliderID);
            return;
        }
        var valueID = this.canvasID + "-" + color + "-value";
        rgbSliderValues[color] = document.getElementById(valueID);
        if (rgbSliders[color] === null) {
            alert("Slider value ID not found: " + sliderID);
            return;
        }
        rgbSliders[color].valueDisplay = rgbSliderValues[color];  // attach to slider

        // Set callback on slider input
        rgbSliders[color].addEventListener("input", sliderCallback);
    }
    this.rgbSliders = rgbSliders;
    
    //set up alpha slider
    var alphaSlider = document.getElementById(this.canvasID + "-a-slider");
    if (alphaSlider === null) {
        alert("Alpha slider ID not found: " + sliderID);
        return;
    }
    var alphaValue = document.getElementById(this.canvasID + "-a-value");
    if (alphaValue === null) {
        alert("Alpha slider value ID not found: " + sliderID);
        return;
    }
    alphaSlider.valueDisplay = alphaValue;  // attach to slider

    // Set callback on slider input
    alphaSlider.addEventListener("input", sliderCallback);
    this.alphaSlider = alphaSlider;

    // Set up all buttons
    var resetButton = document.getElementById(this.canvasID + "-reset-button");
    if (resetButton === null) {
        alert("Reset button ID not found: " + this.canvasID + "-reset-button");
        return;
    }
    var whiteButton = document.getElementById(this.canvasID + "-white-button");
    var blackButton = document.getElementById(this.canvasID + "-black-button");
    var redButton = document.getElementById(this.canvasID + "-red-button");
    var yellowButton = document.getElementById(this.canvasID + "-yellow-button");
    var greenButton = document.getElementById(this.canvasID + "-green-button");
    var cyanButton = document.getElementById(this.canvasID + "-cyan-button");
    var blueButton = document.getElementById(this.canvasID + "-blue-button");
    var magentaButton = document.getElementById(this.canvasID + "-magenta-button");
    if (whiteButton === null || blackButton === null || redButton === null || yellowButton === null || 
            greenButton === null || cyanButton === null || blueButton === null || magentaButton === null) {
        alert("Color button ID not found.");
        return;
    }
    var buttonArray = [whiteButton, blackButton, redButton, yellowButton, greenButton, cyanButton, blueButton, magentaButton];
    var colorsArray = [vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 1.0), vec3(0.0, 0.0, 1.0), vec3(1.0, 0.0, 1.0)];
    for (var i = 0; i < buttonArray.length; i++) {
        buttonArray[i].addEventListener("click", function () {
            var index = 0;
            switch (this.id.slice(10)){
                case "white-button": index = 0; break;
                case "black-button": index = 1; break;
                case "red-button": index = 2; break;
                case "yellow-button": index = 3; break;
                case "green-button": index = 4; break;
                case "cyan-button": index = 5; break;
                case "blue-button": index = 6; break;
                case "magenta-button": index = 7; break;
            }
            var a = 0;
            for (var j in rgbSliders) {
                rgbSliders[j].value = colorsArray[index][a];
                rgbSliders[j].valueDisplay.textContent = rgbSliders[j].value / rgbSliders[j].max;
                a++;
            }
            requestAnimationFrame(render);
        });
    }

    // Set up callback to render a frame
    var render = function () {
        t.Render();
    };

    // Set up the callback for the reset button
    resetButton.addEventListener("click", function () {
        for (var i in rgbSliders) {
            rgbSliders[i].value = rgbSliders[i].max / 2;
            rgbSliders[i].valueDisplay.textContent = rgbSliders[i].value / rgbSliders[i].max;
        }
        alphaSlider.value = alphaSlider.max / 2;
        alphaSlider.valueDisplay.textContent = alphaSlider.value / alphaSlider.max;
        //remove all point and color values from arrays
        t.rectVertexCoords = [];
        t.rectVertexColors = [];

        //Recreate vertex and color buffers to update their values
        gl.bindBuffer(gl.ARRAY_BUFFER, t.rectVertexCoordBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, flatten(t.rectVertexCoords), gl.STATIC_DRAW);
        gl.bindBuffer(gl.ARRAY_BUFFER, t.rectVertexColorBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, flatten(t.rectVertexColors), gl.STATIC_DRAW);

        requestAnimationFrame(render);
    });
  

    // Set up mouse tracking
    this.mouseDown = false;  // track mouse button state
    var mouseX = document.getElementById(this.canvasID + "-mousex");
    var mouseY = document.getElementById(this.canvasID + "-mousey");
    var mouseXClip = document.getElementById(this.canvasID + "-mousex-clip");
    var mouseYClip = document.getElementById(this.canvasID + "-mousey-clip");
    var mouseButton = document.getElementById(this.canvasID + "-mousebutton");
    if (mouseX === null || mouseY === null || mouseXClip === null || mouseYClip === null || mouseButton === null) {
        alert("Mouse output HTML IDs not found");
        return;
    }

    // Add mouse event handlers
    canvas.addEventListener("mousedown", function (e) {
        // IE handles button ID different from everyone else. We'll just assume
        // that any button press is the left button.
        t.mouseDown = true;
        mouseButton.textContent = "down";
    });
    canvas.addEventListener("click", function (e) {
        var point = vec3(parseFloat(mouseXClip.textContent), parseFloat(mouseYClip.textContent), 0.0);
        var color = t.getSliderColor();
        
        //add the first point of the new rectangle to currentRect
        if (!t.midRect){
            t.currentRect[0] = point;
            t.lastColor = color;
            t.midRect = true;
        }
        //add the final three points of the rectangle and draw it
        else {
            t.currentRect[1] = vec3(point[0], t.currentRect[0][1], 0.0);
            t.currentRect[2] = vec3(t.currentRect[0][0], point[1], 0.0);
            t.currentRect[3] = point;
            
            //push coordinates for two triangles making up rectangle to be drawn
            t.rectVertexCoords.push(t.currentRect[0]);
            t.rectVertexCoords.push(t.currentRect[2]);
            t.rectVertexCoords.push(t.currentRect[3]);
            t.rectVertexCoords.push(t.currentRect[0]);
            t.rectVertexCoords.push(t.currentRect[1]);
            t.rectVertexCoords.push(t.currentRect[3]);
            
            var avgColor = new vec3(
                    (parseFloat(t.lastColor[0]) + parseFloat(color[0])) / 2.0, 
                    (parseFloat(t.lastColor[1]) + parseFloat(color[1])) / 2.0, 
                    (parseFloat(t.lastColor[2]) + parseFloat(color[2])) / 2.0);
            
            //push colors for these six points
            t.rectVertexColors.push(t.lastColor);
            t.rectVertexColors.push(avgColor);
            t.rectVertexColors.push(color);
            t.rectVertexColors.push(t.lastColor);
            t.rectVertexColors.push(avgColor);
            t.rectVertexColors.push(color);
            
            //reset currentRect
            t.currentRect = [new vec3(), new vec3(), new vec3(), new vec3()];
            t.midRect = false;
            
            requestAnimationFrame(render);
        }
    });
    canvas.addEventListener("mouseup", function (e) {
        // IE handles button ID different from everyone else. We'll just assume
        // that any button press is the left button.
        t.mouseDown = false;
        mouseButton.textContent = "up";
    });
    canvas.addEventListener("mousemove", function (e) {
        // Calculate mouse position relative to canvas. This unfortunately works
        // differently in different browsers. This method appears to work in Chrome,
        // Firefox, and IE 11.
        var x = e.pageX - e.target.offsetLeft;
        var y = e.pageY - e.target.offsetTop;
        mouseX.textContent = x;
        mouseY.textContent = y;
        mouseXClip.textContent = ((2.0 * x / (canvas.width - 1)) - 1.0).toFixed(3);
        mouseYClip.textContent = (1.0 - (2.0 * y / (canvas.height - 1))).toFixed(3);
        
        t.rbCoords = [];
        t.rbColors = [];
        
        if (t.midRect){
            var bottomRightPoint = vec3(parseFloat(mouseXClip.textContent), parseFloat(mouseYClip.textContent), 0.0);
            var topLeftPoint = t.currentRect[0];
            var topRightPoint = t.currentRect[1] = vec3(bottomRightPoint[0], topLeftPoint[1], 0.0);
            var bottomLeftPoint = t.currentRect[2] = vec3(topLeftPoint[0], bottomRightPoint[1], 0.0);
            t.rbCoords.push(topLeftPoint);
            t.rbCoords.push(topRightPoint);
            t.rbCoords.push(topRightPoint);
            t.rbCoords.push(bottomRightPoint);
            t.rbCoords.push(bottomRightPoint);
            t.rbCoords.push(bottomLeftPoint);
            t.rbCoords.push(bottomLeftPoint);
            t.rbCoords.push(topLeftPoint);
            for (var i = 0; i < t.rbCoords.length; i++){
                t.rbColors.push(vec3(1.0, 1.0, 1.0));
            }
        }
        requestAnimationFrame(render);
    });
    
    // Kick things off with an initial rendering
    requestAnimationFrame(render);
};

/**
 * GetSliderColors - get the current RGB color represented by the sliders
 *   as a vec3.
 *   
 * @returns {vec3} current slider color
 */
Assignment1.prototype.getSliderColor = function () {
    // Build an array of color values based on the current slider colors
    var colorValues = [];
    for (var i in this.colors) {
        var color = this.colors[i];
        var colorValue = this.rgbSliders[color].value;
        colorValues[i] = colorValue;
    }

    return vec3(colorValues);
};

/**
 * Render - draw the frame
 *
 */
Assignment1.prototype.Render = function () {
    var gl = this.gl;
    gl.clearColor(0, 0, 0, 1);
    gl.clear(gl.COLOR_BUFFER_BIT);
    
    var alpha = parseFloat(this.alphaSlider.value);
    gl.uniform1f(this.alphaLoc, alpha);
    
    if(this.rectVertexCoords.length>0){
        //Recreate vertex and color buffers to include new values
        gl.bindBuffer(gl.ARRAY_BUFFER, this.rectVertexCoordBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, flatten(this.rectVertexCoords), gl.STATIC_DRAW);
        gl.vertexAttribPointer(this.vPosition, 3, gl.FLOAT, false, 0, 0);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.rectVertexColorBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, flatten(this.rectVertexColors), gl.STATIC_DRAW);
        gl.vertexAttribPointer(this.vColor, 3, gl.FLOAT, false, 0, 0);
        gl.drawArrays(gl.TRIANGLES, 0, this.rectVertexCoords.length);
    }
    
    if(this.midRect){
        //Draw rubber band buffer
        gl.bindBuffer(gl.ARRAY_BUFFER, this.rbCoordBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, flatten(this.rbCoords), gl.STATIC_DRAW);
        gl.vertexAttribPointer(this.vPosition, 3, gl.FLOAT, false, 0, 0);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.rbColorBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, flatten(this.rbColors), gl.STATIC_DRAW);
        gl.vertexAttribPointer(this.vColor, 3, gl.FLOAT, false, 0, 0);
        gl.drawArrays(gl.LINES, 0, this.rbCoords.length);
    }
};
