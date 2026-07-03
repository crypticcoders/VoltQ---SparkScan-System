#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "#####"; 
const char* password = "*********"; 

WebServer server(80);
String componentData = "Slot Empty";
String historyLog = ""; 

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>VoltQ SparkScan  | Components Diagnostic Hub</title>
    <link href="https://fonts.googleapis.com/css2?family=Plus+Jakarta+Sans:wght@400;500;600;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --bg-base: #080b11;
            --bg-surface: #0f1420;
            --bg-element: #161d2f;
            --border-subtle: rgba(255, 255, 255, 0.06);
            --text-main: #f3f4f6;
            --text-muted: #6b7280;
            --brand-primary: #38bdf8;
            --brand-success: #34d399;
            --brand-danger: #f87171;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Plus Jakarta Sans', sans-serif;
        }

        body {
            background-color: var(--bg-base);
            color: var(--text-main);
            min-height: 100vh;
            padding: 2rem;
            display: flex;
            justify-content: center;
        }

        .dashboard-wrapper {
            width: 100%;
            max-width: 1100px;
            display: flex;
            flex-direction: column;
            gap: 1.5rem;
        }

        header {
            background: var(--bg-surface);
            border: 1px solid var(--border-subtle);
            border-radius: 16px;
            padding: 1.25rem 2rem;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .brand-group h1 {
            font-size: 1.25rem;
            font-weight: 700;
            letter-spacing: -0.5px;
            background: linear-gradient(135deg, var(--brand-primary), var(--brand-success));
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        .brand-group p {
            font-size: 0.75rem;
            color: var(--text-muted);
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-top: 2px;
        }

        .system-badge {
            display: flex;
            align-items: center;
            gap: 8px;
            font-size: 0.8rem;
            background: var(--bg-element);
            padding: 6px 14px;
            border-radius: 20px;
            border: 1px solid var(--border-subtle);
        }

        .pulse-dot {
            width: 8px;
            height: 8px;
            background: var(--brand-success);
            border-radius: 50%;
            box-shadow: 0 0 8px var(--brand-success);
        }

        .workspace-grid {
            display: grid;
            grid-template-columns: 1fr 1.2fr;
            gap: 1.5rem;
        }

        @media (max-width: 900px) {
            .workspace-grid { grid-template-columns: 1fr; }
        }

        .panel {
            background: var(--bg-surface);
            border: 1px solid var(--border-subtle);
            border-radius: 18px;
            padding: 2rem;
            display: flex;
            flex-direction: column;
        }

        .panel-title {
            font-size: 0.9rem;
            text-transform: uppercase;
            letter-spacing: 1px;
            color: var(--text-muted);
            margin-bottom: 1.5rem;
            font-weight: 600;
        }

        .live-display-box {
            background: var(--bg-element);
            border: 1px solid var(--border-subtle);
            border-radius: 14px;
            padding: 2.5rem 1.5rem;
            text-align: center;
            margin-bottom: 1.5rem;
        }

        .telemetry-value {
            font-size: 2.2rem;
            font-weight: 700;
            color: var(--brand-success);
            letter-spacing: -0.5px;
            transition: color 0.25s ease;
        }

        .matrix-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 0.75rem;
        }

        .matrix-item {
            background: var(--bg-element);
            border: 1px solid var(--border-subtle);
            border-radius: 10px;
            padding: 1rem;
            display: flex;
            align-items: center;
            gap: 12px;
        }

        .matrix-icon {
            width: 32px;
            height: 32px;
            background: rgba(56, 189, 248, 0.1);
            border-radius: 6px;
            display: flex;
            align-items: center;
            justify-content: center;
            color: var(--brand-primary);
            font-weight: bold;
            font-size: 0.85rem;
        }

        .matrix-details h4 {
            font-size: 0.85rem;
            font-weight: 600;
        }

        .matrix-details p {
            font-size: 0.75rem;
            color: var(--text-muted);
        }

        .history-controls {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 1rem;
        }

        .clear-btn {
            background: transparent;
            border: 1px solid rgba(248, 113, 113, 0.3);
            color: var(--brand-danger);
            padding: 4px 10px;
            border-radius: 6px;
            font-size: 0.75rem;
            cursor: pointer;
            transition: all 0.2s;
        }

        .clear-btn:hover {
            background: rgba(248, 113, 113, 0.1);
        }

        .history-container {
            flex-grow: 1;
            max-height: 320px;
            overflow-y: auto;
            display: flex;
            flex-direction: column;
            gap: 0.5rem;
            padding-right: 4px;
        }

        .history-container::-webkit-scrollbar { width: 4px; }
        .history-container::-webkit-scrollbar-thumb { background: var(--bg-element); border-radius: 4px; }

        .history-row {
            background: var(--bg-element);
            border: 1px solid var(--border-subtle);
            border-radius: 8px;
            padding: 0.75rem 1rem;
            display: flex;
            justify-content: space-between;
            align-items: center;
            font-size: 0.85rem;
            animation: fadeIn 0.3s ease;
        }

        .history-tag {
            font-weight: 600;
            color: var(--brand-primary);
        }

        .history-time {
            font-size: 0.75rem;
            color: var(--text-muted);
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(4px); }
            to { opacity: 1; transform: translateY(0); }
        }
    </style>
