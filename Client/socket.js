var notClosed = true;
var gauge;
var retry;

clearTimeout(retry)

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
          pushThrottleData(throttleChart, gameValues.throttle);
          ws.send(JSON.stringify(gameValues));
        
        
      }
    }
    var ws;
    
    function init() {
      
      shiftGauge = initShiftGauge();
      speedGauge = initSpeedGauge();

      shiftGauge.set(0);
      speedGauge.set(0);
      // Connect to Web Socket
      
      ws = new WebSocket("ws://localhost:13254/");

      // Set event handlers.
      ws.onopen = function() {
        webStatus(1);
        robotStatus(2);
      };
      
      ws.onmessage = function(e) {
        console.log(e)
        // e.data contains received string.
        //output("onmessage: " + e.data);
        data = JSON.parse(e.data)
        shiftGauge.set(data.shiftVal)
        speedGauge.set(data.rpm)
        robotStatus(1);
      };
      
      ws.onclose = function() {
          retry = setTimeout(function(){init()} , 3000);
          webStatus(2);
          notClosed = false;
      };

      ws.onerror = function(e) {
        robotStatus(3);
        webStatus(3);
        console.log(e)
      };

    }
    
    function onSubmit() {
      var input = document.getElementById("input");
      // You can send message to the Web Socket using ws.send.
      ws.send(input.value);
      //output("send: " + input.value);
      input.value = "";
      input.focus();
    }
    
    function onCloseClick() {
      ws.close();
    }
    
    function webStatus(status) {
      var display = document.getElementById("wsstatus");
      if (status == 1) {
        display.style.backgroundColor = "green";
        
      }else if (status == 2) {
        display.style.backgroundColor = "orange";
        
      }else if (status == 3) {
        display.style.backgroundColor = "red";
        
      }
    }

    function robotStatus(status) {
      var display = document.getElementById("rostatus");
      if (status == 1) {
        display.style.backgroundColor = "green";
        
      }else if (status == 2) {
        display.style.backgroundColor = "orange";
        
      }else if (status == 3) {
        display.style.backgroundColor = "red";
        
      }
    }