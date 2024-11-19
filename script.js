let port;
let reader;

function updateDateTime() {
    const now = new Date();
    const options = { 
        weekday: 'long',
        year: 'numeric', 
        month: 'long', 
        day: 'numeric',
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit'
    };
    document.getElementById('datetime').textContent = now.toLocaleString('id-ID', options);
}

// Update time immediately and then every second
updateDateTime();
setInterval(updateDateTime, 1000);

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

let slideIndex = 1;
        showSlides(slideIndex);

        // Next/previous controls
        function plusSlides(n) {
            showSlides(slideIndex += n);
        }

        // Thumbnail image controls
        function currentSlide(n) {
            showSlides(slideIndex = n);
        }

        function showSlides(n) {
            let i;
            let slides = document.getElementsByClassName("mySlides");
            let dots = document.getElementsByClassName("dot");
            
            if (n > slides.length) {slideIndex = 1}
            if (n < 1) {slideIndex = slides.length}
            
            for (i = 0; i < slides.length; i++) {
                slides[i].style.display = "none";
            }
            for (i = 0; i < dots.length; i++) {
                dots[i].className = dots[i].className.replace(" active", "");
            }
            
            slides[slideIndex-1].style.display = "block";
            dots[slideIndex-1].className += " active";
        }

        // Auto slide every 5 seconds (optional)
        setInterval(function() {
            plusSlides(1);
        },7000);