</head>
<body>

    <div class="dashboard-wrapper">
        <header>
            <div class="brand-group">
                <h1>VoltQ SparkScan</h1>
                <p>Multi Component Verifying Node</p>
            </div>
            <div class="system-badge">
                <div class="pulse-dot"></div>
                <span>Testing Node Online</span>
            </div>
        </header>

        <div class="workspace-grid">
            <div class="panel">
                <div class="panel-title">Active Testing Dashboard</div>
                <div class="live-display-box">
                    <div id="live-value" class="Component-value">Data Fetching...</div>
                </div>

                <div class="panel-title" style="margin-top: 1rem; margin-bottom: 1rem;">System Diagnostic Capabilities</div>
                <div class="matrix-grid">
                    <div class="matrix-item">
                        <div class="matrix-icon">R</div>
                        <div class="matrix-details">
                            <h4>Resistors</h4>
                            <p>Linear Ohm Evaluation</p>
                        </div>
                    </div>
                    <div class="matrix-item">
                        <div class="matrix-icon">C</div>
                        <div class="matrix-details">
                            <h4>Capacitors</h4>
                            <p>Dynamic Tau Decay</p>
                        </div>
                    </div>
                    <div class="matrix-item">
                        <div class="matrix-icon">D</div>
                        <div class="matrix-details">
                            <h4>LEDs / Diodes</h4>
                            <p>Non-Linear Dual Current</p>
                        </div>
                    </div>
                    <div class="matrix-item">
                        <div class="matrix-icon">V</div>
                        <div class="matrix-details">
                            <h4>Regulators</h4>
                            <p>3-Pin Static Bleed Load</p>
                        </div>
                    </div>
                </div>
            </div>

            <div class="panel">
                <div class="history-controls">
                    <div class="panel-title" style="margin-bottom: 0;">Telemetry Log History</div>
                    <button class="clear-btn" onclick="clearHistory()">Flush Log</button>
                </div>
                <div id="history-box" class="history-container">
                </div>
            </div>
        </div>
    </div>

    <script>
        let priorRecord = "";

        function pollTelemetry() {
            fetch('/data')
                .then(res => res.text())
                .then(payload => {
                    const textContainer = document.getElementById('live-value');
                    textContainer.innerText = payload;

                    if(payload.includes("Empty") || payload.includes("SHORT") || payload.includes("COND")) {
                        textContainer.style.color = "var(--brand-danger)";
                    } else {
                        textContainer.style.color = "var(--brand-success)";
                    }

                    if(payload !== priorRecord && payload !== "Slot Empty" && payload !== "Clearing Slot.. " && payload !== "Loading..." && !payload.includes("Empty")) {
                        appendRecord(payload);
                        priorRecord = payload;
                    }
                })
                .catch(err => console.error("Telemetry channel broken:", err));
        }

        function appendRecord(val) {
            const box = document.getElementById('history-box');
            const stamp = new Date().toLocaleTimeString([], {hour: '2-digit', minute:'2-digit', second:'2-digit'});
            
            const row = document.createElement('div');
            row.className = 'history-row';
            row.innerHTML = `<span class="history-tag">${val}</span><span class="history-time">${stamp}</span>`;
            
            box.insertBefore(row, box.firstChild);
            fetch('/add-history?item=' + encodeURIComponent(val));
        }

        function loadPreexistingHistory() {
            fetch('/get-history')
                .then(res => res.text())
                .then(txt => {
                    if(!txt.length) return;
                    const items = txt.split('|');
                    const box = document.getElementById('history-box');
                    box.innerHTML = "";
                    items.forEach(item => {
                        if(!item.trim().length) return;
                        const row = document.createElement('div');
                        row.className = 'history-row';
                        row.innerHTML = `<span class="history-tag">${item}</span><span class="history-time">Saved Session</span>`;
                        box.appendChild(row);
                    });
                });
        }

        function clearHistory() {
            fetch('/clear-history').then(() => {
                document.getElementById('history-box').innerHTML = "";
                priorRecord = "";
            });
        }

        setInterval(pollTelemetry, 1000);
        loadPreexistingHistory();
    </script>
</body>
</html>
  )rawliteral";
  server.send(200, "text/html", html);
}

void handleData() {
  server.send(200, "text/plain", componentData);
}

void handleAddHistory() {
  if (server.hasArg("item")) {
    String newItem = server.arg("item");
    if (historyLog.length() > 0) {
      historyLog = newItem + "|" + historyLog;
    } else {
      historyLog = newItem;
    }
  }
  server.send(200, "text/plain", "OK");
}

void handleGetHistory() {
  server.send(200, "text/plain", historyLog);
}

void handleClearHistory() {
  historyLog = "";
  server.send(200, "text/plain", "CLEARED");
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
  server.on("/add-history", handleAddHistory);
  server.on("/get-history", handleGetHistory);
  server.on("/clear-history", handleClearHistory);
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
