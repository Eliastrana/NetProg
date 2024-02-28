from flask import Flask, request, jsonify, render_template
import subprocess
import uuid
import os

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/compile', methods=['POST'])
def compile_code():
    code_id = uuid.uuid4().hex
    source_file = f"/tmp/{code_id}.py"
    code = request.json.get('code')

    if not code:
        return jsonify({"error": "No code provided"}), 400

    with open(source_file, "w") as f:
        f.write(code)

    try:
        result = subprocess.run(["python", source_file], capture_output=True, text=True, timeout=5)
        os.remove(source_file)  # Ensure the file is removed even if an error occurs
    except subprocess.TimeoutExpired:
        return jsonify({"error": "Code execution timed out"}), 408
    except Exception as e:
        return jsonify({"error": str(e)}), 500

    if result.returncode == 0:
        return jsonify({"output": result.stdout})
    else:
        return jsonify({"error": result.stderr}), 400

if __name__ == '__main__':
    app.run(debug=True)
