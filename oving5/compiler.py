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
    try:
        data = request.get_json()
        app.logger.info('Received data: %s', data)  # Log the received data
        code = data['code']
        execution_id = str(uuid.uuid4())
        result = run_in_docker(code, execution_id)
        return jsonify(result)
    except Exception as e:
        return jsonify ({"error": str(e)}), 400
    


def run_in_docker(code, execution_id):

    exec_dir = f"/tmp/{execution_id}"
    os.makedirs(exec_dir, exist_ok=True)
    
    with open(f"{exec_dir}/execute_code.py", "w") as code_file:
        code_file.write("try:\n")
        code_file.write("    " + "\n    ".join(code.split('\n')) + "\n")
        code_file.write("except Exception as e:\n")
        code_file.write("    import traceback\n")
        code_file.write("    print(traceback.format_exc())\n")
    
    # Copy necessary files
    subprocess.run(["cp", "Dockerfile", f"{exec_dir}/Dockerfile"], check=True)
    
    # Build and run the Docker container
    docker_path = "/opt/homebrew/bin/docker"  # Use the full path to the Docker executable
    build_command = [docker_path, "build", "-t", f"code_execution:{execution_id}", exec_dir]
    run_command = [docker_path, "run", "--rm", f"code_execution:{execution_id}"]

    try:
        subprocess.run(build_command, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        result = subprocess.run(run_command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        return {"output": result.stdout.strip(), "error": result.stderr.strip()}
    except subprocess.CalledProcessError as e:
        return {"error": str(e)}


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

