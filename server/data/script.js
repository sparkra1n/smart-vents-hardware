        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;

        window.addEventListener('load', onLoad);

        function initWebSocket() {
            console.log('Trying to open a WebSocket connection...');
            websocket = new WebSocket(gateway);
            websocket.onopen = onOpen;
            websocket.onclose = onClose;
            websocket.onmessage = onMessage; // <-- add this line
        }

        function onOpen(event) {
            console.log('Connection opened');
        }

        function onClose(event) {
            console.log('Connection closed');
            setTimeout(initWebSocket, 2000);
        }

        function onMessage(event) {
            console.log(event.data);
            document.getElementById('state-value').innerHTML = event.data;
        }

        function onLoad(event) {
            initWebSocket();
            initButton();
        }

        function initButton() {
            document.getElementById('button').addEventListener('click', clicked);
        }

        function clicked() {
            websocket.send(document.getElementById("slider-range").value);
        }

        var slider = document.getElementById("slider-range");
        var output = document.getElementById("slider-value");
        output.innerHTML = slider.value;

        slider.oninput = function() {
            output.innerHTML = this.value;
        }
