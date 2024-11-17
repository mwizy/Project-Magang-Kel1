let port;
let reader;

async function connectToSerial() {
    try {
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 9600 });
        const textDecoder = new TextDecoderStream();
        const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
        reader = textDecoder.readable.getReader();
        readSensorData();
    } catch (error) {
        console.error("Failed to connect: ", error);
    }
}

async function readSensorData() {
    const outputElement = document.getElementById('sensor-output');
    outputElement.textContent = "Connected. Reading data...";

    try {
        while (true) {
            const { value, done } = await reader.read();
            if (done) {
                console.log("Stream closed");
                break;
            }
            if (value) {
                outputElement.textContent = value;
            }
        }
    } catch (error) {
        console.error("Error reading from serial: ", error);
    } finally {
        reader.releaseLock();
    }
}

window.addEventListener('unload', async () => {
    if (reader) reader.cancel();
    if (port) await port.close();
});
