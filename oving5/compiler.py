from flask import Flask, request, jsonify
import subprocess
import uuid
import os

from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')



@app.route('/compile', methods=['POST'])
def compile_code():
    # Generate a unique file name
    code_id = uuid.uuid4().hex
    source_file = f"/tmp/{code_id}.py"

    # Extract code from the request
    code = request.json['code']

    # Save the code to a file
    with open(source_file, "w") as f:
        f.write(code)

    # Execute the Python code
    result = subprocess.run(["python", source_file], capture_output=True, text=True)

    # Clean up the source file
    os.remove(source_file)

    # Return the output or error
    if result.returncode == 0:
        return jsonify({"output": result.stdout})
    else:
        return jsonify({"error": result.stderr})


if __name__ == '__main__':
    app.run(debug=True)