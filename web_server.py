"""Local website bridge: sample-data Python pipeline -> original C++ portfolio -> browser."""
from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
import importlib
import json
import os
import subprocess
import sys
import tempfile
import threading
import time

ROOT = Path(__file__).resolve().parent
PY_DIR = ROOT / "python"
CPP_DIR = ROOT / "cpp"
WEB_DIR = ROOT / "web"
BUILD_LOCK = threading.Lock()
LOG_FILE = ROOT / "website.log"

def log(message):
    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
    with LOG_FILE.open("a", encoding="utf-8") as log_stream:
        log_stream.write(f"[{timestamp}] {message}\n")

# When launched invisibly with pythonw.exe, use python.exe for child scripts
# so their captured stdout/stderr behaves normally on Windows.
PYTHON_EXECUTABLE = Path(sys.executable)
if os.name == "nt" and PYTHON_EXECUTABLE.name.lower() == "pythonw.exe":
    console_python = PYTHON_EXECUTABLE.with_name("python.exe")
    if console_python.exists():
        PYTHON_EXECUTABLE = console_python

# Use the project's existing Python modules and refresh them when the user edits source files.
sys.path.insert(0, str(PY_DIR))
import sample_data  # noqa: E402
import collector     # noqa: E402
import formatter     # noqa: E402
import main as pipeline  # noqa: E402


def executable_path():
    return CPP_DIR / ("portfolio.exe" if os.name == "nt" else "portfolio")


def ensure_cpp_built():
    exe = executable_path()
    sources = list(CPP_DIR.glob("*.cpp")) + list(CPP_DIR.glob("*.h"))
    needs_build = not exe.exists() or any(src.stat().st_mtime > exe.stat().st_mtime for src in sources)
    if not needs_build:
        return exe
    with BUILD_LOCK:
        needs_build = not exe.exists() or any(src.stat().st_mtime > exe.stat().st_mtime for src in sources)
        if needs_build:
            compiler = os.environ.get("CXX", "g++")
            cmd = [compiler, "-std=c++11", "-Wall", "-Wextra", "main.cpp", "Trade.cpp", "Holding.cpp",
                   "RecordParser.cpp", "Portfolio.cpp", "TcpReceiver.cpp", "-o", exe.name]
            if os.name == "nt":
                cmd.append("-lws2_32")
            result = subprocess.run(cmd, cwd=CPP_DIR, capture_output=True, text=True)
            if result.returncode:
                raise RuntimeError("C++ build failed:\n" + result.stdout + result.stderr)
    return exe


def load_actual_portfolio():
    # Start the real C++ TCP receiver, then run the project's original Python entry point.
    # This preserves the existing sample_data -> collector -> formatter -> sender -> TCP -> C++ flow.
    exe = ensure_cpp_built()
    receiver = subprocess.Popen([str(exe), "--json"], cwd=CPP_DIR,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    try:
        # main.py calls sender.py, which retries briefly while C++ starts listening.
        sender = subprocess.run([str(PYTHON_EXECUTABLE), str(PY_DIR / "main.py")], cwd=PY_DIR,
                                capture_output=True, text=True, timeout=18)
        if sender.returncode:
            raise RuntimeError("Python data pipeline failed:\n" + sender.stdout + sender.stderr)
        try:
            stdout, stderr = receiver.communicate(timeout=5)
        except subprocess.TimeoutExpired:
            receiver.kill()
            stdout, stderr = receiver.communicate()
            raise RuntimeError("C++ receiver did not finish in time.\n" + stderr)
        if receiver.returncode:
            raise RuntimeError("C++ portfolio processing failed:\n" + stderr + stdout)
        # C++ logs receiver status before its final JSON object; parse the last JSON line.
        json_lines = [line.strip() for line in stdout.splitlines() if line.strip().startswith("{")]
        if not json_lines:
            raise RuntimeError("C++ did not return portfolio JSON. Output was:\n" + stdout + stderr)
        return json.loads(json_lines[-1])
    finally:
        if receiver.poll() is None:
            receiver.kill()
            receiver.wait()


class Handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(WEB_DIR), **kwargs)

    def end_headers(self):
        self.send_header("Cache-Control", "no-store")
        super().end_headers()

    def do_GET(self):
        if self.path.split("?", 1)[0] == "/api/portfolio":
            try:
                payload = load_actual_portfolio()
                data = json.dumps(payload).encode("utf-8")
                self.send_response(200)
                self.send_header("Content-Type", "application/json; charset=utf-8")
                self.send_header("Content-Length", str(len(data)))
                self.end_headers()
                self.wfile.write(data)
            except Exception as exc:
                data = json.dumps({"error": str(exc)}).encode("utf-8")
                self.send_response(500)
                self.send_header("Content-Type", "application/json; charset=utf-8")
                self.send_header("Content-Length", str(len(data)))
                self.end_headers()
                self.wfile.write(data)
            return
        if self.path == "/":
            self.path = "/index.html"
        return super().do_GET()

    def log_message(self, fmt, *args):
        print("[web] " + (fmt % args))


if __name__ == "__main__":
    host, port = "127.0.0.1", 8000
    try:
        log("Starting Cross-Broker Portfolio website backend")
        ensure_cpp_built()
        # Validate the actual pipeline before serving the page.
        load_actual_portfolio()
        server = ThreadingHTTPServer((host, port), Handler)
        log(f"Website ready at http://{host}:{port}")
        server.serve_forever()
    except KeyboardInterrupt:
        log("Website stopped")
    except Exception as exc:
        log("STARTUP ERROR: " + repr(exc))
        log("Check Python and g++ installation, and see README.md for requirements.")
