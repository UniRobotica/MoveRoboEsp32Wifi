const webSocket = new WebSocket('ws://' + window.location.hostname + ':81/');

function sendCommand(command) {
    webSocket.send(command);
}
