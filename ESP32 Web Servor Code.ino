#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "#####"; 
const char* password = "**********"; 

WebServer server(80);
String componentData = "Slot Empty";

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LCR Diagnostic Hub</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --bg-color: #0b0f19;
            --card-bg: rgba(22, 28, 45, 0.45);
            --border-color: rgba(255, 255, 255, 0.08);
            --accent-blue: #00d2ff;
            --accent-green: #00f5a0;
            --text-primary: #ffffff;
            --text-secondary: #8a99ad;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Inter', sans-serif;
            background-color: var(--bg-color);
            background-image: 
                radial-gradient(at 10% 15%, rgba(0, 210, 255, 0.07) 0px, transparent 45%),
                radial-gradient(at 90% 85%, rgba(0, 245, 160, 0.07) 0px, transparent 45%);
            color: var(--text-primary);
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            padding: 24px;
        }

        .container {
            width: 100%;
            max-width: 760px; /* Widened significantly to fit data horizontally */
            background: var(--card-bg);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 1px solid var(--border-color);
            border-radius: 28px;
            padding: 40px 50px;
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.5);
            text-align: center;
        }

        header h1 {
            font-size: 2.2rem;
            font-weight: 700;
            letter-spacing: -0.5px;
            background: linear-gradient(135deg, var(--accent-blue), var(--accent-green));
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            margin-bottom: 8px;
        }

        header p {
            font-size: 0.85rem;
            color: var(--text-secondary);
            font-weight: 500;
            text-transform: uppercase;
            letter-spacing: 2px;
            margin-bottom: 30px;
        }

        .divider {
            height: 1px;
            background: linear-gradient(90deg, transparent, var(--border-color), transparent);
            margin-bottom: 30px;
        }

        .data-section {
            background: rgba(5, 7, 12, 0.4);
            border-radius: 20px;
            padding: 30px 40px;
            border: 1px solid rgba(255, 255, 255, 0.04);
            box-shadow: inset 0 2px 4px rgba(0,0,0,0.2);
        }

        .label {
            font-size: 0.85rem;
            color: var(--text-secondary);
            font-weight: 600;
            text-transform: uppercase;
            letter-spacing: 1.5px;
            margin-bottom: 16px;
        }

        .value-display {
            font-size: 2.5rem; /* Large and clear */
            font-weight: 700;
            color: var(--accent-green);
            text-shadow: 0 0 25px rgba(0, 245, 160, 0.25);
            white-space: nowrap; /* Prevents text wrapping entirely */
            overflow: hidden;
            text-overflow: ellipsis;
            transition: all 0.3s ease;
        }

        .status-pulse {
            display: inline-block;
            width: 8px;
            height: 8px;
            background-color: var(--accent-green);
            border-radius: 50%;
            margin-right: 10px;
            box-shadow: 0 0 0 0 rgba(0, 245, 160, 0.7);
            animation: pulse 1.8s infinite;
            vertical-align: middle;
        }

        .footer {
            margin-top: 35px;
            font-size: 0.8rem;
            color: var(--text-secondary);
            display: flex;
            justify-content: center;
            align-items: center;
            letter-spacing: 0.5px;
        }

        @keyframes pulse {
            0% {
                transform: scale(0.95);
                box-shadow: 0 0 0 0 rgba(0, 245, 160, 0.7);
            }
            70% {
                transform: scale(1);
                box-shadow: 0 0 0 8px rgba(0, 245, 160, 0);
            }
            100% {
                transform: scale(0.95);
                box-shadow: 0 0 0 0 rgba(0, 245, 160, 0);
            }
        }

        /* Responsive scaling down for mobile devices so it stays crisp */
        @media (max-width: 600px) {
            .container {
                padding: 25px 20px;
            }
            .value-display {
                font-size: 1.8rem;
                white-space: normal;
            }
            header h1 {
                font-size: 1.6rem;
            }
        }
    </style>
</head>
<body>

    <div class="container">
        <header>
            <h1>SparkScan Hub</h1>
            <p>Smart Analyzer for Component Health</p>
        </header>
        
        <div class="divider"></div>
        
        <div class="data-section">
            <div class="label">Live Component Reading</div>
            <div class="value-display" id="component-data">Loading...</div>
        </div>
        
        <div class="footer">
            <span class="status-pulse"></span>
            <span>Connected to Local Server</span>
        </div>
    </div>

    <script>
        function updateData() {
            fetch('/data')
                .then(response => response.text())
                .then(data => {
                    const dataEl = document.getElementById('component-data');
                    dataEl.innerText = data;
                    
                    if(data.includes("Empty") || data.includes("No")) {
                        dataEl.style.color = "#ff4a5a"; 
                        dataEl.style.textShadow = "0 0 25px rgba(255, 74, 90, 0.25)";
                    } else {
                        dataEl.style.color = "#00f5a0"; 
                        dataEl.style.textShadow = "0 0 25px rgba(0, 245, 160, 0.25)";
                    }
                })
                .catch(err => console.error('Error fetching data:', err));
        }

        setInterval(updateData, 1000);
        updateData();
    </script>
</body>
</html>
  )rawliteral";
  server.send(200, "text/html", html);
}

// Separate API endpoint that only returns the raw telemetry string
void handleData() {
  server.send(200, "text/plain", componentData);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi Connected!");
  Serial.print("Local Dashboard IP Address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
  
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');
    incoming.trim();
    if(incoming.length() > 0) {
      componentData = incoming;
      Serial.println("Received: " + componentData); 
    }
  }
}
