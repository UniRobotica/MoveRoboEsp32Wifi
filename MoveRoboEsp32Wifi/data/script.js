let socket;

function connect() {
    const ip = document.getElementById("ip").value;
    if (!ip) {
        alert("Digite o IP do ESP32.");
        return;
    }

    socket = new WebSocket(`ws://${ip}/ws`);

    socket.onopen = () => logMessage("Conectado ao ESP32.");
    socket.onmessage = (event) => logMessage("Resposta: " + event.data);
    socket.onclose = () => logMessage("Conexão encerrada.");
    socket.onerror = (error) => logMessage("Erro de conexão.");
}

function sendCommand(command) {
    if (socket && socket.readyState === WebSocket.OPEN) {
        socket.send(command);
        logMessage(`Comando enviado: ${command}`);
    } else {
        logMessage("Conexão não estabelecida.");
    }
}

function logMessage(message) {
    const logs = document.getElementById("logs");
    const logEntry = document.createElement("p");
    logEntry.textContent = message;
    logs.appendChild(logEntry);
}
