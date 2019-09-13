var notClosed = true;
    window.addEventListener("gamepadconnected", function(e) {
      console.log("Gamepad connected at index %d: %s. %d buttons, %d axes.",
        e.gamepad.index, e.gamepad.id,
        e.gamepad.buttons.length, e.gamepad.axes.length);
        console.log(e.gamepad.buttons)
        window.setInterval(function(){
          sendGamePadState()
        }, 50);
    });

    function sendGamePadState() {
        if (ws) {
          gamepad = navigator.getGamepads();
          gameValues = {
            'throttle': gamepad[0].buttons[7].value,
            'shiftup': gamepad[0].buttons[4].value,
            'shiftdown': gamepad[0].buttons[5].value,

          }
          console.log(gameValues);
          ws.send(JSON.stringify(gameValues));
        
        
      }
    }
    var ws;
    
    function init() {

      // Connect to Web Socket
      
      ws = new WebSocket("ws://localhost:13254/");

      // Set event handlers.
      ws.onopen = function() {
        output("onopen");

      };
      
      ws.onmessage = function(e) {
        // e.data contains received string.
        output("onmessage: " + e.data);
      };
      
      ws.onclose = function() {
        setTimeout(init() , 1000);
        output("onclose");
        notClosed = false;
      };

      ws.onerror = function(e) {
        
        output("onerror");
        console.log(e)
      };

    }
    
    function onSubmit() {
      var input = document.getElementById("input");
      // You can send message to the Web Socket using ws.send.
      ws.send(input.value);
      output("send: " + input.value);
      input.value = "";
      input.focus();
    }
    
    function onCloseClick() {
      ws.close();
    }
    
    function output(str) {
      var log = document.getElementById("log");
      var escaped = str.replace(/&/, "&amp;").replace(/</, "&lt;").
        replace(/>/, "&gt;").replace(/"/, "&quot;"); // "
      log.innerHTML = escaped + "<br>" + log.innerHTML;
    }