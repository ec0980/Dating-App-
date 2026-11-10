
import json
import subprocess
from http.server import SimpleHTTPRequestHandler, HTTPServer
from pathlib import Path

# Find the folder containing this Python file.
BASE_DIR = Path(__file__).resolve().parent

# Run a command from our C bridge.
def run_c(command):
    try:
        result = subprocess.run(
            ["./web_bridge"] + command,
            cwd=BASE_DIR,
            capture_output=True,
            text=True,
            timeout=5
        )

        if result.returncode != 0:
            return {"error": result.stdout or result.stderr}

        return json.loads(result.stdout)

    except Exception as e:
        return {"error": str(e)}


class WebsiteHandler(SimpleHTTPRequestHandler):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(BASE_DIR), **kwargs)

    def send_json(self, data):
        response = json.dumps(data).encode("utf-8")

        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)

    def do_GET(self):
        if self.path == "/api/privacy":
            result = run_c(["load"])
            self.send_json(result)
        else:
            super().do_GET()

    def do_POST(self):
        try:
            length = int(self.headers.get("Content-Length", 0))

            if length > 4096:
                self.send_json({"error": "Request too large"})
                return

            data = json.loads(self.rfile.read(length))

            action = data.get("action")

            if action == "set":
                result = run_c([
                    "set",
                    str(data.get("setting", "")),
                    str(data.get("value", ""))
                ])

            elif action in ("block", "unblock"):
                result = run_c([
                    action,
                    str(data.get("index", ""))
                ])

            elif action == "report":
                result = run_c([
                    "report",
                    str(data.get("index", "")),
                    str(data.get("reason", ""))
                ])

            else:
                result = {"error": "Unknown action"}

            self.send_json(result)

        except Exception as e:
            self.send_json({"error": str(e)})


print("Love at First Write server is running!")
print("Open http://localhost:8001 in your browser.")

server = HTTPServer(("0.0.0.0", 8001), WebsiteHandler)
server.serve_forever()
